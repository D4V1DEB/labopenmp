#include <iostream>
#include <chrono>
#include <fstream>
#include <omp.h>
using namespace std;

struct Node {
    int val;
    Node* next;
    Node(int v, Node* n = nullptr) : val(v), next(n) {}
};

Node* head = nullptr;
omp_lock_t global_lock;

void init_list() {
    head = nullptr;
    omp_init_lock(&global_lock);
}

void clear_list() {
    Node* curr = head;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
}

void insert_node(int val) {
    Node* new_node = new Node(val);

    omp_set_lock(&global_lock);
    new_node->next = head;
    head = new_node;
    omp_unset_lock(&global_lock);
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

void insert_range(int a, int b) {
    #pragma omp parallel for
    for (int v = a; v < b; v++) {
        insert_node(v);
    }
}

int search_subset(int a, int b, int i) {
    int hits = 0;
    #pragma omp parallel for reduction(+:hits)
    for (int v = a; v < b; v += i) {
        if (search_node(v)) hits++;
    }
    return hits;
}

int count_nodes() {
    int n = 0;
    for (Node* curr = head; curr; curr = curr->next) n++;
    return n;
}

bool has_cycle() {
    Node* slow = head;
    Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

int main() {
    init_list();

    ofstream csv("tiempos_bloqueo_global.csv");
    csv << "operaciones,hilos,tiempo_insercion,tiempo_busqueda,nodos,aciertos,hay_ciclos\n";

    int cantidades[] = {1000, 5000, 10000, 20000, 40000};
    int num_hilos = omp_get_max_threads();

    for (int n : cantidades) {
        clear_list();
        auto t0 = chrono::high_resolution_clock::now();
        insert_range(0, n);
        auto t1 = chrono::high_resolution_clock::now();
        chrono::duration<double> t_ins = t1 - t0;

        int paso = (n >= 1000) ? (n / 1000) : 1;
        auto t2 = chrono::high_resolution_clock::now();
        int hits = search_subset(0, n, paso);
        auto t3 = chrono::high_resolution_clock::now();
        chrono::duration<double> t_search = t3 - t2;

        int nodos = count_nodes();
        bool ciclo = has_cycle();

        cout << "operaciones - " << n << endl;
        cout << "tiempo insercion - " << t_ins.count() << endl;
        cout << "tiempo busqueda - " << t_search.count() << endl;
        cout << "numero de nodos - " << nodos << endl;
        cout << "ciclos detectados - " << (ciclo ? "si" : "no") << endl;
        cout << "aciertos busqueda - " << hits << endl;

        csv << n << ","<< num_hilos << ","<< t_ins.count() << ","<< t_search.count() << ","<< nodos << ","<< hits << ","<< (ciclo ? "si" : "no") << "\n";
    }

    csv.close();
    clear_list();
    omp_destroy_lock(&global_lock);

    return 0;
}
