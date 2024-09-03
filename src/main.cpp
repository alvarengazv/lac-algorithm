#include "lac.hpp"

int main() {
    string trainingFile = "datasets/base-avaliacao/poker-hand-training.data";
    string testingFile = "datasets/base-avaliacao/poker-hand-testing.data";

    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;

    bool decreaseCardinality = false;

    cout << "Deseja reduzir a cardinalidade? (1 - Sim, 0 - Não)" << endl;
    cin >> decreaseCardinality;

    Lac lac = Lac(features, classes, decreaseCardinality);

    // treino
    lac.training(trainingFile);

    auto start = chrono::high_resolution_clock::now();

    // teste
    float acuracia = lac.testing(testingFile);

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