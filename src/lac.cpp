#include "lac.hpp"

Lac::Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes) {
    this->features = features;
    this->classes = classes;
}

void Lac::training(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    string line;
    int j = 1;

    while (getline(file, line)) {
        // vector<float> values = splitString(line);
        vector<int> values = splitString(line);

        // for (int i = 0; i < values.size() - 1; i++) {
        for (int i = 0; i < values.size() - 1; i += 2) {
            int naipe = values[i];
            int carta = values[i + 1];
            int indice = (naipe - 1) * 13 + (carta - 1);

            pair<int, int> feature((i + 2) / 2, indice);

            // pair<int, int> feature((i + 1), values[i]);

            if (features.find(feature) != features.end()) {
                features[feature].insert(j);
            } else {
                unordered_set<int> lines;
                lines.insert(j);
                features.insert(pair<pair<int, int>, unordered_set<int>>(feature, lines));
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

float Lac::testing(string path) {
    cout << "Tamanho do features: " << features.size() << endl;
    ifstream file(path);
    if (!file.is_open()) {
        return 0;
    }

    ofstream outFile("datasets/output.txt");
    if (!outFile.is_open()) {
        return 0;
    }

    string line;
    int j = 1, erros = 0, acertos = 0, intersectionCounter = 0, cacheUsingCounter = 0;

    while (getline(file, line)) {
        double result[classes.size()] = {0};
        double totalSupport = 0;

        vector<int> values = splitString(line);
        vector<pair<int, int>> lineFeatures;

        // for (int i = 0; i < values.size() - 1; i++) {
        for (int i = 0; i < values.size() - 1; i += 2) {
            int naipe = values[i];
            int carta = values[i + 1];
            int indice = (naipe - 1) * 13 + (carta - 1);

            pair<int, int> feature((i + 2) / 2, indice);
            // pair<int, int> feature((i + 1), values[i]);

            if (features.find(feature) != features.end()) {
                lineFeatures.push_back(feature);
            }
        }

        // check if the line is 70% similar to any other line that was already classified
        pair<int, double> similarity = checkSimilarity(lineFeatures);

        if (similarity.second >= THRESHOLD && similarity.first != -1) {
            // cout << "Usando cache" << endl;
            // cout << "Line: " << j << " Similaridade: " << similarity.second << " Classe: " << similarity.first << endl;

            int classification = similarity.first;
            j++;
            outFile << (j - 1) << "," << classification << endl;
            if (similarity.first == values.back()) {
                acertos++;
            } else
                erros++;
            continue;
        }

        int n = lineFeatures.size();

        vector<unordered_set<pair<int, int>, pairHash>> combinacoesFeatures = {};

        // Acessando as linhas de cada tupla: 1 a 1, 2 a 2 e assim por diante ate o tamanho maximo da tupla
        bool shouldStop = false;
        for (int q = 1; q <= n; q++) {
            if (shouldStop)
                break;

            combinacoesFeatures = combo(lineFeatures, q);

            // Criar threads para processamento paralelo
            int numThreads = combinacoesFeatures.size();
            pthread_t threads[numThreads];
            ThreadData threadData[numThreads];
            int chunkSize = combinacoesFeatures.size() / numThreads;

            for (int t = 0; t < numThreads; t++) {
                threadData[t].combinacoesFeatures = &combinacoesFeatures;
                threadData[t].features = &features;
                threadData[t].classes = &classes;
                threadData[t].start = t * chunkSize;
                threadData[t].end = (t == numThreads - 1) ? combinacoesFeatures.size() : (t + 1) * chunkSize;
                threadData[t].result = result;
                threadData[t].cacheResults = &cacheResults;

                pthread_create(&threads[t], NULL, threadIntersecao, (void*)&threadData[t]);
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
            acertos++;
            generateLSH(lineFeatures, classification);
        } else {
            erros++;
        }
    }

    cout << "Acertos: " << acertos << " Erros: " << erros << endl;

    // Porcentagem de acertos
    cout << "Porcentagem de acertos: " << (acertos * 100) / (acertos + erros) << "%" << endl;

    outFile << "Acertos(accuracy): " << acertos << " e Erros(loss): " << erros;

    file.close();
    outFile.close();

    return ((float)acertos / (float)(acertos + erros));
}

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

// interseção entre uma lista de unordered_set
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

    // cout << "---------------------------------" << endl;
    for (int i = 0; i < size; ++i) {
        // cout << "Classe: " << i << " Valor: " << arr[i] << endl;
        if (arr[i] > maxValue) {
            maxValue = arr[i];
            maxIndex = i;
        }
    }
    // cout << "---------------------------------" << endl;

    return maxIndex;
}

void Lac::imprimirFeatures() {
    for (auto v : features) {
        cout << "Feature: " << std::fixed << std::setprecision(2) << "(" << v.first.first << "," << std::setprecision(2) << v.first.second << ")";
        for (auto i : v.second) {
            cout << i << " ";
        }
        cout << endl;
    }
}

void Lac::imprimirClasses() {
    for (auto v : classes) {
        cout << "Classe: " << v.first << " >>>>> ";
        for (auto i : v.second) {
            cout << i << " ";
        }
        cout << endl;
    }
}

vector<unordered_set<pair<int, int>, pairHash>> Lac::combo(const vector<pair<int, int>>& c, int k) {
    int n = c.size();
    int combo = (1 << k) - 1;
    vector<unordered_set<pair<int, int>, pairHash>> result = {};
    while ((combo < 1 << n)) {
        unordered_set<pair<int, int>, pairHash> currentCombo;
        for (int i = 0; i < n; i++) {
            if (((combo >> i) & 1))
                currentCombo.insert(c[i]);
        }

        if (currentCombo.size() == k) {
            result.push_back(currentCombo);
        }

        int x = combo & -combo;
        int y = combo + x;
        int z = (combo & ~y);
        combo = z / x;
        combo >>= 1;
        combo |= y;
    }

    return result;
}

void Lac::generateLSH(vector<pair<int, int>> lineFeatures, int classBucket) {
    lshBuckets.insert(pair<vector<pair<int, int>>, int>(lineFeatures, classBucket));
}

pair<int, double> Lac::checkSimilarity(vector<pair<int, int>> lineFeatures) {
    pair<int, double> result = pair<int, double>(-1, 0);
    // find the bucket that has the most similar features
    for (const auto lshBucket : lshBuckets) {
        double similarity = cosineSimilarity(lineFeatures, lshBucket.first);
        // double similarity = jaccardSimilarity(lineFeatures, lshBucket.first);
        // cout << "similarity: " << similarity << endl;
        if (similarity >= THRESHOLD && similarity > result.second) {
            result = pair<int, double>(lshBucket.second, similarity);
        }
    }

    if (result.first != -1)
        generateLSH(lineFeatures, result.first);
    return result;
}

double Lac::cosineSimilarity(const vector<pair<int, int>>& set1, const vector<pair<int, int>>& set2) {
    double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;

    for (int i = 0; i < set1.size(); i++) {
        dotProduct += set1[i].second * set2[i].second;
        magnitude1 += set1[i].second * set1[i].second;
        magnitude2 += set2[i].second * set2[i].second;
    }

    magnitude1 = sqrt(magnitude1);
    magnitude2 = sqrt(magnitude2);

    return dotProduct / (magnitude1 * magnitude2);
}

double Lac::jaccardSimilarity(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2) {
    // genereate set from vector of integers
    unordered_set<int> set1;
    for (const auto& elem : vec1) {
        set1.insert(elem.second);
    }

    unordered_set<int> set2;
    for (const auto& elem : vec2) {
        set2.insert(elem.second);
    }

    int intSize = intersectionSize(set1, set2);
    int unionSize = set1.size() + set2.size() - intSize;
    return static_cast<double>(intSize) / unionSize;
}

int Lac::intersectionSize(const unordered_set<int>& set1, const unordered_set<int>& set2) {
    int count = 0;
    for (const auto& item : set1) {
        if (set2.find(item) != set2.end()) {
            count++;
        }
    }
    return count;
}

void* Lac::threadIntersecao(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int r = data->start; r < data->end; r++) {
        unordered_set<pair<int, int>, pairHashSimilarity> combinacoesCacheSet(data->combinacoesFeatures->at(r).begin(), data->combinacoesFeatures->at(r).end());

        int i = 0;
        if (data->cacheResults->find(combinacoesCacheSet) != data->cacheResults->end()) {
            for (auto c : data->cacheResults->at(combinacoesCacheSet)) {
                // cout << "ENTREIIIII " << c << endl;
                data->result[i++] += c;
            }
            continue;
        }

        vector<unordered_set<int>> combinacoesLinhas;
        for (auto c : data->combinacoesFeatures->at(r)) {
            combinacoesLinhas.push_back(data->features->at(c));
        }

        unordered_set<int> intersectionPerTupla = {};

        if (data->combinacoesFeatures->at(r).size() == 1) {
            intersectionPerTupla = combinacoesLinhas[0];
        } else {
            intersectionPerTupla = intersectionAll(combinacoesLinhas);
            if (intersectionPerTupla.size() <= INTERSECTION_LIMIT)
                continue;
        }

        for (int i = 0; i < data->classes->size(); i++) {
            unordered_set<int> intersecao;
            for (const auto& elem : intersectionPerTupla) {
                if (data->classes->at(i).find(elem) != data->classes->at(i).end()) {
                    intersecao.insert(elem);
                }
            }
            int confident = intersecao.size();
            if (confident > MIN_SUPORTE) {
                double support = (double)confident / (double)data->features->size();

                data->result[i] += (support * data->combinacoesFeatures->size());
                data->cacheResults->operator[](combinacoesCacheSet)[i] = (support * data->combinacoesFeatures->size());
            }
        }
    }

    return NULL;
}