cmake_minimum_required(VERSION 3.12)
project(Testing)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(Testing main.cpp domain/Auction.cpp domain/Auction.h domain/User.cpp domain/User.h domain/Bid.cpp domain/Bid.h service/Evaluator.cpp service/Evaluator.h builder/TestBuilder.cpp builder/TestBuilder.h domain/Payment.cpp domain/Payment.h persistence/FakeAuctionDAO.cpp persistence/FakeAuctionDAO.h persistence/AuctionDAO.cpp persistence/AuctionDAO.h service/Finisher.h service/Finisher.cpp domain/EmailSender.cpp domain/EmailSender.h persistence/PaymentRepository.cpp persistence/PaymentRepository.h service/PaymentGenerator.cpp service/PaymentGenerator.h)

target_link_libraries(Testing gtest gtest_main)
target_link_libraries(Testing gmock gmock_main)