#include "lac.hpp"

// void gridSearch(Lac& lac, const std::string& trainingFile, const std::string& testingFile, const std::vector<int>& intersectionLimitRange, const std::vector<int>& minIntersectionsRange) {
//     double bestPerformance = -1;
//     int bestIntersectionLimit, bestMinIntersections;

//     for (int intersectionLimit : intersectionLimitRange) {
//         for (int minIntersections : minIntersectionsRange) {
//             lac.setIntersectionLimit(intersectionLimit);
//             lac.setMinIntersections(minIntersections);
//             lac.training(trainingFile);
//             double performance = lac.testing(testingFile);
//             if (performance > bestPerformance) {
//                 bestPerformance = performance;
//                 bestIntersectionLimit = intersectionLimit;
//                 bestMinIntersections = minIntersections;
//             }
//         }
//     }

//     std::cout << "Best performance: " << bestPerformance << "\n";
//     std::cout << "Best Intersection Limit: " << bestIntersectionLimit << "\n";
//     std::cout << "Best Min Intersections: " << bestMinIntersections << "\n";
// }

int main() {

    std::string trainingFile = "datasets/poker-hand-training.data";
    std::string testingFile = "datasets/poker-hand-testing-100.data";
    // std::vector<int> intersectionLimitRange = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 7, 18, 19, 20};
    // std::vector<int> minIntersectionsRange = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 7, 18, 19, 20};
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;

    Lac lac = Lac(features, classes);

    // treino
    //  lac.training("datasets/iris.data");
    lac.training("datasets/poker-hand-training.data");

    // teste
    auto start = chrono::high_resolution_clock::now();

    float acuracia = lac.testing("datasets/poker-hand-testing-100.data");
    // float acuracia = lac.testing("datasets/poker-hand-testing-10000.data");
    // lac.testing("datasets/iris-training.data");

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, " << (duration.count() % 60000) / 1000 << " segundos e " << (duration.count() % 60000) % 1000 << " milissegundos" << endl;

    float resultado1 = acuracia / (float)duration.count();
    float resultado2 = (float)duration.count() / acuracia;

    if (resultado1 > resultado2) {
        cout << "Resultado: " << resultado1 << endl;
    } else {
        cout << "Resultado: " << resultado2 << endl;
    }

    // gridSearch(lac, trainingFile, testingFile, intersectionLimitRange, minIntersectionsRange);

    return 0;
}