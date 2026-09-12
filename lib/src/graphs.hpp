#include <iostream>
#include <vector>
#include <stack>

class Graph {

    int numVertices;
    std::vector<std::vector<int>> adjList;

    enum class Color {
        White,  
        Gray, 
        Black 
    };

public:
    Graph(int vertices) : numVertices(vertices), adjList(vertices) {}

    void addEdge(int u, int v) {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
            adjList[u].push_back(v);
        }
    }

    bool topologicalSort(std::vector<int>& result) {
        std::vector<Color> color(numVertices, Color::White);
        result.clear();
        result.reserve(numVertices);

        std::stack<std::pair<int, int>> st;

        for (int start = 0; start < numVertices; ++start) {
            if (color[start] != Color::White) continue;

            st.push({start, 0});
            color[start] = Color::Gray;
            while (!st.empty()) {
                auto& top = st.top();
                int u = top.first;
                int& idx = top.second;

                bool pushed = false;
                while (idx < (int)adjList[u].size()) {
                    int v = adjList[u][idx];
                    idx++;

                    if (color[v] == Color::Gray) {
                        return false;
                    }
                    if (color[v] == Color::White) {
                        color[v] = Color::Gray;
                        st.push({v, 0});
                        pushed = true;
                        break; 
                    }
                }
                if (!pushed) {
                    color[u] = Color::Black;
                    result.push_back(u);
                    st.pop();
                }
            }
        }
        return true;
    }
};