/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}

};
class Solution {
public:
    void inorder(TreeNode* root, vector<int>& res) {
        if (!root) {
            return;
        }
        inorder(root->left, res);
        res.push_back(root->val);
        inorder(root->right, res);
    }
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        inorder(root, res);
        return res;
    }
};


// 解析字符串构建二叉树
TreeNode *parseTree(const std::string &data)
{
    if (data.empty() || data == "[]")
        return nullptr;

    // 去掉方括号
    std::string str = data.substr(1, data.size() - 2);
    
    // 分割字符串
    std::vector<std::string> vals;
    std::stringstream ss(str);
    string item;
    while (getline(ss, item, ','))
    {
        vals.push_back(item);
    }

    if (vals.empty() || vals[0] == "null")
        return nullptr;

    // 创建根节点
    TreeNode *root = new TreeNode(stoi(vals[0]));
    queue<TreeNode *> q;
    q.push(root);

    // 层序遍历构建树
    int i = 1;
    while (!q.empty() && i < vals.size())
    {
        TreeNode *node = q.front();
        q.pop();

        // 处理左子节点
        if (i < vals.size())
        {
            if (vals[i] != "null")
            {
                node->left = new TreeNode(stoi(vals[i]));
                q.push(node->left);
            }
            i++;
        }

        // 处理右子节点
        if (i < vals.size())
        {
            if (vals[i] != "null")
            {
                node->right = new TreeNode(stoi(vals[i]));
                q.push(node->right);
            }
            i++;
        }
    }

    return root;
}

int main()
{
    std::string input_str = "[1,2,3]";

    // 解析输入字符串构建二叉树
    TreeNode *root = parseTree(input_str);

    // 执行中序遍历
    Solution solution;
    vector<int> result = solution.inorderTraversal(root);

    // 输出结果
    cout << "中序遍历结果: [";
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i];
        if (i < result.size() - 1)
            cout << ",";
    }
    cout << "]" << endl;
    cout<<root<<endl;

    return 0;
}