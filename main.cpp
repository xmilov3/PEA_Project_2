#include <iostream>
#include <fstream>


// README
// ============================================================================================================
// Ze względu na przygotowanie do pracy komercyjnej wszystkie zmienne zostały napisane w języku angielskim.
// Jedynie komentarze oraz UI/UX jest w języku polskim aby ułatwić pracę oceniającemu.
// Indeks studenta: 264164
// W celu monitorowania zmian projekt jest dostępny na platformie Github.
// Program został napisany w Środowisku CLion, na MacBook Pro M2.
// ============================================================================================================


std::string filename; // Nazwa pliku tekstowego
const int MAX_CITIES = 10;
int distances[MAX_CITIES][MAX_CITIES];
int numCities = 0;

void load_from_file(const std::string& fileName, int cityDistances[MAX_CITIES][MAX_CITIES], int& citiesCount) {
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cout << "Nie można otworzyć pliku: " << fileName << std::endl;
        return;
    }

    inputFile >> citiesCount;

    for (int i = 0; i < citiesCount; ++i) {
        for (int j = 0; j < citiesCount; ++j) {
            inputFile >> cityDistances[i][j];
        }
    }

    inputFile.close();
    std::cout << "Plik został pomyślnie wczytany.\n";
}

int solve_tsp(int cityDistances[MAX_CITIES][MAX_CITIES], int citiesCount) {
    int subset_size = 1 << citiesCount; // Liczba możliwych podzbiorów miast

    int dp[MAX_CITIES][1 << MAX_CITIES];

    for (int i = 0; i < citiesCount; ++i) {
        for (int j = 0; j < subset_size; ++j) {
            dp[i][j] = -1;
        }
        dp[i][1 << i] = cityDistances[0][i];
    }

    for (int mask = 1; mask < subset_size; ++mask) {
        for (int u = 0; u < citiesCount; ++u) {
            if ((mask & (1 << u)) != 0) {
                for (int v = 0; v < citiesCount; ++v) {
                    if ((mask & (1 << v)) != 0 && dp[v][mask ^ (1 << u)] != -1) {
                        if (dp[u][mask] == -1 || dp[u][mask] > dp[v][mask ^ (1 << u)] + cityDistances[v][u]) {
                            dp[u][mask] = dp[v][mask ^ (1 << u)] + cityDistances[v][u];
                        }
                    }
                }
            }
        }
    }

    int final_mask = (1 << citiesCount) - 1;
    int min_cost = -1;
    for (int u = 0; u < citiesCount; ++u) {
        if (min_cost == -1 || min_cost > dp[u][final_mask] + cityDistances[u][0]) {
            min_cost = dp[u][final_mask] + cityDistances[u][0];
        }
    }

    return min_cost;
}

int main() {
    int choice;

    while(true){
        std::cout << "==================================\n";
        std::cout << "               MENU               \n";
        std::cout << "==================================\n";
        std::cout << "1. Wczytaj dane z pliku\n";
        std::cout << "2. Rozwiąż problem komiwojażera\n";
        std::cout << "3. Zakończ program\n";
        std::cout << "==================================\n";
        std::cin >> choice;

        switch(choice){
            case 1:
                std::cout << "Wybrano wczytanie danych z pliku\n";
                std::cout << "Wpisz nazwę pliku (z rozszerzeniem .txt)\n";
                std::cin >> filename;
                load_from_file(filename, distances, numCities);
                break;

            case 2:
                std::cout << "Wybrano rozwiązywanie problemu komiwojażera\n";
                if (numCities == 0) {
                    std::cout << "Najpierw wczytaj dane z pliku.\n";
                } else {
                    int min_cost = solve_tsp(distances, numCities);
                    std::cout << "Najkrótsza trasa komiwojażera ma koszt: " << min_cost << std::endl;
                }
                break;

            case 3:
                return 0;

            default:
                std::cout << "Nieprawidłowy wybór. Spróbuj ponownie.\n";
                break;
        }
    }
}
