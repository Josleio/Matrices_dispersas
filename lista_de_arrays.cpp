/*
 * Representacion de Matriz Dispersa - Lista de Arrays
 *
 * La estructura combina una lista enlazada de filas con arreglos dinamicos
 * de entradas por fila. Cada nodo de la lista representa una fila de la
 * matriz y contiene un arreglo de pares (columna, valor) para esa fila.
 *
 *   NodoFila -> [fila | entradas[] | siguiente]
 *                         |
 *                   {columna, valor}  {columna, valor} ...
 *
 * Las filas estan ordenadas de menor a mayor y, dentro de cada fila,
 * las entradas tambien estan ordenadas por columna.
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

/* Par (columna, valor) almacenado dentro de cada fila. */
struct Entrada {
    int columna;
    int valor;
};

/* Nodo de la lista: representa una fila completa. */
struct NodoFila {
    int fila;
    Entrada* entradas; // arreglo dinamico de entradas
    int count;         // entradas ocupadas
    int capacidad;     // capacidad actual del arreglo
    NodoFila* siguiente;

    explicit NodoFila(int f, int cap = 4)
        : fila(f), count(0), capacidad(cap), siguiente(nullptr) {
        entradas = new Entrada[cap];
    }

    ~NodoFila() { delete[] entradas; }

    /* Duplicar capacidad del arreglo interno.
     * La nueva memoria se asigna antes de liberar la antigua; si 'new' lanza
     * una excepcion el arreglo original no se ve afectado. Como Entrada solo
     * contiene enteros, la copia nunca puede lanzar una excepcion. */
    void ampliar() {
        int nueva_cap = capacidad * 2;
        Entrada* nuevo = new Entrada[nueva_cap]; // lanza bad_alloc si falla; entradas permanece valido
        for (int i = 0; i < count; i++) nuevo[i] = entradas[i];
        delete[] entradas;
        entradas  = nuevo;
        capacidad = nueva_cap;
    }

    /* Insertar entrada en orden por columna. Retorna false si ya existe. */
    bool insertarEntrada(int col, int val) {
        for (int i = 0; i < count; i++) {
            if (entradas[i].columna == col) return false; // ya existe
        }
        if (count == capacidad) ampliar();
        // Encontrar posicion de insercion
        int pos = count;
        for (int i = 0; i < count; i++) {
            if (entradas[i].columna > col) { pos = i; break; }
        }
        // Desplazar a la derecha
        for (int i = count; i > pos; i--) entradas[i] = entradas[i - 1];
        entradas[pos] = {col, val};
        count++;
        return true;
    }

    /* Eliminar la entrada con la columna dada. Retorna true si la elimino. */
    bool eliminarEntrada(int col) {
        for (int i = 0; i < count; i++) {
            if (entradas[i].columna == col) {
                for (int j = i; j < count - 1; j++) entradas[j] = entradas[j + 1];
                count--;
                return true;
            }
        }
        return false;
    }

    /* Busca el indice de la entrada con columna col. Retorna -1 si no existe. */
    int buscarColumna(int col) const {
        for (int i = 0; i < count; i++) {
            if (entradas[i].columna == col) return i;
        }
        return -1;
    }

    /* Busca el indice de la primera entrada con valor v. Retorna -1 si no existe. */
    int buscarValor(int v) const {
        for (int i = 0; i < count; i++) {
            if (entradas[i].valor == v) return i;
        }
        return -1;
    }
};

class ListaDeArrays {
private:
    NodoFila* cabeza;

