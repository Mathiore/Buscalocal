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

// Função para gerar uma solução vizinha usando perturbação
vector<int> generateNeighborSolution(const vector<int>& assignment, int numMachines) {
    vector<int> neighborAssignment = assignment;
    int taskIdx = rand() % assignment.size();
    int machineIdx = rand() % numMachines;
    neighborAssignment[taskIdx] = machineIdx;
    return neighborAssignment;
}

// Função de busca local por têmpera simulada
vector<int> simulatedAnnealing(const vector<int>& processingTimes, int numMachines, int maxIterations, double initialTemperature, double coolingRate) {
    int numTasks = processingTimes.size();

    // Inicialização da solução inicial aleatória
    vector<int> currentAssignment(numTasks);
    for (int i = 0; i < numTasks; i++) {
        currentAssignment[i] = rand() % numMachines;
    }

    // Melhor solução encontrada até o momento
    vector<int> bestAssignment = currentAssignment;
    int bestMakespan = calculateMakespan(processingTimes, currentAssignment);

    // Realiza a busca por têmpera simulada
    double temperature = initialTemperature;
    int iteration = 0;
    while (iteration < maxIterations && temperature > 1e-6) {
        // Gera uma solução vizinha
        vector<int> neighborAssignment = generateNeighborSolution(currentAssignment, numMachines);
        int neighborMakespan = calculateMakespan(processingTimes, neighborAssignment);

        // Verifica se a solução vizinha é melhor ou se deve ser aceita com uma probabilidade determinada
        if (neighborMakespan < bestMakespan || (exp((bestMakespan - neighborMakespan) / temperature) > static_cast<double>(rand()) / RAND_MAX)) {
            currentAssignment = neighborAssignment;
            bestMakespan = neighborMakespan;
        }

        // Atualiza a melhor solução encontrada até o momento
        if (neighborMakespan < bestMakespan) {
            bestAssignment = neighborAssignment;
            bestMakespan = neighborMakespan;
        }

        // Resfria a temperatura
        temperature *= coolingRate;

        iteration++;
    }

    return bestAssignment;
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

            for (double alpha : {0.8, 0.85, 0.9, 0.95, 0.99}) {
                for (int iteracao : iteracoes) {
                    double averageMakespan = 0;
                    double initialTemperature = 1000; // Valor inicial para a temperatura
                    vector<int> assignment = simulatedAnnealing(processingTimes, m, maxIterations, initialTemperature, alpha);
                    int makespan = calculateMakespan(processingTimes, assignment);
                    averageMakespan += makespan;
                    auto end_time = chrono::high_resolution_clock::now();
                    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
                    outputFile << "TemperaSimulada;" << m << ";" << numTasks
                                << ";" << iteracao
                                << ";" << duration_ms.count() << "ms"
                                << ";" << averageMakespan
                                << ";" << alpha<<endl;
                }
            }
        }
    }

    return 0;
}
