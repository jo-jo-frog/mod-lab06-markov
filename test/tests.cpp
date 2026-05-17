// Copyright 2025 Anisimov
#include "textgen.h"

#include <gtest/gtest.h>

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <string>

TEST(MarkovTest, PrefixCreation) {
  Prefix p;
  p.push_back("hello");
  p.push_back("world");
  EXPECT_EQ(p.size(), 2);
  EXPECT_EQ(p[0], "hello");
  EXPECT_EQ(p[1], "world");
}

TEST(MarkovTest, AddRule) {
  TextGenerator gen(2);
  Prefix p;
  p.push_back("a");
  p.push_back("b");
  gen.addRule(p, "c");
  const auto& table = gen.getTable();
  ASSERT_EQ(table.size(), 1);
  auto it = table.find(p);
  ASSERT_NE(it, table.end());
  EXPECT_EQ(it->second.size(), 1);
  EXPECT_EQ(it->second[0], "c");
}

TEST(MarkovTest, SingleSuffix) {
  TextGenerator gen(2);
  Prefix p;
  p.push_back("x");
  p.push_back("y");
  gen.addRule(p, "only");
  std::string output = gen.generate(3);
  EXPECT_NE(output.find("only"), std::string::npos);
  for (int i = 0; i < 10; ++i) {
    std::string out2 = gen.generate(3);
    EXPECT_EQ(out2, "x y only ");
  }
}

TEST(MarkovTest, MultipleSuffixesRandom) {
  TextGenerator gen(2);
  Prefix p;
  p.push_back("k");
  p.push_back("l");
  gen.addRule(p, "v1");
  gen.addRule(p, "v2");
  gen.addRule(p, "v3");

  bool seen1 = false, seen2 = false, seen3 = false;
  for (int i = 0; i < 50; ++i) {
    std::string out = gen.generate(3);
    if (out.find("v1") != std::string::npos) seen1 = true;
    if (out.find("v2") != std::string::npos) seen2 = true;
    if (out.find("v3") != std::string::npos) seen3 = true;
    if (seen1 && seen2 && seen3) break;
  }
  EXPECT_TRUE(seen1 && seen2 && seen3);
}

TEST(MarkovTest, GenerateFixedLengthManual) {
  TextGenerator gen(2);
  Prefix p1; p1.push_back("a"); p1.push_back("b"); gen.addRule(p1, "c");
  Prefix p2; p2.push_back("b"); p2.push_back("c"); gen.addRule(p2, "d");
  Prefix p3; p3.push_back("c"); p3.push_back("d"); gen.addRule(p3, "e");

  std::string text = gen.generateFrom(p1, 5);
  EXPECT_EQ(text, "a b c d e ");
}

TEST(MarkovTest, WordCountLimit) {
  TextGenerator gen(2);
  Prefix p; p.push_back("only"); p.push_back("one");
  gen.addRule(p, "two");
  std::string text = gen.generate(3);
  int wordCount = std::count(text.begin(), text.end(), ' ');
  EXPECT_LE(wordCount, 3);
}

TEST(MarkovTest, StopWhenNoSuffix) {
  TextGenerator gen(2);
  Prefix p; p.push_back("start"); p.push_back("end");
  gen.addRule(p, "good");
  std::string text = gen.generate(10);
  int wordCount = std::count(text.begin(), text.end(), ' ');
  EXPECT_EQ(wordCount, 3);
}

TEST(MarkovTest, EmptyTableHandling) {
  TextGenerator gen(2);
  std::string text = gen.generate(10);
  EXPECT_NE(text.find("Error"), std::string::npos);
}

TEST(MarkovTest, BuildFromFile) {
  std::ofstream tmp("temp_test.txt");
  tmp << "one two three four five six";
  tmp.close();

  TextGenerator gen(2);
  bool ok = gen.buildFromFile("temp_test.txt");
  EXPECT_TRUE(ok);
  EXPECT_EQ(gen.getTable().size(), 4);

  remove("temp_test.txt");
}

TEST(MarkovTest, PrefixShift) {
  TextGenerator gen(2);
  Prefix p1; p1.push_back("a"); p1.push_back("b"); gen.addRule(p1, "c");
  Prefix p2; p2.push_back("b"); p2.push_back("c"); gen.addRule(p2, "d");
  Prefix p3; p3.push_back("c"); p3.push_back("d"); gen.addRule(p3, "e");

  std::string result = gen.generateFrom(p1, 5);
  EXPECT_EQ(result, "a b c d e ");
}

TEST(MarkovTest, GenerateLargeAmount) {
  TextGenerator gen(2);
  Prefix p; p.push_back("a"); p.push_back("b"); gen.addRule(p, "c");
  std::string result = gen.generate(10000);
  EXPECT_FALSE(result.empty());
}
