#include "lac.hpp"

#include <unordered_set>

#define MIN_SUPORTE 0

Lac::Lac(unordered_map<pair<int, int>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes)
{
    // Lac::Lac(unordered_map<pair<int, float>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes) {
    this->features = features;
    this->classes = classes;
}

void Lac::readFile(string path)
{
}

void Lac::training(string path)
{
    ifstream file(path);
    if (!file.is_open())
    {
        return;
    }

    string line;
    int j = 1;

    while (getline(file, line))
    {
        // vector<float> values = splitString(line);
        vector<int> values = splitString(line);

        int i = 0;
        for (auto v : values)
        {
            // pair<int, float> feature(++i, v);
            pair<int, int> feature(++i, v);

            if (features.find(feature) != features.end())
            {
                features[feature].push_back(j);
            }
            else
            {
                vector<int> lines;
                lines.push_back(j);
                // features.insert(pair<pair<int, float>, vector<int>>(feature, lines));
                features.insert(pair<pair<int, int>, vector<int>>(feature, lines));
            }
            if (i == values.size() - 1)
                break;
        }

        if (classes.find(values.back()) != classes.end())
        {
            classes[values.back()].push_back(j);
        }
        else
        {
            vector<int> lines;
            lines.push_back(j);
            classes.insert(pair<int, vector<int>>(values.back(), lines));
        }

        j++;
    }

    file.close();
}

void Lac::testing(string path)
{
    cout << "Tamanho do features: " << features.size() << endl;
    ifstream file(path);
    if (!file.is_open())
    {
        return;
    }

    ofstream outFile("datasets/output.txt");
    if (!outFile.is_open())
    {
        return;
    }

    string line;
    int j = 1, erros = 0, acertos = 0, intersectionCounter = 0, cacheUsingCounter = 0;
    vector<float> totalResult(10, 0.0f);

    while (getline(file, line))
    {
        double result[classes.size()] = {0};

        for (int i = 0; i < classes.size(); i++)
        {
            result[i] = 0;
        }

        // vector<float> values = splitString(line);
        vector<int> values = splitString(line);
        vector<pair<int, int>> lineFeatures;
        // vector<pair<int, float>> lineFeatures;

        int i = 0;
        for (auto v : values)
        {
            pair<int, float> feature(++i, v);

            if (features.find(feature) != features.end())
                lineFeatures.push_back(feature);

            if (i == values.size() - 1)
                break;
        }
        // cout << "Tamanho do features: " << lineFeatures.size() << endl;

        int n = lineFeatures.size();

        // guardando as linhas referentes as tuplas da linha
        vector<vector<int>> linesPerTupla = {};
        for (int p = 0; p < n; p++)
        {
            // pair<int, float> key = pair(lineFeatures[p].first, lineFeatures[p].second);
            pair<int, int> key = pair(lineFeatures[p].first, lineFeatures[p].second);
            linesPerTupla.push_back(features[key]);
        }

        // Acessando as linhas de cada tupla: 1 a 1, 2 a 2 e assim
        // por diante ate o tamanho maximo da tupla, 11
        for (int q = 1; q <= n; q++)
        {
            for (int r = 0; r < n; r += q)
            {
                // vector para guardar as combinacoes 1 a 1, 2 a 2.....
                vector<vector<int>> combinacoes = {};
                vector<pair<int, int>> combinacoesFeatures = {};
                int end = min(r + q, n);

                for (int k = r; k < end; ++k)
                {
                    combinacoesFeatures.push_back(lineFeatures[k]);
                    combinacoes.push_back(linesPerTupla[k]);
                }

                // fazendo a interseção das combinacoes 1 a 1, 2 a 2.....

                vector<int> intersectionPerTupla = {};

                if (combinacoes.size() == 1)
                {
                    intersectionPerTupla = combinacoes[0];
                }
                else if (cache.find(combinacoesFeatures) != cache.end())
                {
                    intersectionPerTupla = cache[combinacoesFeatures];
                    cacheUsingCounter++;
                }
                else if(combinacoes.size()<4)
                {
                    {
                        intersectionPerTupla = intersectionAll(combinacoes);
                        cache[combinacoesFeatures] = intersectionPerTupla;
                        intersectionCounter++;
                    }
                }

                // cout << "Intersec: " << intersectionCounter << endl;
                // cout << "Cache: " << cacheUsingCounter << endl;
                for (auto c : classes)
                {
                    vector<int> intersecao;

                    set_intersection(intersectionPerTupla.begin(), intersectionPerTupla.end(), c.second.begin(), c.second.end(), back_inserter(intersecao));
                    int confident = intersecao.size();

                    if (confident > MIN_SUPORTE)
                    {
                        double support = (double)confident / (double)features.size();
                        result[c.first] += support;
                    }
                }
            }
        }

        int classification = findMaxIndex(result, classes.size());

        // cout << "Classificação: " << classification << " Valor: " << result[classification] << " Correta: " << values.back() << endl;

        // outFile << (j - 1) << "," << classification << endl;

        cout << j << endl;
        totalResult[classification] += 1.0f;

        if (classification == values.back())
            acertos++;
        else
            erros++;

        j++;
    }

    cout << "Acertos: " << acertos << " Erros: " << erros << endl;

    // Porcentagem de acertos
    cout << "Porcentagem de acertos: " << (acertos * 100) / (acertos + erros) << "%" << endl;

    outFile << "Acertos(loss): " << acertos << " e Erros(accuracy): " << erros;

    file.close();
}

