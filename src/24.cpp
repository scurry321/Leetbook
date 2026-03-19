#include <iostream>
#include <vector>

using namespace std;
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int n) : val(n), next(nullptr) {}
    ListNode(int n, ListNode *next) : val(n), next(next) {}
};
class Solution {
public:
    ListNode *swapPairs(ListNode *head)
    {
        ListNode dummy(0);
        ListNode* p = &dummy;
        while (head && head->next) {
            p->next = new ListNode(head->next->val);
            p = p->next;
            p->next = new ListNode(head->val);
            p = p->next;
            head = head->next->next;
        }
        if (head) { // 剩下一个节点
            p->next = new ListNode(head->val);
        }
        return dummy.next;
    }
};
int main()
{

    return 0;
}