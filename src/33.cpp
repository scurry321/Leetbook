#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) return mid;

            // 情况 A：左半部分是有序的
            if (nums[left] <= nums[mid]) {
                // 如果 target 就在这有序的左半部分里
                if (target >= nums[left] && target < nums[mid]) {
                    right = mid - 1; // 往左找
                } else {
                    left = mid + 1;  // 往右找
                }
            }
            // 情况 B：右半部分是有序的
            else {
                // 如果 target 就在这有序的右半部分里
                if (target > nums[mid] && target <= nums[right]) {
                    left = mid + 1;  // 往右找
                } else {
                    right = mid - 1; // 往左找
                }
            }
        }

        return -1;
    }
};