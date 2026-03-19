#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
       int n = nums.size();
       int res = nums[0] + nums[1] + nums[n-1];
       sort(nums.begin(),nums.end());
       
       for(int i = 0;i < n;i++)
       {       
            int k = n - 1;
            
            for(int j = i + 1;j < n; j++)
            {
                if(abs(nums[i] + nums[j] + nums[k] - target) >= abs(res - target))
                {
                    return res;
                }
                
                while(j < k)
                {
                    if(nums[i] + nums[j] + nums[k] == target)
                    {
                        return target;
                    }
                    else if(abs(nums[i] + nums[j] + nums[k] - target) < abs(res - target))
                    {
                        res = nums[i] + nums[j] + nums[k];
                    }
                    else if(abs(nums[i] + nums[j] + nums[k] - target) >= abs(nums[i] + nums[j] + nums[k+1] - target))
                    {
                        break;
                    }
                    k--;
                }
            }
       }
       return res;
    }
};

int main() {
    Solution sol;

    

    // 测试用例 4: 含负数，预期输出 -1
    {
        vector<int> nums = {-3, -1, 1, 2};
        int target = 0;
        int result = sol.threeSumClosest(nums, target);
        cout << "测试4: nums = [-3,-1,1,2], target = 0" << endl;
        cout << "  输出: " << result << "  (预期: -1 或 0 附近)" << endl << endl;
    }

    // 测试用例 5: 较大数组
    {
        vector<int> nums = {4, 0, 5, -5, 3, 3, 0, -4, -5};
        int target = -2;
        int result = sol.threeSumClosest(nums, target);
        cout << "测试5: nums = [4,0,5,-5,3,3,0,-4,-5], target = -2" << endl;
        cout << "  输出: " << result << "  (预期: -2)" << endl;
        cout << "  " << (result == -2 ? "✅ 通过" : "❌ 失败") << endl << endl;
    }

    return 0;
}
//解法一
// class Solution {
// public:
//     int threeSumClosest(vector<int>& nums, int target) {
//         int i{0}, j{1}, k = nums.size() - 1;
//         int result;
//         int min_delta = INT_MAX;
//         sort(nums.begin(), nums.end()); // 不排序玩个屁的双指针 😂
//         for (; i < nums.size() - 2; ++i) {
//             k = nums.size() - 1;
//             j = i + 1;
//             while (j < k) {
//                 int ijk =nums[i] + nums[j] + nums[k];
//                 int delta = ijk < target ? target - ijk : ijk - target;
//                 if (delta < min_delta) {
//                     min_delta = delta;
//                     result =  ijk;
//                 }
//                 if (ijk < target) {
//                     ++j;
//                 } else if (ijk > target) {
//                     --k;
//                 } else {
//                     return target;
//                 }
//             }
//         }
//         return result;
//     }
// };
// 解法二
// class Solution {
// public:
//     int threeSumClosest(vector<int>& nums, int target) {
//         int n = nums.size();
//         int res = nums[0] + nums[1] + nums[2];
//         sort(nums.begin(), nums.end());
//         for (int i = 0; i < n - 2; ++i) {
//             int j = i + 1, k = n - 1;
//             while (j < k) {
//                 int sum = nums[i] + nums[j] + nums[k];
//                 if (abs(sum - target) < abs(res - target)) {
//                     res = sum;
//                 }
//                 if (sum == target) {
//                     return target;
//                 } else if (sum < target) {
//                     ++j;
//                 } else {
//                     --k;
//                 }
//             }
//         }
//         return res;
//     }
// };