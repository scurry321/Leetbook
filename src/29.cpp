#include <iostream>
using namespace std;

#include <climits>

class Solution {
public:
    int divide(int dividend, int divisor) {
        // 1. 唯一会导致正向溢出的特殊情况
        if (dividend == INT_MIN && divisor == -1) {
            return INT_MAX;
        }

        // 2. 确定最终符号
        bool isNegative = (dividend > 0) ^ (divisor > 0);

        // 3. 全部转为负数处理
        if (dividend > 0) dividend = -dividend;
        if (divisor > 0) divisor = -divisor;

        int res = 0; // 用负数累加商，防止 INT_MIN 无法转正的问题

        // 4. 倍增法核心逻辑
        while (dividend <= divisor) {
            int temp = divisor;
            int multiple = -1; // 倍数也用负数表示

            // 防止 temp << 1 溢出：INT_MIN / 2 = -1073741824
            // 只要 temp >= -1073741824，temp << 1 就不会溢出
            while (temp >= -1073741824 && dividend <= (temp << 1)) {
                temp <<= 1;
                multiple <<= 1;
            }

            dividend -= temp;
            res += multiple; // res 会变成类似 -33 这样的负数
        }

        // 5. 根据符号返回
        // 如果原结果是负数，直接返回 res (例如 -33)
        // 如果原结果是正数，返回 -res (例如 -(-33) = 33)
        return isNegative ? res : -res;
    }
};