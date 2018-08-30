//
// Created by renan on 25/08/18.
//

#include "Bid.h"

Bid::Bid(User *user, double value) {
    Bid::user = user;
    Bid::value = value;
}

User *Bid::getUser() const {
    return user;
}

double Bid::getValue() const {
    return value;
}

bool Bid::operator<(Bid &bid) {
    return (Bid::value < bid.getValue());
}

bool Bid::operator>(Bid &bid) {
    return (Bid::value > bid.getValue());
}

void Bid::setValue(double value) {
    Bid::value = value;
}
/*
bool Bid::operator==(Bid &bid) {
    return (Bid::value == bid.getValue());
}
*/

