#ifndef JAG_ALGO_STRING_ARRAY_HPP
#define JAG_ALGO_STRING_ARRAY_HPP

#include <algorithm>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

namespace jag::algo {
	inline std::vector<int> bruteForceSuffixArray(std::string const& str) {
		std::vector<std::string> suffixes(str.size());
		std::vector<int> suffixArray(str.size(), static_cast<int>(str.size()));
		// Create suffixes
		for_each(suffixes.begin(), suffixes.end(), [&str, idx = 0](std::string& suffix) mutable {str.substr(idx++).swap(suffix); });
		// Sort the suffixes
		std::sort(suffixes.begin(), suffixes.end());
		// Store the starting indices of the sorted suffixes in the suffix array
		std::transform(suffixes.begin(), suffixes.end(), suffixArray.begin(), [len = str.size()](std::string const& s) noexcept {return static_cast<int>(len - s.size()); });

		return suffixArray;
	}


	// Prefix-doubling algo to build a suffixArray
	inline std::vector<int> suffixArray(std::string const& str) {
		int len = static_cast<int>(str.size());

		// I will create a vector of tuples, describing the ranks, the next ranks and the suffixes (meaning rank of the next suffix + k)
		std::vector<std::tuple<int, int, int>> rankedSuffixes(str.size());
		// initial ranks are the ASCII values of the chars, and for id I pick 0 to n-1. the next-rank is -1 for now
		std::transform(str.cbegin(), str.cend(), rankedSuffixes.begin(),
			[i = 0](auto c) mutable noexcept {return std::make_tuple(c, -1, i++);
			});

		// Adjust next-rank to rank of the next element. We won't touch the last one obviously
		// In this case, it means the ascii value of the next char
		std::transform(rankedSuffixes.begin(), prev(rankedSuffixes.end()), next(rankedSuffixes.cbegin()), rankedSuffixes.begin(),
			[](auto& current, auto const& next) noexcept {std::get<1>(current) = std::get<0>(next); return current;
			});

		// Since tuple is rank-followingTank-index, this sorts according to rank value
		sort(rankedSuffixes.begin(), rankedSuffixes.end());
		
		for (int skip = 1; skip < len; skip *= 2)
		{
			std::adjacent_difference(rankedSuffixes.cbegin(), rankedSuffixes.cend(), rankedSuffixes.begin(),
				[rank = 0](auto curr, auto const& prev) mutable noexcept {
					rank += ((std::get<0>(prev) == std::get<0>(curr)) && (std::get<1>(prev) == std::get<1>(curr))) ? 0 : 1;
					std::get<0>(curr) = rank;
					std::get<1>(curr) = -1;
					return curr;
				});
			std::get<0>(rankedSuffixes[0]) = 0;


			// I will sort according to the index, so I can use the index to find the next element
			// This is faster than doing find_if for every index to find where is the index+skip value
			sort(rankedSuffixes.begin(), rankedSuffixes.end(), [](auto const& lhs, auto const& rhs) noexcept {return std::get<2>(lhs) < std::get<2>(rhs); });

			// I will now adjust the values of rank[sa[i]+k] for all i's within range
			std::transform(std::next(rankedSuffixes.cbegin(), skip), rankedSuffixes.cend(), rankedSuffixes.cbegin(), rankedSuffixes.begin(),
				[](auto const& skipRank, auto  currentRank) noexcept {std::get<1>(currentRank) = std::get<0>(skipRank); return currentRank; });

			// And I sort again, according to ranks...
			sort(rankedSuffixes.begin(), rankedSuffixes.end());
			// Optimization: If the last element has rank == len - 1, we can stop
			if (std::get<0>(rankedSuffixes[0]) == len - 1)
				break;
		}

		std::vector<int> suffixes(len);
		transform(rankedSuffixes.cbegin(), rankedSuffixes.cend(), suffixes.begin(), [](auto const& tup) noexcept {return std::get<2>(tup); });
		return suffixes;
	}
} // namespace jag::algo


#endif // JAG_ALGO_STRING_ARRAY_HPP
