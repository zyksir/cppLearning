#include<iostream>
#include<vector>
#include<deque>
using namespace std;

int main() {
    pair<int, int> p = {1, 1};
    auto& [x, y] = p;
    x++; y += 2;
    cout << p.first << "\t" << p.second << endl;

    int a = 1;
    int& b = a;
    auto c = b;
    c++;
    cout << c << "\t" << b << "\t" <<  a << endl;
    return 0;
}