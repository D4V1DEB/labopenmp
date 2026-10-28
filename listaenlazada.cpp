#include <iostream>
#include <chrono>
#include <omp.h>
using namespace std;

struct Node {
    int val;
    Node* next;
    Node(int v, Node* n = nullptr) : val(v), next(n) {}
};

Node* head = nullptr;

void init_list() {
    head = nullptr;
}

void insert_node(int val) {
    head = new Node(val, head);
}

bool search_node(int val) {
    for (Node* curr = head; curr; curr = curr->next)
        if (curr->val == val) return true;
    return false;
}

void print_list() {
    for (Node* curr = head; curr; curr = curr->next)
        cout << curr->val << " -> ";
    cout << endl;
}

// rangos
void insert_range(int a, int b) {
    for (int v = a; v < b; v++) insert_node(v);
}

// conjunto
int search_subset(int a, int b, int i) {
    int hits = 0;
    for (int v = a; v < b; v += i)
        if (search_node(v)) hits++;
    return hits;
}

int count_nodes() {
    int n = 0;
    for (Node* curr = head; curr; curr = curr->next) n++;
    return n;
}

int main() {
    init_list();

    auto t0 = chrono::high_resolution_clock::now();
    insert_range(1, 100);
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> t_ins = t1 - t0;
    cout << "tiempo insercion - " << t_ins.count()<< endl;
 

    return 0;
}