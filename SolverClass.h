#ifndef PROJECT_2_SOLVERCLASS_H
#define PROJECT_2_SOLVERCLASS_H

#include <vector>
#include <stack>
#include <tuple>
#include <unordered_set>
#include <queue>
#include <algorithm>

class Clause;
class Literal;
struct Assignment;

class Literal {
public:
    const int id;
    bool value = false;
    bool isFree = true;
    std::unordered_set<Clause*> pos_occ;
    std::unordered_set<Clause*> neg_occ;
    int branching_level_dp;
    Clause* reason;

    static int count;
    static std::vector<Literal> list;
    static std::unordered_set<int> id_list;
    static std::queue<Literal*> unit_queue;

    explicit Literal(int id) : id(id) {
        count++;
        list.push_back(*this);
        id_list.insert(id);
    };
    void setFree();
    void assignValue(bool);
    void unassignValue();

private:

};

class Clause {
public:
    std::vector<Literal*> pos_literals_list;
    std::vector<Literal*> neg_literals_list;
    std::unordered_set<Literal*> unset_literals = {};
    std::vector<Literal*> sat_by = {};
    bool SAT = false;

    static int count;
    static std::vector<Clause> list;

    Clause() {
        count++;
        list.push_back(*this);
    };
    void appendLiteral(Literal*, bool);
    bool checkSAT();
    int getUnsetLiteralsCount() const;

};

struct Assignment {
    bool isForced;
    Literal* assigned_literal;

    Assignment(bool status, Literal* lit) : isForced(status), assigned_literal(lit) {stack.push(*this);};

    static std::stack<Assignment> stack;
};

#endif //PROJECT_2_SOLVERCLASS_H