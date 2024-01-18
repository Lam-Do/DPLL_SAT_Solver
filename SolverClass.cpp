#include <iostream>
#include <ostream>
#include <algorithm>
#include "SolverClass.h"

void Literal::setFree() {
    this->isFree = true;
}
/**
 * Assign a value to the literal.
 * A New object of assignment class will also be creat here.
 * All associated data will be update accordingly.
 * After data updated, new appear unit UNSAT clauses will have the last literal push to unit_queue.
 * Clauses with no free literal left but still UNSAT will trigger conflict flag.
 * @param value Value assign to the literal
 * @param isForced "true" if by force or "false" if branching
 */
void Literal::assignValue(bool value, bool isForced) {
    //assign value and free status
    // literals could be pushed to unit_queue more than once when they are the last unset literal of more than one UNSAT clauses.
    // do nothing, skip assigning value process if the literal is not free
    if (this->isFree == true) {
        this->isFree = false;
        this->value = value;

        Assignment* new_assign = new Assignment(isForced, this);

        // change data in related clauses accordingly to occurrence
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

/**
 * Unassigning value the literal.
 * Data of related clauses with be updated. Clauses will not be set to UNSAT as long as there is a literal in sat_by list.
 */
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
/**
 * Counting all positive occurrence in UNSAT clauses with number of free literal less or equal w, by the time called.
 * Occurrences in SAT clauses will not be counted.
 * @param w  Maximal number of free literals of clauses in pos_occ. If bigger than actual number of clause's size. Simply count all.
 * @return  Number of occurrence
 */
int Literal::getActualPosOcc(int w) {
    int occ = 0;
    for (auto c : this->pos_occ) {
        if (!c->SAT && c->unset_literals.size() <= w) {
            occ++;
        }
    }
    return occ;
}
/**
 * Counting all negtive occurrence in UNSAT clauses with number of free literal less or equal w, by the time called.
 * Occurrences in SAT clauses will not be counted.
 * @param w Maximal number of free literals of clauses in neg_occ. If bigger than actual number of clause's size. Simply count all.
 * @return Number of occurrence
 */
int Literal::getActualNegOcc(int w) {
    int occ = 0;
    for (auto c : this->neg_occ) {
        if (!c->SAT && c->unset_literals.size() <= w) {
            occ++;
        }
    }
    return occ;
}

/**
 * Print all data saved by this instances of class Literal.
 */
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

/**
 * Update number of literals, unordered map literal's id to adress and list of id.
 */
void Literal::updateStaticData() {
    Literal::count++;
    Literal::unorderedMap[this->id] = this;
    Literal::id_list.insert(id);
}

/**
 * Save literal to clause's positive and negative literal list accordingly, and also all to free literals list.
 *
 * @param literal_ad Pointer to literal
 * @param isPos "true" if positive literal, "false" otherwise
 */
void Clause::appendLiteral(Literal* literal_ad, bool isPos) {
    if (isPos) {
        this->pos_literals_list.push_back(literal_ad);
    } else {
        this->neg_literals_list.push_back(literal_ad);
    }
    this->unset_literals.insert(literal_ad);
}

/**
 * Check if all clauses are SAT
 * @return true if all clauses are SAT, false otherwise.
 */
bool Clause::checkSAT() {
    for (const auto& c : Clause::list) {
        if (!c->SAT) {return false;}
    }
    return true;
}

/**
 * Get number of free literals in the clause.
 * @return Number of free literals
 */
int Clause::getUnsetLiteralsCount() const {return this->unset_literals.size();}

/**
 * Print all data saved by this instances of class Clause.
 */
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

/**
 * Update number of clauses and list of clauses.
 */
void Clause::updateStaticData() {
    Clause::count++;
    Clause::list.emplace_back(this);
}

/**
 * Print all assignment in the stack without changing the stack.
 */
void Assignment::printAll() {
    std::stack<Assignment*> s = Assignment::stack;
    std::stack<Assignment*> reversed_stack;
    while (!s.empty()) {
        reversed_stack.push(s.top());
        s.pop();
    }
    Assignment::assignment_history.emplace_back(reversed_stack);
    while (!reversed_stack.empty()) {
        Literal* l = reversed_stack.top()->assigned_literal;
        std::cout << "[" << l->id << "|" << l->value << "|";
        if (reversed_stack.top()->isForced) {std::cout << "f]";}
        else {std::cout << "b]";}
        std::cout << "-";
        reversed_stack.pop();
    }
    std::cout<<std::endl;
}

/**
 * Print all assignments include removed ones by backtracking in graph form.
 */
void Assignment::printHistory() {
    std::unordered_set<std::string> printed_list;
    for (auto& s : Assignment::assignment_history) {
        bool print_rest = false;
        while (!s.empty()) {
            // get first assignment
            Literal* l = s.top()->assigned_literal;
            std::string a = "[" + std::to_string(l->id) + "|" + std::to_string(l->value) + "|";
            if (s.top()->isForced) {a += "f]";}
            else {a += "b]";}

            if (print_rest || printed_list.count(a) == 0 ) {
                if (print_rest && printed_list.count(a) != 0) {
                    printed_list.erase(a);
                } else printed_list.insert(a);
                std::cout << a << "-";
                print_rest = true; // the first change in assignment history (backtracking), signal print the rest
            } else if (printed_list.count(a) >= 1) {
                std::cout << "        ";
            }
            s.pop();
        }
        std::cout << std::endl;
    }
}
