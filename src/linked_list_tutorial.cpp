/**
 * 链表（Linked List）知识点详解
 * 
 * 链表是一种基本的数据结构，由一系列节点组成，每个节点包含数据和指向下一个节点的指针
 */

#include <iostream>
#include <vector>
using namespace std;

// ============================================================================
// 1. 单链表（Singly Linked List）
// ============================================================================

/**
 * 单链表节点定义
 * 每个节点包含一个值和指向下一个节点的指针
 */
struct ListNode {
    int val;
    ListNode* next;
    
    // 构造函数
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

/**
 * 单链表操作类
 */
class SingleLinkedList {
public:
    ListNode* head;
    
    SingleLinkedList() : head(nullptr) {}
    
    // 在头部插入节点 - O(1)
    void insertAtHead(int val) {
        ListNode* newNode = new ListNode(val);
        newNode->next = head;
        head = newNode;
    }
    
    // 在尾部插入节点 - O(n)
    void insertAtTail(int val) {
        ListNode* newNode = new ListNode(val);
        if (!head) {
            head = newNode;
            return;
        }
        
        ListNode* curr = head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    
    // 在指定位置插入节点 - O(n)
    void insertAtIndex(int index, int val) {
        if (index == 0) {
            insertAtHead(val);
            return;
        }
        
        ListNode* curr = head;
        for (int i = 0; i < index - 1 && curr; i++) {
            curr = curr->next;
        }
        
        if (!curr) return; // 索引越界
        
        ListNode* newNode = new ListNode(val);
        newNode->next = curr->next;
        curr->next = newNode;
    }
    
    // 删除指定值的节点 - O(n)
    void deleteNode(int val) {
        if (!head) return;
        
        // 删除头节点
        if (head->val == val) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        
        ListNode* curr = head;
        while (curr->next && curr->next->val != val) {
            curr = curr->next;
        }
        
        if (curr->next) {
            ListNode* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
        }
    }
    
    // 查找节点 - O(n)
    ListNode* search(int val) {
        ListNode* curr = head;
        while (curr) {
            if (curr->val == val) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
    
    // 打印链表
    void printList() {
        ListNode* curr = head;
        while (curr) {
            cout << curr->val;
            if (curr->next) cout << " -> ";
            curr = curr->next;
        }
        cout << endl;
    }
    
    // 反转链表 - O(n)
    void reverse() {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        ListNode* next = nullptr;
        
        while (curr) {
            next = curr->next;  // 保存下一个节点
            curr->next = prev;  // 反转指针
            prev = curr;        // 移动prev
            curr = next;        // 移动curr
        }
        head = prev;
    }
    
    // 获取链表长度 - O(n)
    int getLength() {
        int count = 0;
        ListNode* curr = head;
        while (curr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
    
    // 析构函数 - 释放所有节点
    ~SingleLinkedList() {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// ============================================================================
// 2. 双向链表（Doubly Linked List）
// ============================================================================

/**
 * 双向链表节点定义
 * 每个节点包含值、指向前一个节点的指针和指向后一个节点的指针
 */
struct DoublyListNode {
    int val;
    DoublyListNode* prev;
    DoublyListNode* next;
    
    DoublyListNode() : val(0), prev(nullptr), next(nullptr) {}
    DoublyListNode(int x) : val(x), prev(nullptr), next(nullptr) {}
};

/**
 * 双向链表操作类
 */
class DoublyLinkedList {
public:
    DoublyListNode* head;
    DoublyListNode* tail;
    
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    
    // 在头部插入 - O(1)
    void insertAtHead(int val) {
        DoublyListNode* newNode = new DoublyListNode(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    // 在尾部插入 - O(1)
    void insertAtTail(int val) {
        DoublyListNode* newNode = new DoublyListNode(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    // 删除节点 - O(1) 如果已知节点位置
    void deleteNode(DoublyListNode* node) {
        if (!node) return;
        
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        
        delete node;
    }
    
    // 正向打印
    void printForward() {
        DoublyListNode* curr = head;
        while (curr) {
            cout << curr->val;
            if (curr->next) cout << " <-> ";
            curr = curr->next;
        }
        cout << endl;
    }
    
    // 反向打印
    void printBackward() {
        DoublyListNode* curr = tail;
        while (curr) {
            cout << curr->val;
            if (curr->prev) cout << " <-> ";
            curr = curr->prev;
        }
        cout << endl;
    }
};

// ============================================================================
// 3. 循环链表（Circular Linked List）
// ============================================================================

/**
 * 循环链表：最后一个节点的next指针指向头节点
 */
class CircularLinkedList {
public:
    ListNode* head;
    
    CircularLinkedList() : head(nullptr) {}
    
    void insertAtEnd(int val) {
        ListNode* newNode = new ListNode(val);
        if (!head) {
            head = newNode;
            newNode->next = head; // 指向自己
        } else {
            ListNode* curr = head;
            while (curr->next != head) {
                curr = curr->next;
            }
            curr->next = newNode;
            newNode->next = head;
        }
    }
    
    void printList() {
        if (!head) return;
        ListNode* curr = head;
        do {
            cout << curr->val << " -> ";
            curr = curr->next;
        } while (curr != head);
        cout << "(back to head)" << endl;
    }
};

// ============================================================================
// 4. 经典链表算法问题
// ============================================================================

/**
 * 快慢指针技巧
 */
class LinkedListAlgorithms {
public:
    
    // 检测链表中是否有环（Floyd判圈算法）- O(n) 时间，O(1) 空间
    bool hasCycle(ListNode* head) {
        if (!head || !head->next) return false;
        
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast && fast->next) {
            slow = slow->next;          // 慢指针走一步
            fast = fast->next->next;    // 快指针走两步
            
            if (slow == fast) {
                return true;  // 相遇说明有环
            }
        }
        return false;
    }
    
    // 找到链表的中间节点
    ListNode* findMiddle(ListNode* head) {
        if (!head) return nullptr;
        
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        return slow;  // 慢指针指向中间节点
    }
    
    // 找到链表倒数第k个节点
    ListNode* findKthFromEnd(ListNode* head, int k) {
        ListNode* fast = head;
        ListNode* slow = head;
        
        // 快指针先走k步
        for (int i = 0; i < k && fast; i++) {
            fast = fast->next;
        }
        
        if (!fast) return nullptr;  // k大于链表长度
        
        // 两个指针同时移动
        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }
        
        return slow;
    }
    
    // 判断链表是否为回文
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        
        // 1. 找到中点
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 2. 反转后半部分
        ListNode* secondHalf = reverseList(slow->next);
        
        // 3. 比较两部分
        ListNode* p1 = head;
        ListNode* p2 = secondHalf;
        bool result = true;
        while (p2) {
            if (p1->val != p2->val) {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        
        // 4. 恢复链表（可选）
        slow->next = reverseList(secondHalf);
        
        return result;
    }
    
    // 合并两个有序链表
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        
        curr->next = l1 ? l1 : l2;
        return dummy.next;
    }
    
    // 删除链表中的重复元素（已排序）
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return nullptr;
        
        ListNode* curr = head;
        while (curr && curr->next) {
            if (curr->val == curr->next->val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        
        return head;
    }
    
    // 两个链表相交的起始节点
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        if (!headA || !headB) return nullptr;
        
        ListNode* pA = headA;
        ListNode* pB = headB;
        
        // 当两个指针相遇时，就是交点（或都为nullptr）
        while (pA != pB) {
            pA = pA ? pA->next : headB;
            pB = pB ? pB->next : headA;
        }
        
        return pA;
    }
    
private:
    // 辅助函数：反转链表
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        
        while (curr) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        
        return prev;
    }
};

// ============================================================================
// 5. 链表的时间复杂度对比
// ============================================================================

/**
 * 数组 vs 链表时间复杂度对比：
 * 
 * 操作          | 数组    | 链表（单向）
 * --------------|---------|-------------
 * 访问元素      | O(1)    | O(n)
 * 头部插入      | O(n)    | O(1)
 * 尾部插入      | O(1)*   | O(n) 或 O(1) 如果有tail指针
 * 中间插入      | O(n)    | O(1) 如果已知前驱节点
 * 删除元素      | O(n)    | O(1) 如果已知前驱节点
 * 查找元素      | O(n)    | O(n)
 * 
 * * 数组尾部插入在不需要扩容时为O(1)
 */

// ============================================================================
// 6. 链表的优缺点
// ============================================================================

/**
 * 优点：
 * 1. 动态大小：不需要预先分配固定大小的内存
 * 2. 插入删除高效：在已知位置的情况下，插入和删除操作为O(1)
 * 3. 内存利用：不需要连续的内存空间
 * 
 * 缺点：
 * 1. 随机访问慢：不支持O(1)的随机访问
 * 2. 额外空间：每个节点需要额外的空间存储指针
 * 3. 缓存不友好：节点在内存中不连续，缓存命中率低
 */

// ============================================================================
// 主函数 - 演示各种操作
// ============================================================================

int main() {
    cout << "========== 单链表演示 ==========" << endl;
    SingleLinkedList list1;
    list1.insertAtTail(1);
    list1.insertAtTail(2);
    list1.insertAtTail(3);
    list1.insertAtHead(0);
    cout << "链表内容: ";
    list1.printList();
    
    cout << "链表长度: " << list1.getLength() << endl;
    
    list1.reverse();
    cout << "反转后: ";
    list1.printList();
    
    cout << "\n========== 双向链表演示 ==========" << endl;
    DoublyLinkedList list2;
    list2.insertAtTail(10);
    list2.insertAtTail(20);
    list2.insertAtTail(30);
    list2.insertAtHead(5);
    cout << "正向打印: ";
    list2.printForward();
    cout << "反向打印: ";
    list2.printBackward();
    
    cout << "\n========== 循环链表演示 ==========" << endl;
    CircularLinkedList list3;
    list3.insertAtEnd(1);
    list3.insertAtEnd(2);
    list3.insertAtEnd(3);
    cout << "循环链表: ";
    list3.printList();
    
    cout << "\n========== 链表算法演示 ==========" << endl;
    LinkedListAlgorithms algo;
    
    // 创建测试链表
    ListNode* testList = new ListNode(1);
    testList->next = new ListNode(2);
    testList->next->next = new ListNode(3);
    testList->next->next->next = new ListNode(4);
    testList->next->next->next->next = new ListNode(5);
    
    ListNode* middle = algo.findMiddle(testList);
    if (middle) {
        cout << "中间节点的值: " << middle->val << endl;
    }
    
    ListNode* kth = algo.findKthFromEnd(testList, 2);
    if (kth) {
        cout << "倒数第2个节点的值: " << kth->val << endl;
    }
    
    cout << "\n程序执行完毕！" << endl;
    
    return 0;
}

/**
 * 学习建议：
 * 1. 掌握链表的基本操作（插入、删除、查找）
 * 2. 熟练使用双指针技巧（快慢指针、前后指针）
 * 3. 理解递归在链表中的应用
 * 4. 多练习LeetCode上的链表题目
 * 5. 注意边界条件的处理（空链表、单节点链表）
 * 
 * 常见易错点：
 * 1. 忘记更新头指针
 * 2. 空指针访问
 * 3. 内存泄漏（删除节点后未释放内存）
 * 4. 环形链表导致的死循环
 */
