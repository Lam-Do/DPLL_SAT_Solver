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

// Declare function
void readDIMACS(string& path);
void parse(const vector<vector<int>>& formula);

int main() {
    vector<vector<int>> formula = {
            {1, 2, 3},
            {-1, -2, 3},
            {1, -2, 3}
    };
    //read DIMACS file, return formula in vector<vector<int>>

    // parse formula into data structure
    parse(formula);
    cout << Literal::count << " " << Clause::count;

    return 0;
}

void parse(const vector<vector<int>>& formula) {
    for (const auto& c : formula){
        Clause new_clause;
        for (const auto& l : c) {
            if (Literal::id_list.count(abs(l)) == 0) {
                if (l >= 0) {
                    Literal literal (abs(l));
                    literal.pos_occ.insert(&new_clause);
                    new_clause.appendLiteral(&literal, true);
                } else {
                    Literal literal (abs(l));
                    literal.neg_occ.insert(&new_clause);
                    new_clause.appendLiteral(&literal, false);
                }
            }
        }
    }
}
