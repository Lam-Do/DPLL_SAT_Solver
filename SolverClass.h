#ifndef PROJECT_2_SOLVERCLASS_H
#define PROJECT_2_SOLVERCLASS_H

#include <vector>
#include <stack>
#include <tuple>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <climits>

class Clause;
class Literal;
struct Assignment;

class Literal {
public:
    const int id;
    bool isFree = true; // decide if the literal is free to assign new value
    bool value = false; // value of true or false, the literal always has a value during processing but consider has no value if it's free.
    std::unordered_set<Clause*> pos_occ;
    std::unordered_set<Clause*> neg_occ;
    int branching_level_dp;
    Clause* reason = nullptr; // the clause which has this as the last unset literal, use in unitPropagation to trace back necessary value for assigning.

    static int count;
    static std::unordered_map<int, Literal*> unorderedMap;
    static std::unordered_set<int> id_list;
    static std::queue<Literal*> unit_queue;

    explicit Literal(int id) : id(id) {
        count++;
        unorderedMap[this->id] = this;
        id_list.insert(id);
    };
    void setFree();
    void assignValue(bool,bool);
    void unassignValue();

    int getActualPosOcc(int);

    int getActualNegOcc(int);
};

class Clause {
public:
    const int id;
    std::vector<Literal*> pos_literals_list;
    std::vector<Literal*> neg_literals_list;
    std::unordered_set<Literal*> unset_literals = {};
    std::unordered_set<Literal*> sat_by = {};
    bool SAT = false;

    static int count;
    static std::vector<Clause*> list;
    static bool conflict;

    explicit Clause(int id) : id(id) {
        count++;
        list.push_back(this);
    };
    void appendLiteral(Literal*, bool);
    bool checkSAT();
    int getUnsetLiteralsCount() const;

};

struct Assignment {
    bool isForced;
    Literal* assigned_literal;

    Assignment(bool status, Literal* lit) : isForced(status), assigned_literal(lit) {stack.push(this);};

    static std::stack<Assignment*> stack;
};

#endif //PROJECT_2_SOLVERCLASS_H