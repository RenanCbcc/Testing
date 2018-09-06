#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"
#include "builder/TestBuilder.h"
#include "persistence/AuctionDAO.h"
#include "service/Finisher.h"

/*
 Instead of using AuctionDAO (witch is not ready) in Finish class, I will use a mock for simulate the database.
 For more examples, I recommend this: https://github.com/CodesBay/Google_Test_Framework/tree/master/Chapter-%203
 */
class AuctionDAOMock : public AuctionDAO {

public:
    MOCK_METHOD1(save, void (Auction*));
    MOCK_METHOD1(update, void (Auction*));//It takes one arguments.
    MOCK_METHOD0(update, std::vector<Auction *>()); //It takes no arguments.
    MOCK_METHOD0(current, std::vector<Auction *>()); //It takes no arguments.

};

struct Environment : testing::Test {
    // Override this to define how to set up the environment.
    Environment() {
        buildAuction = new TestBuilder();
    }

    void SetUp() override {
        john = new User("John", 01);
        joseph = new User("Joseph", 02);
        maria = new User("Maria", 03);
        auctionOne = buildAuction
                ->to("Led Television 50")->atDate(boost::gregorian::date(2017, 8, 24))
                ->build();

        auctionTwo = buildAuction
                ->to("Blue Ray Sony")->atDate(boost::gregorian::date(2017, 8, 24))
                ->build();

    }

    // Override this to define how to tear down the environment.
    virtual void TearDown() {}

    AuctionDAOMock mock;
    TestBuilder *buildAuction;
    Auction *auctionOne;
    Auction *auctionTwo;
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
            ->to("Brainiac computer")
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
    EXPECT_EQ(bids[bids.size() - 1]->getValue(), 3000.0);
}

TEST_F(Environment, shouldNotEvaluateAuctionsWithoutBids) {
    Auction *auction = buildAuction
            ->to("Hall 9000 computer")
            ->build();

    Evaluator auctioner;
    EXPECT_ANY_THROW(auctioner.evaluate(auction));
}


TEST_F(Environment, shouldCloseAuctionsOlderThanOneWeek) {

    std::vector<Auction *> oldAuctions = {auctionOne, auctionTwo};


    EXPECT_CALL(mock, current)
            .Times(1)
            .WillOnce(testing::Return(oldAuctions));


    Finisher finisher(mock);
    finisher.closes();

    EXPECT_EQ(2, oldAuctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_TRUE(auctionTwo->isClosed());
}


int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    return RUN_ALL_TESTS();

}

