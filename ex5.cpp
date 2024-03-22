#include <iostream>
#include <ctime>
#include <cstdlib> // Pentru a utiliza functia srand()
#include <omp.h>

using namespace std;

// Functie pentru interclasarea a doua subtablouri sortate
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Functie pentru sortarea unui subtablou folosind MergeSort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Functie pentru generarea unui tablou cu valori aleatoare
void generateRandomArray(int arr[], int size) {
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000;
    }
}

// Functie pentru afisarea unui tablou
void printArray(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    const int N = 1024;
    int arr[N];
    int arr_parallel[N];

    generateRandomArray(arr, N);

    // Copiem tabloul pentru sortare paralela
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        arr_parallel[i] = arr[i];
    }

    cout << "Vectorul original:" << endl;
    printArray(arr, N);

    // Sortare ne-paralelizata
    double start_time_seq = omp_get_wtime();
    mergeSort(arr, 0, N - 1);
    double end_time_seq = omp_get_wtime();

    cout << "Vectorul sortat cu sortare secventiala:" << endl;
    printArray(arr, N);

    cout << "Timpul de executie al sortarii secventiale: " << (end_time_seq - start_time_seq) * 1000 << " ms" << endl;

    // Sortare paralelizata
    double start_time_par = omp_get_wtime();
    mergeSort(arr_parallel, 0, N - 1);
    double end_time_par = omp_get_wtime();

    cout << "Vectorul sortat cu sortare paralela:" << endl;
    printArray(arr_parallel, N);

    cout << "Timpul de executie al sortarii paralele: " << (end_time_par - start_time_par) * 1000 << " ms" << endl;

    return 0;
}
