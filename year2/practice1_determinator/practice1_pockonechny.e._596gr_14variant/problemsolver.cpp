#include <problemsolver.h>



void ProblemSolver::solveProblem()
{
    scanRegexpAndBuildNfa();

    printNfa();

    scanWordAndFindMaxSubword();
}

void ProblemSolver::scanRegexpAndBuildNfa()
{
    std::cin >> regExp;

    g.resize(2);
    addEdge(0, 1, regExp);
    processEdge(0, 0);
}

const bool isLetter(const unsigned char t) { return (t == 'a' || t == 'b' || t == 'c'); }
const bool isBinar (const unsigned char t) { return (t == '.' || t == '+'); }
const bool isUnar  (const unsigned char t) { return (t == '*'); }

unsigned char ProblemSolver::getOperands(const std::string regExp, std::string &operand1, std::string &operand2)
{
    unsigned char op = regExp.back();
    operand1 = operand2 = "";

    if (isUnar(op))
    {
        for (size_t i = 0; i < regExp.size() - 1; ++i)
            operand1 += regExp[i];

        return op;
    }

    int counter = 0;
    bool flag = true;
    for (int i = regExp.size() - 2; i >= 0; --i)
    {
        char symbol = regExp[i];
        if (isBinar(symbol))
            ++counter;
        else if (!isUnar(symbol))
            --counter;

        if (flag)
            operand2 = symbol + operand2;
        else
            operand1 = symbol + operand1;

        if (counter == -1)
            flag = false;
    }
    return op;
}

void ProblemSolver::processEdge(size_t source, size_t index)
{
    Edge edge = g[source][index];
    if (edge.regExp.length() < 3)
    {
        if (edge.regExp == "1")
            g[source][index].regExp = "~";
        return;
    }

    std::string A, B;
    char op = getOperands(edge.regExp, A, B);

    g[source][index].regExp = "";

    size_t newSource = 0;
    size_t newIndex = 0;

    if (op == '.')
        processOperatorDot(source, index, newSource, newIndex, A, B);
    else if (op == '+')
        processOperatorPlus(source, index, newSource, newIndex, A, B);
    else if (op == '*')
        processOperatorStar(source, index, newSource, newIndex, A);

    processEdge(newSource, newIndex);
    processEdge(source, index);
}

void ProblemSolver::processOperatorDot(const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                                       const std::string &operand1, const std::string &operand2)
{
    g.push_back(std::vector <Edge> ());
    newsource = (int)g.size() - 1;

    addEdge(newsource, g[source][index].to, operand2);

    g[source][index].regExp = operand1;
    g[source][index].to = newsource;
    newindex = g[newsource].size() - 1;
}

void ProblemSolver::processOperatorPlus(const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                                        const std::string &operand1, const std::string &operand2)
{
    addEdge(g[source][index].from, g[source][index].to, operand1);

    g[source][index].regExp = operand2;
    newsource = source;
    newindex = g[source].size() - 1;
}

void ProblemSolver::processOperatorStar(const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                                        const std::string &operand)
{
    addEdge(g[source][index].from, g[source][index].from, operand);

    g[source][index].regExp = "~";
    newsource = source;
    newindex = g[source].size() - 1;
}

void ProblemSolver::printNfa()
{
    std::cout << "NFA : " << std::endl;
    for (int i = 0; i < g.size(); ++i)
    {
        for (int j = 0; j < g[i].size(); ++j)
        {
            std::cout << "edge : " << g[i][j].from << " - " << g[i][j].to << " ; exp = '" << g[i][j].regExp << "'" << std::endl;
        }
    }
    std::cout << "____________________" << std::endl;
}

void ProblemSolver::addEdge(const Edge n)
{
    g[n.from].push_back(n);
}

void ProblemSolver::addEdge(const size_t _from, const size_t _to, const std::string _regexp)
{
    g[_from].push_back(Edge(_from, _to, _regexp));
}

void ProblemSolver::scanWordAndFindMaxSubword()
{
    std::string word;
    std::cin >> word;

    int answer = 0;

    for (size_t it1 = 0; it1 < word.length(); ++it1)
    {
        for (size_t it2 = it1; it2 < word.length(); ++it2)
        {
            if (it2 - it1 + 1 > answer)
            {
                subword = word.substr(it1, it2 - it1 + 1);

                for (size_t nodeIt = 0; nodeIt < g.size(); ++nodeIt)
                {
                    bool success = tryToAcceptWordWithNfa(subword, nodeIt);
                    if (success && it2 - it1 + 1 > answer)
                        answer = it2 - it1 +  1;
                }
            }
        }
    }

    std::cout << "Max len of subword = " << answer << std::endl;
}

bool ProblemSolver::tryToAcceptWordWithNfa(const std::string &word, const size_t startNode)
{
    AdvancedSet set;
    set.set.clear();
    st.clear();

    dfs(startNode, 0, set);

    return !set.set.empty();
}

void ProblemSolver::dfs(size_t v, size_t index, AdvancedSet &final)
{
    if (index >= subword.length())
    {
        final.enable(v);
        return;
    }

    std::pair<size_t, size_t> p(v, index);
    if (!st.count(p))
    {
        st.insert(p);
        for (size_t i = 0; i < g[v].size(); ++i)
        {
            size_t newaim = g[v][i].to;

            if (g[v][i].regExp[0] == subword[index])
            {
                dfs(newaim, index + 1, final);
            }
            else if (g[v][i].regExp[0] == '~')
            {
                dfs(newaim, index, final);
            }
        }
    }
}
