cmake_minimum_required(VERSION 3.12)
project(Testing)

set(CMAKE_CXX_STANDARD 14)

add_executable(Testing main.cpp domain/Auction.cpp domain/Auction.h domain/User.cpp domain/User.h domain/Bid.cpp domain/Bid.h service/Evaluator.cpp service/Evaluator.h builder/TestBuilder.cpp builder/TestBuilder.h domain/Payment.cpp domain/Payment.h persistence/FakeAuctionDAO.cpp persistence/FakeAuctionDAO.h persistence/AuctionDAO.cpp persistence/AuctionDAO.h service/Finisher.cpp service/Finisher.h)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)
target_link_libraries(Testing gtest gtest_main)