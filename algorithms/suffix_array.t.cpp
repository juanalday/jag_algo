#include "suffix_array.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using jag::algo::suffixArray;
using jag::algo::bruteForceSuffixArray;
using ::testing::ElementsAre;

TEST(Algorithms, bruteForceSuffixArray)
{
	EXPECT_THAT(bruteForceSuffixArray("aaaaaa"), ElementsAre(5, 4, 3, 2, 1, 0));
	EXPECT_THAT(bruteForceSuffixArray("aaaaaaa"), ElementsAre(6, 5, 4, 3, 2, 1, 0));
	EXPECT_THAT(bruteForceSuffixArray("abaaaa"), ElementsAre(5, 4, 3, 2, 0, 1));
	EXPECT_THAT(bruteForceSuffixArray("AAAAA$"), ElementsAre(5, 4, 3, 2, 1, 0));
	EXPECT_THAT(bruteForceSuffixArray("banana"), ElementsAre(5, 3, 1, 0, 4, 2));
	EXPECT_THAT(bruteForceSuffixArray("abcabcddd"), ElementsAre(0, 3, 1, 4, 2, 5, 8, 7, 6));
	EXPECT_THAT(bruteForceSuffixArray("ABCABCDDD$"), ElementsAre(9, 0, 3, 1, 4, 2, 5, 8, 7, 6));
	EXPECT_THAT(bruteForceSuffixArray("GATAGACA$"), ElementsAre(8, 7, 5, 3, 1, 6, 4, 0, 2));
	EXPECT_THAT(bruteForceSuffixArray("ABRACADABRA$"), ElementsAre(11, 10, 7, 0, 3, 5, 8, 1, 4, 6, 9, 2));

	std::string longInput = "aacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccb";
	std::vector<int> const longOutput = { 414,314,214,114,14,415,315,215,115,15,416,316,216,116,16,417,317,217,117,17,418,318,218,118,18,443,343,243,143,43,467,367,267,167,67,482,382,282,182,82,464,364,264,164,64,419,319,219,119,19,437,337,237,137,37,444,344,244,144,44,468,368,268,168,68,492,392,292,192,92,400,300,200,100,0,473,373,273,173,73,483,383,283,183,83,465,365,265,165,65,432,332,232,132,32,405,305,205,105,5,408,308,208,108,8,420,320,220,120,20,438,338,238,138,38,445,345,245,145,45,469,369,269,169,69,426,326,226,126,26,493,393,293,193,93,401,301,201,101,1,428,328,228,128,28,474,374,274,174,74,479,379,279,179,79,434,334,234,134,34,496,396,296,196,96,484,384,284,184,84,499,413,313,213,113,13,466,366,266,166,66,399,299,199,99,472,372,272,172,72,404,304,204,104,4,407,307,207,107,7,478,378,278,178,78,433,333,233,133,33,495,395,295,195,95,412,312,212,112,12,403,303,203,103,3,406,306,206,106,6,411,311,211,111,11,410,310,210,110,10,409,309,209,109,9,421,321,221,121,21,439,339,239,139,39,422,322,222,122,22,452,352,252,152,52,440,340,240,140,40,461,361,261,161,61,423,323,223,123,23,453,353,253,153,53,446,346,246,146,46,441,341,241,141,41,462,362,262,162,62,490,390,290,190,90,430,330,230,130,30,424,324,224,124,24,470,370,270,170,70,476,376,276,176,76,454,354,254,154,54,487,387,287,187,87,447,347,247,147,47,456,356,256,156,56,442,342,242,142,42,481,381,281,181,81,463,363,263,163,63,436,336,236,136,36,491,391,291,191,91,431,331,231,131,31,425,325,225,125,25,427,327,227,127,27,498,398,298,198,98,471,371,271,171,71,477,377,277,177,77,494,394,294,194,94,402,302,202,102,2,451,351,251,151,51,460,360,260,160,60,489,389,289,189,89,429,329,229,129,29,475,375,275,175,75,486,386,286,186,86,455,355,255,155,55,480,380,280,180,80,435,335,235,135,35,497,397,297,197,97,450,350,250,150,50,459,359,259,159,59,488,388,288,188,88,485,385,285,185,85,449,349,249,149,49,458,358,258,158,58,448,348,248,148,48,457,357,257,157,57 };
	EXPECT_EQ(longOutput, bruteForceSuffixArray(longInput));
}
TEST(Algorithms, suffixArray)
{
	EXPECT_THAT(suffixArray("aaaaaa"), ElementsAre(5, 4, 3, 2, 1, 0));
	EXPECT_THAT(suffixArray("aaaaaaa"), ElementsAre(6, 5, 4, 3, 2, 1, 0));
	EXPECT_THAT(suffixArray("abaaaa"), ElementsAre(5, 4, 3, 2, 0, 1));
	EXPECT_THAT(suffixArray("AAAAA$"), ElementsAre(5, 4, 3, 2, 1, 0));
	EXPECT_THAT(suffixArray("banana"), ElementsAre(5, 3, 1, 0, 4, 2));
	EXPECT_THAT(suffixArray("abcabcddd"), ElementsAre(0, 3, 1, 4, 2, 5, 8, 7, 6));
	EXPECT_THAT(suffixArray("ABCABCDDD$"), ElementsAre(9, 0, 3, 1, 4, 2, 5, 8, 7, 6));
	EXPECT_THAT(suffixArray("GATAGACA$"), ElementsAre(8, 7, 5, 3, 1, 6, 4, 0, 2));
	EXPECT_THAT(suffixArray("ABRACADABRA$"), ElementsAre(11, 10, 7, 0, 3, 5, 8, 1, 4, 6, 9, 2));

	std::string longInput = "aacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccb";
	std::vector<int> const longOutput = { 414,314,214,114,14,415,315,215,115,15,416,316,216,116,16,417,317,217,117,17,418,318,218,118,18,443,343,243,143,43,467,367,267,167,67,482,382,282,182,82,464,364,264,164,64,419,319,219,119,19,437,337,237,137,37,444,344,244,144,44,468,368,268,168,68,492,392,292,192,92,400,300,200,100,0,473,373,273,173,73,483,383,283,183,83,465,365,265,165,65,432,332,232,132,32,405,305,205,105,5,408,308,208,108,8,420,320,220,120,20,438,338,238,138,38,445,345,245,145,45,469,369,269,169,69,426,326,226,126,26,493,393,293,193,93,401,301,201,101,1,428,328,228,128,28,474,374,274,174,74,479,379,279,179,79,434,334,234,134,34,496,396,296,196,96,484,384,284,184,84,499,413,313,213,113,13,466,366,266,166,66,399,299,199,99,472,372,272,172,72,404,304,204,104,4,407,307,207,107,7,478,378,278,178,78,433,333,233,133,33,495,395,295,195,95,412,312,212,112,12,403,303,203,103,3,406,306,206,106,6,411,311,211,111,11,410,310,210,110,10,409,309,209,109,9,421,321,221,121,21,439,339,239,139,39,422,322,222,122,22,452,352,252,152,52,440,340,240,140,40,461,361,261,161,61,423,323,223,123,23,453,353,253,153,53,446,346,246,146,46,441,341,241,141,41,462,362,262,162,62,490,390,290,190,90,430,330,230,130,30,424,324,224,124,24,470,370,270,170,70,476,376,276,176,76,454,354,254,154,54,487,387,287,187,87,447,347,247,147,47,456,356,256,156,56,442,342,242,142,42,481,381,281,181,81,463,363,263,163,63,436,336,236,136,36,491,391,291,191,91,431,331,231,131,31,425,325,225,125,25,427,327,227,127,27,498,398,298,198,98,471,371,271,171,71,477,377,277,177,77,494,394,294,194,94,402,302,202,102,2,451,351,251,151,51,460,360,260,160,60,489,389,289,189,89,429,329,229,129,29,475,375,275,175,75,486,386,286,186,86,455,355,255,155,55,480,380,280,180,80,435,335,235,135,35,497,397,297,197,97,450,350,250,150,50,459,359,259,159,59,488,388,288,188,88,485,385,285,185,85,449,349,249,149,49,458,358,258,158,58,448,348,248,148,48,457,357,257,157,57 };
	EXPECT_EQ(longOutput, suffixArray(longInput));
}
