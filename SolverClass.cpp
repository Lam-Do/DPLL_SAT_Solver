#include <iostream>
#include <ostream>
#include "SolverClass.h"

void Literal::setFree() {
    this->isFree = true;
}

void Literal::assignValue(bool value, bool isForced) {
    std::cout << "Assign literal " << this->id << " value " << value << "\n";
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
                    auto free_literal = *(clause->unset_literals.begin()); // Last unset literal of this clause after assign this literal
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
        if (!c->SAT && c->unset_literals.size() <= w) {
            occ++;
        }
    }
    return occ;
}

int Literal::getActualNegOcc(int w) {
    int occ = 0;
    for (auto c : this->neg_occ) {
        if (!c->SAT && c->unset_literals.size() <= w) {
            occ++;
        }
    }
    return occ;
}

void Literal::printData() {
    std::cout << "Literal " << this->id << " at " << this <<" -";
    if (this->isFree) std::cout << " free";
    else std::cout << " assigned";
    std::cout << " - pos_occ:";
    for (auto c : this->pos_occ) {
        std::cout << " " << c->id << ",";
    }
    std::cout << " - neg_occ:";
    for (auto c : this->neg_occ) {
        std::cout << " " << c->id << ",";
    }
    if (this->reason == nullptr) std::cout << " - satisfy no clause " << std::endl;
    else std::cout << " - satisfy clause " << this->reason->id << std::endl;
}

void Literal::updateStaticData() {
    Literal::count++;
    Literal::unorderedMap[this->id] = this;
    Literal::id_list.insert(id);
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
    for (const auto& c : Clause::list) {
        if (!c->SAT) {return false;}
    }
    return true;
}

int Clause::getUnsetLiteralsCount() const {return this->unset_literals.size();}

void Assignment::printAll() {
    std::stack<Assignment*> s = Assignment::stack;
    while (!s.empty()) {
        Literal* l = s.top()->assigned_literal;
        std::cout << "Literal " << l->id << ": " << l->value << " by ";
        if (s.top()->isForced) {std::cout << "forcing" << "\n";}
        else {std::cout << "branching" << "\n";}
        s.pop();
    }
}

void Clause::printData() {
    std::cout << "Clause " << this->id << " at " << this <<" -";;
    std::cout << " pos_literals:";
    for (auto l : this->pos_literals_list) {
        std::cout << " " << l->id << ",";
    }
    std::cout << " - neg_literals:";
    for (auto l : this->neg_literals_list) {
        std::cout << " -" << l->id << ",";
    }
    std::cout << " - current unassigned literals:";
    for (auto l : this->unset_literals) {
        std::cout << " " << l->id << ",";
    }
    if (this->SAT) {
        std::cout << " - satisfy by:";
        for (auto l : this->sat_by) {
            std::cout << " " << l->id << ",";
        }
    } else std::cout << " - UNSAT" << std::endl;
}

void Clause::updateStaticData() {
    Clause::count++;
    Clause::list.emplace_back(this);
}
