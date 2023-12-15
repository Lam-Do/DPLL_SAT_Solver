#include "SolverClass.h"

void Literal::setFree() {
    this->isFree = true;
}

void Literal::assignValue(bool value) {
    this->isFree = false;
    this->value = value;
}

void Clause::appendLiteral(Literal* literal_ad, bool isPos) {
    if (isPos) {
        this->pos_literals_list.push_back(literal_ad);}
    else {
        this->neg_literals_list;
    }
    this->active_literals_count++;
}

bool Clause::isSAT() {
    for (const auto& literal : this->pos_literals_list) {
        if (!(literal->isFree)) {
            if (literal->value) return true;
        }
    }
    for (const auto& literal : this->neg_literals_list) {
        if (!(literal->isFree)){
            if (!(literal->value)) return true;
        }
    }

    return false;
}