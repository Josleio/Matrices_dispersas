/*
 * Representacion de Matriz Dispersa - Formato CSC
 * (Compressed Sparse Column / Columna Dispersa Comprimida)
 *
 * Tres arreglos representan todos los elementos no cero:
 *   valores[]     - valores de los elementos no cero
 *   ind_fila[]    - indice de fila de cada elemento no cero
 *   ptr_col[]     - ptr_col[j] = primer indice en valores[] que pertenece a la columna j
 *                   ptr_col[num_cols] = total de elementos no cero
 *
 * Ejemplo para la matriz 3x3:
 *   [8  0  5]
 *   [3  0  0]
 *   [0  7  0]
 *
 *   valores   = [8, 3, 7, 5]
 *   ind_fila  = [0, 1, 2, 0]
 *   ptr_col   = [0, 2, 3, 4]
 *
 * Dentro de cada columna, los elementos estan ordenados por fila.
 *
 * Operaciones:
 *   1. insertarOrdenado     - inserta en orden por (columna, fila)
 *   2. eliminarPorPosicion  - elimina el elemento en (fila, columna)
 *   3. eliminarPorValor     - elimina la primera ocurrencia del valor dado
 *   4. intercambiarPorPosicion - intercambia los valores de dos posiciones
 *   5. sustituirValor       - reemplaza el valor en (fila, columna)
 *   6. sumar                - suma de dos matrices en formato CSC
 *   7. restar               - resta de dos matrices en formato CSC
 */

#include <iostream>
#include <vector>
using namespace std;

class MatrizCSC {
private:
    vector<int> valores;
    vector<int> ind_fila;
    vector<int> ptr_col;
    int num_filas;
    int num_cols;

    /* Retorna el indice en valores[] del elemento (f, c), o -1 si no existe. */
    int buscarIndice(int f, int c) const {
        if (c < 0 || c >= num_cols) return -1;
        for (int k = ptr_col[c]; k < ptr_col[c + 1]; k++) {
            if (ind_fila[k] == f) return k;
        }
        return -1;
    }

    /* Busca la primera ocurrencia del valor v. Retorna el indice o -1. */
    int buscarPorValor(int v) const {
        for (int k = 0; k < (int)valores.size(); k++) {
            if (valores[k] == v) return k;
        }
        return -1;
    }

    /* Devuelve la columna a la que pertenece el indice k en valores[]. */
    int columnaDeIndice(int k) const {
        for (int c = 0; c < num_cols; c++) {
            if (k >= ptr_col[c] && k < ptr_col[c + 1]) return c;
        }
        return -1;
    }

    /* Elimina el elemento en el indice k y actualiza ptr_col. */
    void eliminarIndice(int k) {
        int col = columnaDeIndice(k);
        valores.erase(valores.begin() + k);
        ind_fila.erase(ind_fila.begin() + k);
        for (int c = col + 1; c <= num_cols; c++) ptr_col[c]--;
    }

public:
    /* Constructor: define el tamano de la matriz (filas x cols). */
    MatrizCSC(int filas, int cols) : num_filas(filas), num_cols(cols) {
        ptr_col.assign(cols + 1, 0);
    }

    /* 1. Insertar en orden por (columna, fila). Retorna false si ya existe. */
    bool insertarOrdenado(int v, int f, int c) {
        if (f < 0 || f >= num_filas || c < 0 || c >= num_cols) return false;
        if (buscarIndice(f, c) != -1) return false;

        // Encontrar posicion de insercion dentro de la columna c (orden por fila)
        int pos = ptr_col[c + 1]; // insertar al final de la columna por defecto
        for (int k = ptr_col[c]; k < ptr_col[c + 1]; k++) {
            if (ind_fila[k] > f) { pos = k; break; }
        }

        valores.insert(valores.begin() + pos, v);
        ind_fila.insert(ind_fila.begin() + pos, f);
        for (int j = c + 1; j <= num_cols; j++) ptr_col[j]++;
        return true;
    }

    /* 2. Eliminar el elemento en (f, c). Retorna true si lo elimino. */
    bool eliminarPorPosicion(int f, int c) {
        int k = buscarIndice(f, c);
        if (k == -1) return false;
        eliminarIndice(k);
        return true;
    }

    /* 3. Eliminar la primera ocurrencia del valor v. Retorna true si lo elimino. */
    bool eliminarPorValor(int v) {
        int k = buscarPorValor(v);
        if (k == -1) return false;
        eliminarIndice(k);
        return true;
    }

