// Matriz dispersa 5x5 - representacion por lista de arrays

#include <iostream>
using namespace std;

const int N = 5;

struct Entrada { int columna, valor; };

struct NodoFila {
    int fila;
    Entrada entradas[N];
    int count;
    NodoFila* siguiente;

    NodoFila(int f) : fila(f), count(0), siguiente(nullptr) {}

    /* insertarEntrada(col, val):
     *   si col ya existe -> return false
     *   pos = count
     *   para i de 0 a count-1: si entradas[i].columna > col -> pos=i; break
     *   desplazar derecha desde pos
     *   entradas[pos] = {col, val}; count++ */
    bool insertarEntrada(int col, int val) {
        for (int i = 0; i < count; i++)
            if (entradas[i].columna == col) return false;
        int pos = count;
        for (int i = 0; i < count; i++)
            if (entradas[i].columna > col) { pos = i; break; }
        for (int i = count; i > pos; i--) entradas[i] = entradas[i - 1];
        entradas[pos] = {col, val};
        count++;
        return true;
    }

    /* eliminarEntrada(col):
     *   para i de 0 a count-1
     *     si entradas[i].columna == col
     *       desplazar izquierda desde i; count--; return true
     *   return false */
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

    int buscarColumna(int col) const {
        for (int i = 0; i < count; i++)
            if (entradas[i].columna == col) return i;
        return -1;
    }

    int buscarValor(int v) const {
        for (int i = 0; i < count; i++)
            if (entradas[i].valor == v) return i;
        return -1;
    }
};

class ListaDeArrays {
private:
    NodoFila* cabeza;

    /* buscarFila(f):
     *   recorrer lista
     *   si nodo.fila==f -> return nodo
     *   si nodo.fila > f -> return null */
    NodoFila* buscarFila(int f) const {
        NodoFila* actual = cabeza;
        while (actual != nullptr) {
            if (actual->fila == f) return actual;
            if (actual->fila > f)  return nullptr;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    /* obtenerOCrearFila(f):
     *   si cabeza==null o cabeza.fila>f -> insertar al inicio; return nuevo
     *   buscar posicion donde siguiente.fila >= f
     *   si ya existe -> return existente
     *   crear e insertar nuevo nodo de fila */
    NodoFila* obtenerOCrearFila(int f) {
        if (cabeza == nullptr || cabeza->fila > f) {
            NodoFila* nuevo = new NodoFila(f);
            nuevo->siguiente = cabeza; cabeza = nuevo; return nuevo;
        }
        NodoFila* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->fila < f)
            actual = actual->siguiente;
        if (actual->fila == f) return actual;
        if (actual->siguiente != nullptr && actual->siguiente->fila == f)
            return actual->siguiente;
        NodoFila* nuevo = new NodoFila(f);
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
        return nuevo;
    }

    /* eliminarFilaVacia(f):
     *   si cabeza.fila==f y count==0 -> eliminar cabeza
     *   sino recorrer hasta nodo con fila==f y count==0 -> desenlazar */
    void eliminarFilaVacia(int f) {
        if (cabeza == nullptr) return;
        if (cabeza->fila == f && cabeza->count == 0) {
            NodoFila* temp = cabeza; cabeza = cabeza->siguiente; delete temp; return;
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
            NodoFila* temp = cabeza; cabeza = cabeza->siguiente; delete temp;
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
     *   nodoFila = obtenerOCrearFila(f)
     *   ok = nodoFila->insertarEntrada(c, v)
     *   si !ok y count==0 -> eliminarFilaVacia(f) */
    bool insertarOrdenado(int v, int f, int c) {
        NodoFila* nodoFila = obtenerOCrearFila(f);
        bool ok = nodoFila->insertarEntrada(c, v);
        if (!ok && nodoFila->count == 0) eliminarFilaVacia(f);
        return ok;
    }

    /* eliminarPorPosicion(f, c):
     *   nodo = buscarFila(f)
     *   si null -> return false
     *   ok = nodo->eliminarEntrada(c)
     *   si ok y count==0 -> eliminarFilaVacia(f) */
    bool eliminarPorPosicion(int f, int c) {
        NodoFila* nodo = buscarFila(f);
        if (nodo == nullptr) return false;
        bool ok = nodo->eliminarEntrada(c);
        if (ok && nodo->count == 0) eliminarFilaVacia(f);
        return ok;
    }

    /* eliminarPorValor(v):
     *   recorrer lista de filas
     *     idx = nodo->buscarValor(v)
     *     si idx != -1 -> desplazar izquierda, count--
     *       si count==0 -> eliminarFilaVacia; return true
     *   return false */
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

    /* intercambiarPorPosicion(f1,c1, f2,c2):
     *   nodo1=buscarFila(f1); nodo2=buscarFila(f2)
     *   idx1=nodo1->buscarColumna(c1); idx2=nodo2->buscarColumna(c2)
     *   si alguno null o -1 -> return false
     *   tmp=nodo1->entradas[idx1].valor
     *   nodo1->entradas[idx1].valor = nodo2->entradas[idx2].valor
     *   nodo2->entradas[idx2].valor = tmp */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        NodoFila* nodo1 = buscarFila(f1);
        NodoFila* nodo2 = buscarFila(f2);
        if (nodo1 == nullptr || nodo2 == nullptr) return false;
        int idx1 = nodo1->buscarColumna(c1);
        int idx2 = nodo2->buscarColumna(c2);
        if (idx1 == -1 || idx2 == -1) return false;
        int tmp = nodo1->entradas[idx1].valor;
        nodo1->entradas[idx1].valor = nodo2->entradas[idx2].valor;
        nodo2->entradas[idx2].valor = tmp;
        return true;
    }

    /* sustituirValor(f, c, nv):
     *   nodo = buscarFila(f)
     *   idx = nodo->buscarColumna(c)
     *   si null o -1 -> return false
     *   nodo->entradas[idx].valor = nv */
    bool sustituirValor(int f, int c, int nv) {
        NodoFila* nodo = buscarFila(f);
        if (nodo == nullptr) return false;
        int idx = nodo->buscarColumna(c);
        if (idx == -1) return false;
        nodo->entradas[idx].valor = nv;
        return true;
    }

    void imprimir() const {
        NodoFila* actual = cabeza;
        while (actual != nullptr) {
            for (int i = 0; i < actual->count; i++)
                cout << "(" << actual->fila << "," << actual->entradas[i].columna
                     << ")=" << actual->entradas[i].valor << " ";
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

    ListaDeArrays matriz;
    matriz.inicializar(matrix);

    cout << "Matriz inicial:\n";
    matriz.imprimir();

    matriz.eliminarPorPosicion(0, 3);
    cout << "eliminarPorPosicion(0,3):\n";
    matriz.imprimir();

    matriz.eliminarPorValor(8);
    cout << "eliminarPorValor(8):\n";
    matriz.imprimir();

    matriz.intercambiarPorPosicion(0, 2, 3, 0);
    cout << "intercambiarPorPosicion(0,2, 3,0):\n";
    matriz.imprimir();

    matriz.sustituirValor(3, 0, 99);
    cout << "sustituirValor(3,0, 99):\n";
    matriz.imprimir();

    return 0;
}