    /* Busca el NodoFila con fila == f. Retorna nullptr si no existe. */
    NodoFila* buscarFila(int f) const {
        NodoFila* actual = cabeza;
        while (actual != nullptr) {
            if (actual->fila == f) return actual;
            if (actual->fila > f)  return nullptr;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    /* Obtiene o crea el NodoFila para la fila f, insertando en orden. */
    NodoFila* obtenerOCrearFila(int f) {
        if (cabeza == nullptr || cabeza->fila > f) {
            NodoFila* nuevo = new NodoFila(f);
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
            return nuevo;
        }
        NodoFila* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->fila < f) {
            actual = actual->siguiente;
        }
        if (actual->fila == f) return actual;
        if (actual->siguiente != nullptr && actual->siguiente->fila == f)
            return actual->siguiente;
        NodoFila* nuevo = new NodoFila(f);
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
        return nuevo;
    }

    /* Elimina el NodoFila para la fila f si esta vacia. */
    void eliminarFilaVacia(int f) {
        if (cabeza == nullptr) return;
        if (cabeza->fila == f && cabeza->count == 0) {
            NodoFila* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return;
        }
        NodoFila* actual = cabeza;
        while (actual->siguiente != nullptr) {
            if (actual->siguiente->fila == f && actual->siguiente->count == 0) {
                NodoFila* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                return;
            }
            actual = actual->siguiente;
        }
    }

public:
    ListaDeArrays() : cabeza(nullptr) {}

    ~ListaDeArrays() {
        while (cabeza != nullptr) {
            NodoFila* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    /* 1. Insertar en orden por (fila, columna). */
    bool insertarOrdenado(int v, int f, int c) {
        NodoFila* nodoFila = obtenerOCrearFila(f);
        bool ok = nodoFila->insertarEntrada(c, v);
        // Si se creo una fila nueva pero no se pudo insertar (caso improbable
        // de columna duplicada en fila recien creada), se elimina el nodo vacio.
        if (!ok && nodoFila->count == 0) eliminarFilaVacia(f);
        return ok;
    }

    /* 2. Eliminar el elemento en (f, c). Retorna true si lo elimino. */
    bool eliminarPorPosicion(int f, int c) {
        NodoFila* nodo = buscarFila(f);
        if (nodo == nullptr) return false;
        bool ok = nodo->eliminarEntrada(c);
        if (ok && nodo->count == 0) eliminarFilaVacia(f);
        return ok;
    }

    /* 3. Eliminar la primera ocurrencia del valor v. Retorna true si lo elimino. */
    bool eliminarPorValor(int v) {
        NodoFila* actual = cabeza;
        while (actual != nullptr) {
            int idx = actual->buscarValor(v);
            if (idx != -1) {
                int fila = actual->fila;
                for (int j = idx; j < actual->count - 1; j++)
                    actual->entradas[j] = actual->entradas[j + 1];
                actual->count--;
                if (actual->count == 0) eliminarFilaVacia(fila);
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    /* 4. Intercambiar los valores en (f1,c1) y (f2,c2). Retorna true si existen ambos. */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        NodoFila* nodo1 = buscarFila(f1);
        NodoFila* nodo2 = buscarFila(f2);
        if (nodo1 == nullptr || nodo2 == nullptr) return false;
        int idx1 = nodo1->buscarColumna(c1);
        int idx2 = nodo2->buscarColumna(c2);
        if (idx1 == -1 || idx2 == -1) return false;
        swap(nodo1->entradas[idx1].valor, nodo2->entradas[idx2].valor);
        return true;
    }

    /* 5. Sustituir el valor en (f, c) por nuevo_valor. Retorna true si lo encontro. */
    bool sustituirValor(int f, int c, int nuevo_valor) {
        NodoFila* nodo = buscarFila(f);
        if (nodo == nullptr) return false;
        int idx = nodo->buscarColumna(c);
        if (idx == -1) return false;
        nodo->entradas[idx].valor = nuevo_valor;
        return true;
    }

    void imprimir() const {
        NodoFila* actual = cabeza;
        while (actual != nullptr) {
            for (int i = 0; i < actual->count; i++) {
                cout << "(" << actual->fila << ", "
                     << actual->entradas[i].columna
                     << ") = " << actual->entradas[i].valor << "\n";
            }
            actual = actual->siguiente;
        }
    }
};

int main() {
    ListaDeArrays matriz;

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
