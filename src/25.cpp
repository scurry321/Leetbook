#include <iostream>
using namespace std;

// Definition for singly-linked list.
struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution
{
public:
    ListNode *reverse(ListNode *a, ListNode *b)
    {
        ListNode *prev = nullptr;
        ListNode *curr = a;
        while (curr != b)
        {
            ListNode *nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }
        return prev;
    }

    ListNode *reverseKGroup(ListNode *head, int k)
    {
        ListNode *a = head;
        ListNode *b = head;
        for (int i = 0; i < k; ++i)
        {
            if (!b)
                return head;
            b = b->next;
        }
        ListNode *newHead = reverse(a, b);
        a->next = reverseKGroup(b, k);
        return newHead;
    }
};
// 测试用例
int main()
{
    // 构建链表 1->2->3->4->5
    ListNode *head = new ListNode(1);
    ListNode *node = head;
    for (int i = 2; i <= 5; ++i)
    {
        node->next = new ListNode(i);
        node = node->next;
    }
    int k = 2;
    Solution solution;
    ListNode *res = solution.reverseKGroup(head, k);
    while (res)
    {
        cout << res->val << " ";
        res = res->next;
    }
    cout << endl;
    return 0;
}