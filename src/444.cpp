#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define INT_MIN -2147483648
#define INT_MAX 2147483647

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        int m = nums2.size();

        if (n > m)
        {
            return findMedianSortedArrays(nums2, nums1);
        }

        // Ci 为第i个数组的割，比如C1为2时表示第1个数组只有2个元素。 LMaxi为第i个数组割后的左元素。RMini为第i个元素割后的右元素。
        int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0,hi = 2 * n;  //我们目前时虚拟加'#'所以数组1是2*n长度

        while (lo <= hi)   //二分
        {
            c1 = (lo + hi) / 2; //c1是二分的结果
            c2 = m + n - c1;

            LMax1 = (c1 == 0) ? INT_MIN : nums1[(c1 - 1) / 2];
            RMin1 = (c1 == 2 * n) ? INT_MAX : nums1[c1 / 2];
            LMax2 = (c2 == 0) ? INT_MIN : nums2[(c2 - 1) / 2];
            RMin2 = (c2 == 2 * n) ? INT_MAX : nums2[c2 / 2];
            if (LMax1 > RMin2)
                hi = c1 - 1;
            else if (LMax2 > RMin1)
                lo = c1 + 1;
            else 
                break;        
        }
        return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0;
    }
};
// class Solution {
// public:
//     double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
//         auto p1 = nums1.begin();
//         vector<int> ten;
//         for(;;)
//         {
//             for(int i = 0;i < 100;i++)
//             {   
                
                
//                 if((ten[i-1] != *p1) || (i = 0))
//                 {
//                     ten.push_back(*p1 > nums1[i] ? *p1 : nums1[i] );
//                 }
//                 continue;

                
                
//             }
//             p1++;
//             if(p1 == nums2.end())
//             {   
//                 int n = ten.size();
//                 if(n%2 == 0)
//                 {
//                     return ten[n/2];
//                 }
                
//                 return ten[(n+1)/2];
//             }
//         }
        
        
//     }
// };
int main()
{
    vector<int> p = {1,2,3,4,10};
    vector<int> q = {2,7,8};

    Solution s2;
    std::cout<<s2.findMedianSortedArrays(p, q);
    return 0;
}