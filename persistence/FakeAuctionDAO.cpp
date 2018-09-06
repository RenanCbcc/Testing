//
// Created by renan on 31/08/18.
//

#include "FakeAuctionDAO.h"

void FakeAuctionDAO::save(Auction *auction) {
    FakeAuctionDAO::Auctions.push_back(auction);
}

std::vector<Auction *> FakeAuctionDAO::closed() {
    std::vector<Auction *> filtereds;

    for (Auction *auction : Auctions) {
        if (auction->isClosed()) filtereds.push_back(auction);
    }

    return filtereds;

}

std::vector<Auction *> FakeAuctionDAO::current() {

    std::vector<Auction *> filtereds;

    for (Auction *auction : Auctions) {
        if (!auction->isClosed()) filtereds.push_back(auction);
    }

    return filtereds;

}

void FakeAuctionDAO::update(Auction *) {

}
