//
// Created by renan on 25/08/18.
//

#ifndef TESTING_BID_H
#define TESTING_BID_H


#include <string>
#include <vector>
#include "User.h"

class Bid {
public:

    Bid(User *user, double value);

    User *getUser() const;

    double getValue() const;

private:
    User*user;
    double value;

};

#endif //TESTING_BID_H
