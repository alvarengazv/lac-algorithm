#include "lac.hpp"

int main() {
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;

    Lac lac = Lac(features, classes);

    // treino
    //  lac.training("datasets/iris.data");
    lac.training("datasets/poker-hand-training.data");

    // teste
    auto start = chrono::high_resolution_clock::now();

    lac.testing("datasets/poker-hand-testing-100.data");
    // lac.testing("datasets/iris-training.data");

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, " << (duration.count() % 60000) / 1000 << " segundos e " << (duration.count() % 60000) % 1000 << " milissegundos" << endl;

    return 0;
}