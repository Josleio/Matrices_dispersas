/*
 * Representacion de Matriz Dispersa - Lista Enlazada
 *
 * Cada nodo almacena el valor, fila y columna de un elemento no cero.
 * La lista se mantiene ordenada por fila y, dentro de la misma fila, por columna.
 *
 * Operaciones:
 *   1. insertarOrdenado  - inserta en orden por (fila, columna)
 *   2. eliminarPorPosicion - elimina el nodo en (fila, columna)
 *   3. eliminarPorValor    - elimina el primer nodo con el valor dado
 *   4. intercambiarPorPosicion - intercambia los valores de dos posiciones
 *   5. sustituirValor      - reemplaza el valor en (fila, columna)
 */

#include <iostream>
using namespace std;

struct Nodo {
    int valor;
    int fila;
    int columna;
    Nodo* siguiente;

    Nodo(int v, int f, int c) : valor(v), fila(f), columna(c), siguiente(nullptr) {}
};

class Lista_enlazada {
private:
    Nodo* cabeza;
    Nodo* ventana;

public:
    Lista_enlazada() : cabeza(nullptr), ventana(nullptr) {}

    ~Lista_enlazada() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    /* 1. Insertar en orden ascendente por (fila, columna). */
    void insertarOrdenado(int v, int f, int c) {
        Nodo* nuevo = new Nodo(v, f, c);
        if (cabeza == nullptr || f < cabeza->fila ||
            (f == cabeza->fila && c < cabeza->columna)) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
            return;
        }

        ventana = cabeza;
        while (ventana->siguiente != nullptr &&
               (ventana->siguiente->fila < f ||
                (ventana->siguiente->fila == f && ventana->siguiente->columna < c))) {
            ventana = ventana->siguiente;
        }
        nuevo->siguiente = ventana->siguiente;
        ventana->siguiente = nuevo;
    }

    /* 2. Eliminar el nodo en la posicion (f, c). Retorna true si lo elimino. */
    bool eliminarPorPosicion(int f, int c) {
        if (cabeza == nullptr) return false;
        if (cabeza->fila == f && cabeza->columna == c) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return true;
        }
        ventana = cabeza;
        while (ventana->siguiente != nullptr) {
            if (ventana->siguiente->fila == f && ventana->siguiente->columna == c) {
                Nodo* temp = ventana->siguiente;
                ventana->siguiente = temp->siguiente;
                delete temp;
                return true;
            }
            ventana = ventana->siguiente;
        }
        return false;
    }

    /* 3. Eliminar la primera ocurrencia del valor v. Retorna true si lo elimino. */
    bool eliminarPorValor(int v) {
        if (cabeza == nullptr) return false;
        if (cabeza->valor == v) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return true;
        }
        ventana = cabeza;
        while (ventana->siguiente != nullptr) {
            if (ventana->siguiente->valor == v) {
                Nodo* temp = ventana->siguiente;
                ventana->siguiente = temp->siguiente;
                delete temp;
                return true;
            }
            ventana = ventana->siguiente;
        }
        return false;
    }

    /* 4. Intercambiar los valores de los nodos en (f1,c1) y (f2,c2). */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        Nodo* nodo1 = nullptr;
        Nodo* nodo2 = nullptr;
        ventana = cabeza;
        while (ventana != nullptr) {
            if (ventana->fila == f1 && ventana->columna == c1) nodo1 = ventana;
            if (ventana->fila == f2 && ventana->columna == c2) nodo2 = ventana;
            ventana = ventana->siguiente;
        }
        if (nodo1 == nullptr || nodo2 == nullptr) return false;
        swap(nodo1->valor, nodo2->valor);
        return true;
    }

    /* 5. Sustituir el valor en (f, c) por nuevo_valor. Retorna true si lo encontro. */
    bool sustituirValor(int f, int c, int nuevo_valor) {
        ventana = cabeza;
        while (ventana != nullptr) {
            if (ventana->fila == f && ventana->columna == c) {
                ventana->valor = nuevo_valor;
                return true;
            }
            ventana = ventana->siguiente;
        }
        return false;
    }

    void imprimir() const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << "(" << actual->fila << ", " << actual->columna
                 << ") = " << actual->valor << "\n";
            actual = actual->siguiente;
        }
    }
};

int main() {
    Lista_enlazada lista;

    // Insertar valores en la matriz dispersa
    lista.insertarOrdenado(8, 0, 0);
    lista.insertarOrdenado(5, 0, 2);
    lista.insertarOrdenado(3, 1, 0);
    lista.insertarOrdenado(7, 2, 1);

    cout << "Lista original:\n";
    lista.imprimir();

    // Eliminar por posicion
    lista.eliminarPorPosicion(0, 2);
    cout << "\nDespues de eliminar posicion (0,2):\n";
    lista.imprimir();

    // Eliminar por valor
    lista.eliminarPorValor(3);
    cout << "\nDespues de eliminar valor 3:\n";
    lista.imprimir();

    // Volver a insertar para demostrar intercambio
    lista.insertarOrdenado(3, 1, 0);
    lista.intercambiarPorPosicion(0, 0, 2, 1);
    cout << "\nDespues de intercambiar (0,0) y (2,1):\n";
    lista.imprimir();

    // Sustituir valor
    lista.sustituirValor(0, 0, 99);
    cout << "\nDespues de sustituir (0,0) con 99:\n";
    lista.imprimir();

    return 0;
}
