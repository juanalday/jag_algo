#include "suffix_tree.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


using jag::algo::SuffixTree;
using ::testing::ElementsAre;

TEST(SuffixTree, DISABLED_Constructor) {
	{
		SuffixTree st("aa");
		EXPECT_EQ(st.size(), 5);
		EXPECT_EQ(2, st[0].size()); // root should have 2 edges
		EXPECT_EQ(1, st[0].at('a'));
		EXPECT_EQ(4, st[0].at('$'));

		EXPECT_EQ("a", st[1].str());
		EXPECT_EQ(0, st[1].m_start);

		EXPECT_EQ("a$", st[2].str());
		EXPECT_EQ(1, st[2].m_start);

		EXPECT_EQ("$",  st[3].str());
		EXPECT_EQ("$",  st[4].str());

	}
	{
		SuffixTree st("aaa");
		EXPECT_EQ(st.size(), 7);
		EXPECT_EQ(2, st[0].size()); // root should have 2 edges
		EXPECT_EQ(1, st[0].at('a'));
		EXPECT_EQ(6, st[0].at('$'));

		EXPECT_EQ("a", st[1].str());
		EXPECT_FALSE(st[1].isLeaf());
		EXPECT_EQ(0, st[1].m_start);
		EXPECT_EQ(1, st[1].m_end);

		EXPECT_EQ("a$", st[2].str());
		EXPECT_TRUE(st[2].isLeaf());
		EXPECT_EQ(2, st[2].m_start);



		EXPECT_EQ("$", st[3].str());
		EXPECT_TRUE(st[3].isLeaf());
		EXPECT_EQ(3, st[3].m_start);
		EXPECT_EQ(-1, st[3].m_end);

		EXPECT_EQ("a", st[4].str());
		EXPECT_FALSE(st[4].isLeaf());
		EXPECT_EQ(1, st[4].m_start);
		EXPECT_EQ(2, st[4].m_end);
		EXPECT_EQ(2, st[4].size());
		EXPECT_EQ(2, st[4].at('a'));
		EXPECT_EQ(3, st[4].at('$'));

		EXPECT_EQ("$", st[5].str());
		EXPECT_TRUE(st[5].isLeaf());
		EXPECT_EQ(3, st[5].m_start);
		EXPECT_EQ(-1, st[5].m_end);

		EXPECT_EQ("$", st[6].str());
		EXPECT_TRUE(st[6].isLeaf());
		EXPECT_EQ(3, st[6].m_start);
		EXPECT_EQ(-1, st[6].m_end);
	}
	{
		SuffixTree st("bana");
		EXPECT_EQ(st.size(), 7);
		EXPECT_EQ(4, st[0].size()); // root should have 4 edges

		EXPECT_EQ("bana$", st[1].str());
		EXPECT_TRUE(st[1].isLeaf());

		EXPECT_EQ("a", st[2].str());
		EXPECT_FALSE(st[2].isLeaf());

		EXPECT_EQ("na$", st[3].str());
		EXPECT_TRUE(st[3].isLeaf());

		EXPECT_EQ("na$", st[4].str());
		EXPECT_TRUE(st[4].isLeaf());
		EXPECT_EQ(2, st[4].m_start);
		EXPECT_EQ(-1, st[4].m_end);

		EXPECT_EQ("$", st[5].str());
		EXPECT_TRUE(st[5].isLeaf());
		EXPECT_EQ(4, st[5].m_start);
		EXPECT_EQ(-1, st[5].m_end);

		EXPECT_EQ("$", st[6].str());
		EXPECT_TRUE(st[6].isLeaf());
	}
	SuffixTree st4("abcabxabcd");
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
