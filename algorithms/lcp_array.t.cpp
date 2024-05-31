#include "lcp_array.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using jag::algo::lcpArray;

using ::testing::ElementsAre;

TEST(Algorithms, lcpArray)
{
	EXPECT_THAT(lcpArray("banana"), ElementsAre(0, 1, 3, 0, 0, 2));
	EXPECT_THAT(lcpArray("aaaaaa"), ElementsAre(0, 1, 2, 3, 4, 5));
	EXPECT_THAT(lcpArray("AAAAAA$"), ElementsAre(0, 0, 1, 2, 3, 4, 5));
	EXPECT_THAT(lcpArray("abcabcddd"), ElementsAre(0, 3, 0, 2, 0, 1, 0, 1, 2));
	EXPECT_THAT(lcpArray("GATAGACA$"), ElementsAre(0, 0, 1, 1, 1, 0, 0, 2, 0));
	EXPECT_THAT(lcpArray("ABRACADABRA$"), ElementsAre(0, 0, 1, 4, 1, 1, 0, 3, 0, 0, 0, 2));
}
