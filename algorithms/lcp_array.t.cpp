#include "lcp_array.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using jag::algo::lcpArray;
using jag::algo::suffixLcpArray;

using ::testing::ElementsAre;
using ::testing::Pair;

TEST(Algorithms, lcpArray)
{
	EXPECT_THAT(lcpArray("aaaaaa"), ElementsAre(0, 1, 2, 3, 4, 5));
	EXPECT_THAT(lcpArray("abcabcddd"), ElementsAre(0, 3, 0, 2, 0, 1, 0, 1, 2));
	EXPECT_THAT(lcpArray("banana"), ElementsAre(0, 1, 3, 0, 0, 2));
	EXPECT_THAT(lcpArray("AAAAAA$"), ElementsAre(0, 0, 1, 2, 3, 4, 5));
	EXPECT_THAT(lcpArray("ABRACADABRA$"), ElementsAre(0, 0, 1, 4, 1, 1, 0, 3, 0, 0, 0, 2));
	EXPECT_THAT(lcpArray("GATAGACA$"), ElementsAre(0, 0, 1, 1, 1, 0, 0, 2, 0));
}

TEST(Algorithms, suffixLcpArray)
{
	EXPECT_THAT(suffixLcpArray("aaaaaa"), ElementsAre(Pair(5, 0), Pair(4, 1), Pair(3, 2), Pair(2, 3), Pair(1, 4), Pair(0, 5)));
	EXPECT_THAT(suffixLcpArray("abcabcddd"), ElementsAre(Pair(0, 0), Pair(3, 3), Pair(1, 0), Pair(4, 2), Pair(2, 0), Pair(5, 1), Pair(8, 0), Pair(7, 1), Pair(6, 2)));
	EXPECT_THAT(suffixLcpArray("banana"), ElementsAre(Pair(5, 0), Pair(3, 1), Pair(1, 3), Pair(0, 0), Pair(4, 0), Pair(2, 2)));
	EXPECT_THAT(suffixLcpArray("AAAAAA$"), ElementsAre(Pair(6, 0), Pair(5, 0), Pair(4, 1), Pair(3, 2), Pair(2, 3), Pair(1, 4), Pair(0, 5)));
	EXPECT_THAT(suffixLcpArray("ABRACADABRA$"), ElementsAre(Pair(11, 0), Pair(10, 0), Pair(7, 1), Pair(0, 4), Pair(3, 1), Pair(5, 1), Pair(8, 0), Pair(1, 3), Pair(4, 0), Pair(6, 0), Pair(9, 0), Pair(2, 2)));
	EXPECT_THAT(suffixLcpArray("GATAGACA$"), ElementsAre(Pair(8, 0), Pair(7, 0), Pair(5, 1), Pair(3, 1), Pair(1, 1), Pair(6, 0), Pair(4, 0), Pair(0, 2), Pair(2, 0)));

}