// vector<float> Lac::splitString(string line) {
vector<int> Lac::splitString(string line)
{
    // vector<float> result;
    vector<int> result;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = line.find(delimiter)) != string::npos)
    {
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

// unordered_map<pair<int, float>, vector<int>, pairHash> Lac::getFeatures() {
unordered_map<pair<int, int>, vector<int>, pairHash> Lac::getFeatures()
{
    return features;
}

unordered_map<int, vector<int>> Lac::getClasses()
{
    return classes;
}

// interseção entre uma lista de vector
vector<int> Lac::intersectionAll(vector<std::vector<int>> lists)
{
    unordered_map<int, int> freqMap;
    int numLists = lists.size();

    // Contabiliza a frequência de cada elemento em todas as sublistas
    for (const auto &list : lists)
    {
        unordered_map<int, bool> visited;
        for (auto num : list)
        {
            // Evita contar o mesmo número mais de uma vez por sublista
            if (!visited[num])
            {
                freqMap[num]++;
                visited[num] = true;
            }
        }
    }

    vector<int> result;
    // Adiciona ao resultado apenas os elementos que aparecem em todas as sublistas
    for (const auto &pair : freqMap)
    {
        if (pair.second == numLists)
        {
            result.push_back(pair.first);
        }
    }

    return result;
}

// interseção entre dois vector
int Lac::intersection(vector<int> first, vector<int> second)
{
    std::vector<int> intersection;

    // Armazena os elementos do primeiro vetor em um unordered_set
    std::unordered_set<int> set1(first.begin(), first.end());

    // Verifica quais elementos do segundo vetor estão presentes no set1
    for (int num : second)
    {
        if (set1.find(num) != set1.end())
        {
            intersection.push_back(num);
        }
    }

    return intersection.size();
}

// Acha o indice do maior valor dentro do vetor resultado
int Lac::findMaxIndex(double *arr, int size)
{
    int maxIndex = 0;
    double maxValue = arr[0];

    for (int i = 1; i < size; ++i)
    {
        if (arr[i] > maxValue)
        {
            maxValue = arr[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

void Lac::imprimirFeatures()
{
    for (auto v : features)
    {
        cout << "Feature: " << std::fixed << std::setprecision(2) << v.first.second << " " << std::setprecision(2) << v.first.first << " >>>>> ";
        for (auto i : v.second)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}

void Lac::imprimirClasses()
{
    for (auto v : classes)
    {
        cout << "Classe: " << v.first << " >>>>> ";
        for (auto i : v.second)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}