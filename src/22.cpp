#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Solution {
public:
    vector<string> ans;
    vector<string> generateParenthesis(int n) {
        dfs(0, 0, n, "");
        return ans;
    }
    void dfs(int lc, int rc, int n, string p)
    {
        if(lc == n && rc == n)
        {
            ans.push_back(p);
            return;
        }
        if(lc < n) 
        {
            dfs(lc + 1, rc, n, p + "(");
        }
        if(rc < n && lc > rc)
        {
            dfs(lc, rc + 1, n, p + ")");
        }

    }
};
int main()
{
    Solution s;
    s.generateParenthesis(3);
    return 0;
}