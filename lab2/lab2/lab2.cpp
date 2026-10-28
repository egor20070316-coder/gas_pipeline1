кфуп// Shuralev_lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <clocale>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "gts.hpp"
#include "pipe.hpp"
#include "cs_station.hpp"

using namespace std;

string read_line() {
	string text;
	if (!getline(cin, text)) {
		throw runtime_error("Ввод завершён.");
	}
	return text;
}


int get_int(int min_value, int max_value) {
	while (true) {
		stringstream input(read_line());
		int number;
		char extra;
		if (input >> number && !(input >> extra) &&
			number >= min_value && number <= max_value) {
			return number;
		}
		cout << "Введите целое число от " << min_value
			<< " до " << max_value << ": ";
	}
}

double get_double(double min_value, double max_value) {
	while (true) {
		stringstream input(read_line());
		double number;
		char extra;
		if (input >> number && !(input >> extra) && isfinite(number) &&
			number >= min_value && number <= max_value) {
			return number;
		}
		cout << "Некорректное число. Повторите ввод (дробь через точку): ";
	}
}

string get_name() {
	while (true) {
		string name = read_line();
		for (char c : name) {
			if (c != ' ' && c != '\t' && c != '\r') return name;
		}
		cout << "Строка не должна быть пустой. Повторите: ";
	}
}

void write_log(const string& message) {
	ofstream file("log.txt", ios::app);
	if (file) file << message << '\n';
}

bool find_name(string name, string part) {
	for (char& c : name) {
		c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
	}
	for (char& c : part) {
		c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
	}
	return name.find(part) != string::npos;
}

vector<int> find_pipes(const unordered_map<int, Pipe>& pipes) {
	vector<int> ids;
	if (pipes.empty()) {
		cout << "Труб пока нет.\n";
		return ids;
	}

	cout << "1. По наименованию\n2. По признаку 'в ремонте'\nВыберите: ";
	int filter = get_int(1, 2);
	if (filter == 1) {
		cout << "Введите название или его часть: ";
		string name = read_line();
		for (const auto& item : pipes) {
			if (find_name(item.second.get_name(), name)) {
				ids.push_back(item.first);
			}
		}
	}
	else {
		cout << "1. В ремонте\n2. В работе\nВыберите: ";
		bool repair = (get_int(1, 2) == 1);
		for (const auto& item : pipes) {
			if (item.second.is_in_repair() == repair) {
				ids.push_back(item.first);
			}
		}
	}

	cout << "Найдено труб: " << ids.size() << '\n';
	for (int id : ids) {
		cout << "ID: " << id << '\n';
		pipes.at(id).print();
	}
	write_log("Поиск труб. Найдено: " + to_string(ids.size()));
	return ids;
}

vector<int> find_cs(const unordered_map<int, Cs>& stations) {
	vector<int> ids;
	if (stations.empty()) {
		cout << "КС пока нет.\n";
		return ids;
	}

	cout << "1. По наименованию\n2. По проценту незадействованных цехов\nВыберите: ";
	int filter = get_int(1, 2);
	if (filter == 1) {
		cout << "Введите название или его часть: ";
		string name = read_line();
		for (const auto& item : stations) {
			if (find_name(item.second.get_name(), name)) {
				ids.push_back(item.first);
			}
		}
	}
	else {
		cout << "Минимальный процент незадействованных цехов (0..100): ";
		double percent = get_double(0, 100);
		for (const auto& item : stations) {
			if (item.second.get_percentage() >= percent) {
				ids.push_back(item.first);
			}
		}
	}

	cout << "Найдено КС: " << ids.size() << '\n';
	for (int id : ids) {
		cout << "ID: " << id << '\n';
		stations.at(id).print();
	}
	write_log("Поиск КС. Найдено: " + to_string(ids.size()));
	return ids;
}

bool has_id(const vector<int>& ids, int id) {
	for (int current_id : ids) {
		if (current_id == id) return true;
	}
	return false;
}



vector<int> ids = choose_ids(found);
if (ids.empty()) {
	cout << "Не выбрано ни одного объекта.\n";
	return;
}

if (action == 1) {
	cout << "Новый статус: 1 — в работе, 0 — в ремонте: ";
	bool working = (get_int(0, 1) == 1);
	for (int id : ids) pipes.at(id).set_work(working);
	cout << "Изменено труб: " << ids.size() << '\n';
	write_log("Изменение статуса труб: " + to_string(ids.size()));
}
else {
	for (int id : ids) pipes.erase(id);
	cout << "Удалено труб: " << ids.size() << '\n';
	write_log("Удаление труб: " + to_string(ids.size()));
}
}

