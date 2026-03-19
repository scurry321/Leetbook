#include <iostream>
#include <vector>

using namespace std;

class Solution
{
public:
    int maxArea(vector<int> &height)
    {
        int res = 0;
        int i = 0;
        int j = height.size() - 1;
        while (i < j)
        {
            int area = (j - i) * min(height[i], height[j]);
            res = max(res, area);
            if (height[i] < height[j])
            {
                i++;
            }
            else
            {
                j--;
            }
        }
        return res;
    }
};

int main(int argc, char const *argv[])
{
    Solution solution;
    int passCount = 0, totalCount = 0;

    auto check = [&](vector<int> height, int expected)
    {
        totalCount++;
        int result = solution.maxArea(height);
        if (result == expected)
        {
            cout << "[PASS] maxArea -> " << result << endl;
            passCount++;
        }
        else
        {
            cout << "[FAIL] maxArea -> 期望: " << expected << ", 实际: " << result << endl;
        }
    };

    // 示例用例
    check({1, 8, 6, 2, 5, 4, 8, 3, 7}, 49);
    check({1, 1}, 1);
    check({4, 3, 2, 1, 4}, 16);
    check({1, 2, 1}, 2);
    check({2, 3, 4, 5, 18, 17, 6}, 17);

    cout << "\n结果: " << passCount << " / " << totalCount << " 通过" << endl;

    return 0;
}
