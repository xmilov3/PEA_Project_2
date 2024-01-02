#include <iostream>



// README
// ============================================================================================================
// Ze względu na przygotowanie do pracy komercyjnej wszystkie zmienne zostały napisane w języku angielskim.
// Jedynie komentarze oraz UI/UX jest w języku polskim aby ułatwić pracę oceniającemu.
// Indeks studenta: 264164
// W celu monitorowania zmian projekt jest dostępny na platformie Github.
// Program został napisany w Środowisku CLion, na MacBook Pro M2.
// ============================================================================================================

std::string filename; // Nazwa pliku tekstowego

int main() {
    int choice;

    while(true){
        std::cout<<"==================================\n";
        std::cout<<"               MENU               \n";
        std::cout<<"==================================\n";
        std::cout<<"1. Wczytaj dane z pliku\n";
        std::cout<<"2. Rozwiąż problem komiwojażera\n";
        std::cout<<"3. Zakończ program\n";
        std::cout<<"==================================\n";
        std::cin>>choice;

        switch(choice){
            case 1:
                std::cout<<"Wybrano wczytanie danych z pliku\n";
                std::cout<<"Wpisz nazwę pliku (z rozszerzeniem .txt)\n";
                std::cin>>filename;
                break;

            case 2:
                std::cout<<"Wybrano rozwiązywanie problemu komiwojażera\n";

                break;

            case 3:
                return 0;
        }
    }
}
