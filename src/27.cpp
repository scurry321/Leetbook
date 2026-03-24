#include <iostream>
#include <vector>

using namespace std;

class Solution
{
public:
    int removeElement(vector<int>& nums, int val)
    {
        if (nums.size() == 0)
        {
            return 0;
        }
        int slow = 0;
        for (int fast = 0; fast < nums.size(); ++fast)
        {
            if (nums[fast] != val)
            {
                nums[slow] = nums[fast];
                ++slow;
            }
        }
        return slow;
    }
};

int main()
{
    return 0;
}