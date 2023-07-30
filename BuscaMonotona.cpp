#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>
#include <chrono>

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
vector<int> localSearchMonotonic(const vector<int>& processingTimes, int numMachines, int maxIterations) {
    int numTasks = processingTimes.size();

    // Inicialização da solução inicial aleatória
    vector<int> assignment(numTasks);
    for (int i = 0; i < numTasks; i++) {
        assignment[i] = rand() % numMachines;
    }

    // Realiza a busca local monótona randomizada
    int iteration = 0;
    while (iteration < maxIterations) {
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
            break;
        }

        assignment = bestAssignment;
        iteration++;
    }

    return assignment;
}

int main() {
    srand(time(0));

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

            for(int iteracao : iteracoes){
                double averageMakespan = 0;
                vector<int> assignment = localSearchMonotonic(processingTimes, m, maxIterations);
                int makespan = calculateMakespan(processingTimes, assignment);
                averageMakespan += makespan;
                auto end_time = chrono::high_resolution_clock::now();
                auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
                averageMakespan /= 10.0;
                cout << " Monotona - Maquinas: " << m << ", Tarefas: " << numTasks
                     << ", Makespan: " << averageMakespan
                     << ", iteracao: " << iteracao
                     << ", Tempo: " << duration_ms.count() << " ms "<<endl;

            }
        }
    }

    return 0;
}
