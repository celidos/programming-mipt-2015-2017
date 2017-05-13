#ifndef POLISH_NOTATION_H_INCLUDED
#define POLISH_NOTATION_H_INCLUDED

#include <string>
#include <vector>
#include <stack>
#include <cstdio>
#include <cmath>
#include <iostream>

#define RPN_NUM  1          // константа, означающая: то, что лежит в стеке, есть число

#define DELTA_ZERO 1e-30    // числа, меньшие этой константы, считаются нулем

using namespace std;

struct rpn_stack_elem
{
    char op;            // Определяет тип содержимого: число или операция
    float x;            //  Если число, то чему оно равно
    bool rpriority;     // Если операция, является ли она правосторонней
};

class PolishRecordMaster
{
public:
    float Resolve(string req, bool &ok);
private:
    stack <rpn_stack_elem> RPN_stack;

    bool is_operation(char t);              // Проверяет, является ли символ операцией
    int get_priority(char op);              // Получает приоритет операции
    bool is_right_op(char op);              // Проверяет, является ли символ ПРАВОСТОРОННЕЙ операцией
    bool is_uno(char op);                   // Проверяет, является ли символ УНАРНОЙ операцией
    bool is_number(char t);                 // Проверяет, является ли символ ЦИФРОЙ
    bool more(char op1, char op2);          // Сравнивает приоритеты операций, строго
    bool more_or_eq(char op1, char op2);    // Сравнивает приоритеты операций, нестрого
    bool hasFractionalPart(float val);      // Проверяет, есть ли у числа дробная часть
    float quick_power(float x, int n);      // Быстрое возведение в натуральную степень
    float read_float_from_pos(string &s, int pos, int &nextpos);            // Считывает число с позиции в строке
    void print_rpn_stack(stack <rpn_stack_elem> ST);
    bool get_rpn(string req);               // Получает обратную польскую запись
    float rpn_count(bool &ok);              // Считает значение этой записи
};

#endif // POLISH_NOTATION_H_INCLUDED
