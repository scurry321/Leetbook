#include <iostream>

using namespace std;

class Solution {
public:
    int myAtoi(string s) {
        int n = s.size();

        // 跳过前导空格
        int i = 0;
        while (i < n && s[i] == ' ') {
            i++;
        }

        // 处理正负号
        int sign = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            sign = s[i] == '+' ? 1 : -1;
            i++;
        }

        // 处理数字
        int num = 0;
        for (; i < n && '0' <= s[i] && s[i] <= '9'; i++) {
            int d = s[i] - '0';
            // 如果 num*10+d > INT_MAX，则最终答案已确定，提前返回
            if (num > INT_MAX / 10 || num * 10 > INT_MAX - d) { // 避免溢出
                return sign > 0 ? INT_MAX : INT_MIN;
            }
            num = num * 10 + d;
        }

        return sign * num;
    }
};


int main(int argc, char const* argv[])
{
    Solution solution;
    int passCount = 0, totalCount = 0;

    auto check = [&](const string& s, int expected) {
        int result = solution.myAtoi(s);
        totalCount++;
        if (result == expected) {
            cout << "[PASS] myAtoi(\"" << s << "\") = " << result << endl;
            passCount++;
        } else {
            cout << "[FAIL] myAtoi(\"" << s << "\") = " << result
                 << ", expected " << expected << endl;
        }
    };

    // 示例 1
    check("42", 42);
    // 示例 2
    check(" -042", -42);
    // 示例 3
    check("1337c0d3", 1337);
    // 示例 4
    check("0-1", 0);
    // 示例 5
    check("words and 987", 0);
    // 溢出：超过 INT_MAX
    check("99999999999999999999", 2147483647);
    // 溢出：低于 INT_MIN
    check("-99999999999999999999", -2147483648);

    cout << "\n" << passCount << "/" << totalCount << " passed." << endl;

    return 0;
}
