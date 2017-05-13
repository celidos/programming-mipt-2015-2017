#include <matlog.h>

void InitRandom()
{
    srand( time( 0 ) );
}

int rnd(int from, int to)
{
    return rand() % (to - from + 1) + from;
}

string MatlogGenerator::GenerateTheorem(int num, int len, int hardness)
{
    stringstream str1;
    str1 << num;
    string res = "{\\bf{Теорема " + str1.str() + ".}} ";
    for (int i = 0; i < len; ++i)
    {
        int strlen = rnd(MIN_STRING_LEN, MAX_STRING_LEN);
        if (MAX_STRING_LEN - strlen < INCREASE_TRIGGER)
            strlen += rnd(MIN_STRING_LEN, MAX_STRING_LEN);
        res += GenerateString(strlen, true, 0);
    }

    if (!rnd(0, CHANCE_CONTRADICTION))
    {
        res += "Получили противоречие" + (rnd(0, 1) ? "." : " с условием $" + GenerateString(rnd(5, 9), false, 1)) + "$";
    }

    res += "\n\n";
    //res += "SDFSDFSDFSDFSDF\n";

    return res;
}

string MatlogGenerator::GenerateString(int len, bool start, int state)
{
    string res;

    if (len < 2)
        return letters[rnd(0, MI_LETTERS-1)];

    if (start)
    {
        if (state == 0)
        {
            int op = rnd(1, CHANCE_INTRODUCE_WORDS);
            if (op == 1)
                res += introducewords[rnd(0,MI_INTRODUCEWORDS-1)] + " $" + GenerateString(len, false, state) + "$";

            else
            {
                res += "$" + GenerateString(len, false, state) + "$";
            }
        }
        else if (state == 1)
        {

        }
        else
        {

        }
        res += "\n\n";
    }
    else
    {
        int inbox1 = rnd(0, CHANCE_BRACKET),
            inbox2 = rnd(0, CHANCE_BRACKET);

        int randfactor1 = rnd(1, len);

        res += (!inbox1 ? "(" : "") + GenerateString(randfactor1, false, state) +
               (!inbox1 ? ")" : "") + attitude[rnd(0, MI_ATTITUDE-1)] + " " +
               (!inbox2 ? "(" : "") + GenerateString(len - randfactor1, false, state) +
               (!inbox2 ? ")" : "");
    }



    return res;

}

string MatlogGenerator::GenerateLection()
{
    InitRandom();

    string res;

    res =
"\\documentclass[12pt]{article}\n\
\\usepackage[utf8]{inputenc}\n\
\\usepackage[russian]{babel}\n\
\\usepackage{amssymb,amsmath}\n\
\\textheight=24cm\n\
\\textwidth=16cm\n\
\\oddsidemargin=0pt\n\
\\topmargin=-1.5cm\n\
\\parindent=24pt\n\
\\parskip=0pt\n\
\\tolerance=200\n\
\\flushbottom\n\
\\title{MATLOG}\n\
\\author{\\copyright~~К. В. Воронцов}\n\
\\date{30 мая 2005}\n\
\\begin{document}\n\
\\maketitle % вывести заголовок, автора, дату\n\
\\thispagestyle{empty} % не нумеровать первую страницу\n\
\\begin{abstract} % начало аннотации\n\
Лекция ололо\n\
\\end{abstract} % конец аннотации\n\
";

    int theorems = rnd(MIN_THEOREMS, MAX_THEOREMS);
    for (int i = 0; i < theorems; ++i)
    {
        int theoremlen = rnd(MIN_THEOREM_LEN, MAX_THEOREM_LEN);
        res += GenerateTheorem(i+1, theoremlen, 1);
    }

    res += "\\end{document}";

    return res;
}

