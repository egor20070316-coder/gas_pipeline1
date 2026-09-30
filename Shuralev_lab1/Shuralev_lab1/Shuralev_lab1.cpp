// Shuralev_lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct Truba
{
	string km;
	double dlina;
	int d;
	bool remont = 0;
};

struct KS
{
	string name;
	int ceh;
	int work ;
	int klass;
};

void inputTruba(Truba& truba)

{
	cin.ignore(100, '\n');
	cout << "Введите км: ";
	getline(cin, truba.km);

	bool flag = true;
	while (flag)
	{
		flag = false;

		if (truba.km.empty() || truba.km[0] == '0' || truba.km[0] == '-' || truba.km[0] == '+' || truba.km[0] == ' ')
		{
			flag = true;
		}

		for (int i = 0; i < truba.km.length(); i++)
		{
			if (truba.km[i] < '0' || truba.km[i] > '9')
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			cout << "Ошибка, введите только цифры и без пробелов";
			getline(cin, truba.km);
		}
	}
	cout << "Введите длину: ";
	cin >> truba.dlina;

	while (cin.fail() || truba.dlina <= 0 || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ошибка, введите одно положительное число!";
		cin >> truba.dlina;
	}

	cout << "Введите диаметр: ";
	cin >> truba.d;
	
	while (cin.fail() || truba.d <= 0 || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ошибка, введите положительное число!" << endl;

		cin >> truba.d;
	}
}

void printTruba(const Truba& truba)
{
	cout << "Км: " << truba.km << endl;
	cout << "Длина: " << truba.dlina << endl;
	cout << "Диаметр: " << truba.d << endl;
	cout << "Ремонт" << (truba.remont ? "Да" : "Нет") << endl;
}

void editTruba(Truba& truba)
{
	int x;
	cout << "Введите ремонт(1 - да, 0 - нет) : " << endl;
	cin >> x;
	while (cin.fail() || x != 0 && x != 1)
	{
		cout << "Ошибка, введите число!" << endl;
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> x;
	}
	truba.remont = x;
}

void inputKS(KS& ks)
{
	cout << "Введите название: ";
	cin.ignore(1000, '\n');
	getline(cin, ks.name);

	cout << "Введите цех: ";
	cin >> ks.ceh;
	while (cin.fail() || ks.ceh <= 0 || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ошибка, введите положительное число!" << endl;

		cin >> ks.ceh;
	}

	cout << "Введите работу: ";
	cin >> ks.work;
	while (cin.fail() || ks.work < 0 || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ошибка, введите число!" << endl;

		cin >> ks.work;
	}

	cout << "Введите класс: ";
	cin >> ks.klass;
	while (cin.fail() || ks.klass <= 0 || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ошибка, введите положительное число!" << endl;

		cin >> ks.klass;
	}
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
		{	ks.work = ks.work + 1;
			cout << "Цех запущен" << endl;}
		else
			cout << "Все цеха работают" << endl;
	}
	if (x == 2)
	{
		if (ks.work > 0)
		{	ks.work = ks.work - 1;
			cout << "Цех остановлен" << endl;}
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
