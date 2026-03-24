#include <iostream>
#include <stack>
using namespace std;

class Solution
{
public:
    int longestValidParentheses(string s)
    {
        stack<int> st1;
        st1.emplace(-1);
        int max_len = 0;

        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '(')
            {
                st1.emplace(i);
            }
            else
            {
                st1.pop();
                if (st1.empty())
                {

                    st1.emplace(i);
                }
                else
                {
                    int current_len = i - st1.top();
                    max_len = max_len > current_len ? max_len : current_len;
                }
            }
        }
        return max_len;
    }
};
int main() {
    Solution sol;
    
    // 测试用例 1
    string test1 = "(()";
    cout << "\n测试用例 1: " << test1 << endl;
    int res1 = sol.longestValidParentheses(test1);
    cout << "结果: " << res1 << " (预期: 2)" << endl;

    // 测试用例 2
    string test2 = ")()())";
    cout << "\n测试用例 2: " << test2 << endl;
    int res2 = sol.longestValidParentheses(test2);
    cout << "结果: " << res2 << " (预期: 4)" << endl;

    // 测试用例 3 (空字符串)
    string test3 = "";
    cout << "\n测试用例 3: \"\"" << endl;
    int res3 = sol.longestValidParentheses(test3);
    cout << "结果: " << res3 << " (预期: 0)" << endl;

    return 0;
}
// class Solution
// {
// public:
//     int longestValidParentheses(string s) {
//         stack<int> st1;
//         st1.emplace(-1);
//         int max_len = 0;
//         int res = 0;
//         for (int i = 0; i < s.length(); i++)
//         {
//             if(s[i] == '(')
//             {
//                 st1.emplace(i);
//             }
//             else{
//                 st1.pop();
//             }
//             if (st1.empty())
//             {
//                 max_len = max_len > res ? max_len : res;
//                 st1.emplace(-1);

//             }
//             res ++;
//         }
//         return max_len;
//     }
// };