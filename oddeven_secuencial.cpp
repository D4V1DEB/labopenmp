#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <functional>
using namespace std;

void odd_even_sort(int* a, int n) {
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            for (int i = 1; i < n; i += 2) {
                if (a[i - 1] > a[i]) {
                    int tmp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = tmp;
                }
            }
        } else {
            for (int i = 1; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    int tmp = a[i + 1];
                    a[i + 1] = a[i];
                    a[i] = tmp;
                }
            }
        }
    }
}

vector<int> aleatorio(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = rand() % (n * 2);
    return a;
}

vector<int> ordenado(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i;
    return a;
}

vector<int> inverso(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = n - i;
    return a;
}

vector<int> repetidos(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = rand() % 10;
    return a;
}

int main() {
    ofstream csv("tiempos_oddeven_secuencial.csv");
    csv << "tipo_arreglo,tamano,tiempo_secuencial,ordenado\n";

    vector<pair<string, function<vector<int>(int)>>> casos = {{"aleatorio", aleatorio},{"ordenado",  ordenado},{"inverso",   inverso},{"repetidos", repetidos}};

    int tamanos[] = {5000, 10000, 20000, 40000};

    for (const auto& [tipo, generar] : casos) {
        for (int n : tamanos) {
            vector<int> a = generar(n);

            auto t0 = chrono::high_resolution_clock::now();
            odd_even_sort(a.data(), n);
            auto t1 = chrono::high_resolution_clock::now();
            chrono::duration<double> t_seq = t1 - t0;

            bool ordenado = is_sorted(a.begin(), a.end());

            cout << "tipo - " << tipo << endl;
            cout << "tamano - " << n << endl;
            cout << "tiempo secuencial - " << t_seq.count() << endl;
            cout << "ordenado - " << (ordenado ? "si" : "no") << endl;

            csv << tipo << ","<< n << ","<< t_seq.count() << ","<< (ordenado ? "si" : "no") << "\n";
        }
    }

    csv.close();
    return 0;
}
