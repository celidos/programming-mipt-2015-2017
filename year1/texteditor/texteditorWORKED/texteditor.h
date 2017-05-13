#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <stack>
#include <string>

using namespace std;

enum ActionType {ADDED, NEWLINE, DELETED, STRCONCAT};

struct TextAction
{
    int str;    // № строки
    int index;  // № символа
    ActionType act; // действие
    char del_ch;
    TextAction(ActionType AType, int Str = 0, int Index = 0, char Deleted = 0)
    {
        act = AType;
        str = Str;
        index = Index;
        del_ch = Deleted;
    }
};

class TextEditor
{
private:
    vector < string > text;    // здесь лежит наш текст
    stack <TextAction> aStack;      // стек с действиями
    stack <TextAction> backStack;   // стек с действиями

    int cursor_str;
    int cursor_ind;

    void add_symbol(char ch);
    void divide_strings();
    void delete_symbol();
    void concat_strings();


    void do_backspace();
    void undo();
    void redo();

    void print();
public:
    TextEditor(){};
    ~TextEditor(){};

    void Solve(const char * filename);
};

#endif // TEXTEDITOR_H

