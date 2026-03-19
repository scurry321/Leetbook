#include <iostream>
#include <cmath>
struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}

    ~ListNode()
    {
        // 递归删除后续节点，防止内存泄漏
        if (next)
        {
            delete next;
        }
    }
};

int eval(ListNode *l)
{
    int n1 = 0;
    for (int i = 0;; i++)
    {
        n1 = l->val * pow(10,i) + n1;
        
        if (l->next == nullptr)
        {
            break;
        }
        l = l->next;
    }
    return n1;
}

class Solution
{
public:
    // ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    // {
    //     int num = eval(l1) + eval(l2);
    //     ListNode *l3 = new ListNode;
    //     ListNode *p = l3;
    //     l3->val = num % 10;
    //     while (num)
    //     {    
    //         num = num / 10;
    //         l3->next = new ListNode( num % 10);
    //         if(num<10)
    //         {
    //             break;
    //         }
    //         l3 = l3->next;
    //     }

    //     return p;
    // }
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        ListNode* temp_node = new ListNode;
        ListNode* first_node = temp_node;
        int carry = 0;
        int sum =0;
        while(l1 || l2 || carry)
        {
            sum = carry;
            if(l1)
            {
                sum += l1->val;
                l1 = l1->next;
            }

            if(l2)
            {
                sum += l2->val;
                l2 = l2->next;
            }

            ListNode* current_node = new ListNode(sum % 10);
            carry = sum / 10;
            
            temp_node->next = current_node;
            temp_node = temp_node->next;
        }
        return first_node->next;
    }
};

int main()
{
    ListNode *l1 = new ListNode(2, new ListNode(4, new ListNode(3)));
    ListNode *l2 = new ListNode(5, new ListNode(6, new ListNode(4)));
    Solution solution;
    ListNode *result = solution.addTwoNumbers(l1, l2);

    while (result)
    {
        std::cout << result->val << " ";
        result = result->next;
    }
    std::cout << std::endl;
    delete l1;
    delete l2;
    delete result;
    return 0;
}
