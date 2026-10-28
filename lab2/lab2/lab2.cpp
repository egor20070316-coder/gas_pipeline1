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

void printKS(const KS& ks)
{
	cout << endl;
	cout << "КС" << endl;
	cout << "Название: " << ks.name << endl;
	cout << "Цех: " << ks.ceh << endl;
	cout << "Работа: " << ks.work << endl;
	cout << "Класс: " << ks.klass << endl;
}

void editKS(KS& ks)
{
	int x;

	cout << "1. Запустить цех" << endl;
	cout << "2. Остановить цех" << endl;
	cout << "Выберите";
	cin >> x;

	while (cin.fail() || (x != 1 && x != 2))
	{
		cout << "Ошибка, введите 1 или 2!";
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> x;
	}
	if (x == 1)
	{
		if (ks.work < ks.ceh)
		{
			ks.work = ks.work + 1;
			cout << "Цех запущен" << endl;
		}
		else
			cout << "Все цеха работают" << endl;
	}
	if (x == 2)
	{
		if (ks.work > 0)
		{
			ks.work = ks.work - 1;
			cout << "Цех остановлен" << endl;
		}
		else
			cout << "Все цеха уже остановлены" << endl;
	}
}

void saveTruba(ofstream& file, Truba& truba)
{
	file << truba.km << endl;
	file << truba.dlina << endl;
	file << truba.d << endl;
	file << truba.remont << endl;;
}

void loadTruba(ifstream& file, Truba& truba)
{
	file >> truba.km;
	file >> truba.dlina;
	file >> truba.d;
	file >> truba.remont;
}

void saveKS(ofstream& file, KS& ks)
{
	file << ks.name << endl;
	file << ks.ceh << endl;
	file << ks.work << endl;
	file << ks.klass << endl;
}

void loadKS(ifstream& file, KS& ks)
{
	file >> ks.name;
	file >> ks.ceh;
	file >> ks.work;
	file >> ks.klass;
}

void save(Truba truba, KS ks, bool hasTruba, bool hasKS)
{
	ofstream file("data.txt");
	file << hasTruba << endl;
	file << hasKS << endl;
	if (!hasTruba && !hasKS)
	{
		cout << "Данных не найдено" << endl;
		return;
	}
	if (hasTruba)
		saveTruba(file, truba);

	if (hasKS)
		saveKS(file, ks);

	file.close();
	cout << "Данные сохранены" << endl;
}

void load(Truba& truba, KS& ks, bool& hasTruba, bool& hasKS)
{
	ifstream file("data.txt");
	if (!file)
	{
		cout << "Файл не найден" << endl;
		return;
	}

	file >> hasTruba;
	file >> hasKS;
	if (hasTruba)
		loadTruba(file, truba);
	if (hasKS)
		loadKS(file, ks);
	file.close();
	cout << "Данные сохранены" << endl;
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
