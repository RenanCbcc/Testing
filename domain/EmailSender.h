//
// Created by renan on 06/09/18.
//

#ifndef TESTING_EMAILSENDER_H
#define TESTING_EMAILSENDER_H


#include "Auction.h"

class EmailSender {
public:
    virtual void send(Auction*);
};


#endif //TESTING_EMAILSENDER_H
