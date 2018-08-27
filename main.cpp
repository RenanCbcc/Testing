#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"

class Before {
public:
    Before() {
        this->users = {new User("Joao"), new User("José"), new User("Maria")};
        this->auction = new Auction("Playstation 3 Novo");
    }

    Auction *auction;
    std::vector<User *> users;

};

struct EvaluatorTest : testing::Test {
    EvaluatorTest() {
        before = new Before();
    }

    Before *before;
};

TEST_F(EvaluatorTest, shouldRetunrBiggerAndSmallerOrder) {

    before->auction->propose(new Bid(before->users[2], 250.0));
    before->auction->propose(new Bid(before->users[0], 400.0));
    before->auction->propose(new Bid(before->users[3], 300.0));
    before->auction->propose(new Bid(before->users[2], 450.0));

    Evaluator auctioner;
    auctioner.evaluate(before->auction);

    // comparando a saida com o esperado
    double maiorEsperado = 450;
    double menorEsperado = 250;

    EXPECT_EQ(maiorEsperado, auctioner.getBigger());
    EXPECT_EQ(menorEsperado, auctioner.getSmaller());

}

TEST_F(EvaluatorTest, shouldReturnTheMean) {

    before->auction->propose(new Bid(before->users[2], 250.0));
    before->auction->propose(new Bid(before->users[0], 300.0));
    before->auction->propose(new Bid(before->users[3], 400.0));

    Evaluator auctioner;
    auctioner.calculate(before->auction);
    // comparando a saida com o esperado
    std::cout << auctioner.getMean() << std::endl;

    double meanEsperada = 316.666666667;
    EXPECT_NEAR(meanEsperada, auctioner.getMean(), 0.01);


}

TEST_F(EvaluatorTest, shouldRetunrAuctionWithOnlyOneBid) {
    before->auction->propose(new Bid(before->users[2], 250.0));
    before->auction->propose(new Bid(before->users[0], 400.0));
    before->auction->propose(new Bid(before->users[3], 320.0));
    before->auction->propose(new Bid(before->users[2], 450.0));

    Evaluator auctioner;
    auctioner.evaluate(before->auction);

    EXPECT_EQ(3, auctioner.getBiggest().size());
    EXPECT_EQ(450.0, auctioner.getBiggest()[0]->getValue());
    EXPECT_EQ(400.0, auctioner.getBiggest()[1]->getValue());
    EXPECT_EQ(320.0, auctioner.getBiggest()[2]->getValue());

}


int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    return RUN_ALL_TESTS();

}
