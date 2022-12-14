#include <iostream>
#include <pthread.h>
#include <chrono>
#include <semaphore>
#include <vector>
#include <algorithm>
#include <random>
#include <unistd.h>
#include <cstdlib>
#include <fstream>

std::binary_semaphore anchuaryAttacksTarantery(0), taranteryAttacksAnchuary(0);
std::vector<std::vector<int>> taranteryField;
std::vector<std::vector<int>> anchuaryField;
int anchuaryValueTotal = 0;
int taranteryValueTotal = 0;
const int AMMO_VALUE_RESERVE = 1;
const int SIZE_RESERVE = 8;
const int PROBABILITY_DIVISOR_RESERVE = 4;
const int MIN_VALUE_RESERVE = 1;
const int MAX_VALUE_RESERVE = 10;
int AMMO_VALUE = AMMO_VALUE_RESERVE;
int SIZE = SIZE_RESERVE;
int PROBABILITY_DIVISOR = PROBABILITY_DIVISOR_RESERVE;
int MIN_VALUE = MIN_VALUE_RESERVE;
int MAX_VALUE = MAX_VALUE_RESERVE;
// 0 - nothing
// n - valuable
// -10 - nothing shot
// -n - valuable shot
std::ofstream fout("output.txt");
std::ifstream fin("input.txt");
bool FILE_USAGE = false;

void generateFields() {
    for (int i = 0; i < SIZE; i++) {
        anchuaryField.emplace_back();
        taranteryField.emplace_back();
        for (int j = 0; j < SIZE; j++) {
            int anchuaryRandom = std::rand() % PROBABILITY_DIVISOR;
            int taranteryRandom = std::rand() % PROBABILITY_DIVISOR;
            taranteryField[i].push_back(taranteryRandom == 0 ? (rand() % (MAX_VALUE - MIN_VALUE) + MIN_VALUE) : 0);
            anchuaryField[i].push_back(anchuaryRandom == 0 ? (rand() % (MAX_VALUE - MIN_VALUE) + MIN_VALUE) : 0);
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            anchuaryValueTotal += anchuaryField[i][j];
            taranteryValueTotal += taranteryField[i][j];
        }
    }
}

