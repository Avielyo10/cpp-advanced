#include "Document.h"

Document::Document() {
    currentLine = -1;
}

// Prints the document
void Document::print() {
    if (currentLine < doc.size()) {
        for(std::vector<string>::iterator it = doc.begin(); 
            it != doc.end(); ++it) {
                cout << *it << endl;
        }
    }
}

// Print the line number, tab & the appropriate line in the document
// Set the current line in the document
void Document::xn(int numLine) {
    if (numLine < doc.size() + 1) {
        currentLine = numLine - 1;
        cout << numLine << "\t" << doc.at(currentLine) << endl;
    }
}

// Set the current line in the document
// Remove the appropriate line in the document
void Document::xd(int numLine) {
    if (numLine < doc.size() + 1) {
        currentLine = numLine - 1;
        doc.erase(doc.begin() + currentLine);
    }
}

// Search for some word in the document from the current line to the end 
// If found - print the line with the word & set the current line
// If not - set the current line to the end of the document
void Document::searchFw(string word) {
    for(std::vector<string>::iterator it = doc.begin() + currentLine;
        it != doc.end(); ++it) {
        string currentString = *it;
        if (isWordInLine(currentString, word)) {
            cout << *it << endl;
            break;
        }
        currentLine++;
    }
    if (currentLine > doc.size() - 1) {
        currentLine = doc.size() - 1;
    }
}

// Search for some word in the document from the current line to the beginning 
// If found - print the line with the word & set the current line
// If not - set the current line to the beginning of the document
void Document::searchBw(string word) {
    for(std::vector<string>::iterator it = doc.begin() + currentLine;
        it != doc.begin() - 1; --it) {
        string currentString = *it;
        if (isWordInLine(currentString, word)) {
            cout << *it << endl;
            break;
        }
        currentLine--;    
    }
    if (currentLine < 0) {
        currentLine = 0;
    }
}

// Append two lines and remove the latter line
void Document::appendLines(int line1, int line2) {
    int docLine1 = line1 - 1;
    int docLine2 = line2 - 1;
    doc.at(docLine1) = doc.at(docLine1) + " " + doc.at(docLine2);
    doc.erase(doc.begin() + docLine2);
    if (currentLine > doc.size() - 1) {
        currentLine = doc.size() - 1;
    }
}

// Append one or multiple lines after the current line
void Document::append(vector<string> miniDoc) {
    int i = currentLine + 1;
    for (string line: miniDoc) {
        doc.insert(doc.begin() + i, line);
        i++;
    }
}

// Insert one or multiple lines before the current line
void Document::insert(vector<string> miniDoc) {
    int i = currentLine;
    for (string line: miniDoc) {
        doc.insert(doc.begin() + i, line);
        i++;
    }
}

// Switch the current line with one or multiple lines
void Document::switchLines(vector<string> miniDoc) {
    doc.erase(doc.begin() + currentLine);
    if (currentLine > doc.size() - 1) {
        currentLine = doc.size() - 1;
    }
    insert(miniDoc); // use Document::insert
}

// Replace old word in the current line with new word
void Document::replaceInCurrentLine(string oldWord, string newWord) {
    string currentString = doc.at(currentLine);
    if (isWordInLine(currentString, oldWord)){
        currentString.replace(currentString.find(oldWord), oldWord.size(), newWord);
    }
    doc.at(currentLine) = currentString;
}

// Help method to check whether the word is in a line
bool Document::isWordInLine(string line, string word) {
    return line.find(word + " ") != string::npos 
            || line.find(" " + word) != string::npos;
}