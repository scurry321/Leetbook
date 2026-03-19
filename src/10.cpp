#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        int n = s.length(), m = p.length();
        s = ' ' + s, p = ' ' + p;
        vector<vector<bool>> f(n+1, vector<bool>(m+1));
        f[0][0] = true;

        for (int i = 0; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (j + 1 <= m && p[j + 1] == '*') continue;
                if (p[j] != '*') {
                    f[i][j] = (i > 0) && f[i - 1][j - 1] && (s[i] == p[j] || p[j] == '.');
                } else {
                    f[i][j] = (j >= 2 && f[i][j - 2]) ||
                              (i > 0 && j >= 2 && f[i - 1][j] && (s[i] == p[j - 1] || p[j - 1] == '.'));
                }
            }
        }
        return f[n][m];
    }
};

int main(int argc, char const* argv[])
{
    Solution solution;
    int passCount = 0, totalCount = 0;

    auto check = [&](const string& s, const string& p, bool expected)
    {
        totalCount++;
        bool result = solution.isMatch(s, p);
        bool passed = (result == expected);
        if (passed)
            passCount++;
        cout << (passed ? "[PASS]" : "[FAIL]") << " s=\"" << s << "\", p=\"" << p << "\""
             << " => " << (result ? "true" : "false") << " (期望: " << (expected ? "true" : "false") << ")" << endl;
    };

    // 示例用例
    check("aa", "a", false);
    check("aa", "a*", true);
    check("ab", ".*", true);
    check("aab", "c*a*b", true);
    check("mississippi", "mis*is*p*.", false);
    check("", ".*", true);
    check("a", ".", true);
    check("ab", ".*c", false);

    cout << "\n结果: " << passCount << " / " << totalCount << " 通过" << endl;

    return 0;
}
