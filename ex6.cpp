#include <iostream>
#include <ctime>
#include <cstdlib> // Pentru a utiliza functia rand()
#include <omp.h>

using namespace std;

// Functie pentru initializarea unei matrice cu valori aleatoare
void initializeMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
}

// Functie pentru alocarea dinamica a unei matrice
int** allocateMatrix(int rows, int cols) {
    int **matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

// Functie pentru eliberarea memoriei ocupate de o matrice
void deallocateMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Functie pentru inmultirea a doua matrici in mod secvential
void matrixMultiplicationSequential(int **matrixA, int **matrixB, int **result, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

// Functie pentru inmultirea a doua matrici in mod paralel
void matrixMultiplicationParallel(int **matrixA, int **matrixB, int **result, int rowsA, int colsA, int colsB) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));

    // Dimensiunile matricilor
    const int rowsA = 1000;
    const int colsA = 1000;
    const int colsB = 1000;

    // Alocarea si initializarea matricilor
    int **matrixA = allocateMatrix(rowsA, colsA);
    int **matrixB = allocateMatrix(colsA, colsB);
    int **resultSequential = allocateMatrix(rowsA, colsB);
    int **resultParallel = allocateMatrix(rowsA, colsB);

    initializeMatrix(matrixA, rowsA, colsA);
    initializeMatrix(matrixB, colsA, colsB);

    // Inmultirea matricilor in mod secvential si masurarea timpului de executie
    double start_time_seq = omp_get_wtime();
    matrixMultiplicationSequential(matrixA, matrixB, resultSequential, rowsA, colsA, colsB);
    double end_time_seq = omp_get_wtime();

    // Inmultirea matricilor in mod paralel si masurarea timpului de executie
    double start_time_par = omp_get_wtime();
    matrixMultiplicationParallel(matrixA, matrixB, resultParallel, rowsA, colsA, colsB);
    double end_time_par = omp_get_wtime();

    // Afisarea timpului de executie pentru cele doua moduri de inmultire
    cout << "Timpul de executie al inmultirii matricilor in mod secvential: " << (end_time_seq - start_time_seq) * 1000 << " ms" << endl;
    cout << "Timpul de executie al inmultirii matricilor in mod paralel: " << (end_time_par - start_time_par) * 1000 << " ms" << endl;

    // Eliberarea memoriei
    deallocateMatrix(matrixA, rowsA);
    deallocateMatrix(matrixB, colsA);
    deallocateMatrix(resultSequential, rowsA);
    deallocateMatrix(resultParallel, rowsA);

    return 0;
}
