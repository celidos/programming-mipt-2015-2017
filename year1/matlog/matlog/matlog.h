#ifndef MATLOG
#define MATLOG

#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>

#define MAX_THEOREMS 5
#define MIN_THEOREMS 2
#define MAX_HARDNESS 10
#define MIN_HARDNESS 1
#define MAX_STRING_LEN 15
#define MIN_STRING_LEN 3
#define MIN_THEOREM_LEN 1
#define MAX_THEOREM_LEN 20
#define INCREASE_TRIGGER 5


#define CHANCE_BRACKET 6 // 1 = всегда; чем больше, тем меньше вероятность
#define CHANCE_CONTRADICTION 3
#define CHANCE_INTRODUCE_WORDS 3

#define MI_ATTITUDE 26
#define MI_LETTERS 26
#define MI_INTRODUCEWORDS 9


using namespace std;

string attitude[] = {"<", ">", "=", "\\le", "\\ge", "\\equiv", "\\prec"/**/,
    "\\succ", "\\sim", "\\preceq", "\\succeq", "\\simeq", "\\ll", "\\gg",
    "\\approx", "\\subset"/**/, "\\supset"/**/, "\\subseteq", "\\supseteq", "\\doteq",
    "\\sqsubset", "\\sqsupset", "\\in", "\\ni", "\\vdash" /**/, "\\dashv"};

string letters[] =
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
    "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
};

string introducewords[] = {
    "Заметим, что", "Очевидно, что", "Нетрудно показать, что", "Так как", "Но", "В силу условия", "Неверно, что",
    "Из вышесказанного", "Учитывая предыдущее утверждение,", "Применяя аксиому выборки,"
};

class MatlogGenerator
{
private:
    string GenerateTheorem(int num, int len, int hardness);
    string GenerateString(int len, bool start, int state);

public:
    string GenerateLection();
};

void InitRandom();
int  rnd(int from, int to);

#endif // MATLOG

