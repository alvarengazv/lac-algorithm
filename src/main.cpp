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

    return 0;
}