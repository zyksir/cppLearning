#include <iostream>
#include <vector>
#include <unordered_map>
#include <assert.h>
using namespace std;

unordered_map<string, long long> valueMap;
void statements();

char peekNextChar() {
    int c;
    c = cin.peek();
    while (c == '\t' || c == '\n' || c == ' ') {
        cin.get();
        c = cin.peek();
    }
    // cout << "peek return" << (char)c << endl;
    return c;
}

int getNextChar() {
    int c;
    c = cin.get();
    while (c == '\t' || c == '\n' || c == ' ') {
        c = cin.get();
    }
    return c;
}

long long getNumber() {
    cout << "start getNumber()" << endl;
    long long value = 0;
    int c = peekNextChar();
    while ('0' <= c && c <= '9') {
        value *= 10;
        value += c - '0';
        getNextChar();
        c = peekNextChar();
    }
    cout << "[getNumber]return" << value << "nextChar="<< (char)peekNextChar() << endl;
    return value;
}

string variable() {
    // cout << "start variable()" << endl;
    string var = "";
    int c = peekNextChar();
    // cout << (char)c << endl;
    // assert(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
    while (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
        var += string(1, (char)c);
        getNextChar();
        c = peekNextChar();
    }
    // cout << "[variable]return " << var << endl;
    // cout << "[variable]next is " << (char)peekNextChar() << endl;
    return var;
}

long long getValue() {
    int c = peekNextChar();
    if ('0' <= c && c <= '9') {
        return getNumber();
    }
    string var = variable();
    return valueMap[var];
}

void println(const long long& value) {
    cout << value << endl;
}

void ifzero() {
    int c = getNextChar();
    assert(c == '?');
    long long value = getValue();
    cout << "start ifzero() with value=" << value << endl;
    c = getNextChar();
    assert(c == '{');
    if (value == 0) {
        if (peekNextChar() != '}') {
            statements();
        }
        c = peekNextChar();
        assert(c == '}');
        getNextChar();
    } else {
        c = peekNextChar();
        while (c != '}') {
            getNextChar();
            c = peekNextChar();
        }
        getNextChar();
    }
    cout << "end ifzero() with" << (char)peekNextChar() << endl;
}

void addition(const string& var) {
    auto it = valueMap.find(var);
    assert(it != valueMap.end());
    int c = getNextChar();
    assert(c == '+');
    c = getNextChar();
    assert(c == '=');
    long long value = getValue();
    valueMap[var] += value;
    cout << "[addition]" << var << "+" << value << endl;
}

void subtraction(const string& var) {
    auto it = valueMap.find(var);
    assert(it != valueMap.end());
    int c = getNextChar();
    assert(c == '-');
    c = getNextChar();
    assert(c == '=');
    long long value = getValue();
    valueMap[var] -= value;
    cout << "[subtraction]" << var << "-" << value << endl;
}

void assignment(const string& var) {
    int c = getNextChar();
    assert(c == '=');
    long long value = getValue();
    valueMap[var] = value;
    cout << "[assignment]" << var << "=" << value << endl;
}

void statement() {
    cout << "start statement()" << endl;
    int c = peekNextChar();
    if (c == '?') {
        ifzero();
        return;
    } else if (c == '!') {
        getNextChar();
        long long var = getValue();
        println(var);
        return;
    }
    string var = variable();
    c = peekNextChar();
    if (c == '=') {
        assignment(var);
    } else if (c == '+') {
        addition(var);
    } else if (c == '-') {
        subtraction(var);
    } else {
        cout << "[ERROR][statement] c = " << char(c) << ", var=" << var << endl;
        assert(false);
    }
}

void statements() {
    cout << "start statements()" << endl;
    statement();
    while (peekNextChar() == ';') {
        int c = getNextChar();
        assert(c == ';');
        statement();
    }
    cout << "end statements() with" << (char)peekNextChar() << endl;
}

int main() {
    statements();
    return 0;
}