void edit_cs(unordered_map<int, Cs>& stations) {
	vector<int> found = find_cs(stations);
	if (found.empty()) return;

	cout << "1. Редактировать\n2. Удалить\n0. Отмена\nВыберите: ";
	int action = get_int(0, 2);
	if (action == 0) return;

	vector<int> ids = choose_ids(found);
	if (ids.empty()) {
		cout << "Не выбрано ни одного объекта.\n";
		return;
	}

	if (action == 1) {
		cout << "1. Запустить цех\n2. Остановить цех\nВыберите: ";
		int command = get_int(1, 2);
		for (int id : ids) {
			cout << "КС ID: " << id << '\n';
			stations.at(id).edit(command);
		}
		cout << "Обработано КС: " << ids.size() << '\n';
		write_log("Редактирование КС: " + to_string(ids.size()));
	}
	else {
		for (int id : ids) stations.erase(id);
		cout << "Удалено КС: " << ids.size() << '\n';
		write_log("Удаление КС: " + to_string(ids.size()));
	}
}

void add_pipe(GTS& gts) {
	cout << "Введите километровую отметку (название): ";
	string name = get_name();

	cout << "Введите длину (км, больше нуля): ";
	double length = get_double(0, numeric_limits<double>::max());
	while (length == 0) {
		cout << "Длина должна быть больше нуля: ";
		length = get_double(0, numeric_limits<double>::max());
	}

	cout << "Введите диаметр (мм): ";
	int diameter = get_int(1, numeric_limits<int>::max());

	Pipe pipe;
	pipe.set_name(name);
	pipe.set_len(length);
	pipe.set_diametr(diameter);
	pipe.set_work(true);
	gts.add_pipes(pipe);

	cout << "Труба добавлена!\n";
	write_log("Добавлена новая труба");
}

void add_cs(GTS& gts) {
	cout << "Введите название КС: ";
	string name = get_name();
	cout << "Введите общее количество цехов: ";
	int total = get_int(1, numeric_limits<int>::max());
	cout << "Введите количество цехов в работе: ";
	int working = get_int(0, total);

	cout << "Введите класс станции (A, B, C, D, E): ";
	char station_class;
	while (true) {
		string text = read_line();
		if (text.size() == 1) {
			station_class = static_cast<char>(
				toupper(static_cast<unsigned char>(text[0])));
			if (station_class >= 'A' && station_class <= 'E') break;
		}
		cout << "Введите одну латинскую букву от A до E: ";
	}

	Cs station;
	station.set_name(name);
	station.set_shops(total, working);
	station.set_class(station_class);
	gts.add_css(station);

	cout << "КС добавлена!\n";
	write_log("Добавлена новая КС");
}

int  main()
{
	SetConsoleCP(65001); //https://learn.microsoft.com
	SetConsoleOutputCP(65001);


	Truba truba;
	KS ks;

	bool hasTruba = false;
	bool hasKS = false;

	int menu = -1;
	while (menu != 0)
	{
		cout << endl;
		cout << "1. Добавить трубу" << endl;
		cout << "2. Добавить КС" << endl;
		cout << "3. Показать все" << endl;
		cout << "4. Редкатировать трубу" << endl;
		cout << "5. Редактировать КС" << endl;
		cout << "6. Сохранить" << endl;
		cout << "7. Загрузить" << endl;
		cout << "0. Выйти" << endl;

		cout << "Выберите :";
		cin >> menu;
		while (cin.fail() || (menu < 0 || menu > 7) || cin.peek() != '\n')
		{
			cout << "Ошибка, введите число от 0 до 7!";
			cin.clear();
			cin.ignore(1000, '\n');
			cin >> menu;
		}
		switch (menu)
		{
		case 1:
			inputTruba(truba);
			hasTruba = true;
			break;
		case 2:
			inputKS(ks);
			hasKS = true;
			break;
		case 3:
			if (hasTruba)
				printTruba(truba);
			else
				cout << "Труба не добавлена" << endl;
			if (hasKS)
				printKS(ks);
			else
				cout << "КС не добавлен" << endl;
			break;
		case 4:
			if (hasTruba)
				editTruba(truba);
			else
				cout << "Добавьте трубу" << endl;
			break;
		case 5:
			if (hasKS)
				editKS(ks);
			else
				cout << "Добавьте КС" << endl;
			break;
		case 6:
			save(truba, ks, hasTruba, hasKS);
			break;
		case 7:
			load(truba, ks, hasTruba, hasKS);
			break;
		case 0:
			cout << "Выход" << endl;
			break;
		}
	}
	return 0;
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
