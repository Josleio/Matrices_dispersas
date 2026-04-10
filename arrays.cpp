/*
 * Representacion de Matriz Dispersa - Arrays Paralelos
 *
 * Tres arreglos paralelos almacenan los elementos no cero:
 *   valores[]  - valor de cada elemento
 *   filas[]    - fila del elemento
 *   columnas[] - columna del elemento
 *   count      - cantidad actual de elementos no cero
 *
 * Los arreglos se mantienen ordenados por (fila, columna).
 *
 * Operaciones:
 *   1. insertarOrdenado     - inserta en orden por (fila, columna)
 *   2. eliminarPorPosicion  - elimina el elemento en (fila, columna)
 *   3. eliminarPorValor     - elimina la primera ocurrencia del valor dado
 *   4. intercambiarPorPosicion - intercambia los valores de dos posiciones
 *   5. sustituirValor       - reemplaza el valor en (fila, columna)
 */

#include <iostream>
using namespace std;

const int MAX_NNZ = 100; // Maximo de elementos no cero permitidos

class MatrizArrays {
private:
    int valores[MAX_NNZ];
    int filas[MAX_NNZ];
    int columnas[MAX_NNZ];
    int count;

    /* Busca el indice del elemento en (f, c). Retorna -1 si no existe. */
    int buscarPosicion(int f, int c) const {
        for (int i = 0; i < count; i++) {
            if (filas[i] == f && columnas[i] == c) return i;
        }
        return -1;
    }

    /* Busca el indice del primer elemento con valor v. Retorna -1 si no existe. */
    int buscarValor(int v) const {
        for (int i = 0; i < count; i++) {
            if (valores[i] == v) return i;
        }
        return -1;
    }

    /* Desplaza los elementos desde 'desde' una posicion a la derecha. */
    void desplazarDerecha(int desde) {
        for (int i = count; i > desde; i--) {
            valores[i]  = valores[i - 1];
            filas[i]    = filas[i - 1];
            columnas[i] = columnas[i - 1];
        }
    }

    /* Desplaza los elementos desde 'desde+1' una posicion a la izquierda. */
    void desplazarIzquierda(int desde) {
        for (int i = desde; i < count - 1; i++) {
            valores[i]  = valores[i + 1];
            filas[i]    = filas[i + 1];
            columnas[i] = columnas[i + 1];
        }
    }

public:
    MatrizArrays() : count(0) {}

    /* 1. Insertar en orden ascendente por (fila, columna).
     *    Retorna false si el arreglo esta lleno o la posicion ya existe. */
    bool insertarOrdenado(int v, int f, int c) {
        if (count >= MAX_NNZ) return false;
        if (buscarPosicion(f, c) != -1) return false;

        // Encontrar la posicion de insercion
        int pos = count;
        for (int i = 0; i < count; i++) {
            if (filas[i] > f || (filas[i] == f && columnas[i] > c)) {
                pos = i;
                break;
            }
        }

        desplazarDerecha(pos);
        valores[pos]  = v;
        filas[pos]    = f;
        columnas[pos] = c;
        count++;
        return true;
    }

    /* 2. Eliminar el elemento en la posicion (f, c). Retorna true si lo elimino. */
    bool eliminarPorPosicion(int f, int c) {
        int idx = buscarPosicion(f, c);
        if (idx == -1) return false;
        desplazarIzquierda(idx);
        count--;
        return true;
    }

    /* 3. Eliminar la primera ocurrencia del valor v. Retorna true si lo elimino. */
    bool eliminarPorValor(int v) {
        int idx = buscarValor(v);
        if (idx == -1) return false;
        desplazarIzquierda(idx);
        count--;
        return true;
    }

    /* 4. Intercambiar los valores en (f1,c1) y (f2,c2). Retorna true si existen ambos. */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        int idx1 = buscarPosicion(f1, c1);
        int idx2 = buscarPosicion(f2, c2);
        if (idx1 == -1 || idx2 == -1) return false;
        swap(valores[idx1], valores[idx2]);
        return true;
    }

    /* 5. Sustituir el valor en (f, c) por nuevo_valor. Retorna true si lo encontro. */
    bool sustituirValor(int f, int c, int nuevo_valor) {
        int idx = buscarPosicion(f, c);
        if (idx == -1) return false;
        valores[idx] = nuevo_valor;
        return true;
    }

    void imprimir() const {
        for (int i = 0; i < count; i++) {
            cout << "(" << filas[i] << ", " << columnas[i]
                 << ") = " << valores[i] << "\n";
        }
    }
};

int main() {
    MatrizArrays matriz;

    // Insertar valores en la matriz dispersa
    matriz.insertarOrdenado(8, 0, 0);
    matriz.insertarOrdenado(5, 0, 2);
    matriz.insertarOrdenado(3, 1, 0);
    matriz.insertarOrdenado(7, 2, 1);

    cout << "Matriz original:\n";
    matriz.imprimir();

    // Eliminar por posicion
    matriz.eliminarPorPosicion(0, 2);
    cout << "\nDespues de eliminar posicion (0,2):\n";
    matriz.imprimir();

    // Eliminar por valor
    matriz.eliminarPorValor(3);
    cout << "\nDespues de eliminar valor 3:\n";
    matriz.imprimir();

    // Volver a insertar para demostrar intercambio
    matriz.insertarOrdenado(3, 1, 0);
    matriz.intercambiarPorPosicion(0, 0, 2, 1);
    cout << "\nDespues de intercambiar (0,0) y (2,1):\n";
    matriz.imprimir();

    // Sustituir valor
    matriz.sustituirValor(0, 0, 99);
    cout << "\nDespues de sustituir (0,0) con 99:\n";
    matriz.imprimir();

    return 0;
}
