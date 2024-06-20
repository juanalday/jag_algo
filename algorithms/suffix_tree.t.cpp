#include "suffix_tree.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


using jag::algo::SuffixTree;

TEST(SuffixTree, Constructor) {
	SuffixTree st("abcabxabcd");
}

TEST(SuffixTree, find) {

	SuffixTree st("axabg");
	EXPECT_NE(st.find("xabg"), st.end());
	EXPECT_EQ(st.find("xabt"), st.end());
	EXPECT_EQ(st.find("xabd"), st.end());
	st.reset("axabtxabg");
	EXPECT_NE(st.find("xabg"), st.end());
	EXPECT_NE(st.find("xabt"), st.end());
	EXPECT_EQ(st.find("xabd"), st.end());

}
TEST(SuffixTree, contains) {

	SuffixTree st("axabg");
	EXPECT_TRUE(st.contains("xabg"));
	EXPECT_FALSE(st.contains("xabt"));
	EXPECT_FALSE(st.contains("xabd"));

	st.reset("axabtxabg");
	EXPECT_TRUE(st.contains("xabg"));
	EXPECT_TRUE(st.contains("xabt"));
	EXPECT_FALSE(st.contains("xabd"));
}

