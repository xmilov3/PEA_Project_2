#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <chrono>

// README
// ============================================================================================================
// Ze względu na przygotowanie do pracy komercyjnej wszystkie zmienne zostały napisane w języku angielskim.
// Jedynie komentarze oraz UI/UX jest w języku polskim aby ułatwić pracę oceniającemu.
// Indeks studenta: 264164
// W celu monitorowania zmian projekt jest dostępny na platformie Github.
// Program został napisany w Środowisku CLion, na MacBook Pro M2.
// ============================================================================================================

const int64_t INFINITY_COST = 1e18; // Stała definiująca nieskończoność
std::string filename; // Nazwa pliku tekstowego
const int MAX_CITIES = 15; // Maksymalna liczba miast
int num_of_cities = 0; // Zmienna definiująca liczbę miast

std::vector<std::vector<long long>> distances; // Wektor przechowujący odległości
std::vector<std::vector<int>> next_city; // Wektor przechowujący informacje o następnym mieście
std::vector<std::vector<long long>> memoization; // Wektor przechowujący wyniki algorytmu dynamicznego programowania

//Funckja wczytująca plik tekstowy
void load_from_file(const std::string& fileName, std::vector<std::vector<long long>>& cityDistances, int& cities_count) {
    auto begin = std::chrono::high_resolution_clock::now();
    std::ifstream inputFile(fileName);

    // Funkcja sprawdzająca czy plik został poprawnie otwarty
    if (!inputFile.is_open()) {
        std::cout << "Nie mozna otworzyc pliku: " << fileName << std::endl;
        return;
    }

    // Przypisanie liczby miast z pliku tekstowego do zmiennej cities_count
    inputFile >> cities_count;

    // Dostosowanie rozmiaru wektora do liczby miast
    cityDistances.resize(cities_count, std::vector<long long>(cities_count));

    for (int i = 0; i < cities_count; ++i) {
        for (int j = 0; j < cities_count; ++j) {
            inputFile >> cityDistances[i][j];
        }
    }

    inputFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "Plik zostal pomyslnie wczytany.\n";
    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}
// Pomocnicza funkcja korzystająca z programowania dynamicznego w algorytmie TSP
int64_t solve_tsp_dynamic(std::bitset<MAX_CITIES> set, int last) {
    auto begin = std::chrono::high_resolution_clock::now();

    // Sprawdzenie czy w tablicy memoization został obliczony wynik danego zbioru miast oraz ostatniego odiwedzonego miasta
    if (memoization[set.to_ulong()][last] != -1)
        return memoization[set.to_ulong()][last];

    // Sprawdzenie czy w zbiorze zostało jedno miasto i czy jest ostatnim
    if (set.count() == 1 && set.test(last)) {
        return last == 0 ? 0 : distances[0][last];
    }

    // Zainicjowanie zmiennej wartością nieskończoność
    int64_t min_cost = INFINITY_COST;
    // Usunięcie ostatniego miasta ze zbioru
    set.reset(last);

    // Pętla obliczająca najkrótszą trasę komiwojażera
    for (int city = 0; city < num_of_cities; city++) {
        if (city != last && set.test(city)) {
            // Wejście do rekurencji dla zaaktualizowanego zbioru i ostatniego odwiedzonego miasta
            int64_t cost = distances[city][last] + solve_tsp_dynamic(set, city);
            // Warunek sprawdzający czy koszt trasy jest mniejszy niż aktualna minimalna wartość
            if (cost < min_cost) {
                min_cost = cost;
                next_city[set.to_ulong()][last] = city;
            }
        }
    }

    // Zapisanie obliczonej wartości do tablicy memoization
    memoization[set.to_ulong()][last] = min_cost == INFINITY_COST ? -1 : min_cost;
    // Zwrócenie obliczonej wartości kosztu trasy dla zbioru oraz ostatniego miasta
    return memoization[set.to_ulong()][last];
}
    // Funkcja realizująca algorytm TSP
