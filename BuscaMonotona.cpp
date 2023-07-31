#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>
#include <chrono>
#include <fstream>
using namespace std;

// Função para calcular o makespan
int calculateMakespan(const vector<int>& processingTimes, const vector<int>& assignment) {
    vector<int> machineTimes(processingTimes.size(), 0);
    for (int i = 0; i < assignment.size(); i++) {
        int machineIdx = assignment[i];
        machineTimes[machineIdx] += processingTimes[i];
    }
    return *max_element(machineTimes.begin(), machineTimes.end());
}

// Função de busca local monótona randomizada
vector<int> localSearchMonotonic(const vector<int>& processingTimes, int numMachines, int maxIterations, double alpha) {
    int numTasks = processingTimes.size();

    // Inicialização da solução inicial aleatória
    vector<int> assignment(numTasks);
    for (int i = 0; i < numTasks; i++) {
        assignment[i] = rand() % numMachines;
    }

    // Realiza a busca local monótona
    int iteration = 0;
    int noImprovementCount = 0; // Contador de iterações sem melhora
    while (iteration < maxIterations && noImprovementCount < 1000) {
        vector<int> bestAssignment = assignment;
        int bestMakespan = calculateMakespan(processingTimes, assignment);
        // Gera uma solução vizinha e a avalia
        for (int i = 0; i < numTasks; i++) {
            vector<int> neighborAssignment = assignment;
            neighborAssignment[i] = rand() % numMachines;
            int neighborMakespan = calculateMakespan(processingTimes, neighborAssignment);

            // Atualiza a melhor solução
            if (neighborMakespan < bestMakespan) {
                bestAssignment = neighborAssignment;
                bestMakespan = neighborMakespan;
            }
        }

        // Verifica se houve melhora
        if (bestAssignment == assignment) {
            noImprovementCount++; // Incrementa o contador de iterações sem melhora
        } else {
            assignment = bestAssignment;
            noImprovementCount = 0; // Reseta o contador de iterações sem melhora
        }

        // Realiza a caminhada aleatória com probabilidade alpha
        if (static_cast<double>(rand()) / RAND_MAX < alpha) {
            for (int i = 0; i < numTasks; i++) {
                assignment[i] = rand() % numMachines;
            }
        }

        iteration++;
    }

    return assignment;
}

int main() {
    srand(time(0));
    ofstream outputFile("output.txt");
    // Definição dos parâmetros
    vector<int> numMachines = {10, 20, 50};
    vector<double> rValues = {1.5, 2.0};
    vector<int> iteracoes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int maxIterations = 10000;
    int run;
    for (int m : numMachines) {
        auto start_time = chrono::high_resolution_clock::now();
        for (double r : rValues) {
            int numTasks = pow(m, r);
            vector<int> processingTimes(numTasks);
            for (int i = 0; i < numTasks; i++) {
                processingTimes[i] = rand() % 100 + 1; // Tempo entre 1 e 100
            }
            for (double alpha: {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9}){
                for(int iteracao : iteracoes){
                    double averageMakespan = 0;
                    vector<int> assignment = localSearchMonotonic(processingTimes, m, maxIterations, alpha);
                    int makespan = calculateMakespan(processingTimes, assignment);
                    averageMakespan += makespan;
                    auto end_time = chrono::high_resolution_clock::now();
                    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
                    outputFile << "Monotona;" << m << ";" << numTasks
                         << ";" << iteracao
                         << ";" << duration_ms.count() << "ms"
                         << ";" <<averageMakespan
                         << ";"<< alpha <<endl;
                    cout << "Monotona;" << m << ";" << numTasks
                         << ";" << iteracao
                         << ";" << duration_ms.count() << "ms"
                         << ";" <<averageMakespan
                         << ";"<<alpha <<endl;

                }
            }
        }
    }

    return 0;
}
