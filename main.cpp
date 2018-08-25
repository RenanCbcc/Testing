#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct BankAcount {
    int balance;

    explicit BankAcount(const int balance)
            : balance{balance} {}
};

TEST(AcountTest, ShoulstartEmpty) {
    BankAcount acount(0);
    EXPECT_EQ(0, acount.balance);
}

int main(int argc, char *arvg[]) {
    testing::InitGoogleTest(&argc, arvg);
    RUN_ALL_TESTS();
    return 0;
}