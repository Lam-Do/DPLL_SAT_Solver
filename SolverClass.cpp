#include "SolverClass.h"

void Literal::setFree() {
    this->isFree = true;
    this->value = -1;
}

void Clause::appendLiteral(Literal &literal) {
    this->literals_list.push_back(&literal);
    this->active_literals_count++;
}