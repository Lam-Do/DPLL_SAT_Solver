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
bool Clause::conflict = false;

// Declare function
vector<vector<int>> readDIMACS(string& path);
void parse(const vector<vector<int>>& formula);
void unitPropagation();
void backtracking();
void simplify();
void pureLiteralsEliminate();
void checkBasicUNSAT();

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
    // find and propagate all literal in queue by assigning value
    while (!(Literal::unit_queue.empty())) {
        Literal* next_literal = Literal::unit_queue.front();
        Literal::unit_queue.pop();
        Clause* unit_clause = next_literal->reason;
        // check if the literal is positive or negative in the unit clause to assign fitting value
        if (find(unit_clause->pos_literals_list.begin(), unit_clause->pos_literals_list.end(), next_literal) != unit_clause->pos_literals_list.end()) {
            next_literal->assignValue(true);
        } else next_literal->assignValue(false) ;
    }
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

void checkBasicUNSAT(){
    // TODO: check basic unSAT condition
    // TODO: check a clause contain a literal both pos and neg
}