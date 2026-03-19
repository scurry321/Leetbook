#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        for(char c:s){
            if(c=='('||c=='{'||c=='[')
            st.push(c);
            else{
                if(st.empty())
                return false;
                char top=st.top();
                st.pop();
                if(c==')'&&top!='(')
                return false;
                if(c=='}'&&top!='{')
                return false;
                if(c==']'&&top!='[')
                return false;
            }
            
        }
        return st.empty();
    }
};
int main()
{
    Solution p;
    bool m = p.isValid("(){}}{");
    cout << m;

    return 0;
}