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
		int m_root, m_needSL, m_activeNode, m_ActiveEdge, m_activeLen, m_remainder;

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
			: m_root(0)
			, m_needSL(0)
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
				if (m_activeNode == m_root && m_activeLen > 0) { //rule 1
					--m_activeLen;
					m_ActiveEdge = pos - m_remainder + 1;
				}
				else {
					// Rule 3
					// After an insert from the active node which is not the root node, 
					// we must follow the suffix link and set the active node to the node it points to.
					// If there is no a suffix link, set the active node to the root node.
					// Either way, active edge and active length stay unchanged.
					m_activeNode = m_nodes[m_activeNode].m_link > 0 ? m_nodes[m_activeNode].m_link : m_root;
				}
			}
		}
	};



} // end of namespace jag::algo

#endif // JAG_ALGO_SUFFIX_TREE_HPP