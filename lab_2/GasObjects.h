#pragma once
#include <iosfwd>
#include <string>

using namespace std;

class Pipe
{
private:
    string name;
    double length = 1.0;
    int diameter = 1;
    bool repair = false;

public:
    const string& getName() const { return name; }
    bool inRepair() const { return repair; }
    void setRepair(bool value) { repair = value; }
    void save(ostream& out) const;
    bool load(istream& in);

    friend ostream& operator<<(ostream& out, const Pipe& pipe);
    friend istream& operator>>(istream& in, Pipe& pipe);
};

class Station
{
private:
    string name;
    int shops = 1;
    int working = 0;
    int stationClass = 1;

public:
    const string& getName() const { return name; }
    int shopCount() const { return shops; }
    void setWorking(int value);
    double idlePercent() const;
    void save(ostream& out) const;
    bool load(istream& in);

    friend ostream& operator<<(ostream& out, const Station& station);
    friend istream& operator>>(istream& in, Station& station);
};
