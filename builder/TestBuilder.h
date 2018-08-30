//
// Created by renan on 30/08/18.
//

#ifndef TESTING_BUILDER_H
#define TESTING_BUILDER_H


#include <string>
#include "../domain/User.h"
#include "../domain/Auction.h"

class TestBuilder {
public:
    TestBuilder();

    TestBuilder *to(std::string);

    TestBuilder *withBid(User *, double);
    TestBuilder *withDoubleBid(User *);

    Auction *build();

private:
    Auction* auction;
};


#endif //TESTING_BUILDER_H
