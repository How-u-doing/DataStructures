#include <iostream>
#include <cstring>
#include <cstdint>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

//      1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
//      ^         ^                   ^
//      |         |                   |
//     head      left                right
//
//           +------------------------+
//           |                        |
//           |                        v
//      1 -> 2    3 <- 4 <- 5 <- 6 <- 7    8
//                |                        ^
//                |                        |
//                +------------------------+
ListNode *reverse_sublist(ListNode *head, int m, int n) {
    if (n == -1) {
        n = INT32_MAX;
    }
    if (m <= 0) {
        m = 1;
    }
    if (head == nullptr || n <= m) {
        return head;
    }
    ListNode header{0};
    header.next = head;
    ListNode *left_prev = &header;

    for (int i = 1; i < m && left_prev; i++) {
        left_prev = left_prev->next;
    }

    // reverse the sublist from m to n
    ListNode *cur = left_prev ? left_prev->next : nullptr;
    ListNode *next = cur ? cur->next : nullptr;
    ListNode *after_next = next ? next->next : nullptr;
    for (int i = m; i < n && next; i++) {
        next->next = cur;  // reverse the link

        // advance (cur, next, after_next)
        cur = next;
        next = after_next;
        if (after_next) {
            after_next = after_next->next;
        }

        if (i == n - 1 || !next) {
            left_prev->next->next = next;
            left_prev->next = cur;
        }
    }

    return header.next;
}

ListNode *create_list(int n) {
    ListNode *head = new ListNode(1);
    ListNode *cur = head;
    for (int i = 2; i <= n; i++) {
        cur->next = new ListNode(i);
        cur = cur->next;
    }
    return head;
}

void destroy_list(ListNode *head) {
    for (ListNode *cur = head; cur;) {
        head = cur->next;
        delete cur;
        cur = head;
    }
}

using std::cout;

void print_list(const char *msg, ListNode *head) {
    cout << msg;
    while (head) {
        cout << head->val << ' ';
        head = head->next;
    }
    cout << '\n';
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " nodes m n\n";
        return EXIT_FAILURE;
    }
    const int nodes = std::stoi(argv[1]);
    const int m = std::stoi(argv[2]);
    const int n = std::stoi(argv[3]);
    ListNode *head = create_list(nodes);

    print_list("original list: ", head);

    // reverse the sublist from position m to n (1-based indexing)
    head = reverse_sublist(head, m, n);

    print_list("reversed list: ", head);

    destroy_list(head);
    return EXIT_SUCCESS;
}
