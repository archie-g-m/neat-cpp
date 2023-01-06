#include "config_parser.h"

#include <string>
#include <gtest/gtest.h>

TEST(CONFIGPARSER, BlankFileName){
  ConfigParser test = ConfigParser("");
  ASSERT_TRUE(test.data.empty());
}

TEST(CONFIGPARSER, EmptyFile){
  ConfigParser test = ConfigParser("blank.cfg");
  ASSERT_TRUE(test.data.empty());
}

TEST(CONFIGPARSER, TestFile){
  ConfigParser test = ConfigParser("test_config.cfg");
  ASSERT_TRUE(!test.data.empty());
  ASSERT_TRUE(!test.data["TITLE1"].empty());
  ASSERT_EQ(test.data["TITLE1"]["test"], "1");
  ASSERT_EQ(test.data["TITLE1"]["test2"], "two");
  ASSERT_TRUE(!test.data["TITLE2"].empty());
  ASSERT_EQ(test.data["TITLE2"]["test1"], "other One");
  ASSERT_EQ(test.data["TITLE2"]["test3"], "null");
  ASSERT_EQ(test.data["TITLE2"]["test4"], "-1");
}

TEST(CONFIGPARSER, StripWhitespace){
  ConfigParser test = ConfigParser("space_test.cfg");
  ASSERT_EQ(test.data["TESTYTEST"]["asdf"], "asdfa");
  ASSERT_EQ(test.data["TESTYTEST"]["abcd"], "asdf");
  ASSERT_EQ(test.data["TESTYTEST"]["123"], "2458");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
