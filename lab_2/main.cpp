#define NOMINMAX
#include <windows.h>
#include <chrono>
#include <format>
#include <fstream>
#include <filesystem>
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

void Edit(Pipe& pipe)
{
    pipe.setRepair(GetCorrectNumber(0, 1, "Труба в ремонте (1 — да, 0 — нет): ") == 1);
}

void Edit(Station& station)
{
    station.setWorking(GetCorrectNumber(0, station.shopCount(), "Новое число работающих цехов: "));
}

template<typename T>
void Change(map<int, T>& objects, const string& title)
{
    PrintAll(title, objects);
    if (objects.empty())
        return;
    int id = GetCorrectNumber(1, numeric_limits<int>::max(), "ID объекта: ");
    auto item = objects.find(id);
    if (item == objects.end()) {
        Report("Объект с таким ID не найден.");
        return;
    }
    switch (GetCorrectNumber(0, 2, "1 — редактировать, 2 — удалить, 0 — назад: ")) {
    case 1:
        Edit(item->second);
        Report("Изменён объект ID " + to_string(id));
        break;
    case 2:
        objects.erase(item);
        Report("Удалён объект ID " + to_string(id));
        break;
    }
}

void Save(const map<int, Pipe>& pipes, const map<int, Station>& stations, int nextId)
{
    string name = ReadText("Имя файла для сохранения: ");
    try {
        filesystem::path path = filesystem::path(u8string(name.begin(), name.end()));
        filesystem::path directory = path.has_parent_path() ? path.parent_path() : L".";
        wchar_t temporary[MAX_PATH];
        if (!GetTempFileNameW(directory.c_str(), L"gts", 0, temporary)) {
            Report("Не удалось создать временный файл для сохранения: " + name);
            return;
        }

        ofstream file{filesystem::path(temporary)};
        file
             << "GAS_PIPELINE 2\n" << nextId << '\n'
             << pipes.size() << '\n' << stations.size() << '\n';
        auto writeObjects = [&](const auto& objects) {
            for (const auto& [id, object] : objects) {
                file << id << '\n';
                object.save(file);
            }
        };
        writeObjects(pipes);
        writeObjects(stations);
        file.close();
        if (!file || !MoveFileExW(temporary, path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            DeleteFileW(temporary);
            Report("Не удалось сохранить данные: " + name);
            return;
        }
        Report("Данные сохранены: " + name);
    }
    catch (const filesystem::filesystem_error&) {
        Report("Некорректный путь: " + name);
    }
}

bool ReadStorageInt(istream& file, int& value)
{
    string line;
    return getline(file, line) && ParseNumber(line, value);
}

bool ReadStorage(istream& file, map<int, Pipe>& pipes, map<int, Station>& stations, int& nextId)
{
    string header;
    if (!getline(file, header))
        return false;

    bool legacy = header == "0" || header == "1";
    int pipeCount, stationCount;
    if (legacy) {
        pipeCount = header == "1";
        if (!ReadStorageInt(file, stationCount) || stationCount < 0 || stationCount > 1)
            return false;
        nextId = 1 + pipeCount + stationCount;
    }
    else if (header != "GAS_PIPELINE 2" ||
             !ReadStorageInt(file, nextId) || nextId < 1 ||
             !ReadStorageInt(file, pipeCount) || pipeCount < 0 ||
             !ReadStorageInt(file, stationCount) || stationCount < 0 ||
             pipeCount > nextId - 1 - stationCount)
        return false;

    int legacyId = 1;
    auto readObjects = [&](int count, auto& objects, const auto& other) {
        for (int i = 0; i < count; ++i) {
            int id;
            if (legacy)
                id = legacyId++;
            else if (!ReadStorageInt(file, id) || id < 1 || id >= nextId)
                return false;
            if (other.contains(id))
                return false;
            auto [it, inserted] = objects.try_emplace(id);
            if (!inserted || !it->second.load(file))
                return false;
        }
        return true;
    };
    if (!readObjects(pipeCount, pipes, stations) || !readObjects(stationCount, stations, pipes))
        return false;
    file >> ws;
    return file.eof() && !file.bad();
}

void Load(map<int, Pipe>& pipes, map<int, Station>& stations, int& nextId)
{
    string name = ReadText("Имя файла для загрузки: ");
    try {
        ifstream file(filesystem::path(u8string(name.begin(), name.end())));
        if (!file) {
            Report("Не удалось открыть файл: " + name);
            return;
        }
        map<int, Pipe> newPipes;
        map<int, Station> newStations;
        int newNextId;
        if (!ReadStorage(file, newPipes, newStations, newNextId)) {
            Report("Ошибка формата файла: " + name);
            return;
        }
        pipes.swap(newPipes);
        stations.swap(newStations);
        nextId = newNextId;
        Report("Данные загружены: " + name);
    }
    catch (const filesystem::filesystem_error&) {
        Report("Некорректный путь: " + name);
    }
}

void Run()
{
    map<int, Pipe> pipes;
    map<int, Station> stations;
    int nextId = 1;
    while (true) {
        cout << "\n1. Добавить трубу\n2. Добавить КС\n3. Показать все объекты\n"
             << "4. Изменить или удалить трубу\n5. Изменить или удалить КС\n"
             << "6. Сохранить\n7. Загрузить\n0. Выход\n";
        switch (GetCorrectNumber(0, 7, "Действие: ")) {
        case 1: Add(pipes, nextId); break;
        case 2: Add(stations, nextId); break;
        case 3:
            PrintAll("Трубы", pipes);
            PrintAll("КС", stations);
            break;
        case 4: Change(pipes, "Трубы"); break;
        case 5: Change(stations, "КС"); break;
        case 6: Save(pipes, stations, nextId); break;
        case 7: Load(pipes, stations, nextId); break;
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
