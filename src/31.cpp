#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        int thes = -1; // 初始设为-1，表示还没找到

        // 1. 从右往左找第一个“升序对”
        for (int i = n - 1; i > 0; i--) {
            if (nums[i - 1] < nums[i]) {
                thes = i - 1;
                break;
            }
        }

        if (thes >= 0) { // 如果找到了
            // 2. 从右往左找第一个比 nums[thes] 大的数
            int temp = n - 1;
            while (nums[temp] <= nums[thes]) { // 注意是 <=
                temp--;
            }
            // 3. 交换
            swap(nums[thes], nums[temp]); // C++自带 swap，比手动 temp2 简洁
            
            // 4. 将 thes 之后的序列变为升序（你用的 sort 没问题！）
            sort(nums.begin() + thes + 1, nums.end());
        } else {
            // 如果已经是最大排列，直接翻转成最小
            sort(nums.begin(), nums.end());
        }
    }
};

// class Solution
// {
// public:
//     void nextPermutation(vector<int>& nums)
//     {
//         int n = nums.size();
//         int res = 0;
//         int thes = 0;
//         for (int i = n - 1;i > 0;i--)
//         {
            

//             if (nums[i-1] < nums[i])
//             {
//                 thes = i - 1;
//                 res++;
//                 break;
//             }            
//         }
//         int temp = n-1;
//         while(nums[temp] < nums[thes])
//         {
//             temp--;
//         }
//         int temp2;
//         temp2 = nums[thes];
//         nums[thes] = nums[temp];
//         nums[temp] = temp2;
//         sort(nums.begin() + thes + 1, nums.end()); 

//         if(!res)
//         {
//             sort(nums.begin(), nums.end());
    
//         }


//     }
// };
