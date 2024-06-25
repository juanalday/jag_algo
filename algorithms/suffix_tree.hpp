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

			int m_start, m_end, m_link, m_suffixIndex;
			std::map<char, int> m_edges;
			std::string_view m_str;

			using const_iterator = std::map<char, int>::const_iterator;

			Node(int start, int end, int link) : m_start(start), m_end(end), m_link(link), m_suffixIndex{ -1 } {}
			bool contains(char c) const noexcept { return m_edges.count(c) != 0; }
			int& at(char c) noexcept { return m_edges[c]; }
			int at(char c) const noexcept { auto it = m_edges.find(c); return it == m_edges.end() ? -1 : it->second; }
			bool empty() const noexcept { return m_edges.empty(); }
			bool isLeaf() const noexcept { return empty(); }
			const_iterator begin() const noexcept { return m_edges.begin(); }
			const_iterator end() const noexcept { return m_edges.end(); }

			int edgeLength() const noexcept { return static_cast<int>(m_str.size()); }
			int edgeLength(int pos) const noexcept {return std::max(m_end, pos + 1) - m_start;}

			void adjust(std::string const& m_data) {
				if (m_start == -1)
					return;
				m_str = std::string_view(m_data);
				m_str.remove_prefix(m_start);
				if (m_end != -1)
					m_str = m_str.substr(0, m_end - m_start);

			}
		};

		
		int m_activeNode, m_activeEdge, m_activeLength, m_remainder;
		int m_needSL;
		std::vector <Node> m_nodes;
		std::string m_data;

		using const_iterator = std::vector<Node>::const_iterator;

		int createNode(int start, int end = -1) {
			m_nodes.emplace_back(start, end, 0);
			m_nodes.back().adjust(m_data);
			return static_cast<int>(m_nodes.size()) - 1;
		}

		char activeEdge() const { return m_data[m_activeEdge]; }
		void addSuffixLink(int node) {
			if (m_needSL > 0) m_nodes[m_needSL].m_link = node;
			m_needSL = node;
		}

		bool walkDown(int nodeId){
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
		
		SuffixTree(std::string const& str) 
			: m_activeNode{ 0 }
			, m_activeEdge{0}
			, m_activeLength{ 0 }
			, m_remainder{ 0 } 
			, m_needSL{0}
		{
			m_nodes.reserve(str.size() * 2 + 1);
			m_nodes.emplace_back(-1, -1, 0);
			for_each(str.begin(), str.end(), [&](char c) { add(c); });
			add('$');
			for_each(m_nodes.begin(), m_nodes.end(), [&](Node& node) { node.adjust(m_data); });
			setSuffixIndex(0, 0);
		}

		void setSuffixIndex(int index, int labelHeight) {
			Node& node = m_nodes[index];
			if (node.isLeaf()) {
				node.m_suffixIndex = static_cast<int>(m_data.size()) - labelHeight;
				return;
			}

			for (auto [key, idx] : node.m_edges) {
				setSuffixIndex(idx, labelHeight + m_nodes[idx].edgeLength());
			}
		}
		void reset(std::string const& str)
		{
			SuffixTree(str).swap(*this);
		}

		void swap(SuffixTree& other) noexcept {
			using std::swap;
			swap(m_activeNode, other.m_activeNode);
			swap(m_activeEdge, other.m_activeEdge);
			swap(m_activeLength, other.m_activeLength);
			swap(m_remainder, other.m_remainder);
			swap(m_needSL, other.m_needSL);
			swap(m_nodes, other.m_nodes);
			swap(m_data, other.m_data);
		}

		void add(char c) {
	
			m_data.push_back(c);
			int pos = static_cast<int>(m_data.size()) - 1;

			m_needSL = 0;
			++m_remainder;
			while (m_remainder > 0) {

				if (m_activeLength == 0)
					m_activeEdge = pos;

				if (!m_nodes[m_activeNode].contains(m_data[m_activeEdge])) {
					int leaf = createNode(pos);
					m_nodes[m_activeNode].at(activeEdge()) = leaf;
					addSuffixLink(m_activeNode); //Rule 2
				}
				else {
					int nextId = m_nodes[m_activeNode].at(activeEdge());
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

					int splitId = createNode(m_nodes[nextId].m_start, m_nodes[nextId].m_start + m_activeLength);
					Node& split = m_nodes.back();
					m_nodes[m_activeNode].at(activeEdge()) = splitId;
					// New leaf coming out of the new internal node
					split.at(c) = createNode(pos);
					m_nodes[nextId].m_start += m_activeLength;
					split.at(m_data[m_nodes[nextId].m_start]) = nextId;

					// We got a new internal node. If there is any internal node created in last extensions
					// of same phase which is still waiting for it's suffix link reset, do it now.
					addSuffixLink(splitId); //rule 2

				}
				--m_remainder;
				// Rule 1: If after an insertion from the active node = root, the active length is greater than 0, then:
				// 1. active node is not changed
				// 2. active length is decremented
				// 3. active edge is shifted right(to the first character of the next suffix we must insert)
				if ( (m_activeNode == 0) && (m_activeLength > 0) ) { //rule 1
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

		const_iterator begin() const noexcept { return m_nodes.begin(); }
		const_iterator end() const noexcept { return m_nodes.end(); }
		size_t size() const noexcept { return m_nodes.size(); }
		Node const& operator[](int index) const { return m_nodes[index]; }

		bool contains(std::string const& str) const {
			size_t patternIndex = 0; // Index to track position in the pattern string

			const_iterator nodeIter = m_nodes.begin(); // Start from the root node
			while (patternIndex < str.size()) {
				char c = str[patternIndex];
				// Check if the current node has an edge starting with the current character
				if (!nodeIter->contains(c)) {
					return false; // Pattern not found if the edge doesn't exist
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
						return false; // Pattern not found
					}
				}
				
			}

			return true;
		}

		

		std::vector<int> findAll(std::string const& str) const {
			int currentNode = 0;
			size_t patternIndex = 0; // Index to track position in the pattern string
			const_iterator nodeIter = m_nodes.begin(); // Start from the root node

			int lenCounter(0);
			int lastNodeLen = 0;
			while (patternIndex < str.size()) {
				lenCounter = 0;
				char c = str[patternIndex];
				// Check if the current node has an edge starting with the current character
				if (!nodeIter->contains(c)) {
					return {}; // Pattern not found if the edge doesn't exist
				}
				// Move to the next node along the edge labeled with currentChar
				currentNode = nodeIter->at(c);
				auto const& node = m_nodes[currentNode];
				lastNodeLen = static_cast<int>(node.m_str.size());
				auto view = node.m_str; // Substring represented by the edge [start, end]
				int start = node.m_start; // Start index of the substring represented by the edge
				int end = (node.m_end == -1) ? static_cast<int>(m_data.size()) : 1 + node.m_end; // Correct end index
				for (int i = start; i < end && patternIndex < str.size(); i++, patternIndex++) {
					++lenCounter;
					if (m_data[i] != str[patternIndex]) {
						return {}; // Pattern not found
					}
				}

			}

			std::vector<int> result;
			std::vector<std::pair<int, int> > nodes = { {currentNode, 0} };

			while (!nodes.empty()) {
				auto [level, currentNodeId] = nodes.back();
				nodes.pop_back();
				Node const& currentNode = m_nodes[currentNodeId];

				if (currentNode.isLeaf()) {
					int startPoint = currentNode.m_start - lenCounter;
					result.push_back(startPoint);
				}
				else {
					++level;
					for_each(currentNode.m_edges.begin(), currentNode.m_edges.end(), [&](auto const& edge) {nodes.push_back({ level, edge.second }); });
				}
			}

			return result;
		}
	};



} // end of namespace jag::algo

#endif // JAG_ALGO_SUFFIX_TREE_HPP