#include <iostream>
#include <string>
#include <vector>

using namespace std;

const pair<int, string> valueSymbols[] = {
    {1000, "M"},
    {900,  "CM"},
    {500,  "D"},
    {400,  "CD"},
    {100,  "C"},
    {90,   "XC"},
    {50,   "L"},
    {40,   "XL"},
    {10,   "X"},
    {9,    "IX"},
    {5,    "V"},
    {4,    "IV"},
    {1,    "I"},
};

class Solution {
public:
    string intToRoman(int num) {
        string roman;
        for (const auto &[value, symbol] : valueSymbols) {
            while (num >= value) {
                num -= value;
                roman += symbol;
            }
            if (num == 0) {
                break;
            }
        }
        return roman;
    }
};

int main()
{
    Solution solution;
    int passCount = 0, totalCount = 0;

    auto check = [&](int num, const string& expected)
    {
        totalCount++;
        string result = solution.intToRoman(num);
        if (result == expected)
        {
            passCount++;
            cout << "[通过] intToRoman(" << num << ") = \"" << result << "\"" << endl;
        }
        else
        {
            cout << "[失败] intToRoman(" << num << ") 期望 \"" << expected << "\", 实际 \"" << result << "\"" << endl;
        }
    };

    check(3749, "MMMDCCXLIX");
    check(58, "LVIII");
    check(1994, "MCMXCIV");
    check(1, "I");
    check(4, "IV");
    check(9, "IX");
    check(40, "XL");
    check(90, "XC");
    check(400, "CD");
    check(900, "CM");
    check(3999, "MMMCMXCIX");
    check(1000, "M");      // 新增测试
    check(100, "C");       // 新增测试
    check(10, "X");        // 新增测试

    cout << "\n结果: " << passCount << " / " << totalCount << " 通过" << endl;

    return 0;
}