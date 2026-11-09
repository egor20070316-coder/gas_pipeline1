#include "GasObjects.h"
#include "utils.h"
#include <iostream>
#include <limits>

namespace
{
    bool readName(istream& in, string& name)
    {
        if (!getline(in, name))
            return false;
        if (!name.empty() && name.back() == '\r')
            name.pop_back();
        return name.find_first_not_of(" \t\r\n") != string::npos;
    }

    template<typename T>
    bool readNumber(istream& in, T& value)
    {
        string line;
        return getline(in, line) && ParseNumber(line, value);
    }
}

istream& operator>>(istream& in, Pipe& pipe)
{
    if (&in != &cin) {
        if (!pipe.load(in))
            in.setstate(ios::failbit);
        return in;
    }
    Pipe p;
    p.name = ReadText("Название трубы (км отметка): ");
    p.length = GetCorrectNumber(numeric_limits<double>::denorm_min(),
                                numeric_limits<double>::max(), "Длина трубы, км: ");
    p.diameter = GetCorrectNumber(1, numeric_limits<int>::max(), "Диаметр трубы, мм: ");
    p.repair = GetCorrectNumber(0, 1, "Труба в ремонте (1 — да, 0 — нет): ") == 1;
    pipe = p;
    return in;
}

ostream& operator<<(ostream& out, const Pipe& pipe)
{
    return out << pipe.name << ", длина: " << pipe.length << " км, диаметр: "
               << pipe.diameter << " мм, " << (pipe.repair ? "в ремонте" : "в работе");
}

void Pipe::save(ostream& out) const
{
    const auto oldPrecision = out.precision(numeric_limits<double>::max_digits10);
    out << name << '\n' << length << '\n' << diameter << '\n' << int(repair) << '\n';
    out.precision(oldPrecision);
}

bool Pipe::load(istream& in)
{
    Pipe p;
    int status;
    if (!readName(in, p.name) || !readNumber(in, p.length) || p.length <= 0 ||
        !readNumber(in, p.diameter) || p.diameter <= 0 ||
        !readNumber(in, status) || (status != 0 && status != 1))
        return false;
    p.repair = status == 1;
    *this = p;
    return true;
}

istream& operator>>(istream& in, Station& station)
{
    if (&in != &cin) {
        if (!station.load(in))
            in.setstate(ios::failbit);
        return in;
    }
    Station s;
    s.name = ReadText("Название КС: ");
    s.shops = GetCorrectNumber(1, numeric_limits<int>::max(), "Общее число цехов: ");
    s.working = GetCorrectNumber(0, s.shops, "Число работающих цехов: ");
    s.stationClass = GetCorrectNumber(1, numeric_limits<int>::max(), "Класс станции: ");
    station = s;
    return in;
}

ostream& operator<<(ostream& out, const Station& s)
{
    return out << s.name << ", цехов в работе: " << s.working << " из " << s.shops
               << ", простаивает: " << s.idlePercent() << "%, класс: " << s.stationClass;
}

double Station::idlePercent() const
{
    return 100.0 * (shops - working) / shops;
}

void Station::setWorking(int value)
{
    if (value >= 0 && value <= shops)
        working = value;
}

void Station::save(ostream& out) const
{
    out << name << '\n' << shops << '\n' << working << '\n' << stationClass << '\n';
}

bool Station::load(istream& in)
{
    Station s;
    if (!readName(in, s.name) || !readNumber(in, s.shops) || s.shops <= 0 ||
        !readNumber(in, s.working) || s.working < 0 || s.working > s.shops ||
        !readNumber(in, s.stationClass) || s.stationClass <= 0)
        return false;
    *this = s;
    return true;
}
