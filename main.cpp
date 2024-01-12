#include <iostream>
#include "SolverClass.h"

using namespace std;

// Declare static variable
int Literal::count = 0;
int Clause::count = 0;
unordered_map<int, Literal*> Literal::unorderedMap = {};
vector<Clause*> Clause::list = {};
stack<Assignment*> Assignment::stack = {};
unordered_set<int> Literal::id_list = {};
queue<Literal*> Literal::unit_queue= {};
bool Clause::conflict = false;

// Declare function
vector<vector<int>> readDIMACS(string& path);
void parse(const vector<vector<int>>& formula);
void unitPropagation();
void backtracking();
void pureLiteralsEliminate();
void branching();
void simplify();
void removeSATClauses();

tuple<Literal *, bool> heuristicMOM();

// Global definition
bool isForced = true;
bool isSAT = false;
bool isUNSAT = false;

int main() {
    vector<vector<int>> formula = {
            {1, 2, 3},
            {-1, -2, 3},
            {1, -2, 3}
    };
    //read DIMACS file, return formula in vector<vector<int>>

    // parse formula into data structure
    parse(formula);
    cout << Literal::count << " " << Clause::count << endl;

    return 0;
}

vector<vector<int>> readDIMACS(string& path) {
    vector<vector<int>> formula;

    return formula;
}

void parse(const vector<vector<int>>& formula) {
    for (const auto& c : formula){
        Clause new_clause(Clause::count);
        for (const auto l : c) {
            if (Literal::id_list.count(abs(l)) == 0) {
                if (l >= 0) {
                    Literal new_literal (abs(l));
                    // connecting literals and clauses
                    new_literal.pos_occ.insert(&new_clause);
                    new_clause.appendLiteral(&new_literal, true);
                } else {
                    Literal literal (abs(l));
                    // connecting literals and clauses
                    literal.neg_occ.insert(&new_clause);
                    new_clause.appendLiteral(&literal, false);
                }
            } else {
                Literal* current_literal = Literal::unorderedMap[abs(l)];
                if (l >= 0) {
                    current_literal->pos_occ.insert(&new_clause);
                    new_clause.appendLiteral(current_literal, true);
                } else {
                    current_literal->neg_occ.insert(&new_clause);
                    new_clause.appendLiteral(current_literal, false);
                }
            }
        }
    }
}

void unitPropagation() {
    // find and propagate all literal in queue by assigning value
    while (!(Literal::unit_queue.empty())) {
        Literal* next_literal = Literal::unit_queue.front();
        Literal::unit_queue.pop();
        Clause* unit_clause = next_literal->reason;
        // check if the literal is positive or negative in the unit clause to assign fitting value
        if (find(unit_clause->pos_literals_list.begin(), unit_clause->pos_literals_list.end(), next_literal) != unit_clause->pos_literals_list.end()) {
            next_literal->assignValue(true, isForced);
        } else next_literal->assignValue(false, isForced) ;
    }
}

void backtracking() {
    // Backtracking in case conflict flag
    while (!Assignment::stack.empty() && Assignment::stack.top()->isForced) {
        Assignment::stack.top()->assigned_literal->unassignValue();
        Assignment::stack.pop();
    }
    if (!Assignment::stack.empty()) {
        // Save value of the top assignment before assigning new one which push a new assignment to top of stack
        Literal* top_literal = Assignment::stack.top()->assigned_literal;
        bool old_value = top_literal->value;

        top_literal->unassignValue();
        Assignment::stack.pop();

        while (!Literal::unit_queue.empty()) {
            Literal::unit_queue.pop();
        }

        top_literal->assignValue(!old_value, isForced); // no need to push new assignment here since assignValue() does it.
        Clause::conflict = false; // remove conflict flag
    } else {
        isUNSAT = true; // flag UNSAT in case stack is empty meaning all assignments is forced and there isn't any another branch
    }
}


void pureLiteralsEliminate() {
    // assign value to all pure literals with forced assignment, pureLit can appear after clauses are SAT and remove from consideration.
    for (const auto& id2ad : Literal::unorderedMap) {
        Literal* l = id2ad.second;
        if (l->isFree) {
            if (l->getActualPosOcc(INT_MAX) == 0) {
                l->assignValue(false, isForced);
            } else if (l->getActualNegOcc(INT_MAX) == 0) {
                l->assignValue(true, isForced);
            }
        }
    }
}

void branching() {
    // branching in case unit_queue is empty (no unit clause) and also no conflict, SAT or UNSAT flag
    if (!isSAT && !isUNSAT && Literal::unit_queue.empty() && !Clause::conflict) {
        tuple<Literal*, bool> t = heuristicMOM(); // use MOM heuristic to choose branching literal
        std::get<0>(t)->assignValue(std::get<1>(t), !isForced);
    }
}

std::tuple<Literal*, bool> heuristicMOM() {
    // check all clauses for the shortest
    Clause* shortest_clause = nullptr;
    int shortest_width = INT_MAX;
    for (auto c : Clause::list) {
        int clause_actual_width = c->unset_literals.size();
        if (!c->SAT && clause_actual_width < shortest_width) {
            shortest_width = clause_actual_width;
            shortest_clause = c;
        }
    }

    //choose literal
    Literal* chosen_literal = nullptr;
    int n = INT_MIN;
    bool value = true;
    for (auto l : shortest_clause->unset_literals) {
        int actual_pos_occ = l->getActualPosOcc(shortest_width); // get number occ of literal in clauses with the exact shortest_width
        int actual_neg_occ = l->getActualNegOcc(shortest_width);
        int v = (actual_pos_occ + actual_neg_occ) * 2 ^ 1 + actual_pos_occ * actual_neg_occ;
        if (v > n) {
            n = v;
            chosen_literal = l;
            value = (actual_pos_occ >= actual_neg_occ) ? true : false;
        }
    }
    return std::make_tuple(chosen_literal, value);
}

template<typename T>
unordered_set<T> findIntersection(const unordered_set<T>& s1, unordered_set<T>& s2) {
    unordered_set<T> intersection;

    for (const T& e : s1) {
        if (s2.find(e) != s2.end()) {
            intersection.insert(e);
        }
    }
    return intersection;
}

void simplify() {
    // TODO: impliment simplify technique here
    removeSATClauses();
}

void removeSATClauses(){
    // check basic SAT condition
    // check a clause contain a literal both pos and neg
    for (const auto& id2ad : Literal::unorderedMap) {
        Literal* literal = id2ad.second;
        // a literal appear both pos and neg in a clause, that clause is alway SAT, can remove from the process.
        unordered_set<Clause*> intersect = findIntersection(literal->pos_occ, literal->neg_occ);
        if (!intersect.empty()) {
            for (auto c : intersect) {
                Clause::list.erase(Clause::list.begin() + c->id - 1);
                // erase in all connected literals
                for (auto l : c->pos_literals_list) {
                    l->pos_occ.erase(c);
                }
                for (auto l : c->neg_literals_list) {
                    l->neg_occ.erase(c);
                }
            }
        }
    }
}