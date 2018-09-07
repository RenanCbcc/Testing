#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"
#include "builder/TestBuilder.h"
#include "persistence/AuctionDAO.h"
#include "service/Finisher.h"
#include "persistence/PaymentRepository.h"
#include "service/PaymentGenerator.h"

/*
 Instead of using AuctionDAO (witch is not ready) in Finish class, I will use a mockDAO for simulate the database.
 For more examples, I recommend this: https://github.com/CodesBay/Google_Test_Framework/tree/master/Chapter-%203
 */
class MockAuctionDAO : public AuctionDAO {

public:

    MOCK_METHOD1(save, void (Auction
            *));

    MOCK_METHOD1(update, void (Auction
            *));//It takes one arguments.
    MOCK_METHOD0(closed, std::vector<Auction *>()); //It takes no arguments.
    MOCK_METHOD0(current, std::vector<Auction *>()); //It takes no arguments.

};

class MockEmailSender : public EmailSender {
public:
    MOCK_METHOD1(send, void(Auction
            *));
};

class MockPaymentRepository : public PaymentRepository {
public:
    MOCK_METHOD1(save, void(Payment
            &));
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
                ->to("Led Television 50")->atDate(boost::gregorian::day_clock::local_day() - boost::gregorian::weeks(1))
                ->build();

        auctionTwo = buildAuction
                ->to("Blue Ray Sony")->atDate(boost::gregorian::day_clock::local_day() - boost::gregorian::weeks(1))
                ->build();

        auctionThree = buildAuction
                ->to("Playstation 4 pro")
                ->atDate(boost::gregorian::day_clock::local_day())
                ->withBid(maria, 250.0)
                ->withBid(john, 400.0)
                ->withBid(joseph, 300.0)
                ->withBid(maria, 450.0)
                ->build();

    }

    // Override this to define how to tear down the environment.
    virtual void TearDown() {}

    MockPaymentRepository mockPayment;
    MockAuctionDAO mockDAO;
    MockEmailSender mockESender;
    Evaluator evaluator;
    TestBuilder *buildAuction;
    Auction *auctionOne;
    Auction *auctionTwo;
    Auction *auctionThree;
    User *john{};
    User *joseph{};
    User *maria{};

};


TEST_F(Environment, shouldRetunrBiggerAndSmallerOrder) {

    Auction *auction = buildAuction->to("Playstation 4 pro")->withBid(maria, 250.0)->withBid(john, 400.0)->withBid(
            joseph, 300.0)->withBid(
            maria, 450.0)->build();

    evaluator.evaluate(auction);

    // comparando a saida com o esperado
    double maiorEsperado = 450;
    double menorEsperado = 250;

    EXPECT_EQ(maiorEsperado, evaluator.getBigger());
    EXPECT_EQ(menorEsperado, evaluator.getSmaller());

}

TEST_F(Environment, shouldReturnTheMean) {

    Auction *auction = buildAuction
            ->to("Playstation 4 pro")
            ->withBid(maria, 250.0)
            ->withBid(john, 300.0)
            ->withBid(joseph, 400.0)
            ->withBid(maria, 450.0)
            ->build();

    evaluator.calculate(auction);
    // comparing the returned result with the expected result.
    double expectedMean = 350.0;
    EXPECT_NEAR(expectedMean, evaluator.getMean(), 0.01);


}

TEST_F(Environment, shouldRetunrAuctionWithOnlyOneBid) {
    Auction *auction = buildAuction
            ->to("Playstation 4 pro")
            ->withBid(maria, 250.0)
            ->build();

    evaluator.evaluate(auction);

    EXPECT_EQ(250.0, evaluator.getBiggest()[0]->getValue());

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

    evaluator.evaluate(auction);

    EXPECT_EQ(3, evaluator.getBiggest().size());
    EXPECT_EQ(400.0, evaluator.getBiggest()[2]->getValue());
    EXPECT_EQ(450.0, evaluator.getBiggest()[1]->getValue());
    EXPECT_EQ(800.0, evaluator.getBiggest()[0]->getValue());

}

TEST_F(Environment, shouldRetunrNoBid) {
    Auction *auction = buildAuction
            ->to("Broken Playstation 4 pro")
            ->build();

    Evaluator evaluator;

    EXPECT_EQ(true, evaluator.getBiggest().empty());


}

TEST_F(Environment, shouldSelectBidBetween1000And3000) {
    Auction *auction = buildAuction
            ->to("Macbook pro retina")
            ->withBid(maria, 1000.0)
            ->withBid(john, 1200.0)
            ->withBid(joseph, 2000.0)
            ->withBid(maria, 3000.0)
            ->build();

    auto bids = evaluator.filter(auction);
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

    auto bids = evaluator.filter(auction);
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

    EXPECT_ANY_THROW(evaluator.evaluate(auction));
}

