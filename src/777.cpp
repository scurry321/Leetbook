#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

// class Solution
// {
// public:
//     int reverse(int x)
//     {
//         double count = 0;
//         vector<int> p;
        
//         while (x)
//         {
//             int n = x % 10;
//             p.emplace_back(n);
//             x = x / 10;
//         }
//         auto p1 = p.begin();
//         for (int i = 1; i <= p.size(); i++)
//         {

//             count += *p1 * pow(10, p.size() - i);
//             p1++;
//         }
//         if ((count > pow(2, 31) - 1) || (count < -pow(2, 31)))
//         {
//             return 0;
//         }
//         return int(count);
//     }
// };
class Solution {
public:
    int reverse(int x) {
        int rev = 0;
        while (x != 0) {
            if (rev < INT_MIN / 10 || rev > INT_MAX / 10) {
                return 0;
            }
            int digit = x % 10;
            x /= 10;
            rev = rev * 10 + digit;
        }
        return rev;
    }
};
int main(int argc, char const *argv[])
{
    Solution solution;

    int testCases[] = {123, -123, 120, 0};
    int expected[] = {321, -321, 21, 0};

    for (int i = 0; i < 4; i++)
    {
        int result = solution.reverse(testCases[i]);
        cout << "输入：" << testCases[i]
             << "  输出：" << result
             << "  预期：" << expected[i]
             << (result == expected[i] ? "  ✓" : "  ✗") << endl;
    }

    return 0;
}
