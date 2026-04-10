// Matriz dispersa 5x5 - representacion por lista enlazada

#include <iostream>
using namespace std;

const int N = 5;

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
     *   para f de 0 a N-1
     *     para c de 0 a N-1
     *       si m[f][c] != 0 -> insertarOrdenado(m[f][c], f, c) */
    void inicializar(int m[N][N]) {
        for (int f = 0; f < N; f++)
            for (int c = 0; c < N; c++)
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

    /* intercambiarPorPosicion(f1,c1, f2,c2):
     *   nodo1 = null; nodo2 = null
     *   recorrer lista buscando (f1,c1) y (f2,c2)
     *   si alguno es null -> return false
     *   tmp = nodo1->valor; nodo1->valor = nodo2->valor; nodo2->valor = tmp */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        Nodo* nodo1 = nullptr; Nodo* nodo2 = nullptr;
        ventana = cabeza;
        while (ventana != nullptr) {
            if (ventana->fila == f1 && ventana->columna == c1) nodo1 = ventana;
            if (ventana->fila == f2 && ventana->columna == c2) nodo2 = ventana;
            ventana = ventana->siguiente;
        }
        if (nodo1 == nullptr || nodo2 == nullptr) return false;
        int tmp = nodo1->valor; nodo1->valor = nodo2->valor; nodo2->valor = tmp;
        return true;
    }

    /* sustituirValor(f, c, nv):
     *   recorrer lista
     *   si nodo.(fila,col) == (f,c) -> nodo.valor = nv; return true
     *   return false */
    bool sustituirValor(int f, int c, int nv) {
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
    int matrix[N][N] = {
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

    lista.intercambiarPorPosicion(0, 2, 3, 0);
    cout << "intercambiarPorPosicion(0,2, 3,0):\n";
    lista.imprimir();

    lista.sustituirValor(3, 0, 99);
    cout << "sustituirValor(3,0, 99):\n";
    lista.imprimir();

    return 0;
}
