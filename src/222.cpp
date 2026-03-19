#include <iostream>
#include <string>
#include <unordered_set>
#include <cmath>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace std::chrono;


/**
 * LeetCode 3. 无重复字符的最长子串
 * 给定一个字符串 s ，请你找出其中不含有重复字符的最长子串的长度。
 * 
 * 算法思路：暴力法 + 滑动窗口
 * 时间复杂度：O(n^2) - 双层循环
 * 空间复杂度：O(1) - 只使用了几个变量
 */
class Solution {
public:

    int lengthOfLongestSubstring(std::string s) {
        bool b;         // 标记是否找到重复字符
        int m = 0;      // 当前窗口的起始位置（左边界）
        int max = 0;    // 记录最长无重复子串的长度
        int j = 0;      // 内层循环索引，用于查找重复字符
        
        // 外层循环：遍历字符串的每个字符作为窗口的右边界
        for(int i = 0; i < s.size(); i++)
        {
            j = m;  // 从当前窗口的起始位置开始检查
            
            // 内层循环：检查 s[i] 是否在当前窗口 [m, i) 中出现过
            for(; j < i; j++)
            {
                b = s[i] == s[j] ? 1 : 0;  // 比较是否重复
                
                if(b == 1)  // 发现重复字符
                {
                    // 在移动窗口前，先更新最大长度
                    if(max < (i - m))
                    {
                        max = i - m;
                    }
                    
                    // 将窗口起始位置移动到重复字符的下一个位置
                    // 例如："abcab"，当i=3(第二个'a')，j=0(第一个'a')
                    // 则窗口应从索引1('b')开始，即 m = j + 1 = 1
                    m = j + 1;  
                    b = 0;
                    
                    break;  // 找到重复后跳出内层循环
                }
            }
        }
        
        // 循环结束后，检查最后一个窗口 [m, s.size()) 的长度
        // 因为如果后面都没有重复字符，这段可能是最长的
        if(max < (int)s.size() - m)
        {
            max = s.size() - m;
        }
        
        return max;
    }
};
// class Solution {
// public:
//     int lengthOfLongestSubstring(string s) {
//         unordered_map<char,int>Hashmap;
//         int res=0;
//         int n=s.size();
//         for(int i=0,j=0;i<n;i++){
//             Hashmap[s[i]]++;
//             while(Hashmap[s[i]] > 1){
//                 Hashmap[s[j]] --;
//                 j++;
//             }
//             res=max(res,i-j+1);
//         }
//         return res;
//     }
// };



int main(int argc, char const *argv[])
{
    Solution solution;
    
    // 测试用例1
    string s1 = "abcasadafweafasgadrhgmsfdbcbdsfasfb";
    cout << "输入: s = \"" << s1 << "\"" << endl;
    
    // 计时器开始
    auto start = high_resolution_clock::now();
    for (int i=0; i < 10000;i++)
    {
        solution.lengthOfLongestSubstring(s1);
    }
    auto end = high_resolution_clock::now();
    // 计算耗时
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "输出: " << solution.lengthOfLongestSubstring(s1) << endl;
    cout << "预期: 3" << endl;
    cout << "执行10000次耗时: " << duration.count() << " 微秒 (" 
         << duration.count() / 1000.0 << " 毫秒)" << endl << endl;
    
    // 测试用例2
    string s2 = "bbbbb";
    cout << "输入: s = \"" << s2 << "\"" << endl;
    
    start = high_resolution_clock::now();
    int result2 = solution.lengthOfLongestSubstring(s2);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    
    cout << "输出: " << result2 << endl;
    cout << "预期: 1" << endl;
    cout << "执行耗时: " << duration.count() << " 微秒" << endl << endl;
    
    // 测试用例3
    string s3 = "pwwkew";
    cout << "输入: s = \"" << s3 << "\"" << endl;
    
    start = high_resolution_clock::now();
    int result3 = solution.lengthOfLongestSubstring(s3);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    
    cout << "输出: " << result3 << endl;
    cout << "预期: 3" << endl;
    cout << "执行耗时: " << duration.count() << " 微秒" << endl << endl;
    
    // 测试用例4
    string s4 = "";
    cout << "输入: s = \"" << s4 << "\"" << endl;
    
    start = high_resolution_clock::now();
    int result4 = solution.lengthOfLongestSubstring(s4);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    
    cout << "输出: " << result4 << endl;
    cout << "预期: 0" << endl;
    cout << "执行耗时: " << duration.count() << " 微秒" << endl << endl;
    
    return 0;
}