    /* 4. Intercambiar los valores en (f1,c1) y (f2,c2). Retorna true si existen ambos. */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        int k1 = buscarIndice(f1, c1);
        int k2 = buscarIndice(f2, c2);
        if (k1 == -1 || k2 == -1) return false;
        swap(valores[k1], valores[k2]);
        return true;
    }

    /* 5. Sustituir el valor en (f, c) por nuevo_valor. Retorna true si lo encontro. */
    bool sustituirValor(int f, int c, int nuevo_valor) {
        int k = buscarIndice(f, c);
        if (k == -1) return false;
        valores[k] = nuevo_valor;
        return true;
    }

    /* 6. Suma de esta matriz con otra (A + B). Retorna la matriz resultado. */
    MatrizCSC sumar(const MatrizCSC& B) const {
        MatrizCSC resultado(num_filas, num_cols);
        for (int c = 0; c < num_cols; c++) {
            int ia = ptr_col[c], ib = B.ptr_col[c];
            int ea = ptr_col[c + 1], eb = B.ptr_col[c + 1];
            while (ia < ea || ib < eb) {
                int fila_a = (ia < ea) ? ind_fila[ia] : num_filas;
                int fila_b = (ib < eb) ? B.ind_fila[ib] : num_filas;
                if (fila_a < fila_b) {
                    resultado.insertarOrdenado(valores[ia], fila_a, c);
                    ia++;
                } else if (fila_b < fila_a) {
                    resultado.insertarOrdenado(B.valores[ib], fila_b, c);
                    ib++;
                } else {
                    int suma = valores[ia] + B.valores[ib];
                    if (suma != 0) resultado.insertarOrdenado(suma, fila_a, c);
                    ia++; ib++;
                }
            }
        }
        return resultado;
    }

    /* 7. Resta de esta matriz menos otra (A - B). Retorna la matriz resultado. */
    MatrizCSC restar(const MatrizCSC& B) const {
        MatrizCSC resultado(num_filas, num_cols);
        for (int c = 0; c < num_cols; c++) {
            int ia = ptr_col[c], ib = B.ptr_col[c];
            int ea = ptr_col[c + 1], eb = B.ptr_col[c + 1];
            while (ia < ea || ib < eb) {
                int fila_a = (ia < ea) ? ind_fila[ia] : num_filas;
                int fila_b = (ib < eb) ? B.ind_fila[ib] : num_filas;
                if (fila_a < fila_b) {
                    resultado.insertarOrdenado(valores[ia], fila_a, c);
                    ia++;
                } else if (fila_b < fila_a) {
                    resultado.insertarOrdenado(-B.valores[ib], fila_b, c);
                    ib++;
                } else {
                    int diff = valores[ia] - B.valores[ib];
                    if (diff != 0) resultado.insertarOrdenado(diff, fila_a, c);
                    ia++; ib++;
                }
            }
        }
        return resultado;
    }

    void imprimir() const {
        cout << "valores  : [";
        for (int i = 0; i < (int)valores.size(); i++)
            cout << (i ? ", " : "") << valores[i];
        cout << "]\n";

        cout << "ind_fila : [";
        for (int i = 0; i < (int)ind_fila.size(); i++)
            cout << (i ? ", " : "") << ind_fila[i];
        cout << "]\n";

        cout << "ptr_col  : [";
        for (int i = 0; i <= num_cols; i++)
            cout << (i ? ", " : "") << ptr_col[i];
        cout << "]\n";
    }

    /* Imprime la matriz en formato de tabla. */
    void imprimirMatriz() const {
        for (int f = 0; f < num_filas; f++) {
            for (int c = 0; c < num_cols; c++) {
                int k = buscarIndice(f, c);
                cout << (c ? "  " : "") << (k != -1 ? valores[k] : 0);
            }
            cout << "\n";
        }
    }
};

int main() {
    /*
     * Matriz A (3x3):
     *   [8  0  5]
     *   [3  0  0]
     *   [0  7  0]
     */
    MatrizCSC A(3, 3);
    A.insertarOrdenado(8, 0, 0);
    A.insertarOrdenado(3, 1, 0);
    A.insertarOrdenado(7, 2, 1);
    A.insertarOrdenado(5, 0, 2);

    cout << "=== Matriz A ===\n";
    A.imprimir();
    cout << "Tabla:\n";
    A.imprimirMatriz();

    // Eliminar por posicion
    A.eliminarPorPosicion(0, 2);
    cout << "\nDespues de eliminar posicion (0,2):\n";
    A.imprimir();

    // Eliminar por valor
    A.eliminarPorValor(3);
    cout << "\nDespues de eliminar valor 3:\n";
    A.imprimir();

    // Reinsertar para continuar demostracion
    A.insertarOrdenado(5, 0, 2);
    A.insertarOrdenado(3, 1, 0);

    // Intercambiar
    A.intercambiarPorPosicion(0, 0, 2, 1);
    cout << "\nDespues de intercambiar (0,0) y (2,1):\n";
    A.imprimir();

    // Sustituir valor
    A.sustituirValor(0, 0, 99);
    cout << "\nDespues de sustituir (0,0) con 99:\n";
    A.imprimir();

    /*
     * Demostracion de suma y resta
     *
     * Matriz B (3x3):
     *   [1  0  2]
     *   [0  4  0]
     *   [0  3  6]
     */
    cout << "\n=== Suma y Resta de matrices CSC ===\n";
    MatrizCSC MA(3, 3), MB(3, 3);
    MA.insertarOrdenado(8, 0, 0);
    MA.insertarOrdenado(5, 0, 2);
    MA.insertarOrdenado(3, 1, 0);
    MA.insertarOrdenado(7, 2, 1);

    MB.insertarOrdenado(1, 0, 0);
    MB.insertarOrdenado(2, 0, 2);
    MB.insertarOrdenado(4, 1, 1);
    MB.insertarOrdenado(3, 2, 1);
    MB.insertarOrdenado(6, 2, 2);

    cout << "Matriz A:\n";
    MA.imprimirMatriz();
    cout << "Matriz B:\n";
    MB.imprimirMatriz();

    MatrizCSC suma = MA.sumar(MB);
    cout << "A + B:\n";
    suma.imprimirMatriz();
    suma.imprimir();

    MatrizCSC resta = MA.restar(MB);
    cout << "A - B:\n";
    resta.imprimirMatriz();
    resta.imprimir();

    return 0;
}
