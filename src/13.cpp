// #include <iostream>
// #include <vector>
// #include <string>

// using namespace std;

// const pair<int, string> valueSymbols[] = {
//     {1000, "M"},
//     {900, "CM"},
//     {500, "D"},
//     {400, "CD"},
//     {100, "C"},
//     {90, "XC"},
//     {50, "L"},
//     {40, "XL"},
//     {10, "X"},
//     {9, "IX"},
//     {5, "V"},
//     {4, "IV"},
//     {1, "I"},
// };

// class Solution{
// public:
//     int romanToInt(string s) {
//         int num = 0;
//         int i = 0;
//         int j = 0;
        
//         if (s.size() == 1)
//         {
//             return 1;
//         }
//         for (const auto &[value, symbol] : valueSymbols)
//         {
            

//             if (s[i+1] == s.back() || symbol.size() == 1)
//             {
//                 if (s[i] == symbol[j] && s[i+1] != symbol )
//                 {
                    
//                     i++;
//                     j++;
//                     num += value;
//                 }
//             }
//             else 
//             {
//                 if(s[i] == symbol[j] && s[i+1] == symbol[j+1])
//                 {
//                     num += value;
//                     if(s[i+2] == s.back())
//                     {
//                         return num;
//                     }
//                     i += 2;
//                     j += 2;

//                 }
//             }
            
            
//         }
//         return num;
//     }
// };

//失败
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;
class Solution {
public:
    int romanToInt(string s) {
        unordered_map<char,int> hash;
        hash['I']=1;
        hash['V']=5;
        hash['X']=10;
        hash['L']=50;
        hash['C']=100;
        hash['D']=500;
        hash['M']=1000;
        int size = s.size();
        char temp=s[0];
        int sum=0;
        for(int i = 0;i<size;i++){
            if(hash[s[i]] > hash[temp]){
                sum=sum+hash[s[i]]-hash[temp]-hash[temp];
            }
            else{
                sum+=hash[s[i]];
                temp = s[i];
            }
        }
        return sum;
    }
};