#include <iostream>
#include <climits>

using namespace std;

class Solution
{
public:
    bool isPalindrome(int x) {
        if(x < 0)
        {
            return false;
        }
        int count = 0;
        int pre;
        int p = x;
        while(x)
        {
            pre = x % 10;
            x = x / 10;
            if(count > INT_MAX / 10) 
            {
                return false;
            }
            count = (10 * count) + pre;
        }
        if(count == p)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

int main(int argc, char const* argv[])
{
    Solution solution;
    int passCount = 0, totalCount = 0;

    auto check = [&](int x, bool expected)
    {
        bool result = solution.isPalindrome(x);
        totalCount++;
        if (result == expected)
        {
            passCount++;
            cout << "[PASS] isPalindrome(" << x << ") = " << boolalpha << result << endl;
        }
        else
        {
            cout << "[FAIL] isPalindrome(" << x << ") expected " << boolalpha << expected << " but got " << result
                 << endl;
        }
    };

    // 示例用例
    check(121, true);
    check(-121, false);
    check(10, false);
    check(0, true);
    check(11, true);
    check(1221, true);
    check(12321, true);
    check(2346335, false);

    cout << "\n结果: " << passCount << " / " << totalCount << " 通过" << endl;

    return 0;
}
