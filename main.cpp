#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "domain/User.h"
#include "domain/Auction.h"
#include "service/Evaluator.h"


TEST(EvaluatorTest, shouldComprehendBidsInOrder) {
    User joao("Joao");
    User jose("José");
    User maria("Maria");
    Auction auction("Playstation 3 Novo");
    auction.propose(new Bid(&maria, 250.0));
    auction.propose(new Bid(&joao, 300.0));
    auction.propose(new Bid(&jose, 400.0));

    Evaluator auctioner;
    auctioner.evaluate(&auction);

    // comparando a saida com o esperado
    double maiorEsperado = 400;
    double menorEsperado = 250;

    EXPECT_EQ(maiorEsperado, auctioner.getBigger());
    EXPECT_EQ(menorEsperado, auctioner.getSmaller());

}

int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    RUN_ALL_TESTS();
    return 0;
}
