#include "lac.hpp"

Lac::Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes) {
    this->features = features;
    this->classes = classes;
}

void Lac::readFile(string path) {
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

        int i = 0;
        for (auto v : values) {
            // pair<int, float> feature(++i, v);
            pair<int, int> feature(++i, v);

            if (features.find(feature) != features.end()) {
                features[feature].insert(j);
            } else {
                unordered_set<int> lines;
                lines.insert(j);
                features.insert(pair<pair<int, int>, unordered_set<int>>(feature, lines));
            }
            if (i == values.size() - 1)
                break;
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

void Lac::testing(string path) {
    cout << "Tamanho do features: " << features.size() << endl;
    ifstream file(path);
    if (!file.is_open()) {
        return;
    }

    ofstream outFile("datasets/output.txt");
    if (!outFile.is_open()) {
        return;
    }

    string line;
    int j = 1, erros = 0, acertos = 0, intersectionCounter = 0, cacheUsingCounter = 0;
    vector<float> totalResult(10, 0.0f);

    while (getline(file, line)) {
        double result[classes.size()] = {0};

        // vector<float> values = splitString(line);
        vector<int> values = splitString(line);
        vector<pair<int, int>> lineFeaturesCartas;
        vector<pair<int, int>> lineFeaturesNaipes;
        // vector<pair<int, float>> lineFeatures;

        int i = 0;
        for (auto v : values) {
            pair<int, int> feature(++i, v);

            if (features.find(feature) != features.end()) {
                if (i % 2 != 0) {
                    lineFeaturesNaipes.push_back(feature);
                } else {
                    lineFeaturesCartas.push_back(feature);
                }
            }

            if (i == values.size() - 1)
                break;
        }
        // cout << "Tamanho do features: " << lineFeatures.size() << endl;
        int n = 10;

        vector<unordered_set<pair<int, int>, pairHash>> combinacoesFeatures = {};

        // Acessando as linhas de cada tupla: 1 a 1, 2 a 2 e assim
        // por diante ate o tamanho maximo da tupla, 11.
        bool shouldStop = false;
        for (int q = 1; q <= n; q++) {
            if (shouldStop)
                break;
            combinacoesFeatures = combo(lineFeaturesNaipes, q);
            vector<unordered_set<pair<int, int>, pairHash>> combinacoesAux = combo(lineFeaturesCartas, q);
            combinacoesFeatures.insert(combinacoesFeatures.end(), combinacoesAux.begin(), combinacoesAux.end());

            // mostrar as combinacoes
            // for (auto c : combinacoesFeatures) {
            //     for (auto pair : c) {
            //         cout << pair.first << " " << pair.second << " ";
            //     }
            //     cout << endl;
            // }
            // cout << "Tamanho das combinacoes: " << combinacoesFeatures.size() << endl;
            // int aux1 = 0;
            // cin >> aux1;

            for (int r = 0; r < combinacoesFeatures.size(); r++) {
                // vector para guardar as combinacoes das linhas das features, 1 a 1, 2 a 2.....
                vector<unordered_set<int>> combinacoesLinhas = {};

                for (auto c : combinacoesFeatures[r]) {
                    // pair<int, float> key = pair(c.first, c.second);
                    pair<int, int> key = pair(c.first, c.second);
                    combinacoesLinhas.push_back(features[key]);
                }

                // fazendo a interseção das combinacoes 1 a 1, 2 a 2.....

                unordered_set<int> intersectionPerTupla = {};

                if (combinacoesFeatures[r].size() == 1) {
                    intersectionPerTupla = combinacoesLinhas[0];
                } else if (cache.find(combinacoesFeatures[r]) != cache.end()) {
                    // cout << "Usou o cache!" << endl;
                    intersectionPerTupla = cache[combinacoesFeatures[r]];
                    // cout << "Size: " << intersectionPerTupla.size() << endl;
                    cacheUsingCounter++;
                } else {
                    intersectionPerTupla = intersectionAll(combinacoesLinhas);
                    if (intersectionPerTupla.size() > INTERSECTION_LIMIT) {
                        cache[combinacoesFeatures[r]] = intersectionPerTupla;
                        intersectionCounter++;
                    } else {
                        shouldStop = true;
                        continue;
                    }
                }

                // std::cout << "Cache size: " << cache.size() << std::endl;

                // cout << "Intersec: " << intersectionCounter << endl;
                // cout << "Cache: " << cacheUsingCounter << endl;
                for (auto c : classes) {
                    if (cacheResults.find(pair<unordered_set<pair<int, int>, pairHash>, int>(combinacoesFeatures[r], c.first)) != cacheResults.end()) {
                        // cout << "Usou o cache2!" << endl;
                        result[c.first] += cacheResults[pair<unordered_set<pair<int, int>, pairHash>, int>(combinacoesFeatures[r], c.first)];
                        continue;
                    }
                    unordered_set<int> intersecao;
                    for (const auto &elem : intersectionPerTupla) {
                        if (c.second.find(elem) != c.second.end()) {
                            intersecao.insert(elem);
                        }
                    }
                    int confident = intersecao.size();

                    if (confident > MIN_SUPORTE) {
                        double support = (double)confident / (double)features.size();
                        cacheResults[pair<unordered_set<pair<int, int>, pairHash>, int>(combinacoesFeatures[r], c.first)] = support;
                        result[c.first] += support;
                    }
                }
            }
        }

        cout << "j: " << j << endl;
        j++;

        int classification = findMaxIndex(result, classes.size());

        // cout << "Classificação: " << classification << " Valor: " << result[classification] << " Correta: " << values.back() << endl;

        outFile << (j - 1) << "," << classification << endl;

        if (classification == values.back())
            acertos++;
        else
            erros++;
    }

    cout << "Acertos: " << acertos << " Erros: " << erros << endl;

    // Porcentagem de acertos
    cout << "Porcentagem de acertos: " << (acertos * 100) / (acertos + erros) << "%" << endl;

    outFile << "Acertos(accuracy): " << acertos << " e Erros(loss): " << erros;

    file.close();
}

// vector<float> Lac::splitString(string line) {
vector<int> Lac::splitString(string line) {
    // vector<float> result;
    vector<int> result;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        // result.push_back(stof(token));
        result.push_back(stoi(token));
        line.erase(0, pos + delimiter.length());
    }
    // result.push_back(stof(line));
    result.push_back(stoi(line));
    return result;
}

// gets

unordered_map<pair<int, int>, unordered_set<int>, pairHash> Lac::getFeatures() {
    return features;
}

unordered_map<int, unordered_set<int>> Lac::getClasses() {
    return classes;
}

// interseção entre uma lista de unordered_set
unordered_set<int> Lac::intersectionAll(vector<unordered_set<int>> lists) {
    if (lists.empty())
        return {};

    unordered_set<int> result = lists[0];
    for (size_t i = 1; i < lists.size(); ++i) {
        unordered_set<int> temp;
        for (const auto &elem : result) {
            if (lists[i].find(elem) != lists[i].end()) {
                temp.insert(elem);
            }
        }
        result = temp;
    }
    return result;
}

// interseção entre dois unordered_set
int Lac::intersection(unordered_set<int> first, unordered_set<int> second) {
    unordered_set<int> intersection;
    for (const auto &elem : first) {
        if (second.find(elem) != second.end()) {
            intersection.insert(elem);
        }
    }
    return intersection.size();
}

// Acha o indice do maior valor dentro do vetor resultado
int Lac::findMaxIndex(double *arr, int size) {
    int maxIndex = 0;
    double maxValue = arr[0];

    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

void Lac::imprimirFeatures() {
    for (auto v : features) {
        cout << "Feature: " << std::fixed << std::setprecision(2) << v.first.second << " " << std::setprecision(2) << v.first.first << " >>>>> ";
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

// void Lac::pretty_print(const vector<pair<int, float>> &c, int combo) {
void Lac::pretty_print(const vector<pair<int, int>> &c, int combo) {
    int n = c.size();
    for (int i = 0; i < n; i++) {
        if ((combo >> i) & 1)
            cout << "(" << c[i].first << ", " << c[i].second << ") ";
    }
    cout << endl;
}

// vector<vector<pair<int, float>>> Lac::combo(const vector<pair<int, float>> &c, int k) {
vector<unordered_set<pair<int, int>, pairHash>> Lac::combo(const vector<pair<int, int>> &c, int k) {
    int n = c.size();
    int combo = (1 << k) - 1;
    // vector<vector<pair<int, float>>> result = {};
    vector<unordered_set<pair<int, int>, pairHash>> result = {};
    while ((combo < 1 << n)) {
        // pretty_print(c, combo);

        unordered_set<pair<int, int>, pairHash> currentCombo;
        // vector<pair<int, float>> currentCombo;
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