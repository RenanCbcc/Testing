#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"


struct EvaluatorTest : testing::Test {
    EvaluatorTest() {
        auction = new Auction("Playstation 3 Novo");
    }

    // Override this to define how to set up the environment.
    void SetUp() override {
        john = new User("John", 01);
        joseph = new User("Joseph", 02);
        maria = new User("Maria", 03);
    }

    // Override this to define how to tear down the environment.
    virtual void TearDown() {}

    Auction *auction;
    User *john;
    User *joseph;
    User *maria;

};

struct AuctionTest : testing::Test {
    AuctionTest() {
        auction = new Auction("Macbook pro 13");
    }

    // Override this to define how to set up the environment.
    void SetUp() override {
        john = new User("John", 01);
        joseph = new User("Joseph", 02);
        maria = new User("Maria", 03);
    }

    // Override this to define how to tear down the environment.
    virtual void TearDown() {}

    Auction *auction;
    User *john;
    User *joseph;
    User *maria;

};

TEST_F(EvaluatorTest, shouldRetunrBiggerAndSmallerOrder) {

    auction->propose(new Bid(maria, 250.0));
    auction->propose(new Bid(john, 400.0));
    auction->propose(new Bid(joseph, 300.0));
    auction->propose(new Bid(maria, 450.0));

    Evaluator auctioner;
    auctioner.evaluate(auction);

    // comparando a saida com o esperado
    double maiorEsperado = 450;
    double menorEsperado = 250;

    EXPECT_EQ(maiorEsperado, auctioner.getBigger());
    EXPECT_EQ(menorEsperado, auctioner.getSmaller());

}

TEST_F(EvaluatorTest, shouldReturnTheMean) {

    auction->propose(new Bid(maria, 250.0));
    auction->propose(new Bid(john, 300.0));
    auction->propose(new Bid(joseph, 400.0));

    Evaluator auctioner;
    auctioner.calculate(auction);
    // comparando a saida com o esperado
    std::cout << auctioner.getMean() << std::endl;

    double meanEsperada = 316.666666667;
    EXPECT_NEAR(meanEsperada, auctioner.getMean(), 0.01);


}

TEST_F(EvaluatorTest, shouldRetunrAuctionWithOnlyOneBid) {
    auction->propose(new Bid(maria, 250.0));

    Evaluator auctioner;
    auctioner.evaluate(auction);

    EXPECT_EQ(250.0, auctioner.getBiggest()[0]->getValue());

}

TEST_F(EvaluatorTest, shouldRetunrAuctionThreeBiggestsBids) {
    auction->propose(new Bid(maria, 250.0));
    auction->propose(new Bid(joseph, 320.0));
    auction->propose(new Bid(john, 400.0));
    auction->propose(new Bid(maria, 450.0));
    Evaluator auctioner;
    auctioner.evaluate(auction);

    EXPECT_EQ(3, auctioner.getBiggest().size());
    EXPECT_EQ(400.0, auctioner.getBiggest()[2]->getValue());
    EXPECT_EQ(320.0, auctioner.getBiggest()[1]->getValue());
    EXPECT_EQ(450.0, auctioner.getBiggest()[0]->getValue());

}

TEST_F(EvaluatorTest, shouldRetunrNoBid) {

    Evaluator auctioner;
    auctioner.evaluate(auction);
    EXPECT_EQ(true, auctioner.getBiggest().empty());


}

TEST_F(EvaluatorTest, shouldSelectBidBetween1000And3000) {
    auction->propose(new Bid(maria, 2000.0));
    auction->propose(new Bid(john, 1000.0));
    auction->propose(new Bid(joseph, 3000.0));
    auction->propose(new Bid(maria, 800.0));

    Evaluator auctioner;
    auto bids = auctioner.filter(auction);
    EXPECT_EQ(1, bids.size());
    EXPECT_EQ(2000.0, bids[0]->getValue());


}

TEST_F(EvaluatorTest, shouldSelectBidAbove5000) {
    auction->propose(new Bid(maria, 800.0));
    auction->propose(new Bid(john, 1000.0));
    auction->propose(new Bid(maria, 2000.0));
    auction->propose(new Bid(joseph, 3000.0));
    auction->propose(new Bid(maria, 5800.0));

    Evaluator auctioner;
    auto bids = auctioner.filter(auction);
    EXPECT_EQ(2, bids.size());
    EXPECT_EQ(2000.0, bids[0]->getValue());
    EXPECT_EQ(5800.0, bids[1]->getValue());


}

TEST_F(AuctionTest, shouldNotAcceptTwoConsecutiveBids) {

    auction->propose(new Bid(maria, 800.0));
    auction->propose(new Bid(maria, 1000.0));

    auto bids = auction->getBids();
    EXPECT_EQ(1, bids.size());
    EXPECT_EQ(800.0, bids[0]->getValue());
}

TEST_F(AuctionTest, shouldNotAcceptMoreThanFiveBidsFromSameUser) {
    auction->propose(new Bid(john, 800.0));
    auction->propose(new Bid(joseph, 1000.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2500.0));
    auction->propose(new Bid(joseph, 2900.0));

    auction->propose(new Bid(john, 3000.0));
    auction->propose(new Bid(joseph, 3600.0));

    //This one must be ignored.
    auction->propose(new Bid(john, 3700.0));

    auto bids = auction->getBids();
    EXPECT_EQ(10, bids.size());
    EXPECT_EQ(3600.0, bids[bids.size() - 1]->getValue());
}

TEST_F(AuctionTest, mustDoubleTheBidsFromAnUser) {
    auction->propose(new Bid(john, 800.0));
    auction->propose(new Bid(joseph, 1000.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2500.0));
    auction->propose(new Bid(joseph, 3000.0));

    auction->propose(new Bid(john, 3300.0));
    auction->doubleBid(joseph);


    //This one must be ignored.
    auction->propose(new Bid(john, 10300.0));

    auto bids = auction->getBids();
    EXPECT_EQ(10, bids.size());
    EXPECT_EQ(6000.0, bids[bids.size() - 1]->getValue());
}

TEST_F(AuctionTest, mustNotDoubleTheBidIfUserHasNoBid) {
    auction->propose(new Bid(john, 800.0));
    auction->propose(new Bid(joseph, 1000.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2000.0));
    auction->propose(new Bid(joseph, 2200.0));

    auction->propose(new Bid(john, 2500.0));
    auction->propose(new Bid(joseph, 3000.0));

    auction->propose(new Bid(john, 3300.0));
    auction->doubleBid(maria);


    //This one must be ignored.
    auction->propose(new Bid(john, 10300.0));

    auto bids = auction->getBids();
    EXPECT_EQ(9, bids.size());
    EXPECT_EQ(3300.0, bids[bids.size() - 1]->getValue());
}


int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    return RUN_ALL_TESTS();

}
