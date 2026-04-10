// Matriz dispersa 5x5 - representacion por lista enlazada

#include <iostream>
using namespace std;

struct Nodo {
    int valor, fila, columna;
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
        /* ventana = cabeza
         * mientras ventana != null
         *   temp = ventana; ventana = ventana->sig; delete temp */
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    /* inicializar(m):
     *   para f de 0 a 4
     *     para c de 0 a 4
     *       si m[f][c] != 0 -> insertarOrdenado(m[f][c], f, c) */
    void inicializar(int m[5][5]) {
        for (int f = 0; f < 5; f++)
            for (int c = 0; c < 5; c++)
                if (m[f][c] != 0)
                    insertarOrdenado(m[f][c], f, c);
    }

    /* insertarOrdenado(v, f, c):
     *   nuevo = Nodo(v, f, c)
     *   si cabeza==null o (f,c) < (cabeza.fila, cabeza.columna)
     *     nuevo->sig = cabeza; cabeza = nuevo; return
     *   ventana = cabeza
     *   mientras ventana->sig != null y (ventana->sig.fila,col) < (f,c)
     *     ventana = ventana->sig
     *   nuevo->sig = ventana->sig; ventana->sig = nuevo */
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
                (ventana->siguiente->fila == f && ventana->siguiente->columna < c)))
            ventana = ventana->siguiente;
        nuevo->siguiente = ventana->siguiente;
        ventana->siguiente = nuevo;
    }

    /* eliminarPorPosicion(f, c):
     *   si cabeza.(fila,col) == (f,c) -> eliminar cabeza; return true
     *   ventana = cabeza
     *   mientras ventana->sig != null
     *     si ventana->sig.(fila,col) == (f,c) -> desenlazar; return true
     *   return false */
    bool eliminarPorPosicion(int f, int c) {
        if (cabeza == nullptr) return false;
        if (cabeza->fila == f && cabeza->columna == c) {
            Nodo* temp = cabeza; cabeza = cabeza->siguiente; delete temp; return true;
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

    /* eliminarPorValor(v):
     *   si cabeza.valor == v -> eliminar cabeza; return true
     *   ventana = cabeza
     *   mientras ventana->sig != null
     *     si ventana->sig.valor == v -> desenlazar; return true
     *   return false */
    bool eliminarPorValor(int v) {
        if (cabeza == nullptr) return false;
        if (cabeza->valor == v) {
            Nodo* temp = cabeza; cabeza = cabeza->siguiente; delete temp; return true;
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

    /* sustituirPorPosicion(f, c, nv):
     *   recorrer lista
     *   si nodo.(fila,col) == (f,c) -> nodo.valor = nv; return true
     *   return false */
    bool sustituirPorPosicion(int f, int c, int nv) {
        ventana = cabeza;
        while (ventana != nullptr) {
            if (ventana->fila == f && ventana->columna == c) {
                ventana->valor = nv; return true;
            }
            ventana = ventana->siguiente;
        }
        return false;
    }

    void imprimir() const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << "(" << actual->fila << "," << actual->columna
                 << ")=" << actual->valor << " ";
            actual = actual->siguiente;
        }
        cout << "\n";
    }
};

int main() {
    int matrix[5][5] = {
        {0,0,5,1,0},
        {0,0,0,8,0},
        {0,0,0,0,0},
        {1,0,0,0,0},
        {0,0,0,0,0}
    };

    Lista_enlazada lista;
    lista.inicializar(matrix);

    cout << "Lista inicial:\n";
    lista.imprimir();

    lista.eliminarPorPosicion(0, 3);
    cout << "eliminarPorPosicion(0,3):\n";
    lista.imprimir();

    lista.eliminarPorValor(8);
    cout << "eliminarPorValor(8):\n";
    lista.imprimir();

    lista.sustituirPorPosicion(0, 2, 99);
    cout << "sustituirPorPosicion(0,2, 99):\n";
    lista.imprimir();

    return 0;
}
