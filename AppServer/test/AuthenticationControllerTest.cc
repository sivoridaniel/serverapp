/*
 * AuthenticationTest.cc
 *
 *  Created on: 17 de abr. de 2016
 *      Author: pablo
 */
#include <limits>
#include "include/gtest/gtest.h"
#include "../src/controller/AuthenticationController.h"

namespace {

// The fixture for testing class Foo.
class AuthenticationControllerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	AuthenticationControllerTest() {
    // You can do set-up work for each test here.
  }

  virtual ~AuthenticationControllerTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST(AuthenticationControllerTest,Trivial){

	AuthenticationController ac;

	EXPECT_FALSE(ac.isRegisterUser("psivori","1235"));
	EXPECT_TRUE(ac.isRegisterUser("psivori","1234"));
}

} //namespace

