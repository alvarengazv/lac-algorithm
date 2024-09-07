#include "lac.hpp"

int Lac::INTERSECTION_LIMIT = 0;
unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual> Lac::similarityCache;
pthread_mutex_t Lac::mutex = PTHREAD_MUTEX_INITIALIZER;

// Construtor da classe
Lac::Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes) {
    this->features = features;
    this->classes = classes;
}

// Treinamento do algoritmo
void Lac::training(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    string line;
    int j = 1;

    while (getline(file, line)) {
        vector<int> values = splitString(line);

        if (DECREASE_CARDINALITY) {
            for (int i = 0; i < values.size() - 1; i += 2) {
                int value1 = values[i];
                int value2 = values[i + 1];
                int index = (value1 - 1) * 13 + (value2 - 1);

                pair<int, int> feature((i + 2) / 2, index);

                if (features.find(feature) != features.end()) {
                    features[feature].insert(j);
                } else {
                    unordered_set<int> lines;
                    lines.insert(j);
                    features.insert(pair<pair<int, int>, unordered_set<int>>(feature, lines));
                }
            }
        } else {
            INTERSECTION_LIMIT = 10;

            for (int i = 0; i < values.size() - 1; i++) {
                pair<int, int> feature((i + 1), values[i]);

                if (features.find(feature) != features.end()) {
                    features[feature].insert(j);
                } else {
                    unordered_set<int> lines;
                    lines.insert(j);
                    features.insert(pair<pair<int, int>, unordered_set<int>>(feature, lines));
                }
            }
        }

        if (classes.find(values.back()) != classes.end()) {
            classes[values.back()].insert(j);
        } else {
            unordered_set<int> lines;
            lines.insert(j);
            classes.insert(pair<int, unordered_set<int>>(values.back(), lines));
        }

        j++;
    }

    file.close();
}

// Teste do algoritmo
float Lac::testing(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        return 0;
    }

    ofstream outFile("datasets/output.txt");
    if (!outFile.is_open()) {
        return 0;
    }

    string line;
    int j = 1, loss = 0, accuracy = 0;
    pthread_mutex_init(&mutex, NULL);

    while (getline(file, line)) {
        double result[classes.size()] = {0};
        double totalSupport = 0;

        vector<int> values = splitString(line);
        vector<pair<int, int>> lineFeatures;

        if (DECREASE_CARDINALITY) {
            for (int i = 0; i < values.size() - 1; i += 2) {
                int value1 = values[i];
                int value2 = values[i + 1];
                int index = (value1 - 1) * 13 + (value2 - 1);

                pair<int, int> feature((i + 2) / 2, index);

                if (features.find(feature) != features.end()) {
                    lineFeatures.push_back(feature);
                }
            }
        } else {
            for (int i = 0; i < values.size() - 1; i++) {
                pair<int, int> feature((i + 1), values[i]);

                if (features.find(feature) != features.end()) {
                    lineFeatures.push_back(feature);
                }
            }
        }

        int n = lineFeatures.size();
        if (MAX_COMBS > 0 && MAX_COMBS < n) {
            n = MAX_COMBS;
        }

        vector<unordered_set<pair<int, int>, pairHash>> combinationsFeatures = {};

        // Acessando as linhas de cada tupla: 1 a 1, 2 a 2 e assim por diante ate o tamanho maximo da tupla
        bool shouldStop = false;
        for (int q = 1; q <= n; q++) {
            if (shouldStop)
                break;

            combinationsFeatures = combinations(lineFeatures, q);

            // Criar threads para processamento paralelo
            int numThreads = 5;
            pthread_t threads[numThreads];
            ThreadData threadData[numThreads];
            int chunkSize = combinationsFeatures.size() / numThreads;

            for (int t = 0; t < numThreads; t++) {
                threadData[t].combinationsFeatures = &combinationsFeatures;
                threadData[t].features = &features;
                threadData[t].classes = &classes;
                threadData[t].start = t * chunkSize;
                threadData[t].end = (t == numThreads - 1) ? combinationsFeatures.size() : (t + 1) * chunkSize;
                threadData[t].result = result;
                threadData[t].shouldStop = &shouldStop;
                threadData[t].similarityCache = &similarityCache;

                pthread_create(&threads[t], NULL, threadIntersection, (void*)&threadData[t]);
            }

            // Aguardar todas as threads terminarem
            for (int t = 0; t < numThreads; t++) {
                pthread_join(threads[t], NULL);
            }
        }

        j++;

        int classification = findMaxIndex(result, classes.size());

        outFile << (j - 1) << "," << classification << endl;

        if (classification == values.back()) {
            accuracy++;
        } else {
            loss++;
        }
    }

    pthread_mutex_destroy(&mutex);

    cout << "Acertos: " << accuracy << " Erros: " << loss << endl;

    // Porcentagem de acertos
    cout << "Porcentagem de acertos: " << (double)(accuracy * 100) / (accuracy + loss) << "%" << endl;

    outFile << "Acertos(accuracy): " << accuracy << " e Erros(loss): " << loss;

    file.close();
    outFile.close();

    return ((float)accuracy / (float)(accuracy + loss));
}

// String para vetor de inteiros
vector<int> Lac::splitString(string line) {
    vector<int> result;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        result.push_back(stoi(token));
        line.erase(0, pos + delimiter.length());
    }
    result.push_back(stoi(line));
    return result;
}

