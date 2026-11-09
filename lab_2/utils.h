#pragma once
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

class redirect_output_wrapper
{
    ostream& stream;
    streambuf* const old_buf;
public:
    explicit redirect_output_wrapper(ostream& src) : stream(src), old_buf(src.rdbuf()) {}
    void redirect(ostream& dest) { stream.rdbuf(dest.rdbuf()); }
    ~redirect_output_wrapper() { stream.rdbuf(old_buf); }
};

inline void Report(const string& message)
{
    cout << message << '\n';
    cerr << message << '\n';
}

inline string ReadLine(const string& prompt)
{
    cout << prompt;
    string line;
    if (!getline(cin, line))
        throw runtime_error("Ввод завершён.");
    cerr << prompt << line << '\n';
    return line;
}

inline string ReadText(const string& prompt)
{
    while (true) {
        string line = ReadLine(prompt);
        if (line.find_first_not_of(" \t\r") != string::npos)
            return line;
        Report("Введите непустую строку.");
    }
}

template<typename T>
bool ParseNumber(const string& line, T& value)
{
    istringstream input(line);
    char extra;
    return (input >> value) && !(input >> extra) && isfinite(value);
}

template<typename T>
T GetCorrectNumber(T min, T max, const string& prompt)
{
    T value;
    while (!ParseNumber(ReadLine(prompt), value) || value < min || value > max)
        Report("Число вне диапазона или неверный формат. Повторите ввод.");
    return value;
}