void printFields() {
    int taranteryShots = 0;
    int taranteryShotsValuable = 0;
    int taranteryShotsTotal = 0;
    int anchuaryShots = 0;
    int anchuaryShotsValuable = 0;
    int anchuaryShotsTotal = 0;
    if (!FILE_USAGE) {
        std::cout << "Tarantery Field";
        for (int i = 0; i < taranteryField.size() * 6 - 21; i++) {
            std::cout << " ";
        }
        std::cout << "Anchuary Field" << std::endl;
        for (int i = 0; i < taranteryField.size(); i++) {
            std::cout << "---";
        }
        std::cout << "--    --";
        for (int i = 0; i < taranteryField.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;
        for (int i = 0; i < taranteryField.size(); i++) {
            std::cout << "|";
            for (int j: taranteryField[i]) {
                std::string cell = "   ";
                if (j == 0)
                    std::cout << "   ";
                else if (j == -10)
                    std::cout << " x ";
                else if (j < 0)
                    std::cout << " * ";
                else
                    std::cout << " " << j << " ";
                if (j < 0) {
                    anchuaryShots += 1;
                    anchuaryShotsTotal += j == -10 ? 0 : j;
                    anchuaryShotsValuable += j == -10 ? 0 : 1;
                }
            }
            std::cout << "|    |";
            for (int j: anchuaryField[i]) {
                std::string cell = "   ";
                if (j == 0)
                    std::cout << "   ";
                else if (j == -10)
                    std::cout << " x ";
                else if (j < 0)
                    std::cout << " * ";
                else
                    std::cout << " " << j << " ";
                if (j < 0) {
                    taranteryShots += 1;
                    taranteryShotsTotal += j == -10 ? 0 : j;
                    taranteryShotsValuable += j == -10 ? 0 : 1;
                }
            }
            std::cout << "|" << std::endl;
        }
        for (int i = 0; i < taranteryField.size(); i++) {
            std::cout << "---";
        }
        std::cout << "--    --";
        for (int i = 0; i < taranteryField.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;
        std::cout << "Anchuary Stats: " << std::endl;
        std::cout << "> Total Value: " << anchuaryValueTotal << std::endl;
        std::cout << "> Shots: " << anchuaryShots << std::endl;
        std::cout << "> Valuable Shots: " << anchuaryShotsValuable << std::endl;
        std::cout << "> Total Shots Value: " << -anchuaryShotsTotal << std::endl;
        std::cout << "> Total Spent on Shots: " << anchuaryShots * AMMO_VALUE << std::endl;
        std::cout << "Tarantery Stats: " << std::endl;
        std::cout << "> Total Value: " << taranteryValueTotal << std::endl;
        std::cout << "> Shots: " << taranteryShots << std::endl;
        std::cout << "> Valuable Shots: " << taranteryShotsValuable << std::endl;
        std::cout << "> Total Shots Value: " << -taranteryShotsTotal << std::endl;
        std::cout << "> Total Spent on Shots: " << taranteryShots * AMMO_VALUE << std::endl;
        return;
    }
    fout << "Tarantery Field";
    for (int i = 0; i < taranteryField.size() * 6 - 21; i++) {
        fout << " ";
    }
    fout << "Anchuary Field" << std::endl;
    for (int i = 0; i < taranteryField.size(); i++) {
        fout << "---";
    }
    fout << "--    --";
    for (int i = 0; i < taranteryField.size(); i++) {
        fout << "---";
    }
    fout << std::endl;
    for (int i = 0; i < taranteryField.size(); i++) {
        fout << "|";
        for (int j: taranteryField[i]) {
            std::string cell = "   ";
            if (j == 0)
                fout << "   ";
            else if (j == -10)
                fout << " x ";
            else if (j < 0)
                fout << " * ";
            else
                fout << " " << j << " ";
            if (j < 0) {
                anchuaryShots += 1;
                anchuaryShotsTotal += j == -10 ? 0 : j;
                anchuaryShotsValuable += j == -10 ? 0 : 1;
            }
        }
        fout << "|    |";
        for (int j: anchuaryField[i]) {
            std::string cell = "   ";
            if (j == 0)
                fout << "   ";
            else if (j == -10)
                fout << " x ";
            else if (j < 0)
                fout << " * ";
            else
                fout << " " << j << " ";
            if (j < 0) {
                taranteryShots += 1;
                taranteryShotsTotal += j == -10 ? 0 : j;
                taranteryShotsValuable += j == -10 ? 0 : 1;
            }
        }
        fout << "|" << std::endl;
    }
    for (int i = 0; i < taranteryField.size(); i++) {
        fout << "---";
    }
    fout << "--    --";
    for (int i = 0; i < taranteryField.size(); i++) {
        fout << "---";
    }
    fout << std::endl;
    fout << "Anchuary Stats: " << std::endl;
    fout << "> Total Value: " << anchuaryValueTotal << std::endl;
    fout << "> Shots: " << anchuaryShots << std::endl;
    fout << "> Valuable Shots: " << anchuaryShotsValuable << std::endl;
    fout << "> Total Shots Value: " << -anchuaryShotsTotal << std::endl;
    fout << "> Total Spent on Shots: " << anchuaryShots * AMMO_VALUE << std::endl;
    fout << "Tarantery Stats: " << std::endl;
    fout << "> Total Value: " << taranteryValueTotal << std::endl;
    fout << "> Shots: " << taranteryShots << std::endl;
    fout << "> Valuable Shots: " << taranteryShotsValuable << std::endl;
    fout << "> Total Shots Value: " << -taranteryShotsTotal << std::endl;
    fout << "> Total Spent on Shots: " << taranteryShots * AMMO_VALUE << std::endl;
}

void anchuaryShot() {
    bool choice = true;
    int x = 0;
    int y = 0;
    while (choice) {
        x = rand() % SIZE;
        y = rand() % SIZE;
        if (taranteryField[y][x] >= 0)
            choice = false;
    }
    if (FILE_USAGE)
        fout << "[Anchuary] Shot Cell (" << x << "," << y << "). Value: " << taranteryField[y][x] << std::endl;
    else
        std::cout << "[Anchuary] Shot Cell (" << x << "," << y << "). Value: " << taranteryField[y][x] << std::endl;
    taranteryField[y][x] = -taranteryField[y][x];
    if (taranteryField[y][x] == 0)
        taranteryField[y][x] = -10;
}

void taranteryShot() {
    bool choice = true;
    int x = 0;
    int y = 0;
    while (choice) {
        x = rand() % SIZE;
        y = rand() % SIZE;
        if (anchuaryField[y][x] >= 0)
            choice = false;
    }
    if (FILE_USAGE)
        fout << "[Tarantery] Shot Cell (" << x << "," << y << "). Value: " << anchuaryField[y][x] << std::endl;
    else
        std::cout << "[Tarantery] Shot Cell (" << x << "," << y << "). Value: " << anchuaryField[y][x] << std::endl;
    anchuaryField[y][x] = -anchuaryField[y][x];
    if (anchuaryField[y][x] == 0)
        anchuaryField[y][x] = -10;
}

bool anchuaryCheck() {
    int valuableCellsLeft = 0;
    int shotCells = 0;
    for (int i = 0; i < taranteryField.size(); i++) {
        for (int j = 0; j < taranteryField[j].size(); j++) {
            if (taranteryField[i][j] < 0) {
                shotCells += 1;
            } else if (taranteryField[i][j] > 0) {
                valuableCellsLeft += 1;
            }
        }
    }
    if (valuableCellsLeft == 0) {
        return false;
    }
    if (shotCells * AMMO_VALUE > taranteryValueTotal) {
        return false;
    }
    return true;
}

bool taranteryCheck() {
    int valuableCellsLeft = 0;
    int shotCells = 0;
    for (int i = 0; i < anchuaryField.size(); i++) {
        for (int j = 0; j < anchuaryField[j].size(); j++) {
            if (anchuaryField[i][j] < 0) {
                shotCells += 1;
            } else if (anchuaryField[i][j] > 0) {
                valuableCellsLeft += 1;
            }
        }
    }
    if (valuableCellsLeft == 0) {
        return false;
    }
    if (shotCells * AMMO_VALUE > anchuaryValueTotal) {
        return false;
    }
    return true;
}

void *anchuaryAttack(void *args) {
    if (FILE_USAGE) {
        bool run = true;
        while (run) {
            anchuaryAttacksTarantery.acquire();
            if (!taranteryCheck())
                break;
            fout << "[Anchuary] Attacking the Tarantery\n";
            fout << "[Anchuary] Chooses Cell\n";
            anchuaryShot();
            usleep(1e6);
            fout << "[Anchuary] Attack ended.\n";
            run = anchuaryCheck();
            if (!run) {
                fout << "[Stats] Anchuary defeats Tarantery!\n";
            }
            printFields();
            taranteryAttacksAnchuary.release();
        }
    } else {
        bool run = true;
        while (run) {
            anchuaryAttacksTarantery.acquire();
            if (!taranteryCheck())
                break;
            std::cout << "[Anchuary] Attacking the Tarantery\n";
            std::cout << "[Anchuary] Chooses Cell\n";
            anchuaryShot();
            usleep(1e6);
            std::cout << "[Anchuary] Attack ended.\n";
            run = anchuaryCheck();
            if (!run) {
                std::cout << "[Stats] Anchuary defeats Tarantery!\n";
            }
            printFields();
            taranteryAttacksAnchuary.release();
        }
    }
}

[[noreturn]] void *taranteryAttack(void *args) {
    if (FILE_USAGE) {
        bool run = true;
        while (run) {
            taranteryAttacksAnchuary.acquire();
            if (!anchuaryCheck())
                break;
            fout << "[Tarantery] Attacking the Anchuary\n";
            fout << "[Tarantery] Chooses Cell\n";
            taranteryShot();
            usleep(1e6);
            fout << "[Tarantery] Attack ended.\n";
            run = taranteryCheck();
            if (!run) {
                fout << "[Stats] Tarantery defeats Anchuary!\n";
            }
            printFields();
            anchuaryAttacksTarantery.release();
        }
    } else {
        bool run = true;
        while (run) {
            taranteryAttacksAnchuary.acquire();
            if (!anchuaryCheck())
                break;
            std::cout << "[Tarantery] Attacking the Anchuary\n";
            std::cout << "[Tarantery] Chooses Cell\n";
            taranteryShot();
            usleep(1e6);
            std::cout << "[Tarantery] Attack ended.\n";
            run = taranteryCheck();
            if (!run) {
                std::cout << "[Stats] Tarantery defeats Anchuary!\n";
            }
            printFields();
            anchuaryAttacksTarantery.release();
        }
    }
}

void resetSettings() {
    AMMO_VALUE = AMMO_VALUE_RESERVE;
    SIZE = SIZE_RESERVE;
    PROBABILITY_DIVISOR = PROBABILITY_DIVISOR_RESERVE;
    MIN_VALUE = MIN_VALUE_RESERVE;
    MAX_VALUE = MAX_VALUE_RESERVE;
}

bool getSettings() {
    int dataSource = 0;
    // 0 - defaults
    // 1 - console
    // 2 - file
    // 3 - random
    // 4 - exit
    while (true) {
        std::cout << "> 0. Defaults" << std::endl;
        std::cout << "> 1. Console" << std::endl;
        std::cout << "> 2. File" << std::endl;
        std::cout << "> 3. Random" << std::endl;
        std::cout << "> 4. Exit" << std::endl;
        std::cout << "> ";
        std::cin >> dataSource;
        if (dataSource < 0 || dataSource > 4) {
            std::cout << "You have chosen the wrong option." << std::endl;
            continue;
        }
        break;
    }
    if (dataSource == 0) {
        std::cout << "You have chosen to use defauls" << std::endl;
        return true;
    }
    if (dataSource == 4) {
        std::cout << "You have chosen to exit app." << std::endl;
        std::cout << "Bye!";
        return false;
    }
    if (dataSource == 1) {
        std::cout << "You have chosen to use console." << std::endl;
        while (true) {
            std::cout << "Field Size (5 to 25): ";
            std::cin >> SIZE;
            if (SIZE < 5 || SIZE > 25) {
                std::cout << "Error Field Size!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            std::cout << "Ammo Price (1 to 9): ";
            std::cin >> AMMO_VALUE;
            if (AMMO_VALUE < 1 || AMMO_VALUE > 9) {
                std::cout << "Error Ammo Price!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            std::cout << "Min Cell Value (1 to 9): ";
            std::cin >> MIN_VALUE;
            if (MIN_VALUE < 1 || MIN_VALUE > 9) {
                std::cout << "Error Min Cell Value!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            std::cout << "Max Cell Value (1 to 9): ";
            std::cin >> MAX_VALUE;
            if (MAX_VALUE < 1 || MAX_VALUE > 9 || MAX_VALUE < MIN_VALUE) {
                std::cout << "Error Max Cell Value!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            std::cout
                    << "Probability Divisor of valuable cell (if u want 1 valuable cell out of 4, type in 4) (1 to 10): ";
            std::cin >> PROBABILITY_DIVISOR;
            if (PROBABILITY_DIVISOR < 1 || PROBABILITY_DIVISOR > 10) {
                std::cout << "Error Probability Divisor!" << std::endl;
                continue;
            }
            break;
        }
        std::cout << std::endl;
    }
    if (dataSource == 2) {
        fout << "You have chosen to use file." << std::endl;
        FILE_USAGE = true;
        while (true) {
            fout << "Field Size (5 to 25): ";
            fin >> SIZE;
            if (SIZE < 5 || SIZE > 25) {
                fout << "Error Field Size!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            fout << "Ammo Price (1 to 9): ";
            fin >> AMMO_VALUE;
            fout << AMMO_VALUE;
            if (AMMO_VALUE < 1 || AMMO_VALUE > 9) {
                fout << "Error Ammo Price Value!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            fout << "Min Cell Value (1 to 9): ";
            fin >> MIN_VALUE;
            if (MIN_VALUE < 1 || MIN_VALUE > 9) {
                fout << "Error Min Cell Value!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            fout << "Max Cell Value (1 to 9): ";
            fin >> MAX_VALUE;
            if (MAX_VALUE < 1 || MAX_VALUE > 9 || MAX_VALUE < MIN_VALUE) {
                fout << "Error Max Cell Value!" << std::endl;
                continue;
            }
            break;
        }
        while (true) {
            fout
                    << "Probability Divisor of valuable cell (if u want 1 valuable cell out of 4, type in 4) (1 to 10): ";
            fin >> PROBABILITY_DIVISOR;
            if (PROBABILITY_DIVISOR < 1 || PROBABILITY_DIVISOR > 10) {
                fout << "Error Probability Divisor!" << std::endl;
                continue;
            }
            break;
        }
        fout << std::endl;
    }
    if (dataSource == 3) {
        std::cout << "You have chosen to use random." << std::endl;
        AMMO_VALUE = rand() % 9 + 1;
        MIN_VALUE = rand() % 9 + 1;
        MAX_VALUE = rand() % 9 + 1;
        while (MAX_VALUE < MIN_VALUE) {
            MAX_VALUE = rand() % 9 + 1;
        }
        SIZE = rand() % 21 + 5;
        PROBABILITY_DIVISOR = rand() % 10 + 1;
    }
    return true;
}

bool getSettings(int argc, char *argv[]) {
    if (argc != 1) {
        if (argc != 6) {
            std::cout << "Failed to read command line arguments" << std::endl;
            std::cout << "Expected 5 arguments. Got: " << argc << std::endl;
            std::cout << "Format: SIZE AMMO_VALUE MIN_VALUE MAX_VALUE PROBABILITY_DIVISOR" << std::endl;
            std::cout << "SIZE - field size (default: " << SIZE_RESERVE << ")" << std::endl;
            std::cout << "AMMO_VALUE - ammo price (default: " << AMMO_VALUE_RESERVE << ")" << std::endl;
            std::cout << "MIN_VALUE - min cell value (default: " << MIN_VALUE_RESERVE << ")" << std::endl;
            std::cout << "MAX_VALUE - max cell value (default: " << MAX_VALUE_RESERVE << ")" << std::endl;
            std::cout << "PROBABILITY_DIVISOR - probability of valuable field (default: " << PROBABILITY_DIVISOR_RESERVE
                      << ")" << std::endl;
            return true;
        }
        SIZE = atoi(argv[1]);
        AMMO_VALUE = atoi(argv[2]);
        MIN_VALUE = atoi(argv[3]);
        MAX_VALUE = atoi(argv[4]);
        PROBABILITY_DIVISOR = atoi(argv[5]);
        return true;
    }
    return getSettings();
}

void printSettings() {
    if (FILE_USAGE) {
        fout << "Field Sizes: " << SIZE << "x" << SIZE << std::endl;
        fout << "Ammo Price: " << AMMO_VALUE << std::endl;
        fout << "Min Cell Value: " << MIN_VALUE << std::endl;
        fout << "Max Cell Value: " << MAX_VALUE << std::endl;
        fout << "Probability of valuable cell: " << 1.0 / PROBABILITY_DIVISOR << std::endl;
    }
    std::cout << "Field Sizes: " << SIZE << "x" << SIZE << std::endl;
    std::cout << "Ammo Price: " << AMMO_VALUE << std::endl;
    std::cout << "Min Cell Value: " << MIN_VALUE << std::endl;
    std::cout << "Max Cell Value: " << MAX_VALUE << std::endl;
    std::cout << "Probability of valuable cell: " << 1.0 / PROBABILITY_DIVISOR << std::endl;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    if (!getSettings(argc, argv)) {
        return 0;
    }
    while (true) {
        printSettings();
        generateFields();
        printFields();
        pthread_t taranteryWorker;
        pthread_t anchuaryWorker;
        pthread_create(&taranteryWorker, NULL, taranteryAttack, NULL);
        pthread_create(&anchuaryWorker, NULL, anchuaryAttack, NULL);
        taranteryAttacksAnchuary.release();
        pthread_join(taranteryWorker, NULL);
        pthread_join(anchuaryWorker, NULL);
        resetSettings();
        if (!getSettings())
            break;
    }
}
