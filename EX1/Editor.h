#pragma once

#include <bits/stdc++.h>
#include "Document.h"

using namespace std;

// enum for all possible commands we support in our Editor.
enum struct CMD {
    XN,
    PRINT,
    APPEND,
    INSERT,
    DOT,
    SWITCH,
    XD,
    SEARCH_FW,
    SEARCH_BW,
    REPLACE,
    APPEND_LINES,
    QUIT,
    NONE
};

class Editor {
private:
    Document doc;
    bool DOT;
public:
    Editor();
    void flipDot();
    void loop();
    CMD commandIfAny(string);
    pair <string, string> getOldAndNew(string);
    pair <int, int> getIndexes(string);
    int removeLastChar(string);
    string removeFirstChar(string);
};