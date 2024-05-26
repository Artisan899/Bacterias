#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

void spawnBacteriaWithChance(int x, int y, int day, vector<vector<pair<int, int>>>& field, mt19937& rng, uniform_real_distribution<double>& distP, double P) {
    if (day != 1) {
        if (field[x][y].second <= day) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) {
                        continue;
                    }
                    int xi = x + i, yj = y + j;
                    if (xi >= 0 && xi < field.size() && yj >= 0 && yj < field[0].size() && field[xi][yj].first == 0 && distP(rng) < P) {
                        field[xi][yj].first = 1;
                        field[xi][yj].second = day + 1;
                        cout << "New Bacteria stay in position (" << xi << ", " << yj << ") by created bacteria (" << x << ", " << y << ")" << endl;
                    }
                }
            }
        }
    }
}

void spawnBacteria(int countBacter, int day, vector<vector<pair<int, int>>>& field, mt19937& rng, uniform_int_distribution<int>& distX, uniform_int_distribution<int>& distY) {
    for (int i = 0; i < countBacter; ++i) {
        int x, y;
        do {
            x = distX(rng);
            y = distY(rng);
        } while (field[x][y].first != 0);

        field[x][y].first = 1;
        field[x][y].second = day;
        cout << "Bacteria " << i + 1 << " created in position (" << x << ", " << y << ")" << endl;
    }
}

void killBacteriaWithChance(vector<vector<pair<int, int>>>& field, mt19937& rng, uniform_real_distribution<double>& distP, int day, double Pm, int T) {
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            if (field[i][j].first > 0) {
                int age = day - field[i][j].second;
                if (age >= (T / 2) && distP(rng) < Pm) {
                    field[i][j].first = 1;
                    cout << "Bacteria (" << i << ", " << j << ") died" << endl;
                }
            }
        }
    }
    cout << endl;
}

void liveOfBacterias(int N, int D, int K1, int K2, double P, double Pm, int T, vector<vector<pair<int, int>>>& field, mt19937& rng, uniform_int_distribution<int>& distX, uniform_int_distribution<int>& distY, uniform_real_distribution<double>& distP, vector<int>& daysToFill) {
    uniform_int_distribution<int> distcountBacter(K1, K2);
    int dayToFill = -1;

    for (int day = 1; day <= D; ++day) {
        int freeSpaces = N * N;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (field[i][j].first > 0) {
                    freeSpaces--;
                }
            }
        }

        if (freeSpaces == 0) {
            dayToFill = day; 
            cout << "No more free spaces for bacteria. Simulation ended" << endl;
            break;
        }

        int countBacter = min(distcountBacter(rng), freeSpaces);
        cout << endl << "Day " << day << ": " << countBacter << " bacteria" << endl;

        spawnBacteria(countBacter, day, field, rng, distX, distY);

        killBacteriaWithChance(field, rng, distP, day, Pm, T);

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (field[i][j].first > 0 && field[i][j].second == day) {
                    spawnBacteriaWithChance(i, j, day, field, rng, distP, P);
                }
            }
        }
    }

    if (dayToFill > 0) {
        daysToFill.push_back(dayToFill);
    }
}

void simulationType1(int N, int D, int K1, int K2, double P, double Pm, int T, int numSimulat, int& minDays, int& maxDays, double& noFillProbability, double& averageDays) {
    uniform_int_distribution<int> distX(0, N - 1);
    uniform_int_distribution<int> distY(0, N - 1);
    vector<vector<pair<int, int>>> field(N, vector<pair<int, int>>(N));
    mt19937 rng(time(nullptr));
    uniform_real_distribution<double> distP(0.0, 1.0);

    vector<int> daysToFill;

    for (int simul = 0; simul < numSimulat; ++simul) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                field[i][j].first = 0;
                field[i][j].second = 0;
            }
        }

        liveOfBacterias(N, D, K1, K2, P, Pm, T, field, rng, distX, distY, distP, daysToFill);
    }

    noFillProbability = static_cast<double>(numSimulat - daysToFill.size()) / numSimulat;

    minDays = D + 1;
    maxDays = 0;
    averageDays = 0.0;

    if (!daysToFill.empty()) {
        minDays = *min_element(daysToFill.begin(), daysToFill.end());
        maxDays = *max_element(daysToFill.begin(), daysToFill.end());
        averageDays = static_cast<double>(accumulate(daysToFill.begin(), daysToFill.end(), 0)) / daysToFill.size();
    }

    if (noFillProbability == 1) {
        minDays = 0;
    }
}

