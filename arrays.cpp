// Matriz dispersa 5x5 - representacion por arrays paralelos

#include <iostream>
using namespace std;

const int N     = 5;
const int MAX_NNZ = N * N;

class MatrizArrays {
private:
    int valores[MAX_NNZ];
    int filas[MAX_NNZ];
    int columnas[MAX_NNZ];
    int count;

    /* buscarPosicion(f, c):
     *   para i de 0 a count-1
     *     si filas[i]==f y columnas[i]==c -> return i
     *   return -1 */
    int buscarPosicion(int f, int c) const {
        for (int i = 0; i < count; i++)
            if (filas[i] == f && columnas[i] == c) return i;
        return -1;
    }

    /* buscarValor(v):
     *   para i de 0 a count-1
     *     si valores[i]==v -> return i
     *   return -1 */
    int buscarValor(int v) const {
        for (int i = 0; i < count; i++)
            if (valores[i] == v) return i;
        return -1;
    }

    /* desplazarDerecha(desde):
     *   para i de count hasta desde+1
     *     valores[i]=valores[i-1]; filas[i]=filas[i-1]; columnas[i]=columnas[i-1] */
    void desplazarDerecha(int desde) {
        for (int i = count; i > desde; i--) {
            valores[i]  = valores[i - 1];
            filas[i]    = filas[i - 1];
            columnas[i] = columnas[i - 1];
        }
    }

    /* desplazarIzquierda(desde):
     *   para i de desde a count-2
     *     valores[i]=valores[i+1]; filas[i]=filas[i+1]; columnas[i]=columnas[i+1] */
    void desplazarIzquierda(int desde) {
        for (int i = desde; i < count - 1; i++) {
            valores[i]  = valores[i + 1];
            filas[i]    = filas[i + 1];
            columnas[i] = columnas[i + 1];
        }
    }

public:
    MatrizArrays() : count(0) {}

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
     *   si count >= MAX_NNZ o (f,c) ya existe -> return false
     *   pos = count
     *   para i de 0 a count-1: si (filas[i],col[i]) > (f,c) -> pos=i; break
     *   desplazarDerecha(pos)
     *   valores[pos]=v; filas[pos]=f; columnas[pos]=c; count++ */
    bool insertarOrdenado(int v, int f, int c) {
        if (count >= MAX_NNZ || buscarPosicion(f, c) != -1) return false;
        int pos = count;
        for (int i = 0; i < count; i++) {
            if (filas[i] > f || (filas[i] == f && columnas[i] > c)) { pos = i; break; }
        }
        desplazarDerecha(pos);
        valores[pos] = v; filas[pos] = f; columnas[pos] = c;
        count++;
        return true;
    }

    /* eliminarPorPosicion(f, c):
     *   idx = buscarPosicion(f, c)
     *   si idx == -1 -> return false
     *   desplazarIzquierda(idx); count-- */
    bool eliminarPorPosicion(int f, int c) {
        int idx = buscarPosicion(f, c);
        if (idx == -1) return false;
        desplazarIzquierda(idx);
        count--;
        return true;
    }

    /* eliminarPorValor(v):
     *   idx = buscarValor(v)
     *   si idx == -1 -> return false
     *   desplazarIzquierda(idx); count-- */
    bool eliminarPorValor(int v) {
        int idx = buscarValor(v);
        if (idx == -1) return false;
        desplazarIzquierda(idx);
        count--;
        return true;
    }

    /* intercambiarPorPosicion(f1,c1, f2,c2):
     *   idx1 = buscarPosicion(f1,c1); idx2 = buscarPosicion(f2,c2)
     *   si alguno == -1 -> return false
     *   tmp=valores[idx1]; valores[idx1]=valores[idx2]; valores[idx2]=tmp */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        int idx1 = buscarPosicion(f1, c1);
        int idx2 = buscarPosicion(f2, c2);
        if (idx1 == -1 || idx2 == -1) return false;
        int tmp = valores[idx1]; valores[idx1] = valores[idx2]; valores[idx2] = tmp;
        return true;
    }

    /* sustituirValor(f, c, nv):
     *   idx = buscarPosicion(f, c)
     *   si idx == -1 -> return false
     *   valores[idx] = nv */
    bool sustituirValor(int f, int c, int nv) {
        int idx = buscarPosicion(f, c);
        if (idx == -1) return false;
        valores[idx] = nv;
        return true;
    }

    void imprimir() const {
        for (int i = 0; i < count; i++)
            cout << "(" << filas[i] << "," << columnas[i] << ")=" << valores[i] << " ";
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

    MatrizArrays matriz;
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