// Interseção entre uma lista de unordered_set
unordered_set<int> Lac::intersectionAll(vector<unordered_set<int>> lists) {
    if (lists.empty())
        return {};

    unordered_set<int> result = lists[0];
    for (size_t i = 1; i < lists.size(); ++i) {
        unordered_set<int> temp;
        for (const auto& elem : result) {
            if (lists[i].find(elem) != lists[i].end()) {
                temp.insert(elem);
            }
        }
        if (temp.empty()) {
            return {};
        }

        result = move(temp);
    }

    return result;
}

// Acha o indice do maior valor dentro do vetor resultado
int Lac::findMaxIndex(double* arr, int size) {
    int maxIndex = 0;
    double maxValue = arr[0];

    for (int i = 0; i < size; ++i) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

// Encontra todas as combinações possíveis até as de tamanho k no vetor c
vector<unordered_set<pair<int, int>, pairHash>> Lac::combinations(const vector<pair<int, int>>& c, int k) {
    int n = c.size();
    int comb = (1 << k) - 1;
    vector<unordered_set<pair<int, int>, pairHash>> result = {};
    while ((comb < 1 << n)) {
        unordered_set<pair<int, int>, pairHash> currentComb;
        for (int i = 0; i < n; i++) {
            if (((comb >> i) & 1))
                currentComb.insert(c[i]);
        }

        if (currentComb.size() == k) {
            result.push_back(currentComb);
        }

        int x = comb & -comb;
        int y = comb + x;
        int z = (comb & ~y);
        comb = z / x;
        comb >>= 1;
        comb |= y;
    }

    return result;
}

// Encontra a linha de features mais similar no cache
pair<vector<double>, double> Lac::checkSimilarity(cacheKey lineFeatures) {
    pair<vector<double>, double> result = pair<vector<double>, double>({-1}, 0);

    for (const auto combResult : similarityCache) {
        if (combResult.first.size() != lineFeatures.size()) {
            continue;
        }

        double similarity = cosineSimilarity(lineFeatures, combResult.first);
        if (similarity == 1) {
            return pair<vector<double>, double>(combResult.second, similarity);
        }

        if (similarity >= THRESHOLD && similarity > result.second) {
            result = pair<vector<double>, double>(combResult.second, similarity);
        }
    }

    return result;
}

// Similaridade de cosseno entre dois vetores
double Lac::cosineSimilarity(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2) {
    double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;

    for (int i = 0; i < vec1.size(); i++) {
        dotProduct += vec1[i].second * vec2[i].second;
        magnitude1 += vec1[i].second * vec1[i].second;
        magnitude2 += vec2[i].second * vec2[i].second;
    }

    magnitude1 = sqrt(magnitude1);
    magnitude2 = sqrt(magnitude2);

    return dotProduct / (magnitude1 * magnitude2);
}

// Função que será executada pelas threads
void* Lac::threadIntersection(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int r = data->start; r < data->end; r++) {
        vector<pair<int, int>> combinacoesCacheVec(data->combinationsFeatures->at(r).begin(), data->combinationsFeatures->at(r).end());

        if (data->similarityCache->find(combinacoesCacheVec) != data->similarityCache->end()) {
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < data->classes->size(); i++) {
                data->result[i] += data->similarityCache->at(combinacoesCacheVec)[i];
            }
            pthread_mutex_unlock(&mutex);
            continue;
        } else if (USE_COSINE_SIMILARITY && data->similarityCache->size() > 0 && combinacoesCacheVec.size() > 1) {
            pair<vector<double>, double> similarity = checkSimilarity(combinacoesCacheVec);
            if (similarity.second >= THRESHOLD && similarity.first[0] != -1) {
                pthread_mutex_lock(&mutex);
                for (int i = 0; i < data->classes->size(); i++) {
                    data->result[i] += similarity.first[i];
                }

                pthread_mutex_unlock(&mutex);
                continue;
            }
        }

        vector<unordered_set<int>> combinationsLines;
        for (auto c : data->combinationsFeatures->at(r)) {
            combinationsLines.push_back(data->features->at(c));
        }

        unordered_set<int> intersectionPerTuple = {};

        if (data->combinationsFeatures->at(r).size() == 1) {
            intersectionPerTuple = combinationsLines[0];
        } else {
            intersectionPerTuple = intersectionAll(combinationsLines);
            if (intersectionPerTuple.size() <= INTERSECTION_LIMIT) {
                *(data->shouldStop) = true;
                continue;
            }
        }

        for (int i = 0; i < data->classes->size(); i++) {
            unordered_set<int> intersection;
            for (const auto& elem : intersectionPerTuple) {
                if (data->classes->at(i).find(elem) != data->classes->at(i).end()) {
                    intersection.insert(elem);
                }
            }
            int confidence = intersection.size();
            if (confidence > MIN_SUPORTE) {
                double support = (double)confidence / (double)data->features->size();

                pthread_mutex_lock(&mutex);
                data->result[i] += (support);
                if (USE_COSINE_SIMILARITY) {
                    if (data->similarityCache->find(combinacoesCacheVec) != data->similarityCache->end()) {
                        data->similarityCache->at(combinacoesCacheVec).push_back(support);
                    } else {
                        vector<double> classSupports = {};
                        classSupports.push_back(support);
                        data->similarityCache->insert(pair<vector<pair<int, int>>, vector<double>>(combinacoesCacheVec, classSupports));
                    }
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    return NULL;
}