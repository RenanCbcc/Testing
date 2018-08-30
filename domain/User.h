//
// Created by renan on 25/08/18.
//

#ifndef TESTING_USER_H
#define TESTING_USER_H


#include <string>

class User {
public:
    explicit User(std::string, int id = 0);
    bool operator==(User &);

    int getId() const;

    const std::string &getName() const;

private:
    int id;
    std::string name;

};


#endif //TESTING_USER_H