TEST_F(Environment, shouldCloseAuctionsOlderThanOneWeek) {

    std::vector<Auction *> auctions = {auctionOne, auctionTwo, auctionThree};

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(3, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_TRUE(auctionTwo->isClosed());
    ASSERT_FALSE(auctionThree->isClosed());
}

TEST_F(Environment, shouldNotCloseAuctionsStartedYesterday) {
    auctionTwo->setDate(boost::gregorian::date(2018, 9, 5));
    std::vector<Auction *> auctions = {auctionOne, auctionTwo};


    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(2, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_FALSE(auctionTwo->isClosed());
}

TEST_F(Environment, shouldNotReturnAnyAuction) {
    std::vector<Auction *> auctions;

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(0, auctions.size());

}

TEST_F(Environment, shouldUpdateClosedAuction) {
    std::vector<Auction *> auctions = {auctionOne};

    EXPECT_CALL(mockDAO, update(testing::_))
            .Times(testing::Exactly(
                    1)); // If either you comment dao.update(auction) or duplicate it , the test you fail.

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(1, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
}

TEST_F(Environment, shouldUpdateAndSendAuction) {
    std::vector<Auction *> auctions = {auctionOne};

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockDAO, update(testing::_))
            .Times(testing::Exactly(
                    1)); // If either you comment dao.update(auction) or duplicate it , the test you fail.

    EXPECT_CALL(mockESender, send(testing::_))// Not really interested in the argument. Anything goes here.
            .Times(testing::Exactly(
                    1)); // If either you comment dao.update(auction) or duplicate it , the test you fail.



    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(1, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
}

TEST_F(Environment, shouldWorkEvenWhenUpdateThrowException) {
    std::vector<Auction *> auctions = {auctionOne, auctionTwo};

    EXPECT_CALL(mockDAO, current)
            .Times(testing::Exactly(1))
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockDAO, update(testing::_))
            .Times(testing::AtLeast(1))
            .WillOnce(testing::Throw(std::runtime_error(
                    "DAO exception has been thrown!"))); // If either you comment dao.update(auction) or duplicate it , the test you fail.

    EXPECT_CALL(mockESender, send(testing::_))// Not really interested in the argument. Anything goes here.
            .Times(testing::Exactly(1)); // If an exception has been thrown, is not necessary to send an email.



    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(2, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_TRUE(auctionTwo->isClosed());
}

/*This will work because Checked std::exception is invalid for this method
ACTION(Exception) {
    throw "DAO exception has been thrown!";
}
EXPECT_CALL(mockESender, send(testing::_))
            .Times(testing::Exactly(2))
            .WillOnce(Exception());
 */

TEST_F(Environment, shouldWorkEvenWhenSendThrowException) {
    std::vector<Auction *> auctions = {auctionOne, auctionTwo};

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockDAO, update(testing::_))
            .Times(testing::Exactly(2));

    EXPECT_CALL(mockESender, send(testing::_))// Not really interested in the argument. Anything goes here.
            .Times(testing::Exactly(2))
            .WillOnce(testing::Throw(std::runtime_error(
                    "DAO exception has been thrown!"))); // If either you comment dao.update(auction) or duplicate it , the test you fail.



    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(2, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_TRUE(auctionTwo->isClosed());
}

TEST_F(Environment, shouldNeverCallSendBecauseThrownException) {
    std::vector<Auction *> auctions = {auctionOne, auctionTwo};

    EXPECT_CALL(mockDAO, current)
            .Times(1)
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockDAO, update(testing::_))
            .Times(testing::Exactly(2))
            .WillRepeatedly(testing::Throw(std::runtime_error(
                    "DAO exception has been thrown!"))); // If either you comment dao.update(auction) or duplicate it , the test you fail.

    EXPECT_CALL(mockESender, send(testing::_))// Not really interested in the argument. Anything goes here.
            .Times(testing::Exactly(0));


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    EXPECT_EQ(2, auctions.size());
    ASSERT_TRUE(auctionOne->isClosed());
    ASSERT_TRUE(auctionTwo->isClosed());
}

TEST_F(Environment, shouldGeneratePaymentForClosedAuction) {

    Payment *buffer;
    std::vector<Auction *> auctions = {auctionThree};

    EXPECT_CALL(mockDAO, closed())
            .Times(1)
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockPayment, save(testing::_))
            .Times(1)
            .WillOnce(testing::SaveArg<0>(buffer));// It  will return the argument received by save method.



    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    PaymentGenerator paymentGenerator(mockDAO, evaluator, mockPayment);
    paymentGenerator.generate();

    EXPECT_EQ(1, auctions.size());
    ASSERT_FALSE(auctionThree->isClosed());
    ASSERT_DOUBLE_EQ(evaluator.getBigger(), buffer->getValue());

}


TEST_F(Environment, shouldPushPaymentForTheNextBusinessDay) {

    Payment *buffer;
    std::vector<Auction *> auctions = {auctionThree};

    EXPECT_CALL(mockDAO, closed())
            .Times(1)
            .WillOnce(testing::Return(auctions));

    EXPECT_CALL(mockPayment, save(testing::_))
            .Times(1)
            .WillOnce(testing::SaveArg<0>(buffer));// It  will return the argument received by save method.


    Finisher finisher(mockDAO, mockESender);
    finisher.closes();

    PaymentGenerator paymentGenerator(mockDAO, evaluator, mockPayment);
    paymentGenerator.generate();

    EXPECT_EQ(1, auctions.size());
    ASSERT_FALSE(auctionThree->isClosed());
    ASSERT_EQ(boost::gregorian::Monday, buffer->getDate().day_of_week());

}

int main(int argc, char *arvg[]) {
    testing::GTEST_FLAG(throw_on_failure) = true;
    testing::InitGoogleTest(&argc, arvg);
    return RUN_ALL_TESTS();

}

