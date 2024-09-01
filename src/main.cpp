#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <chrono>
#include "lac.hpp"

using namespace std;

// Função para carregar dados
void loadData(const std::string& fileName, std::vector<std::vector<int>>& features, std::vector<int>& labels) {
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> featureRow;
        int value;
        while (ss >> value) {
            featureRow.push_back(value);
            if (ss.peek() == ',') ss.ignore();
        }
        labels.push_back(featureRow.back());  // Último valor é o rótulo
        featureRow.pop_back();  // Remove o rótulo dos recursos
        features.push_back(featureRow);
    }
}

// Função para calcular o valor do teste do qui-quadrado
double chiSquared(const unordered_map<pair<int, int>, int, pairHash>& observed, 
                  const unordered_map<pair<int, int>, double, pairHash>& expected) {
    double chi2 = 0.0;
    for (const auto& pair : observed) {
        int obs = pair.second;
        double exp = expected.at(pair.first);
        if (exp != 0) {
            chi2 += pow(obs - exp, 2) / exp;
        }
    }
    return chi2;
}

// Função para aplicar o teste do qui-quadrado e selecionar características
vector<int> featureSelectionChiSquared(const std::vector<std::vector<int>>& features, 
                                       const std::vector<int>& labels, int topK) {
    int numSamples = features.size();
    int numFeatures = features[0].size();
    int numClasses = *max_element(labels.begin(), labels.end()) + 1;

    vector<double> chi2Scores(numFeatures, 0.0);
    
    // Para cada característica
    for (int featureIdx = 0; featureIdx < numFeatures; ++featureIdx) {
        unordered_map<pair<int, int>, int, pairHash> observed;
        unordered_map<pair<int, int>, double, pairHash> expected;

        // Contar a frequência observada
        for (int i = 0; i < numSamples; ++i) {
            int featureValue = features[i][featureIdx];
            int label = labels[i];
            observed[{featureValue, label}]++;
        }

        // Calcular as frequências esperadas
        unordered_map<int, int> featureCounts;
        unordered_map<int, int> labelCounts;

        for (const auto& pair : observed) {
            featureCounts[pair.first.first]++;
            labelCounts[pair.first.second]++;
        }

        int total = numSamples;
        for (const auto& featureCount : featureCounts) {
            for (const auto& labelCount : labelCounts) {
                double exp = (static_cast<double>(featureCount.second) * labelCount.second) / total;
                expected[{featureCount.first, labelCount.first}] = exp;
            }
        }

        // Calcular o valor do qui-quadrado
        double chi2Value = chiSquared(observed, expected);
        chi2Scores[featureIdx] = chi2Value;
    }

    // Ordenar características com base nos valores do qui-quadrado
    vector<int> selectedFeatures;
    vector<pair<double, int>> chi2FeaturePairs;
    for (int i = 0; i < numFeatures; ++i) {
        chi2FeaturePairs.push_back({chi2Scores[i], i});
    }
    sort(chi2FeaturePairs.rbegin(), chi2FeaturePairs.rend()); // Ordena do maior para o menor

    for (int i = 0; i < topK; ++i) {
        selectedFeatures.push_back(chi2FeaturePairs[i].second);
    }

    return selectedFeatures;
}

int main() {
    std::vector<std::vector<int>> features;
    std::vector<int> labels;

    // Carregar dados
    loadData("datasets/poker-hand-training.data", features, labels);

    // Selecionar características usando o teste do qui-quadrado
    int topK = 10;  // Número de características a selecionar
    vector<int> selectedFeatures = featureSelectionChiSquared(features, labels, topK);

    cout << "Características selecionadas (índices): ";
    for (int idx : selectedFeatures) {
        cout << idx << " ";
    }
    cout << endl;

    // Criar mapas de características e classes
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> featureMap;
    unordered_map<int, unordered_set<int>> classMap;

    // Preencher featureMap e classMap com as características selecionadas
    for (int i = 0; i < features.size(); ++i) {
        vector<int> filteredFeatures;
        for (int idx : selectedFeatures) {
            filteredFeatures.push_back(features[i][idx]);
        }
        int label = labels[i];
        // Preencher featureMap e classMap conforme necessário
        // Exemplo fictício de preenchimento
        for (int f : filteredFeatures) {
            featureMap[{f, label}].insert(f);
        }
        classMap[label].insert(filteredFeatures.begin(), filteredFeatures.end());
    }

    Lac lac(featureMap, classMap);

    // Treinar e testar o modelo
    lac.training("datasets/poker-hand-training.data");
    auto start = chrono::high_resolution_clock::now();
    float accuracy = lac.testing("datasets/poker-hand-testing-100.data");
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, "
         << (duration.count() % 60000) / 1000 << " segundos e "
         << (duration.count() % 1000) << " milissegundos" << endl;

    cout << "Acurácia: " << accuracy << endl;

    return 0;
}
