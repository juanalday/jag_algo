#ifndef JAG_ALGO_SUFFIX_TREE_HPP
#define JAG_ALGO_SUFFIX_TREE_HPP

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace jag::algo {

	struct SuffixTree {


		struct Node {

			int m_start, m_end, m_link;
			std::map<char, int> m_edges;
			std::string_view m_str;

			Node(int start, int end, int link) : m_start(start), m_end(end), m_link(link) {}
			bool contains(char c) const noexcept { return m_edges.count(c) != 0; }
			int at(char c) const noexcept { auto it = m_edges.find(c); return it == m_edges.end() ? -1 : it->second; }

			int edge_length(int pos) {
				return std::max(m_end, pos + 1) - m_start;
			}

			void adjust(std::string const& m_data) {
				if (m_start == -1)
					return;
				m_str = std::string_view(m_data);
				m_str.remove_prefix(m_start);
				if (m_end != -1)
					m_str.remove_suffix(m_str.size() - m_end);

			}
		};

		std::vector <Node> m_nodes;
		std::string m_data;
		int m_needSL, m_activeNode, m_ActiveEdge, m_activeLen, m_remainder;

		using const_iterator = std::vector<Node>::const_iterator;

		int new_node(int start, int end = -1) {
			m_nodes.emplace_back(start, end, 0);
			return static_cast<int>(m_nodes.size()) - 1;
		}

		char active_edge() {
			return m_data[m_ActiveEdge];
		}

		// Rule 2: 
		// If we create a new internal node OR make an inserter from an internal node, 
		// and this is not the first SUCH internal node at current step, 
		// then we link the previous SUCH node with THIS one through a suffix link.
		void addSuffixLink(int node) {
			if (m_needSL > 0) m_nodes[m_needSL].m_link = node;
			m_needSL = node;
		}

		bool walk_down(int node) {
			int pos = static_cast<int>(m_data.size()) - 1;
			if (m_activeLen >= m_nodes[node].edge_length(pos)) {
				m_ActiveEdge += m_nodes[node].edge_length(pos);
				m_activeLen -= m_nodes[node].edge_length(pos);
				m_activeNode = node;
				return true;
			}
			return false;
		}




		SuffixTree()
			: m_needSL(0)
			, m_activeNode(0)
			, m_ActiveEdge(0)
			, m_activeLen(0)
			, m_remainder{ 0 } {
			m_nodes.emplace_back(-1, -1, 0);
		}

		SuffixTree(std::string const& str) : SuffixTree() {
			for_each(str.begin(), str.end(), [&](char c) { add(c); });
			for_each(m_nodes.begin(), m_nodes.end(), [&](Node& node) { node.adjust(m_data); });
		}

		void reset(std::string const& str)
		{
			SuffixTree(str).swap(*this);
		}

		void swap(SuffixTree& other) noexcept {
			using std::swap;
			swap(m_nodes, other.m_nodes);
			swap(m_data, other.m_data);
			swap(m_needSL, other.m_needSL);
			swap(m_activeNode, other.m_activeNode);
			swap(m_ActiveEdge, other.m_ActiveEdge);
			swap(m_activeLen, other.m_activeLen);
			swap(m_remainder, other.m_remainder);
		}

		void add(char c) {
			m_data.push_back(c);
			int pos = static_cast<int>(m_data.size()) - 1;

			m_needSL = 0;
			++m_remainder;
			while (m_remainder > 0) {
				if (m_activeLen == 0) m_ActiveEdge = pos;
				if (!m_nodes[m_activeNode].contains(active_edge())) {
					int leaf = new_node(pos);
					m_nodes[m_activeNode].m_edges[active_edge()] = leaf;
					addSuffixLink(m_activeNode); //Rule 2
				}
				else {
					int nxt = m_nodes[m_activeNode].m_edges[active_edge()];
					// Observation 2:
					// If at some point active_length is greater or equal to the length of current edge(edge_length),
					// we move our active point down until edge_length is strictly greater than active_length.
					if (walk_down(nxt))
						continue;
					// Observation 1:
					// When the final suffix we need to insert is found to exist in the tree already, 
					// the tree itself is not changed at all(we only update the active point and remainder).
					if (m_data[m_nodes[nxt].m_start + m_activeLen] == c) {
						m_activeLen++;
						// Observation 3:
						// When the symbol we want to add to the tree is already on the edge, 
						// we, according to Observation 1, update only active point and remainder, 
						// leaving the tree unchanged.
						// BUT if there is an internal node marked as needing suffix link, 
						// we must connect that node with our current active node through a suffix link.
						addSuffixLink(m_activeNode);
						break;
					}
					int split = new_node(m_nodes[nxt].m_start, m_nodes[nxt].m_start + m_activeLen);
					m_nodes[m_activeNode].m_edges[active_edge()] = split;
					int leaf = new_node(pos);
					m_nodes[split].m_edges[c] = leaf;
					m_nodes[nxt].m_start += m_activeLen;
					m_nodes[split].m_edges[m_data[m_nodes[nxt].m_start]] = nxt;
					addSuffixLink(split); //rule 2
				}
				--m_remainder;
				// Rule 1: If after an insertion from the active node = root, the active length is greater than 0, then:
				// 1. active node is not changed
				// 2. active length is decremented
				// 3. active edge is shifted right(to the first character of the next suffix we must insert)
				if ( (m_activeNode == 0) && (m_activeLen > 0) ) { //rule 1
					--m_activeLen;
					m_ActiveEdge = pos - m_remainder + 1;
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

		const_iterator end() const noexcept { return m_nodes.end(); }

		const_iterator find(std::string const& str) const{
			size_t patternIndex = 0; // Index to track position in the pattern string

			const_iterator nodeIter = m_nodes.begin(); // Start from the root node
			while (patternIndex < str.size()) {
				char c = str[patternIndex];
				// Check if the current node has an edge starting with the current character
				if (!nodeIter->contains(c)) {
					return m_nodes.end(); // Pattern not found if the edge doesn't exist
				}
				// Move to the next node along the edge labeled with currentChar
				int nextNode = nodeIter->m_edges.at(c);
				nodeIter = next(m_nodes.begin(), nextNode);
				auto& node = *nodeIter;
				auto view = node.m_str; // Substring represented by the edge [start, end]
				int start = node.m_start; // Start index of the substring represented by the edge
				int end = (node.m_end == -1) ? static_cast<int>(m_data.size()) : node.m_end; // Correct end index
				for (int i = start; i < end && patternIndex < str.size(); i++, patternIndex++) {
					if (m_data[i] != str[patternIndex]) {
						return m_nodes.end(); // Pattern not found
					}
				}
				
			}

			return nodeIter;
		}

		bool contains(std::string const& str) const {return find(str) != m_nodes.end();}
			
		
		std::vector<int> collectLeaves(int node) {
			std::vector<int> result;
			std::vector<int> stack = { node };

			while (!stack.empty()) {
				int currentNode = stack.back();
				stack.pop_back();

				if (m_nodes[currentNode].m_edges.empty()) {
					// This is a leaf node
					result.push_back(m_nodes[currentNode].m_start - m_activeLen + 1);
				}
				else {
					for (const auto& [key, value] : m_nodes[currentNode].m_edges) {
						stack.push_back(value);
					}
				}
			}

			return result;
		}
	};



} // end of namespace jag::algo

#endif // JAG_ALGO_SUFFIX_TREE_HPP