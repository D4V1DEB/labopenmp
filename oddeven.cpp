#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <functional>
#include <omp.h>
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

void odd_even_sort_parallel(int* a, int n, int thread_count) {
    #pragma omp parallel num_threads(thread_count) default(none) shared(a, n)
    {
        for (int phase = 0; phase < n; phase++) {
            if (phase % 2 == 0) {
                #pragma omp for schedule(static)
                for (int i = 1; i < n; i += 2) {
                    if (a[i - 1] > a[i]) {
                        int tmp = a[i - 1];
                        a[i - 1] = a[i];
                        a[i] = tmp;
                    }
                }
            } else {
                #pragma omp for schedule(static)
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
    ofstream csv("tiempos_oddeven.csv");
    csv << "tipo_arreglo,tamano,hilos,tiempo_secuencial,tiempo_paralelo,speedup,eficiencia,ordenado,coincide_secuencial\n";

    vector<pair<string, function<vector<int>(int)>>> casos = {{"aleatorio", aleatorio},{"ordenado",  ordenado},{"inverso",   inverso},{"repetidos", repetidos}};

    int tamanos[] = {5000, 10000, 20000, 40000};
    int hilos[] = {1, 2, 4, 8, 16, 32};

    for (const auto& [tipo, generar] : casos) {
        for (int n : tamanos) {
            vector<int> base = generar(n);

            vector<int> a_seq = base;
            auto t0 = chrono::high_resolution_clock::now();
            odd_even_sort(a_seq.data(), n);
            auto t1 = chrono::high_resolution_clock::now();
            chrono::duration<double> dur_seq = t1 - t0;
            double t_seq = dur_seq.count();

            for (int t : hilos) {
                vector<int> a_par = base;

                auto t2 = chrono::high_resolution_clock::now();
                odd_even_sort_parallel(a_par.data(), n, t);
                auto t3 = chrono::high_resolution_clock::now();
                chrono::duration<double> dur_par = t3 - t2;
                double t_par = dur_par.count();

                double speedup = t_seq / t_par;
                double eficiencia = speedup / t;

                bool ordenado = is_sorted(a_par.begin(), a_par.end());
                bool coincide = (a_par == a_seq);

                cout << "tipo - " << tipo << endl;
                cout << "tamano - " << n << endl;
                cout << "hilos - " << t << endl;
                cout << "tiempo secuencial - " << t_seq << endl;
                cout << "tiempo paralelo - " << t_par << endl;
                cout << "speedup - " << speedup << endl;
                cout << "eficiencia - " << eficiencia << endl;
                cout << "ordenado - " << (ordenado ? "si" : "no") << endl;
                cout << "coincide secuencial - " << (coincide ? "si" : "no") << endl;

                csv << tipo << "," << n << "," << t << ","<< t_seq << ","<< t_par << ","<< speedup << ","<< eficiencia << ","<< (ordenado ? "si" : "no") << ","<< (coincide ? "si" : "no") << "\n";
            }
        }
    }

    csv.close();
    return 0;
}
