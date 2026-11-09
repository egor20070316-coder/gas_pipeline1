#define NOMINMAX
#include <windows.h>
#include <chrono>
#include <format>
#include <fstream>
#include <map>
#include "GasObjects.h"
#include "utils.h"

using namespace std;
using namespace chrono;

template<typename T>
void Add(map<int, T>& objects, int& nextId)
{
    if (nextId == numeric_limits<int>::max()) {
        Report("Свободных ID больше нет.");
        return;
    }
    T object;
    cin >> object;
    objects.emplace(nextId, object);
    Report("Добавлен объект ID " + to_string(nextId));
    ++nextId;
}

template<typename T>
void PrintAll(const string& title, const map<int, T>& objects)
{
    Report(title + " — " + to_string(objects.size()));
    for (const auto& [id, object] : objects)
        cout << "ID " << id << ": " << object << '\n';
}

void Run()
{
    map<int, Pipe> pipes;
    map<int, Station> stations;
    int nextId = 1;
    while (true) {
        cout << "\n1. Добавить трубу\n2. Добавить КС\n3. Показать все объекты\n0. Выход\n";
        switch (GetCorrectNumber(0, 3, "Действие: ")) {
        case 1: Add(pipes, nextId); break;
        case 2: Add(stations, nextId); break;
        case 3:
            PrintAll("Трубы", pipes);
            PrintAll("КС", stations);
            break;
        case 0: Report("Выход."); return;
        }
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    string time = format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now());
    ofstream logfile("log_" + time);
    if (!logfile) {
        cout << "Не удалось создать журнал в текущей папке.\n";
        return 1;
    }
    redirect_output_wrapper cerr_out(cerr);
    cerr_out.redirect(logfile);
    try {
        Run();
    } catch (const runtime_error& error) {
        Report(error.what());
    }
}
