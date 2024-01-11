#include "SolverClass.h"

void Literal::setFree() {
    this->isFree = true;
}

void Literal::assignValue(bool value, bool isForced) {
    //assign value and free status
    if (!this->isFree) {
        // literals could be pushed to queue more than once when they are the last unset literal of more than one clause.
        // do nothing, skip assigning value process if the literal is not free
    } else {
        this->isFree = false;
        this->value = value;
        Assignment new_assign (isForced, this);

        // change related clauses accordingly
        //std::unordered_set<Clause*> sat_clauses;
        //std::unordered_set<Clause*> unsat_clauses;
        if (value == true) {
            for (auto clause : this->pos_occ) {
                clause->unset_literals.erase(this);
                clause->SAT = true;
                clause->sat_by.insert(this);
            }
            for (auto clause : this->neg_occ) {
                clause->unset_literals.erase(this);
                if (clause->getUnsetLiteralsCount() == 1 && !clause->SAT) {
                    auto free_literal = *(clause->unset_literals.begin()); // Last unset literal in the unorderedMap
                    Literal::unit_queue.push(free_literal);
                    free_literal->reason = clause;
                }
                if (clause->getUnsetLiteralsCount() == 0 && !clause->SAT) {
                    //report conflict when a clause has no free literal but still UNSAT
                    Clause::conflict = true;
                }
            }
        } else {
            for (auto clause : this->neg_occ) {
                clause->unset_literals.erase(this);
                clause->SAT = true;
                clause->sat_by.insert(this);
            }
            for (auto clause : this->pos_occ) {
                clause->unset_literals.erase(this);
                if (clause->getUnsetLiteralsCount() == 1 && !clause->SAT) {
                    auto free_literal = *(clause->unset_literals.begin());
                    Literal::unit_queue.push(free_literal); //
                    free_literal->reason = clause;
                }
                if (clause->getUnsetLiteralsCount() == 0 && !clause->SAT) {
                    // check SAT status, if unSAT report conflict
                    Clause::conflict = true;
                }
            }
        }
    }
}

void Literal::unassignValue() {
    this->setFree();
    this->reason = nullptr;
    if (this->value == true) {
        for (auto clause : this->pos_occ) {
            clause->sat_by.erase(this);
            if (clause->sat_by.empty()) {
                clause->SAT = false;
            }
            clause->unset_literals.insert(this);
        }
        for (auto clause : this->neg_occ) {
            clause->unset_literals.insert(this);
        }
    } else {
        for (auto clause : this->neg_occ) {
            clause->sat_by.erase(this);
            if (clause->sat_by.empty()) {
                clause->SAT = false;
            }
            clause->unset_literals.insert(this);
        }
        for (auto clause : this->pos_occ) {
            clause->unset_literals.insert(this);
        }
    }
}

int Literal::getActualPosOcc(int w) {
    int occ = 0;
    for (auto c : this->pos_occ) {
        if (!c->SAT && c->unset_literals.size() == w) {
            occ++;
        }
    }
    return occ;
}

int Literal::getActualNegOcc(int w) {
    int occ = 0;
    for (auto c : this->neg_occ) {
        if (!c->SAT && c->unset_literals.size() == w) {
            occ++;
        }
    }
    return occ;
}

void Clause::appendLiteral(Literal* literal_ad, bool isPos) {
    if (isPos) {
        this->pos_literals_list.push_back(literal_ad);
    } else {
        this->neg_literals_list.push_back(literal_ad);
    }
    this->unset_literals.insert(literal_ad);
}

bool Clause::checkSAT() {
    for (const auto literal : this->pos_literals_list) {
        if (!(literal->isFree)) {
            if (literal->value) return true;
        }
    }
    for (const auto literal : this->neg_literals_list) {
        if (!(literal->isFree)){
            if (!(literal->value)) return true;
        }
    }
    return false;
}

int Clause::getUnsetLiteralsCount() const {return this->unset_literals.size();}