std::pair<long long, int> solve_tsp() {
    auto begin = std::chrono::high_resolution_clock::now();
    // Utworzenie bitsetu który reprezentuje pełny zbiór miast do odwiedzenia
    std::bitset<MAX_CITIES> final_set = (1 << num_of_cities) - 1;
    // Zainicjowanie tablicy memoization w celu przechowywania wyników pośrednich obliczneń dla różnych podzbiorów miast
    memoization.assign(1 << num_of_cities, std::vector<long long>(num_of_cities, -1));
    // Zainicjowanie tablicy next_city do przechowywania informacji o następnym odiwedzanym mieście w optymalnej trasie
    next_city.assign(1 << num_of_cities, std::vector<int>(num_of_cities, -1));

    // Zainicjowanie zmiennej wartością nieskończoność
    int64_t min_cost = INFINITY_COST;
    // Inicjalizacja zmiennej reprezentującej ostatnie odwiedzone miasto
    int last_city = -1;
    for (int city = 1; city < num_of_cities; city++) {
        // Obliczenie kosztu trasy z miasta początkowego do miasta city przy pomocy funkcji solve_tsp_dynamic
        int64_t cost = distances[0][city] + solve_tsp_dynamic(final_set, city);
        // Jeśli obliczony koszt trasy jest mniejszy niż aktualny to aktualizuje min_cost i zapisuje miasto jako odwiedzone
        if (cost < min_cost) {
            min_cost = cost;
            last_city = city;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
    // Zwrócenie minimalnego kosztu komiwojażera oraz ostatniego odwiedzonego miasta
    return {min_cost, last_city};
}

// Funkcja pomocnicza do wypisywania trasy
void print_path_recursive(int city, std::bitset<MAX_CITIES> set) {
    // Sprawdzenie czy city nie jest miastem początkowym
    if (city != 0) {
        // Rekurencyjne wywołanie funckji dla następnego miasta na trasie
        print_path_recursive(next_city[set.to_ulong()][city], set.reset(city));
        // Wypisanie aktualnego miasta
        std::cout << city << " ";
    }
}
// Funkcja wywołująca trasę miast, zaczynając i kończąć w mieście 0
void print_path(int last_city, std::bitset<MAX_CITIES> set) {
    std::cout << "Trasa: 0 ";
    print_path_recursive(last_city, set);
    std::cout << "0" << std::endl;
}
// Funkcja Main
int main() {
    // Zainicjowanie zmiennej choice
    int choice;

    // Menu
    while (true) {
        std::cout << "==================================\n";
        std::cout << "               MENU               \n";
        std::cout << "==================================\n";
        std::cout << "1. Wczytaj dane z pliku\n";
        std::cout << "2. Rozwiaz problem komiwojazera\n";
        std::cout << "3. Zakoncz program\n";
        std::cout << "==================================\n";
        std::cin >> choice;

        switch (choice) {
            // Opcja 1: wczytanie danych z pliku
            case 1:
                std::cout << "Wybrano wczytanie danych z pliku\n";
                std::cout << "Wpisz nazwe pliku (z rozszerzeniem .txt)\n";
                std::cin >> filename;
                load_from_file(filename, distances, num_of_cities);
                break;
            // Opcja 2: rozwiązanie problemu komiwojażera
            case 2:
                std::cout << "Wybrano rozwiazywanie problemu komiwojazera\n";
                // Sprawdzenie czy plik został wczytany do pamięci
                if (num_of_cities == 0) {
                    std::cout << "Najpierw wczytaj dane z pliku.\n";
                } else {
                    auto result = solve_tsp();
                    std::cout << "Najkrotsza trasa komiwojazera dla "<< num_of_cities << " miast ma koszt: " << result.first << std::endl;
                    print_path(result.second, (1 << num_of_cities) - 1);
                }
                break;
            // Opcja 3: wyjście z programu
            case 3:
                return 0;
            // Wiadomość jeżeli wybierze się inną opcję niż jest w menu
            default:
                std::cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
                break;
        }
    }
}
