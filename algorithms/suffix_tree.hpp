#ifndef JAG_ALGO_SUFFIX_TREE_HPP
#define JAG_ALGO_SUFFIX_TREE_HPP

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <stack>
#include <vector>

namespace jag::algo {

	struct SuffixTree {

		struct Node {

			int m_start, m_end;
			std::string_view m_str;
			int m_link;
			std::map<char, int> m_edges;

			using const_iterator = std::map<char, int>::const_iterator;

			Node(int start, int end, int link) : m_start(start), m_end(end), m_link(link) {}

			bool contains(char c) const noexcept { return m_edges.count(c) != 0; }
			int& at(char c) noexcept { return m_edges[c]; }
			int at(char c) const noexcept { auto it = m_edges.find(c); return it == m_edges.end() ? -1 : it->second; }
			bool empty() const noexcept { return m_edges.empty(); }
			size_t size() const noexcept { return m_edges.size(); }
			bool isLeaf() const noexcept { return empty(); }
			const_iterator begin() const noexcept { return m_edges.begin(); }
			const_iterator end() const noexcept { return m_edges.end(); }
			std::string_view const& str() const noexcept { return m_str; }
			int start() const noexcept { return m_start; }
			void setLength(int len) noexcept { m_end = m_start + len; m_str = m_str.substr(0, len); }

			int edgeLength() const noexcept { return static_cast<int>(m_str.size()); }
			int edgeLength(int pos) const noexcept { return std::max(m_end, pos + 1) - m_start; }

			void clear() noexcept { m_edges.clear(); m_start = 0; m_end = 0;}

			void adjust(std::string const& m_data) {
				m_str = std::string_view(m_data);
				m_str.remove_prefix(m_start);
				if (m_end != -1)
					m_str = m_str.substr(0, m_end - m_start);

			}
			void swap(Node& rhs) noexcept {
				using std::swap;
				swap(m_start, rhs.m_start);
				swap(m_end, rhs.m_end);
				swap(m_str, rhs.m_str);
				swap(m_link, rhs.m_link);
				swap(m_edges, rhs.m_edges);
			}
		};


		int m_activeNode, m_activeEdge, m_activeLength, m_remainder;
		int m_needSL;
		std::vector <Node> m_nodes;
		std::string m_data;

		using const_iterator = std::vector<Node>::const_iterator;


		SuffixTree(std::string const& str)
			: m_activeNode{ 0 }
			, m_activeEdge{ 0 }
			, m_activeLength{ 0 }
			, m_remainder{ 0 }
			, m_needSL{ 0 }
			, m_data{ str }
		{
			m_data.push_back('$'); // Add a terminator character
			m_nodes.reserve(str.size() * 2 + 1);

			m_nodes.emplace_back(0, 0, 0);
			int pos(0);
			for (int pos = 0; pos < m_data.size(); ++pos)
			{
				char c = m_data[pos];
				m_needSL = 0;
				++m_remainder;

				while (m_remainder > 0) {

					if (m_activeLength == 0)
						m_activeEdge = pos;

					char activeEdgeLetter = m_data[m_activeEdge];

					if (!m_nodes[m_activeNode].contains(activeEdgeLetter)) {
						int leaf = createNode(pos);
						m_nodes[m_activeNode].at(activeEdgeLetter) = leaf;
						addSuffixLink(m_activeNode); //Rule 2
					}
					else {
						int nextId = m_nodes[m_activeNode].at(activeEdgeLetter);
						// Observation 2:
						// If at some point active_length is greater or equal to the length of current edge(edge_length),
						// we move our active point down until edge_length is strictly greater than active_length.
						if (walkDown(nextId))
							continue;
						// Observation 1:
						// When the final suffix we need to insert is found to exist in the tree already, 
						// the tree itself is not changed at all(we only update the active point and remainder).
						if (m_data[m_nodes[nextId].m_start + m_activeLength] == c) {
							++m_activeLength;
							// Observation 3:
							// When the symbol we want to add to the tree is already on the edge, 
							// we, according to Observation 1, update only active point and remainder, 
							// leaving the tree unchanged.
							// BUT if there is an internal node marked as needing suffix link, 
							// we must connect that node with our current active node through a suffix link.
							addSuffixLink(m_activeNode);
							break;
						}
						// We get here when the activePoint is in the middle of an edge, 
						// and the edge does not contain the next character.
						// In this case, we must split the edge.
						// We create a new internal node and a new leaf node,
						// and we connect the new internal node with the old one through the new leaf node.
						// We also create a suffix link from the old internal node to the new one.
						// The new internal node will have an edge for the new character,

						/* THIS IS THE OLD CODE, WHERE THE SPLIT NODE IS A NEW ONE 
						
						int splitId = createNode(m_nodes[nextId].m_start, m_nodes[nextId].m_start + m_activeLength);
						int leafId = createNode(pos);
						Node& activeNode = m_nodes[m_activeNode];
						Node& split = m_nodes[splitId];
						Node& leaf = m_nodes.back();

						
						// New leaf coming out of the new internal node
						m_nodes[nextId].m_start += m_activeLength;
						auto nextIdStartChar = m_data[m_nodes[nextId].m_start];

						activeNode.at(activeEdgeLetter) = splitId; // Now activenode has an edge pointing here
						split.at(c) = leafId;
						split.at(nextIdStartChar) = nextId;

						// We got a new internal node. If there is any internal node created in last extensions
						// of same phase which is still waiting for it's suffix link reset, do it now.
						addSuffixLink(splitId); //rule 2
						*/

						// New code, I reuse the old node for the split node (I like the topology of the tree better)

						m_nodes.push_back(m_nodes[nextId]); // Copy the node, I want to reuse the bucket for the split node
						int leafId = createNode(pos);
						int splitId = leafId - 1;

						Node& internalNode = m_nodes[nextId];
						Node& split = m_nodes[splitId];
						Node& leaf = m_nodes.back();

						internalNode.setLength(m_activeLength);
						split.m_start += m_activeLength;
						//split.adjust(m_data);

						char activeLetter = m_data[split.m_start];
						// The internal node now will point to the 'old' activeEdge and the new character c
						internalNode.at(c) = leafId;
						internalNode.at(activeLetter) = splitId;
						// We got a new internal node. If there is any internal node created in last extensions
						// of same phase which is still waiting for it's suffix link reset, do it now.
						addSuffixLink(splitId); // rule 2


					}
					--m_remainder;
					// Rule 1: If after an insertion from the active node = root, the active length is greater than 0, then:
					// 1. active node is not changed
					// 2. active length is decremented
					// 3. active edge is shifted right(to the first character of the next suffix we must insert)
					if ((m_activeNode == 0) && (m_activeLength > 0)) { //rule 1
						--m_activeLength;
						m_activeEdge = pos - m_remainder + 1;
					}
					else {
						// Rule 3
						// After an insert from the active node which is not the root node, 
						// we must follow the suffix link and set the active node to the node it points to.
						// If there is no a suffix link, set the active node to the root node.
						// Either way, active edge and active length stay unchanged.
						m_activeNode = m_nodes[m_activeNode].m_link > 0 ? m_nodes[m_activeNode].m_link : 0;
					}
				}
			}
			for_each(m_nodes.begin(), m_nodes.end(), [&](Node& node) { node.adjust(m_data); });
		}

