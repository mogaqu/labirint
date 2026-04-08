#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
using namespace std;
#include <clocale>
#include <string>
#include <time.h>
void color(int c) {
    // Получаем доступ к окну консоли и меняем атрибут текста
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void print_maze(const vector<vector<char>>& maze, const int& stroki, const int& stolbci) {
    for (int i = 0; i < stroki; i++) {
        for (int y = 0; y < stolbci; y++) {
            if (maze[i][y] == '#'){
                color(10);
            }
            else if (maze[i][y] == '*'){
                color(12);
            }
            else if (maze[i][y] == '~'){
                color(11);
            }
            cout << maze[i][y] << " ";color(7);
        }
        cout << "\n";
    }
}

void bfs_solve(const int& choose, vector<vector<char>>& labirint, int stroki, int stolbci) {
    // Откуда начинаем и куда идём
    int start_x = 0, start_y = 0;
    int finish_x = stroki - 1, finish_y = stolbci - 1;

    // Очередь клеток для обхода
    vector<int> ochered_x;
    vector<int> ochered_y;

    // Для каждой клетки помним: откуда в неё пришли
    vector<vector<int>> otkuda_x(stroki, vector<int>(stolbci, -1));
    vector<vector<int>> otkuda_y(stroki, vector<int>(stolbci, -1));

    // Смещения: вверх, вниз, влево, вправо
    int shag_x[] = { -1, 1, 0, 0 };
    int shag_y[] = { 0, 0, -1, 1 };

    // Кладём старт в очередь
    ochered_x.push_back(start_x);
    ochered_y.push_back(start_y);
    labirint[start_x][start_y] = '~';  // помечаем как посещённую

    int tekushiy = 0;  // какую клетку сейчас смотрим
    bool nashli = false;

    // Пока есть клетки в очереди
    while (tekushiy < ochered_x.size() && !nashli) {

        int x = ochered_x[tekushiy];
        int y = ochered_y[tekushiy];
        tekushiy++;

        // Визуализация
        if (choose == 1) {

            system("cls");
            print_maze(labirint, stroki, stolbci);
            Sleep(30);

        }


        // Смотрим все 4 направления
        for (int i = 0; i < 4; i++) {
            int noviy_x = x + shag_x[i];
            int noviy_y = y + shag_y[i];

            // Проверка границ
            if (noviy_x < 0 || noviy_x >= stroki) continue;
            if (noviy_y < 0 || noviy_y >= stolbci) continue;

            // Если стена или уже были — пропускаем
            if (labirint[noviy_x][noviy_y] != ' ') continue;

            // Помечаем и запоминаем откуда пришли
            labirint[noviy_x][noviy_y] = '~';
            otkuda_x[noviy_x][noviy_y] = x;
            otkuda_y[noviy_x][noviy_y] = y;

            // Добавляем в очередь
            ochered_x.push_back(noviy_x);
            ochered_y.push_back(noviy_y);

            // Дошли до финиша?
            if (noviy_x == finish_x && noviy_y == finish_y) {
                nashli = true;
                break;
            }
        }
    }

    // Рисуем путь
    if (nashli) {
        int x = finish_x;
        int y = finish_y;

        while (x != -1) {
            labirint[x][y] = '*';

            int predok_x = otkuda_x[x][y];
            int predok_y = otkuda_y[x][y];

            x = predok_x;
            y = predok_y;
        }
        if (choose == 1) {
            system("cls");
            cout << "Путь найден!\n";
            print_maze(labirint, stroki, stolbci);
        }
        else {
            print_maze(labirint, stroki, stolbci);
            
        }


    }
    else {
        if (choose != 1) {
            print_maze(labirint, stroki, stolbci);
        }
        cout << "Пути нет!\n";
    }
}
vector<vector<char>> generator_maze(const int& choose, const int& stroki, const int& stolbci) {

    vector<vector<char>> labirint(stroki, vector<char>(stolbci, '#'));
    vector<int> passed_squares;
    passed_squares.push_back(0); // x
    passed_squares.push_back(0); // y

    while (!passed_squares.empty()) {

        int y = passed_squares.back(); passed_squares.pop_back();
        int x = passed_squares.back(); passed_squares.pop_back();


        vector<int> directions;
        if (x - 2 >= 0 && labirint[x - 2][y] == '#') directions.push_back(0);
        if (x + 2 < stroki && labirint[x + 2][y] == '#') directions.push_back(1);
        if (y - 2 >= 0 && labirint[x][y - 2] == '#') directions.push_back(2);
        if (y + 2 < stolbci && labirint[x][y + 2] == '#') directions.push_back(3);
        if (!directions.empty()) {
            // Случайное направление
            int dir = directions[rand() % directions.size()];

            // Смещения
            int dx = 0, dy = 0;
            if (dir == 0) dx = -2;
            if (dir == 1) dx = 2;
            if (dir == 2) dy = -2;
            if (dir == 3) dy = 2;

            // Пробиваем стену
            labirint[x + dx / 2][y + dy / 2] = ' '; // стена между
            labirint[x + dx][y + dy] = ' ';           // новая клетка

            // Возвращаем текущую в стек (ещё могут быть ходы)
            passed_squares.push_back(x);
            passed_squares.push_back(y);

            // Новую кладём сверху
            passed_squares.push_back(x + dx);
            passed_squares.push_back(y + dy);


        }
        if (choose == 1) {
            labirint[x][y] = '@';
            system("cls");
            print_maze(labirint, stroki, stolbci);
            labirint[x][y] = ' ';
        }
    }

    return labirint;
}




int main() {
    //setlocale(LC_ALL, "Russian");
    srand(time(0));
    system("chcp 65001>nul");
    int choose = 0;
    cout << "напишите единицу, если хотите видеть процесс выполнения, иначе напишите что-нибудь другое(другую цифру).";
    color(12);cout <<"\n\n\n\n\n\nПРЕДУПРЕЖДАЮ ЭПИЛЕПТИКОВ, что лучше не писать единицу, потому что все будет моргать.\n";color(7);
    cin >> choose;
    bool wait;
    int stroki, stolbci;
    cout << "невидимый символ - проход, # - стена, местоположение алгоритма - @, ~ - это работа алгоритма поиска в ширину, * - самый короткий путь к левой нижней точке";
    cout << "введите нечетное кол-во строк и столбцов, типа вначале строки(пишете число и жмете enter), потом столбцы(пишете число ещё один раз и жмете enter):\n";
    cin >> stroki >> stolbci;
    if (stroki == 0 || stolbci == 0){
        cout << "постарайся без ноликов";
        Sleep(1000);
        main();


    }
    if (choose == 1) {
        auto labirint = generator_maze(choose,stroki, stolbci)  ;
        system("cls");
        cout << "теперь я буду проходить этот лабиринт";
        bfs_solve(choose,labirint, stroki, stolbci);
        cin >> wait;
        Sleep(1000);
    }
    else {
        auto labirint = generator_maze(choose, stroki, stolbci);
        print_maze(labirint, stroki,stolbci);
        bfs_solve(choose, labirint, stroki, stolbci);
        cin >> wait;

    }


}
