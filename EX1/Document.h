#pragma once

#include <bits/stdc++.h>

using namespace std;

class Document {
private:
    int currentLine;
    vector<string> doc{};
public:
    Document();
    void print();
    void xn(int);
    void xd(int);
    void searchFw(string);
    void searchBw(string);
    void appendLines(int, int);
    void append(vector<string>);
    void insert(vector<string>);
    void switchLines(vector<string>);
    void replaceInCurrentLine(string, string);
    bool isWordInLine(string, string);
};

