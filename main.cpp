#include <iostream>
#include "SolverClass.h"

using namespace std;

// Declare static variable
int Literal::count = 0;
int Clause::count = 0;
vector<Literal> Literal::list = {};
vector<Clause> Clause::list = {};
stack<Assignment> Assignment::stack = {};
unordered_set<int> Literal::id_list = {};
queue<Literal*> Literal::unit_queue= {};

// Declare function
void readDIMACS(string& path);
void parse(const vector<vector<int>>& formula);
void unitPropagation();
void backtracking();
void simplify();
void pureLiteralsEliminate();

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

void parse(const vector<vector<int>>& formula) {
    for (const auto c : formula){
        Clause new_clause;
        for (const auto l : c) {
            if (Literal::id_list.count(abs(l)) == 0) {
                if (l >= 0) {
                    Literal literal (abs(l));
                    // connecting literals and clauses
                    literal.pos_occ.insert(&new_clause);
                    new_clause.appendLiteral(&literal, true);
                } else {
                    Literal literal (abs(l));
                    // connecting literals and clauses
                    literal.neg_occ.insert(&new_clause);
                    new_clause.appendLiteral(&literal, false);
                }
            }
        }
    }
}

void unitPropagation() {
    // TODO: find and propagate
}

void backtracking() {
    // TODO: Undo assignment
}

void simplify() {
    // TODO: impliment simplify technique here
}

void pureLiteralsEliminate() {
    // TODO: assign value to all pure literal with forced assignment
}
