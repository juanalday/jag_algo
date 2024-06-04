#ifndef JAG_ALGO_SUFFIX_AUTOMATON_HPP
#define JAG_ALGO_SUFFIX_AUTOMATON_HPP

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace jag::algo {
    class SuffixAutomaton {
    public:
        struct State {
            char m_character;
            std::unordered_map<char, int> m_edges;
            int m_length, m_link;
            int m_size;
            bool m_final;
        };

        SuffixAutomaton() : m_last(0)
        {
            m_states.emplace_back();
            State& state = m_states.back();
            state.m_link = -1;
            state.m_length = 0;
            state.m_size = 1;
            state.m_final = false;
            state.m_character = '$';

        }
        SuffixAutomaton(const std::string& s) : SuffixAutomaton() {
            for (auto c : s)
                append(c);

            finalize();

            std::vector<std::vector<int>> statesPerLen(s.length() + 1);
            for (int i  = 0; i < static_cast<int>(m_states.size()); ++i)
                statesPerLen[m_states[i].m_length].push_back(i);
            for (auto it = statesPerLen.rbegin(); it != statesPerLen.rend(); ++it)
                for (auto ind : *it)
                    if (m_states[ind].m_link != -1)
                        m_states[m_states[ind].m_link].m_size += m_states[ind].m_size;
        }

        SuffixAutomaton& finalize() {
			for (int i = m_last; i != -1; i = m_states[i].m_link)
				m_states[i].m_final = true;
			return *this;
		}

        int traverse(const std::string& s, int stateIndex = 0) const {
            for (auto c : s)
                stateIndex = traverse(c, stateIndex);
            return stateIndex;
        }

        int traverse(char c, int stateIndex) const {
            if (stateIndex == -1)
                return -1;
            auto itNext = m_states[stateIndex].m_edges.find(c);
            if (itNext != end(m_states[stateIndex].m_edges))
                return itNext->second;
            else
                return -1;
        }

        int size(int index) const { return m_states[index].m_size; }

        bool isFinal(int index) const { return index != -1 && m_states[index].m_final; }

        std::vector<State> const& states() const noexcept { return m_states; }

        bool contains(const std::string& s) const { return traverse(s) != -1; }
        bool isSuffix(const std::string& s) const { return isFinal(traverse(s)); }

        constexpr bool empty() const noexcept { return m_states.size() <2; }
        constexpr size_t size() const noexcept { return m_states.size();}
        State const& operator[](size_t i) const { return m_states[i]; }

        SuffixAutomaton& append(char c) {
            // We add new state corresponding to s + c
            // It will has index curr.
            m_states.emplace_back();
            int curr = static_cast<int>(m_states.size() -1);
            {
                State& state = m_states.back();
                state.m_length = m_states[m_last].m_length + 1;
                state.m_link = -2;
                state.m_size = 1;
                state.m_final = false;
                state.m_character = c;
            }

            int p = m_last;
            while (p != -1 && m_states[p].m_edges.count(c) == 0) {
                m_states[p].m_edges[c] = curr;
                p = m_states[p].m_link;
            }
            if (p == -1)
                m_states[curr].m_link = 0;
            else {
                int q = m_states[p].m_edges.at(c);
                if (m_states[p].m_length + 1 == m_states[q].m_length)  // (p, q) is solid edge
                    m_states[curr].m_link = q;
                else {
                    int clone = static_cast<int>(m_states.size());     // (p, q) is not solid,
                    m_states.push_back(m_states[q]);  // we do a split q ---> clone, q
                    m_states[clone].m_length = m_states[p].m_length + 1;
                    m_states[clone].m_size = 0;
                    m_states[curr].m_link = clone;
                    m_states[q].m_link = clone;
                    for (; p != -1; p = m_states[p].m_link) {
                        auto it = m_states[p].m_edges.find(c);
                        if (it != end(m_states[p].m_edges) && it->second == q)
                            it->second = clone;
                        else
                            break;
                    }
                }
            }

            assert(m_states[curr].m_link != -2);
            m_last = curr;
			return *this;
        }

        std::vector<State> m_states;
        int m_last;
	};
} // namespace jag::algo
#endif //JAG_ALGO_SUFFIX_AUTOMATON_HPP
