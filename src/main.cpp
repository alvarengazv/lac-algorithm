#include "lac.hpp"

void gridSearch(Lac& lac, const string& trainingFile, const string& testingFile, const vector<int>& intersectionLimitRange, const vector<float>& thresholdRange) {
    vector<vector<double>> resultsPerformaceAcuracy;
    vector<vector<double>> resultsPerformaceTime;
    vector<vector<double>> resultsPerformace;
    int bestIntersectionLimit;
    float bestThreshold;

    for (int intersectionLimit : intersectionLimitRange) {
        for (float threshold : thresholdRange) {
            lac.setIntersectionLimit(intersectionLimit);
            lac.setThreshold(threshold);
            lac.training(trainingFile);

            cout << "Limite de interseção: " << intersectionLimit << " Threshold: " << threshold << endl;

            auto start = chrono::high_resolution_clock::now();
            float acuracia = lac.testing(testingFile);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, " << (duration.count() % 60000) / 1000 << " segundos e " << (duration.count() % 60000) % 1000 << " milissegundos" << endl;

            double performanceAcuracy = acuracia;;
            double performanceTime = (double)duration.count(); 
            double performace;

            float resultado1 = acuracia / (float)duration.count();
            float resultado2 = (float)duration.count() / acuracia;

            if (resultado1 > resultado2) {
                performace = resultado1;
            } else {
                performace = resultado2;
            }
            cout << "Resultado: " << performace << endl << endl;
                        
            resultsPerformaceAcuracy.push_back({(double)intersectionLimit, (double)threshold, performanceAcuracy});
            resultsPerformaceTime.push_back({(double)intersectionLimit, (double)threshold, performanceTime});
            resultsPerformace.push_back({(double)intersectionLimit, (double)threshold, performanceAcuracy, performanceTime, performace});
        }
    }

    double bestPerformanceAcuracy = 0;
    double bestPerformanceTime = 999999999999999;
    double bestPerformance = 999999999999999;

    for (auto result : resultsPerformaceAcuracy) {
        if (result[2] > bestPerformanceAcuracy) {
            bestPerformanceAcuracy = result[2];
            bestIntersectionLimit = result[0];
            bestThreshold = result[1];
        }
    }

    cout << "-----------------------------------" << endl;                                                                                                                                                                  
    cout << "Melhor acurácia: " << bestPerformanceAcuracy * 100 << "%" << " com limite de interseção " << bestIntersectionLimit << " e threshold " << bestThreshold << endl;

    for (auto result : resultsPerformaceTime) {
        if (result[2] < bestPerformanceTime) {
            bestPerformanceTime = result[2];
            bestIntersectionLimit = result[0];
            bestThreshold = result[1];
        }                                      

    }
    auto time = chrono::duration_cast<chrono::milliseconds>(chrono::milliseconds((int)bestPerformanceTime)); // convertendo para milissegundos
    cout << "Melhor tempo: " << time.count() / 60000 << ":" << (time.count() % 60000) / 1000 << ":" << (time.count() % 60000) % 1000 << " com limite de interseção " << bestIntersectionLimit << " e threshold " << bestThreshold << endl;

    for (auto result : resultsPerformace) {
        if (result[4] < bestPerformance) {
            bestPerformanceAcuracy = result[2];
            bestPerformanceTime = result[3];
            bestPerformance = result[4];
            bestIntersectionLimit = result[0];
            bestThreshold = result[1];
        }
    }

    auto time2 = chrono::duration_cast<chrono::milliseconds>(chrono::milliseconds((int)bestPerformanceTime)); // convertendo para milissegundos
    cout << "Melhor performance: " << bestPerformance << " com acurácia de " << bestPerformanceAcuracy * 100 << "%" << " com tempo de " << time2.count() / 60000 << ":" << (time2.count() % 60000) / 1000 << ":" << (time2.count() % 60000) % 1000 << " com limite de interseção " << bestIntersectionLimit << " e threshold " << bestThreshold << endl;

    cout << "\nMELHOR INERSECTION_LIMIT: " << bestIntersectionLimit << "\nMELHOR THRESHOLD: " << bestThreshold << endl;
}

int main() {
    vector<int> intersectionLimitRange = {0, 1, 5, 10, 15, 20};
    vector<float> thresholdRange = {0.7, 0.75, 0.8, 0.85, 0.9, 0.95};

    string trainingFile = "datasets/base-avaliacao/poker-hand-training.data";
    string testingFile = "datasets/base-avaliacao/poker-hand-testing.data";

    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;

    Lac lac = Lac(features, classes);

    gridSearch(lac, trainingFile, testingFile, intersectionLimitRange, thresholdRange);

    // // treino
    // lac.training(trainingFile);

    // auto start = chrono::high_resolution_clock::now();

    // // teste
    // float acuracia = lac.testing(testingFile);

    // auto end = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    // cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, " << (duration.count() % 60000) / 1000 << " segundos e " << (duration.count() % 60000) % 1000 << " milissegundos" << endl;

    // float resultado1 = acuracia / (float)duration.count();
    // float resultado2 = (float)duration.count() / acuracia;

    // if (resultado1 > resultado2) {
    //     cout << "Resultado: " << resultado1 << endl;
    // } else {
    //     cout << "Resultado: " << resultado2 << endl;
    // }

    return 0;
}