		void reset(std::string const& str)
		{
			SuffixTree(str).swap(*this);
		}

		void swap(SuffixTree& other) noexcept
		{
			using std::swap;
			swap(m_activeNode, other.m_activeNode);
			swap(m_activeEdge, other.m_activeEdge);
			swap(m_activeLength, other.m_activeLength);
			swap(m_remainder, other.m_remainder);
			swap(m_needSL, other.m_needSL);
			swap(m_data, other.m_data);
			swap(m_nodes, other.m_nodes);
			for_each(m_nodes.begin(), m_nodes.end(), [&](Node& node) { node.adjust(m_data); });
			for_each(other.m_nodes.begin(), other.m_nodes.end(), [&](Node& node) { node.adjust(other.m_data); });
		}

 
		const_iterator begin() const noexcept { return m_nodes.begin(); }
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    const_iterator end() const noexcept { return m_nodes.end(); }
		size_t size() const noexcept { return m_nodes.size(); }
		Node const& operator[](int index) const { return m_nodes[index]; }

		int createNode(int start, int end = -1) {
			m_nodes.emplace_back(start, end, 0);
			m_nodes.back().adjust(m_data);
			return static_cast<int>(m_nodes.size()) - 1;
		}

		void addSuffixLink(int node) {
			if (m_needSL > 0) m_nodes[m_needSL].m_link = node;
			m_needSL = node;
		}

		bool walkDown(int nodeId) {
			int pos = static_cast<int>(m_data.size()) - 1;
			auto const& node = m_nodes[nodeId];
			int l = node.edgeLength(pos);
			if (m_activeLength >= l) {
				m_activeEdge += l;
				m_activeLength -= l;
				m_activeNode = nodeId;
				return true;
			}
			return false;
		}

		bool contains(std::string const& str) const {
			size_t patternIndex = 0; // Index to track position in the pattern string

			const_iterator nodeIter = m_nodes.begin(); // Start from the root node
			while (patternIndex < str.size()) {
				char c = str[patternIndex];
				if (!nodeIter->contains(c))
					return false;
				int nextNode = nodeIter->m_edges.at(c); // Just for debugging purposes
				nodeIter = next(m_nodes.begin(), nodeIter->m_edges.at(c));

				auto& node = *nodeIter;
				auto view = node.m_str; // Substring represented by the edge [start, end]
				int start = node.m_start; // Start index of the substring represented by the edge
				int end = (node.m_end == -1) ? static_cast<int>(m_data.size()) : node.m_end; // Correct end index
				for (int i = start; i < end && patternIndex < str.size(); i++, patternIndex++) {
					if (m_data[i] != str[patternIndex]) {
						return false; // Pattern not found
					}
				}
			}

			return true;
		}

	};



} // end of namespace jag::algo

#endif // JAG_ALGO_SUFFIX_TREE_HPP