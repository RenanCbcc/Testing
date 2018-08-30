#include <utility>

//
// Created by renan on 25/08/18.
//

#include "User.h"

int User::getId() const {
    return id;
}

const std::string &User::getName() const {
    return name;
}

User::User(std::string name, int id) {
    User::name = std::move(name);
    User::id = id;
}

bool User::operator==(User &other) {
    return User::id == other.getId();
}
