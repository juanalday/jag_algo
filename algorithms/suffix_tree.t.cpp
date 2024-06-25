#include "suffix_tree.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


using jag::algo::SuffixTree;
using ::testing::ElementsAre;

TEST(SuffixTree, Constructor) {
	{
		SuffixTree st1("aa");
		EXPECT_EQ(st1.size(), 5);
	}
	SuffixTree st2("aaa");
	SuffixTree st3("bana");
	SuffixTree st4("abcabxabcd");
}

TEST(SuffixTree, contains) {

	SuffixTree st("axabg");
	//EXPECT_TRUE(st.contains("xabg"));
	//EXPECT_FALSE(st.contains("xabt"));
	//EXPECT_FALSE(st.contains("xabd"));

	//st.reset("axabtxabg");
	//EXPECT_TRUE(st.contains("xabg"));
	//EXPECT_TRUE(st.contains("xabt"));
	//EXPECT_FALSE(st.contains("xabd"));
}

TEST(SuffixTree, findAllOccurrences) {
	SuffixTree st("axabtxabxagg");
	EXPECT_THAT(st.findAll("xab"), ElementsAre(1, 5));
	EXPECT_THAT(st.findAll("bt"), ElementsAre(3));
	EXPECT_THAT(st.findAll("xa"), ElementsAre(1, 5));
	EXPECT_THAT(st.findAll("a"), ElementsAre(0, 2, 6));
	
}

