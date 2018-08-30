#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"
#include "builder/TestBuilder.h"


struct Environment : testing::Test {
    // Override this to define how to set up the environment.
    Environment() {
        buildAuction = new TestBuilder();
    }

    void SetUp() override {
        john = new User("John", 01);
        joseph = new User("Joseph", 02);
        maria = new User("Maria", 03);
    }

    // Override this to define how to tear down the environment.
    virtual void TearDown() {}

    TestBuilder *buildAuction;
    User *john{};
    User *joseph{};
    User *maria{};

};


TEST_F(Environment, shouldRetunrBiggerAndSmallerOrder) {

    Auction *auction = buildAuction->to("Playstation 4 pro")->withBid(maria, 250.0)->withBid(john, 400.0)->withBid(
            joseph, 300.0)->withBid(
            maria, 450.0)->build();

    Evaluator auctioner;
    auctioner.evaluate(auction);

    // comparando a saida com o esperado
    double maiorEsperado = 450;
    double menorEsperado = 250;

    EXPECT_EQ(maiorEsperado, auctioner.getBigger());
    EXPECT_EQ(menorEsperado, auctioner.getSmaller());

}

TEST_F(Environment, shouldReturnTheMean) {

    Auction *auction = buildAuction
            ->to("Playstation 4 pro")
            ->withBid(maria, 250.0)
            ->withBid(john, 300.0)
            ->withBid(joseph, 400.0)
            ->withBid(maria, 450.0)
            ->build();

    Evaluator auctioner;
    auctioner.calculate(auction);
    // comparing the returned result with the expected result.
    double expectedMean = 350.0;
    EXPECT_NEAR(expectedMean, auctioner.getMean(), 0.01);


}

TEST_F(Environment, shouldRetunrAuctionWithOnlyOneBid) {
    Auction *auction = buildAuction
            ->to("Playstation 4 pro")
            ->withBid(maria, 250.0)
            ->build();

    Evaluator auctioner;
    auctioner.evaluate(auction);

    EXPECT_EQ(250.0, auctioner.getBiggest()[0]->getValue());

}

TEST_F(Environment, shouldRetunrAuctionThreeBiggestsBids) {
    Auction *auction = buildAuction
            ->to("Electric bicycle A7 model")
            ->withBid(maria, 250.0)
            ->withBid(john, 400.0)
            ->withBid(joseph, 300.0)
            ->withBid(maria, 450.0)
            ->withDoubleBid(john)
            ->build();
    Evaluator auctioner;
    auctioner.evaluate(auction);

    EXPECT_EQ(3, auctioner.getBiggest().size());
    EXPECT_EQ(400.0, auctioner.getBiggest()[2]->getValue());
    EXPECT_EQ(450.0, auctioner.getBiggest()[1]->getValue());
    EXPECT_EQ(800.0, auctioner.getBiggest()[0]->getValue());

}

TEST_F(Environment, shouldRetunrNoBid) {
    Auction *auction = buildAuction
            ->to("Broken Playstation 4 pro")
            ->build();

    Evaluator auctioner;
    auctioner.evaluate(auction);
    EXPECT_EQ(true, auctioner.getBiggest().empty());


}

TEST_F(Environment, shouldSelectBidBetween1000And3000) {
    Auction *auction = buildAuction
            ->to("Macbook pro retina")
            ->withBid(maria, 1000.0)
            ->withBid(john, 1200.0)
            ->withBid(joseph, 2000.0)
            ->withBid(maria, 3000.0)
            ->build();

    Evaluator auctioner;
    auto bids = auctioner.filter(auction);
    EXPECT_EQ(2, bids.size());
    EXPECT_EQ(1200.0, bids[0]->getValue());


}

TEST_F(Environment, shouldSelectBidAbove5000) {
    Auction *auction = buildAuction
            ->to("Macbook pro retina")
            ->withBid(maria, 800.0)
            ->withBid(john, 1200.0)
            ->withBid(joseph, 2000.0)
            ->withBid(maria, 3000.0)
            ->withBid(joseph, 5800.0)
            ->build();

    Evaluator auctioner;
    auto bids = auctioner.filter(auction);
    EXPECT_EQ(3, bids.size());
    EXPECT_EQ(1200.0, bids[0]->getValue());
    EXPECT_EQ(2000.0, bids[1]->getValue());
    EXPECT_EQ(5800.0, bids[2]->getValue());

}

TEST_F(Environment, shouldNotAcceptTwoConsecutiveBids) {
    Auction *auction = buildAuction
            ->to("Macbook pro retina")
            ->withBid(maria, 800.0)
            ->withBid(maria, 1200.0)
            ->build();

    auto bids = auction->getBids();
    EXPECT_EQ(1, bids.size());
    EXPECT_EQ(800.0, bids[0]->getValue());
}

TEST_F(Environment, shouldNotAcceptMoreThanFiveBidsFromSameUser) {

    Auction *auction = buildAuction
            ->to("Heart of Tarrasque")
            ->withBid(john, 800.0)
            ->withBid(joseph, 1200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2500.0)
            ->withBid(joseph, 2900.0)
            ->withBid(john, 3000.0)
            ->withBid(joseph, 3600.0)
            ->build();


    //This one must be ignored.
    auction->propose(new Bid(john, 3700.0));

    auto bids = auction->getBids();
    EXPECT_EQ(10, bids.size());
    EXPECT_EQ(3600.0, bids[bids.size() - 1]->getValue());
}

TEST_F(Environment, shouldDoubleTheBidsFromAnUser) {
    Auction *auction = buildAuction
            ->to("Heart of Tarrasque")
            ->withBid(john, 800.0)
            ->withBid(joseph, 1200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2500.0)
            ->withBid(joseph, 2900.0)
            ->withBid(john, 3000.0)
            ->withDoubleBid(joseph)
            ->withBid(john, 10300.0)//This one must be ignored.
            ->build();

    auto bids = auction->getBids();
    EXPECT_EQ(10, bids.size());
    EXPECT_EQ(5800.0, bids[bids.size() - 1]->getValue());
}

TEST_F(Environment, shouldNotDoubleTheBidIfUserHasNoBid) {
    Auction *auction = buildAuction
            ->to("Heart of Tarrasque")
            ->withBid(john, 800.0)
            ->withBid(joseph, 1200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2000.0)
            ->withBid(joseph, 2200.0)
            ->withBid(john, 2500.0)
            ->withBid(joseph, 2900.0)
            ->withBid(john, 3000.0)
            ->withDoubleBid(maria) //This one must be ignored.
            ->withBid(john, 10300.0)//This one must be ignored.
            ->build();

    auto bids = auction->getBids();
    EXPECT_EQ(9, bids.size());
    EXPECT_EQ(3000.0, bids[bids.size() - 1]->getValue());
}


int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    return RUN_ALL_TESTS();

}
