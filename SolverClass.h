#ifndef PROJECT_2_SOLVERCLASS_H
#define PROJECT_2_SOLVERCLASS_H

#endif //PROJECT_2_SOLVERCLASS_H

#include <vector>
#include <stack>

class Clause;
class Literal;
struct Assignment;

class Literal {
public:
    const int id;
    int value = -1;
    bool isFree = true;
    std::vector<Clause*> pos_occ;
    std::vector<Clause*> neg_occ;
//    int branching_level_dp;
//    Clause* reason;

    static int count;
    static std::vector<Literal> list;

    explicit Literal(int id) : id(id) {
        count++;
        list.push_back(*this);
    };
    void setFree();

private:

};

class Clause {
public:
    std::vector<Literal*> literals_list;
    int active_literals_count = 0;
//    Literal* sat_by;

    static int count;
    static std::vector<Clause> list;

    Clause() {
        count++;
        list.push_back(*this);
    };
    void appendLiteral(Literal& literal);

private:

};

struct Assignment {
    bool isForced;
    Literal* assigned_literal;
    Assignment(bool status, Literal* lit) : isForced(status), assigned_literal(lit) {stack.push(*this);};
    static std::stack<Assignment> stack;
};