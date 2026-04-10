/*
 * Representacion de Matriz Dispersa - Formato CSR
 * (Compressed Sparse Row / Fila Dispersa Comprimida)
 *
 * Tres arreglos representan todos los elementos no cero:
 *   valores[]     - valores de los elementos no cero
 *   ind_col[]     - indice de columna de cada elemento no cero
 *   ptr_fila[]    - ptr_fila[i] = primer indice en valores[] que pertenece a la fila i
 *                   ptr_fila[num_filas] = total de elementos no cero
 *
 * Ejemplo para la matriz 3x3:
 *   [8  0  5]
 *   [3  0  0]
 *   [0  7  0]
 *
 *   valores   = [8, 5, 3, 7]
 *   ind_col   = [0, 2, 0, 1]
 *   ptr_fila  = [0, 2, 3, 4]
 *
 * Dentro de cada fila, los elementos estan ordenados por columna.
 *
 * Operaciones:
 *   1. insertarOrdenado     - inserta en orden por (fila, columna)
 *   2. eliminarPorPosicion  - elimina el elemento en (fila, columna)
 *   3. eliminarPorValor     - elimina la primera ocurrencia del valor dado
 *   4. intercambiarPorPosicion - intercambia los valores de dos posiciones
 *   5. sustituirValor       - reemplaza el valor en (fila, columna)
 *   6. sumar                - suma de dos matrices en formato CSR
 *   7. restar               - resta de dos matrices en formato CSR
 */

#include <iostream>
#include <vector>
using namespace std;

class MatrizCSR {
private:
    vector<int> valores;
    vector<int> ind_col;
    vector<int> ptr_fila;
    int num_filas;
    int num_cols;

    /* Retorna el indice en valores[] del elemento (f, c), o -1 si no existe. */
    int buscarIndice(int f, int c) const {
        if (f < 0 || f >= num_filas) return -1;
        for (int k = ptr_fila[f]; k < ptr_fila[f + 1]; k++) {
            if (ind_col[k] == c) return k;
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

    /* Devuelve la fila a la que pertenece el indice k en valores[]. */
    int filaDeIndice(int k) const {
        for (int f = 0; f < num_filas; f++) {
            if (k >= ptr_fila[f] && k < ptr_fila[f + 1]) return f;
        }
        return -1;
    }

    /* Elimina el elemento en el indice k y actualiza ptr_fila. */
    void eliminarIndice(int k) {
        int fila = filaDeIndice(k);
        valores.erase(valores.begin() + k);
        ind_col.erase(ind_col.begin() + k);
        for (int f = fila + 1; f <= num_filas; f++) ptr_fila[f]--;
    }

public:
    /* Constructor: define el tamano de la matriz (filas x cols). */
    MatrizCSR(int filas, int cols) : num_filas(filas), num_cols(cols) {
        ptr_fila.assign(filas + 1, 0);
    }

    /* 1. Insertar en orden por (fila, columna). Retorna false si ya existe. */
    bool insertarOrdenado(int v, int f, int c) {
        if (f < 0 || f >= num_filas || c < 0 || c >= num_cols) return false;
        if (buscarIndice(f, c) != -1) return false;

        // Encontrar posicion de insercion dentro de la fila f (orden por columna)
        int pos = ptr_fila[f + 1]; // insertar al final de la fila por defecto
        for (int k = ptr_fila[f]; k < ptr_fila[f + 1]; k++) {
            if (ind_col[k] > c) { pos = k; break; }
        }

        valores.insert(valores.begin() + pos, v);
        ind_col.insert(ind_col.begin() + pos, c);
        for (int i = f + 1; i <= num_filas; i++) ptr_fila[i]++;
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
    MatrizCSR sumar(const MatrizCSR& B) const {
        MatrizCSR resultado(num_filas, num_cols);
        for (int f = 0; f < num_filas; f++) {
            int ia = ptr_fila[f], ib = B.ptr_fila[f];
            int ea = ptr_fila[f + 1], eb = B.ptr_fila[f + 1];
            while (ia < ea || ib < eb) {
                int col_a = (ia < ea) ? ind_col[ia] : num_cols;
                int col_b = (ib < eb) ? B.ind_col[ib] : num_cols;
                if (col_a < col_b) {
                    resultado.insertarOrdenado(valores[ia], f, col_a);
                    ia++;
                } else if (col_b < col_a) {
                    resultado.insertarOrdenado(B.valores[ib], f, col_b);
                    ib++;
                } else {
                    int suma = valores[ia] + B.valores[ib];
                    if (suma != 0) resultado.insertarOrdenado(suma, f, col_a);
                    ia++; ib++;
                }
            }
        }
        return resultado;
    }

    /* 7. Resta de esta matriz menos otra (A - B). Retorna la matriz resultado. */
    MatrizCSR restar(const MatrizCSR& B) const {
        MatrizCSR resultado(num_filas, num_cols);
        for (int f = 0; f < num_filas; f++) {
            int ia = ptr_fila[f], ib = B.ptr_fila[f];
            int ea = ptr_fila[f + 1], eb = B.ptr_fila[f + 1];
            while (ia < ea || ib < eb) {
                int col_a = (ia < ea) ? ind_col[ia] : num_cols;
                int col_b = (ib < eb) ? B.ind_col[ib] : num_cols;
                if (col_a < col_b) {
                    resultado.insertarOrdenado(valores[ia], f, col_a);
                    ia++;
                } else if (col_b < col_a) {
                    resultado.insertarOrdenado(-B.valores[ib], f, col_b);
                    ib++;
                } else {
                    int diff = valores[ia] - B.valores[ib];
                    if (diff != 0) resultado.insertarOrdenado(diff, f, col_a);
                    ia++; ib++;
                }
            }
        }
        return resultado;
    }

    void imprimir() const {
        cout << "valores   : [";
        for (int i = 0; i < (int)valores.size(); i++)
            cout << (i ? ", " : "") << valores[i];
        cout << "]\n";

        cout << "ind_col   : [";
        for (int i = 0; i < (int)ind_col.size(); i++)
            cout << (i ? ", " : "") << ind_col[i];
        cout << "]\n";

        cout << "ptr_fila  : [";
        for (int i = 0; i <= num_filas; i++)
            cout << (i ? ", " : "") << ptr_fila[i];
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
    MatrizCSR A(3, 3);
    A.insertarOrdenado(8, 0, 0);
    A.insertarOrdenado(5, 0, 2);
    A.insertarOrdenado(3, 1, 0);
    A.insertarOrdenado(7, 2, 1);

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
    cout << "\n=== Suma y Resta de matrices CSR ===\n";
    MatrizCSR MA(3, 3), MB(3, 3);
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

    MatrizCSR suma = MA.sumar(MB);
    cout << "A + B:\n";
    suma.imprimirMatriz();
    suma.imprimir();

    MatrizCSR resta = MA.restar(MB);
    cout << "A - B:\n";
    resta.imprimirMatriz();
    resta.imprimir();

    return 0;
}
