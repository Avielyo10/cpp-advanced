#include "Editor.h"

// Regex helpers
regex xn{"\\d+n"};
regex xd{"\\d+d"};
regex xyj{"\\d+,\\d+j"};
regex reReplace{"s/\\w+/\\w+/"};
regex searchFw{"\\/\\w+"};
regex searchBw{"\\?\\w+"};

Editor::Editor() {
    DOT = false;
}

// Main function 
// Gets line by line from stdin in a loop
// and decide what to do with it.
void Editor::loop() {
    string line;
    CMD cmd = CMD::NONE;
    vector<string> lines {};
    do {
        getline(cin, line);
        switch (commandIfAny(line)) {
        case CMD::XN:
            doc.xn(removeLastChar(line));
            break;
        case CMD::XD:
            doc.xd(removeLastChar(line));
            break;
        case CMD::PRINT:
            doc.print();
            break;
        case CMD::APPEND:
            cmd = CMD::APPEND;
            break;
        case CMD::INSERT:
            cmd = CMD::INSERT;
            break;
        case CMD::SWITCH:
            cmd = CMD::SWITCH;
            break;
        case CMD::DOT:
            switch (cmd) {
            case CMD::APPEND:
                doc.append(lines);
                break;
            case CMD::INSERT:
                doc.insert(lines);
                break;
            case CMD::SWITCH:
                doc.switchLines(lines);
                break;
            }
            cmd = CMD::NONE;
            lines.clear();
            break;
        case CMD::SEARCH_FW:
            doc.searchFw(removeFirstChar(line));
            break;
        case CMD::SEARCH_BW:
            doc.searchBw(removeFirstChar(line));
            break;
        case CMD::REPLACE: {
            pair <string, string> oldAndNew = getOldAndNew(line);
            doc.replaceInCurrentLine(oldAndNew.first, oldAndNew.second);
            break; 
        }
        case CMD::APPEND_LINES: {
            pair <int, int> indexes = getIndexes(line);
            doc.appendLines(indexes.first, indexes.second);
            break;
        }
        case CMD::NONE:
            if (DOT) {
              lines.push_back(line);  
            }
            break;
        case CMD::QUIT:
            break;
        }
    } while (line != "Q");
}

// When we get the commands a/i/c we want to be able to remember all
// the following lines till we hit the '.'.
void Editor::flipDot() {
    DOT = !DOT;
}

// Removes the first character in the line
// Used by CMD::SEARCH_FW & CMD::SEARCH_BW
string Editor::removeFirstChar(string line) {
    line.erase(line.begin()); // remove '/' or '?'
    return line;
}

// Removes the last character in the line
// Used by CMD::XD & CMD::XN
int Editor::removeLastChar(string line) {
    line.pop_back(); // remove 'n' or 'd'
    return stoi(line);
}

// Extract the indexes from the line
// Used by CMD::APPEND_LINES
pair <int, int> Editor::getIndexes(string line) {
    string delimiter = ",";
    line.pop_back(); // remove 'j'
    int first = stoi(line.substr(0, line.find(delimiter))); // get the first element
    line.erase(0, line.find(delimiter) + delimiter.length()); // remove it including the delimiter 
    int second = stoi(line); // what left from the string is the second and last element
    return make_pair(first, second);
}

// Extract the 2 strings from the line
// Old string needs to be replaced with the new string.
// Used by CMD::REPLACE
pair <string, string> Editor::getOldAndNew(string line) {
    string delimiter = "/";
    line.erase(0, line.find(delimiter) + delimiter.length()); // remove "s/"

    string sOld = line.substr(0, line.find(delimiter));
    line.erase(0, line.find(delimiter) + delimiter.length()); // remove "old/"
    string sNew = line.substr(0, line.find(delimiter));
    return make_pair(sOld, sNew);
}

// Commands decider
// Is this a supported command?
// If true, returns its type
// Else it's probably just text
CMD Editor::commandIfAny(string line) {
    CMD cmd;
    if (regex_match(line, xn)) {
        cmd = CMD::XN;
    }
    else if (regex_match(line, xd)) {
        cmd = CMD::XD;
    }
    else if (regex_match(line, xyj)) {
        cmd = CMD::APPEND_LINES;
    }
    else if (regex_match(line, reReplace)) {
        cmd = CMD::REPLACE;
    }
    else if (regex_match(line, searchFw)) {
        cmd = CMD::SEARCH_FW;
    }
    else if (regex_match(line, searchBw)) {
        cmd = CMD::SEARCH_BW;
    }
    else if (line == "%p") {
        cmd = CMD::PRINT;
    }
    else if (line == "a") {
        cmd = CMD::APPEND;
        flipDot(); // now DOT is true
    }
    else if (line == "i") {
        cmd = CMD::INSERT;
        flipDot(); // now DOT is true
    }
    else if (line == "c") {
        cmd = CMD::SWITCH;
        flipDot(); // now DOT is true
    }
    else if (line == ".") {
        cmd = CMD::DOT;
        flipDot(); // now DOT is false
    }
    else if (line == "Q") {
        cmd = CMD::QUIT;
    }
    else {
        cmd = CMD::NONE;
    }
    return cmd;
}

// For debugging, can be commented out
inline const string toString(CMD cmd) {
    switch (cmd) {
    case CMD::XN:
        return "CMD::XN";
    case CMD::PRINT:
        return "CMD::PRINT";
    case CMD::APPEND:
        return "CMD::APPEND";
    case CMD::INSERT:
        return "CMD::INSERT";
    case CMD::DOT:
        return "CMD::DOT";
    case CMD::SWITCH:
        return "CMD::SWITCH";
    case CMD::XD:
        return "CMD::XD";
    case CMD::SEARCH_FW:
        return "CMD::SEARCH_FW";
    case CMD::SEARCH_BW:
        return "CMD::SEARCH_BW";
    case CMD::REPLACE:
        return "CMD::REPLACE";
    case CMD::APPEND_LINES:
        return "CMD::APPEND_LINES";
    case CMD::QUIT:
        return "CMD::QUIT";
    case CMD::NONE:
        return "CMD::NONE";
    }
}