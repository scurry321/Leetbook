#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        vector<char> p;
        int max = 0;
        for(int n = 0; n < strs.size(); n++)
        {
            if(n == 0)
            {
                max = strs[0].size();
            }
            max = max < strs[n].size() ? max : strs[n].size();
        }
        char m;
        int res = 0;
        for(int j = 0;j < max;j++)
        {
            m = strs[0].at(j);
            
            for (int i = 0;; i++)
            {
                if (strs[i].at(j) != m)
                {
                    break;
                }
                else if (i == strs.size() - 1)
                {
                    p.emplace_back(m);
                    res ++;
                    break;    
                }
                
            }
            if(res == j)
            {
                break;
            }
        }
        string sss(p.begin(),p.end());
        return sss;
    }
};