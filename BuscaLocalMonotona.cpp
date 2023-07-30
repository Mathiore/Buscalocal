#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <limits>
#include <locale.h>

using namespace std;

class Cluster {
public:
    int makespan;
    vector<vector<int>> machines;

    Cluster(int makespan, const vector<vector<int>>& machines) : makespan(makespan), machines(machines) {}
};

int calculateMakespan(const vector<int>& tasks, const vector<vector<int>>& machines) {
    int numMachines = machines.size();
    vector<int> machineTimes(numMachines, 0);

    for (int task : tasks) {
        int machineIndex = min_element(machineTimes.begin(), machineTimes.end()) - machineTimes.begin();
        machineTimes[machineIndex] += task;
    }

    return *max_element(machineTimes.begin(), machineTimes.end());
}

Cluster vizinho(const Cluster& old_S) {
    Cluster S(old_S.makespan, old_S.machines);
    int val = S.makespan;
    int index = 0;

    for (size_t i = 0; i < S.machines.size(); ++i) {
        int new_sum = 0;
        for (int tk : S.machines[i]) {
            new_sum += tk;
        }
        if (val == new_sum) {
            index = i;
            break;
        }
    }

    int task = S.machines[index].back();
    S.machines[index].pop_back();

    bool found = false;
    for (size_t i = 0; i < S.machines.size(); ++i) {
        int new_sum = 0;
        for (int tk : S.machines[i]) {
            new_sum += tk;
        }
        new_sum += task;
        if (new_sum < val) {
            found = true;
            S.machines[i].push_back(task);
            break;
        }
    }

    if (!found) {
        S.machines[index].push_back(task);
    } else {
        int new_make = 0;
        for (const auto& mach : S.machines) {
            int new_sum = 0;
            for (int tk : mach) {
                new_sum += tk;
            }
            if (new_make < new_sum) {
                new_make = new_sum;
            }
        }
        S.makespan = new_make;
    }
    return S;
}

Cluster localSearchMonotona(const vector<int>& tasks, const vector<int>& initialSolution, int maxIterations) {
    Cluster currentSolution(calculateMakespan(tasks, vector<vector<int>>(1, initialSolution)), vector<vector<int>>(1, initialSolution));


    int numTasks = tasks.size();
    int iterations = 0;

    while (iterations < maxIterations) {
        bool improved = false;

        for (int i = 0; i < numTasks; ++i) {
            for (int j = i + 1; j < numTasks; ++j) {
                vector<vector<int>> neighborSolution = currentSolution.machines;
                swap(neighborSolution[0][i], neighborSolution[0][j]);
                int neighborMakespan = calculateMakespan(tasks, neighborSolution);

                if (neighborMakespan < currentSolution.makespan) {
                    currentSolution.machines = neighborSolution;
                    currentSolution.makespan = neighborMakespan;
                    improved = true;
                }
            }
        }

        if (!improved) {
            break;
        }

        iterations++;
    }

    return currentSolution;
}

int main() {
    setlocale(LC_ALL, "");
    int num_machines = 20;
    int num_tasks = 400;

    vector<vector<int>> machines(num_machines, vector<int>(num_tasks));
    vector<int> tasks(num_tasks);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < num_tasks; ++i) {
        tasks[i] = dist(gen);
    }

    vector<int> initialSolution(num_tasks);
    iota(initialSolution.begin(), initialSolution.end(), 0);

    for(int i = 1; i<=10; i++){
    auto startTime = chrono::high_resolution_clock::now();
    Cluster optimizedSolution = localSearchMonotona(tasks, initialSolution, 10000);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    cout <<"Monotona;"<<num_tasks<<";"<<num_machines<<";"<<i<<";"<<duration.count()<<"ms;"<<optimizedSolution.makespan<<";"<<endl;
    }

    return 0;
}
