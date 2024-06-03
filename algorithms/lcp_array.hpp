#ifndef JAG_ALGO_LCP_ARRAY_HPP
#define JAG_ALGO_LCP_ARRAY_HPP

#include "suffix_array.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace jag::algo {

	// Longest Common Prefix array
	inline std::vector<int> lcpArray(std::string const& str, std::vector<int> const& sa)
	{
		int n = static_cast<int>(sa.size());
		std::vector<int> lcpWithPrev(n); // To store LCP with the previous suffix

		std::vector<int> prevSuffix(n, -1);  // To store the index of the previous suffix in the suffix array
		int lcpLength(0);  // Length of the current longest common prefix

		// Compute the prevSuffix array
		for (int i = 1; i < n; ++i) prevSuffix[sa[i]] = sa[i - 1];

		// Compute lcpWithPrev using the prevSuffix array
		for (int i = 0; i < n; ++i) {
			if (prevSuffix[i] == -1) {
				lcpWithPrev[i] = 0;
				continue;
			}

			// Calculate the longest common prefix length
			while (str[i + lcpLength] == str[prevSuffix[i] + lcpLength]) {
				++lcpLength;
			}
			lcpWithPrev[i] = lcpLength; // Store the LCP length with the previous suffix
			lcpLength = std::max(lcpLength - 1, 0); // Decrease lcpLength for the next iteration
		}

		// Construct the final LCP array from lcpWithPrev
		//for (int i = 0; i < n; ++i) lcp[i] = lcpWithPrev[sa[i]];
		std::vector<int> lcp(n, 0); // To store the final LCP array
		std::transform(sa.begin(), sa.end(), lcp.begin(), [&lcpWithPrev](int i) {return lcpWithPrev[i]; });
		return lcp;
	}

	inline std::vector<int> lcpArray(std::string const& str) {
		return lcpArray(str, suffixArray(str));
	}

	inline std::vector<std::pair<int, int>> suffixLcpArray(std::string const& str) {
		std::vector<int> const suffixes = suffixArray(str);
		std::vector<int> const lcp = lcpArray(str, suffixes);
		std::vector<std::pair<int, int>> ret(str.size());
		transform(suffixes.cbegin(), suffixes.cend(), lcp.cbegin(), ret.begin(), [](int start, int len) noexcept { return std::make_pair(start, len); });
		return ret;
	}

} // namespace jag::algo

#endif // JAG_ALGO_LCP_ARRAY_HPP
