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

// Função para calcular o valor do F-test para ANOVA
double fTestAnova(const vector<double>& groupMeans, const vector<double>& groupVariances, 
                  const vector<int>& groupSizes, double overallMean, int totalSamples) {
    int numGroups = groupMeans.size();

    double ssBetween = 0.0; // Soma dos quadrados entre grupos
    double ssWithin = 0.0;  // Soma dos quadrados dentro dos grupos

    for (int i = 0; i < numGroups; ++i) {
        ssBetween += groupSizes[i] * pow(groupMeans[i] - overallMean, 2);
        ssWithin += (groupSizes[i] - 1) * groupVariances[i];
    }

    double msBetween = ssBetween / (numGroups - 1);          // Mean Square Between
    double msWithin = ssWithin / (totalSamples - numGroups);  // Mean Square Within

    return msBetween / msWithin; // F-value
}

// Função para aplicar o teste ANOVA e selecionar características
vector<int> featureSelectionAnova(const std::vector<std::vector<int>>& features, 
                                  const std::vector<int>& labels, int topK) {
    int numSamples = features.size();
    int numFeatures = features[0].size();
    int numClasses = *max_element(labels.begin(), labels.end()) + 1;

    vector<double> fScores(numFeatures, 0.0);

    // Para cada característica
    for (int featureIdx = 0; featureIdx < numFeatures; ++featureIdx) {
        vector<double> groupMeans(numClasses, 0.0);
        vector<double> groupVariances(numClasses, 0.0);
        vector<int> groupSizes(numClasses, 0);

        double overallMean = 0.0;

        // Calcular médias de cada grupo
        for (int i = 0; i < numSamples; ++i) {
            int featureValue = features[i][featureIdx];
            int label = labels[i];
            groupMeans[label] += featureValue;
            groupSizes[label]++;
        }

        // Calcular média global
        for (int j = 0; j < numClasses; ++j) {
            if (groupSizes[j] > 0) {
                groupMeans[j] /= groupSizes[j];
                overallMean += groupMeans[j] * groupSizes[j];
            }
        }
        overallMean /= numSamples;

        // Calcular variâncias de cada grupo
        for (int i = 0; i < numSamples; ++i) {
            int featureValue = features[i][featureIdx];
            int label = labels[i];
            groupVariances[label] += pow(featureValue - groupMeans[label], 2);
        }
        for (int j = 0; j < numClasses; ++j) {
            if (groupSizes[j] > 1) {
                groupVariances[j] /= (groupSizes[j] - 1);
            }
        }

        // Calcular valor F da ANOVA
        double fValue = fTestAnova(groupMeans, groupVariances, groupSizes, overallMean, numSamples);
        fScores[featureIdx] = fValue;
    }

    // Ordenar características com base nos valores de F
    vector<int> selectedFeatures;
    vector<pair<double, int>> fFeaturePairs;
    for (int i = 0; i < numFeatures; ++i) {
        fFeaturePairs.push_back({fScores[i], i});
    }
    sort(fFeaturePairs.rbegin(), fFeaturePairs.rend()); // Ordena do maior para o menor

    for (int i = 0; i < topK; ++i) {
        selectedFeatures.push_back(fFeaturePairs[i].second);
    }

    return selectedFeatures;
}

int main() {
    std::vector<std::vector<int>> features;
    std::vector<int> labels;

    // Carregar dados
    loadData("datasets/poker-hand-training.data", features, labels);

    // Selecionar características usando ANOVA
    int topK = 10;  // Número de características a selecionar
    vector<int> selectedFeatures = featureSelectionAnova(features, labels, topK);

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