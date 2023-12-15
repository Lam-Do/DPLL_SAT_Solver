#ifndef PROJECT_2_SOLVERCLASS_H
#define PROJECT_2_SOLVERCLASS_H

#include <vector>
#include <stack>
#include <tuple>
#include <unordered_set>

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
//    int branching_level_dp;
//    Clause* reason;

    static int count;
    static std::vector<Literal> list;
    static std::unordered_set<int> id_list;

    explicit Literal(int id) : id(id) {
        count++;
        list.push_back(*this);
        id_list.insert(id);
    };
    void setFree();
    void assignValue(bool);

private:

};

class Clause {
public:
    std::vector<Literal*> pos_literals_list;
    std::vector<Literal*> neg_literals_list;
    int active_literals_count = 0;
//    Literal* sat_by;

    static int count;
    static std::vector<Clause> list;

    Clause() {
        count++;
        list.push_back(*this);
    };
    void appendLiteral(Literal*, bool);
    bool isSAT();
private:

};

struct Assignment {
    bool isForced;
    Literal* assigned_literal;

    Assignment(bool status, Literal* lit) : isForced(status), assigned_literal(lit) {stack.push(*this);};

    static std::stack<Assignment> stack;
};

#endif //PROJECT_2_SOLVERCLASS_H