#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

class Node
{
public:
    int val;
    vector<Node *> neighbors;
    Node()
    {
        val = 0;
        neighbors = vector<Node *>();
    }
    Node(int _val)
    {
        val = _val;
        neighbors = vector<Node *>();
    }
    Node(int _val, vector<Node *> _neighbors)
    {
        val = _val;
        neighbors = _neighbors;
    }
};

class Solution
{
public:
    Node *cloneGraph(Node *node)
    {
        if (node == nullptr)
        {
            return node;
        }

        unordered_map<Node *, Node *> visited;

        // 将题目给定的节点添加到队列
        queue<Node *> Q;
        Q.push(node);
        // 克隆第一个节点并存储到哈希表中
        visited[node] = new Node(node->val);

        // 广度优先搜索
        while (!Q.empty())
        {
            // 取出队列的头节点
            auto n = Q.front();
            Q.pop();
            // 遍历该节点的邻居
            for (auto &neighbor : n->neighbors)
            {
                if (visited.find(neighbor) == visited.end())
                {
                    // 如果没有被访问过，就克隆并存储在哈希表中
                    visited[neighbor] = new Node(neighbor->val);
                    // 将邻居节点加入队列中
                    Q.push(neighbor);
                }
                // 更新当前节点的邻居列表
                visited[n]->neighbors.emplace_back(visited[neighbor]);
            }
        }

        return visited[node];
    }
};

// 根据邻接表构建图
Node *buildGraph(vector<vector<int>> &adjList)
{
    if (adjList.empty())
        return nullptr;

    int n = adjList.size();
    vector<Node *> nodes(n + 1, nullptr);

    // 创建所有节点
    for (int i = 1; i <= n; i++)
    {
        nodes[i] = new Node(i);
    }

    // 添加邻居关系
    for (int i = 0; i < n; i++)
    {
        for (int neighbor : adjList[i])
        {
            nodes[i + 1]->neighbors.push_back(nodes[neighbor]);
        }
    }

    return nodes[1];
}

// 打印图的邻接表
void printGraph(Node *node)
{
    if (!node)
    {
        cout << "[]" << endl;
        return;
    }

    // BFS 遍历图并打印
    vector<Node *> visited;
    vector<Node *> queue;
    queue.push_back(node);
    visited.push_back(node);

    cout << "[";
    bool first = true;

    while (!queue.empty())
    {
        Node *curr = queue.front();
        queue.erase(queue.begin());

        if (!first)
            cout << ",";
        first = false;

        cout << "[";
        for (int i = 0; i < curr->neighbors.size(); i++)
        {
            if (i > 0)
                cout << ",";
            cout << curr->neighbors[i]->val;

            // 检查邻居是否已访问
            bool found = false;
            for (Node *v : visited)
            {
                if (v->val == curr->neighbors[i]->val)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                visited.push_back(curr->neighbors[i]);
                queue.push_back(curr->neighbors[i]);
            }
        }
        cout << "]";
    }
    cout << "]" << endl;
}

int main()
{
    // 输入格式：[[2,4],[1,3],[2,4],[1,3]]
    cout << "请输入邻接表(格式：[[2,4],[1,3],[2,4],[1,3]]):" << endl;

    // 简单的输入示例，你也可以手动构建
    vector<vector<int>> adjList = {{2, 4}, {1, 3}, {2, 4}, {1, 3}};

    // 构建图
    Node *originalGraph = buildGraph(adjList);

    // 调用克隆函数
    Solution solution;
    Node *clonedGraph = solution.cloneGraph(originalGraph);

    // 打印原图
    cout << "原图：";
    printGraph(originalGraph);

    // 打印克隆图
    cout << "克隆图：";
    printGraph(clonedGraph);
    delete clonedGraph;
    return 0;
}