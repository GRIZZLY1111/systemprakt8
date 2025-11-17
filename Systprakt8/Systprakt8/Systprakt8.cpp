// Systprakt8.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <Windows.h>
#include <conio.h>

const int Turtles = 10;
const int arr_height = 3;
const int arr_width = 53;
int positions[Turtles];
int rows[Turtles];
int num_turtles = 0;
volatile int winner = -1;
volatile int finished_count = 0;

void print() {
    system("cls");
    int arr[arr_height][arr_width] = { 0 };
    for (int i = 0; i < arr_height; i++) {
        for (int j = 0; j < arr_width; j++) {
            if ((i == 0 || i == arr_height - 1 || j == 1 || j == 51) && j != 0) {
                arr[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < num_turtles; i++) {
        int x = rows[i];
        int y = positions[i];
        if (y >= 0 && y < arr_width) {
            arr[x][y] = i+1;
        }
    }
    for (int i = 0; i < arr_height; i++) {
        for (int j = 0; j < arr_width; j++) {
            if (arr[i][j] == 0) std::cout << "  ";
            else std::cout << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
}

DWORD WINAPI TurtleThread(LPVOID lpParam) {
    int id = (int)lpParam;
    srand(GetTickCount64() + id);

    while (true) {
        if (positions[id] >= 52) {
            return 0;
        }

        int step = rand() % 3;

        positions[id] += step;
        if (positions[id] > 52) {
            positions[id] = 52;
        }

        if (positions[id] >= 52 && winner == -1) {
            winner = id + 1;
        }

        if (positions[id] >= 52) {
            finished_count++;
        }

        Sleep(rand() % 2001 + 1000);
    }
    return 0;
}

int main() {
    setlocale(0, "rus");
    int num = 0;
    while (num < 2 || num > 10) {
        std::cout << "Сколько черепах участвует в гонке? (2–10): ";
        std::cin >> num;
    }

    num_turtles = num;
    for (int i = 0; i < num; i++) {
        rows[i] = 1;
        positions[i] = 0;
    }

    print();
    std::cout << "\nНажмите любую клавишу для старта гонки\n";
    _getch();

    winner = -1;
    finished_count = 0;
    HANDLE threads[Turtles];

    for (int i = 0; i < num; i++) {
        threads[i] = CreateThread(NULL, 0, TurtleThread, (void*)i, 0, NULL);
    }
    for (int i = 0; i < num; i++) {
        if (threads[i] == NULL) {
            return GetLastError();
        }
    }

    while (true) {
        print();
        bool all_finished = (finished_count >= num_turtles);

        if (all_finished) break;

        Sleep(rand() % 2001 + 1000);
    }

    std::cout << "\nГонка завершена!\n";
    std::cout << "Победителем становится черепашка под номером " << winner << std::endl;

    WaitForMultipleObjects(num, threads, TRUE, INFINITE);

    for (int i = 0; i < num; i++) {
        CloseHandle(threads[i]);
    }
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
