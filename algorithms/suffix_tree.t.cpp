#include "suffix_tree.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>


using jag::algo::SuffixTree;

TEST(SuffixTree, Constructor) {
	 char const* str = "abcabxabcd";

	 SuffixTree st(str);

}