void specialSpawnType2(int N, int day, vector<vector<pair<int, int>>>& field, int K1) {
    
    vector<pair<int, int>> positions;

    int sqrtK1 = static_cast<int>(sqrt(K1)); 
    int count = 0;

    if (K1 > N * N)
    {
        K1 = N * N;
    }
   
    for (int i = 0; i < sqrtK1 && count < K1; ++i) {
        for (int j = 0; j < sqrtK1 && count < K1; ++j) {
            int x = N / 2 - sqrtK1 / 2 + i;
            int y = N / 2 - sqrtK1 / 2 + j;
            if (x >= 0 && x < N && y >= 0 && y < N)
            {
                positions.push_back({ x, y });
                count++;
            }
        }
    }

   
    while (count < K1) {
        int x = rand() % N;
        int y = rand() % N;
        if (field[x][y].first == 0)
        { 
            positions.push_back({ x, y });
            count++;
        }
    }

   
    for (const auto& pos : positions)
    {
        field[pos.first][pos.second].first = 1;
        field[pos.first][pos.second].second = day;
        cout << "Bacteria created in position (" << pos.first << ", " << pos.second << ")" << endl;
    }
}

void liveOfBacteriasType2(int N, int K1, vector<vector<pair<int, int>>>& field) {
    int day = 1;
    double P = 1.0;
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> distP(0.0, 1.0);

    cout << endl << "Day " << day << endl;
    specialSpawnType2(N, day, field, K1);

    while (true) {
        int freeSpaces = N * N;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (field[i][j].first > 0) {
                    freeSpaces--;
                }
            }
        }

        if (freeSpaces == 0) {
            cout << "No free spaces" << endl << endl;
            break;
        }

        if (day != 1) {
            cout << endl << "Day " << day << ": spreading bacteria" << endl;
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (field[i][j].first > 0) {
                    spawnBacteriaWithChance(i, j, day, field, rng, distP, P);
                }
            }
        }

        day++;
    }

    if (day == 1)
    {
        day ;
    }
    cout << "It took " << day - 1 << " days to fill the field" << endl << endl;
}

int main() {
    int minDays, maxDays;
    double noFillProbability, averageDays;
    int simulationType;
    int N;
    int K1, K2, numSimulat;

    while (true) {
        cout << "Select simulation type 1 or 2 (for exit write 0) ";
        cin >> simulationType;

        if (simulationType == 0) {
            break;
        }

        if (simulationType == 1) {
            cout << "Enter N (maximum 12113, but N < 100 recommended): ";
            cin >> N;
            while (N > 12113) {
                cout << "N must < 12113. Enter again: ";
                cin >> N;
            }
            cout << "Enter the K1: ";
            cin >> K1;
            cout << "Enter the K2: ";
            cin >> K2;
            cout << "Enter the number of simulations: ";
            cin >> numSimulat;

            int D = 7;
            double P = 0.05;
            double Pm = 0.02;
            int T = 14;

            simulationType1(N, D, K1, K2, P, Pm, T, numSimulat, minDays, maxDays, noFillProbability, averageDays);

            cout << "Minimum days -  " << minDays << endl;
            cout << "Average days -  " << averageDays << endl;
            cout << "Maximum days - " << maxDays << endl;
            cout << "Probability field have free spaces " << noFillProbability << endl << endl;
        }
        else if (simulationType == 2) {
            cout << "Enter N : ";
            cin >> N;
            while (N > 12113) {
                cout << "N must < 12113. Enter again: ";
                cin >> N;
            }
            cout << "Enter the K1: ";
            cin >> K1;

            int K2 = K1;
            int T = 12114;

            vector<vector<pair<int, int>>> field(N, vector<pair<int, int>>(N, { 0,0 }));

            liveOfBacteriasType2(N, K1, field);

        }
        else {
            cout << "Invalid type. Please enter 1 or 2, or 0 to exit." << endl;
        }
    }

    return 0;
}