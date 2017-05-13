#ifndef PROBLEMSOLVER_H
#define PROBLEMSOLVER_H

#include <iostream>
#include <unordered_set>
#include <set>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

using std::size_t;

const size_t MAX_LETTERS_IN_SIGMA = 3;

class AdvancedSet
{
public:

    AdvancedSet() = default;
    ~AdvancedSet() = default;

    void enable(const size_t index)
    {
        set.insert(index);
    }

    void disable(const size_t index)
    {
        set.erase(index);
    }

    size_t size() const
    {
        return set.size();
    }

    bool operator ==(const AdvancedSet &b) const
    {
        if (this->size() != b.set.size())
            return false;

        return this->set == b.set;
    }

    bool inter(const AdvancedSet &b)
    {
        for (auto it = this->set.begin(); it != this->set.end(); ++it)
            if (b.set.count(*it))
                return true;

        return false;
    }

    void print()
    {
        std::cout << "set:[ ";
        for (auto it = this->set.begin(); it != this->set.end(); ++it)
            std::cout << *it << " ";
        std::cout << "]" << std::endl;
    }

    std::unordered_set<size_t> set;
};

// -----------------------------------------------------

class Edge
{
public:
    Edge(size_t _from, size_t _to, std::string _regexp) :
        from(_from), to(_to), regExp(_regexp)
    { }

    size_t from;
    size_t to;
    std::string regExp;
};

class ProblemSolver
{
public:
    void solveProblem();
    void printNfa();

private:
    void scanRegexpAndBuildNfa();

    unsigned char getOperands(const std::string reg, std::string &operand1, std::string &operand2);
    void processEdge(size_t source, size_t index);

    void processOperatorDot (const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                             const std::string &operand1, const std::string &operand2);
    void processOperatorPlus(const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                             const std::string &operand1, const std::string &operand2);
    void processOperatorStar(const size_t source, const size_t index, size_t &newsource, size_t &newindex,
                             const std::string &operand);

    void addEdge(const Edge n);
    void addEdge(const size_t _from, const size_t _to, const std::string _regexp);

    void scanWordAndFindMaxSubword();
    bool tryToAcceptWordWithNfa(const std::string &word, const size_t startNode);
    void dfs(size_t v, size_t index, AdvancedSet &final);

    std::string regExp;
    std::vector<std::vector <Edge> > g;

    std::string subword;
    std::set<std::pair<size_t, size_t> > st;
};




#endif // PROBLEMSOLVER_H
