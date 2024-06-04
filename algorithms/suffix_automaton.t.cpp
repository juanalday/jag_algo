
#include <gtest/gtest.h>

#include "suffix_automaton.hpp"

using jag::algo::SuffixAutomaton;

TEST(SuffixAutomaton, Constructor) {
	SuffixAutomaton sa("aba");
	EXPECT_EQ(4, sa.size());
}

TEST(SuffixAutomaton, add) {
	SuffixAutomaton sa;
	ASSERT_EQ(true, sa.empty());
	EXPECT_EQ(0, sa[0].m_length);
	sa.append('a');
	EXPECT_EQ(2, sa.size());
	EXPECT_EQ(1, sa[1].m_length);
	EXPECT_EQ(0, sa[1].m_link);
	sa.append('b');
	EXPECT_EQ(3, sa.size());
	EXPECT_EQ(2, sa[2].m_length);
	EXPECT_EQ(0, sa[2].m_link);
	sa.append('a');
	EXPECT_EQ(4, sa.size());
	EXPECT_EQ(3, sa[3].m_length);
	EXPECT_EQ(1, sa[3].m_link);
}

TEST(SuffixAutomaton, suffixLinks) {
	SuffixAutomaton sa("dababd");
	EXPECT_EQ(9, sa.size());
	EXPECT_EQ(-1, sa[0].m_link);
	EXPECT_EQ(0, sa[1].m_link);
	EXPECT_EQ(5, sa[2].m_link);
	EXPECT_EQ(7, sa[3].m_link);
}

TEST(SuffixAutomaton, contains) {
	SuffixAutomaton sa("abcbc");
	EXPECT_TRUE(sa.contains("cbc"));
	EXPECT_TRUE(sa.contains("abcbc"));
	EXPECT_FALSE(sa.contains("bb"));
}
TEST(SuffixAutomatonTest, traverse) {
	SuffixAutomaton sa("abcbc");
	EXPECT_EQ(6, sa.traverse("cbc"));
	EXPECT_EQ(6, sa.traverse("abcbc"));
	EXPECT_EQ(3, sa.traverse("abc"));
	EXPECT_EQ(-1, sa.traverse("bb"));

	EXPECT_EQ(2, sa.traverse("ab"));
	EXPECT_EQ(7, sa.traverse("bc"));
	EXPECT_EQ(4, sa.traverse("bcb"));

	EXPECT_EQ(-1, sa.traverse("ba"));
	EXPECT_EQ(-1, sa.traverse("ac"));
}