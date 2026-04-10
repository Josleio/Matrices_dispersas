// Matriz dispersa 5x5 - formato CSC (Compressed Sparse Column)

#include <iostream>
using namespace std;

const int N       = 5;
const int MAX_NNZ = N * N;

class MatrizCSC {
private:
    int valores[MAX_NNZ];
    int ind_fila[MAX_NNZ];
    int ptr_col[N + 1];
    int nnz;

    /* buscarIndice(f, c):
     *   para k de ptr_col[c] a ptr_col[c+1]-1
     *     si ind_fila[k] == f -> return k
     *   return -1 */
    int buscarIndice(int f, int c) const {
        if (c < 0 || c >= N) return -1;
        for (int k = ptr_col[c]; k < ptr_col[c + 1]; k++)
            if (ind_fila[k] == f) return k;
        return -1;
    }

    /* buscarPorValor(v):
     *   para k de 0 a nnz-1
     *     si valores[k] == v -> return k
     *   return -1 */
    int buscarPorValor(int v) const {
        for (int k = 0; k < nnz; k++)
            if (valores[k] == v) return k;
        return -1;
    }

    /* columnaDeIndice(k):
     *   para c de 0 a N-1
     *     si ptr_col[c] <= k < ptr_col[c+1] -> return c
     *   return -1 */
    int columnaDeIndice(int k) const {
        for (int c = 0; c < N; c++)
            if (k >= ptr_col[c] && k < ptr_col[c + 1]) return c;
        return -1;
    }

    /* eliminarIndice(k):
     *   col = columnaDeIndice(k)
     *   para i de k a nnz-2: valores[i]=valores[i+1]; ind_fila[i]=ind_fila[i+1]
     *   nnz--
     *   para j de col+1 a N: ptr_col[j]-- */
    void eliminarIndice(int k) {
        int col = columnaDeIndice(k);
        for (int i = k; i < nnz - 1; i++) {
            valores[i]  = valores[i + 1];
            ind_fila[i] = ind_fila[i + 1];
        }
        nnz--;
        for (int j = col + 1; j <= N; j++) ptr_col[j]--;
    }

public:
    MatrizCSC() : nnz(0) {
        for (int i = 0; i <= N; i++) ptr_col[i] = 0;
    }

    /* inicializar(m):
     *   ptr_col[0] = 0
     *   para c de 0 a N-1
     *     ptr_col[c+1] = ptr_col[c]
     *     para f de 0 a N-1
     *       si m[f][c] != 0
     *         valores[nnz]=m[f][c]; ind_fila[nnz]=f; nnz++; ptr_col[c+1]++ */
    void inicializar(int m[N][N]) {
        nnz = 0;
        ptr_col[0] = 0;
        for (int c = 0; c < N; c++) {
            ptr_col[c + 1] = ptr_col[c];
            for (int f = 0; f < N; f++) {
                if (m[f][c] != 0) {
                    valores[nnz]  = m[f][c];
                    ind_fila[nnz] = f;
                    nnz++;
                    ptr_col[c + 1]++;
                }
            }
        }
    }

    /* insertarOrdenado(v, f, c):
     *   si nnz >= MAX_NNZ o (f,c) ya existe -> return false
     *   pos = ptr_col[c+1]
     *   para k de ptr_col[c] a ptr_col[c+1]-1: si ind_fila[k]>f -> pos=k; break
     *   desplazar derecha desde pos
     *   valores[pos]=v; ind_fila[pos]=f; nnz++
     *   para i de c+1 a N: ptr_col[i]++ */
    bool insertarOrdenado(int v, int f, int c) {
        if (nnz >= MAX_NNZ || buscarIndice(f, c) != -1) return false;
        int pos = ptr_col[c + 1];
        for (int k = ptr_col[c]; k < ptr_col[c + 1]; k++) {
            if (ind_fila[k] > f) { pos = k; break; }
        }
        for (int k = nnz; k > pos; k--) {
            valores[k]  = valores[k - 1];
            ind_fila[k] = ind_fila[k - 1];
        }
        valores[pos]  = v;
        ind_fila[pos] = f;
        nnz++;
        for (int j = c + 1; j <= N; j++) ptr_col[j]++;
        return true;
    }

    /* eliminarPorPosicion(f, c):
     *   k = buscarIndice(f, c)
     *   si k == -1 -> return false
     *   eliminarIndice(k) */
    bool eliminarPorPosicion(int f, int c) {
        int k = buscarIndice(f, c);
        if (k == -1) return false;
        eliminarIndice(k);
        return true;
    }

    /* eliminarPorValor(v):
     *   k = buscarPorValor(v)
     *   si k == -1 -> return false
     *   eliminarIndice(k) */
    bool eliminarPorValor(int v) {
        int k = buscarPorValor(v);
        if (k == -1) return false;
        eliminarIndice(k);
        return true;
    }

    /* intercambiarPorPosicion(f1,c1, f2,c2):
     *   k1=buscarIndice(f1,c1); k2=buscarIndice(f2,c2)
     *   si alguno == -1 -> return false
     *   tmp=valores[k1]; valores[k1]=valores[k2]; valores[k2]=tmp */
    bool intercambiarPorPosicion(int f1, int c1, int f2, int c2) {
        int k1 = buscarIndice(f1, c1);
        int k2 = buscarIndice(f2, c2);
        if (k1 == -1 || k2 == -1) return false;
        int tmp = valores[k1]; valores[k1] = valores[k2]; valores[k2] = tmp;
        return true;
    }

    /* sustituirValor(f, c, nv):
     *   k = buscarIndice(f, c)
     *   si k == -1 -> return false
     *   valores[k] = nv */
    bool sustituirValor(int f, int c, int nv) {
        int k = buscarIndice(f, c);
        if (k == -1) return false;
        valores[k] = nv;
        return true;
    }

    /* sumar(B) -> R:
     *   para c de 0 a N-1
     *     ia=ptr_col[c], ib=B.ptr_col[c]
     *     mientras ia < ptr_col[c+1] o ib < B.ptr_col[c+1]
     *       si fila_a < fila_b -> R.insertar(valores[ia], fila_a, c); ia++
     *       si fila_b < fila_a -> R.insertar(B.valores[ib], fila_b, c); ib++
     *       sino suma = valores[ia]+B.valores[ib]; si != 0 -> R.insertar(suma,...); ia++; ib++ */
    MatrizCSC sumar(const MatrizCSC& B) const {
        MatrizCSC R;
        for (int c = 0; c < N; c++) {
            int ia = ptr_col[c],   ib = B.ptr_col[c];
            int ea = ptr_col[c+1], eb = B.ptr_col[c+1];
            while (ia < ea || ib < eb) {
                int fa = (ia < ea) ? ind_fila[ia] : N;
                int fb = (ib < eb) ? B.ind_fila[ib] : N;
                if (fa < fb)      { R.insertarOrdenado(valores[ia],   fa, c); ia++; }
                else if (fb < fa) { R.insertarOrdenado(B.valores[ib], fb, c); ib++; }
                else {
                    int s = valores[ia] + B.valores[ib];
                    if (s != 0) R.insertarOrdenado(s, fa, c);
                    ia++; ib++;
                }
            }
        }
        return R;
    }

    /* restar(B) -> R:
     *   igual que sumar pero con diff = valores[ia] - B.valores[ib]
     *   y para solo-B: insertar -B.valores[ib] */
    MatrizCSC restar(const MatrizCSC& B) const {
        MatrizCSC R;
        for (int c = 0; c < N; c++) {
            int ia = ptr_col[c],   ib = B.ptr_col[c];
            int ea = ptr_col[c+1], eb = B.ptr_col[c+1];
            while (ia < ea || ib < eb) {
                int fa = (ia < ea) ? ind_fila[ia] : N;
                int fb = (ib < eb) ? B.ind_fila[ib] : N;
                if (fa < fb)      { R.insertarOrdenado(valores[ia],    fa, c); ia++; }
                else if (fb < fa) { R.insertarOrdenado(-B.valores[ib], fb, c); ib++; }
                else {
                    int d = valores[ia] - B.valores[ib];
                    if (d != 0) R.insertarOrdenado(d, fa, c);
                    ia++; ib++;
                }
            }
        }
        return R;
    }

    void imprimir() const {
        cout << "valores  : [";
        for (int i = 0; i < nnz; i++) cout << (i ? ", " : "") << valores[i];
        cout << "]\n";
        cout << "ind_fila : [";
        for (int i = 0; i < nnz; i++) cout << (i ? ", " : "") << ind_fila[i];
        cout << "]\n";
        cout << "ptr_col  : [";
        for (int i = 0; i <= N; i++) cout << (i ? ", " : "") << ptr_col[i];
        cout << "]\n";
    }

    void imprimirMatriz() const {
        for (int f = 0; f < N; f++) {
            for (int c = 0; c < N; c++) {
                int k = buscarIndice(f, c);
                cout << (c ? "  " : "") << (k != -1 ? valores[k] : 0);
            }
            cout << "\n";
        }
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

    MatrizCSC A;
    A.inicializar(matrix);

    cout << "=== Matriz A ===\n";
    A.imprimir();
    cout << "Tabla:\n";
    A.imprimirMatriz();

    A.eliminarPorPosicion(0, 3);
    cout << "\neliminarPorPosicion(0,3):\n";
    A.imprimir();

    A.eliminarPorValor(8);
    cout << "\neliminarPorValor(8):\n";
    A.imprimir();

    A.intercambiarPorPosicion(0, 2, 3, 0);
    cout << "\nintercambiarPorPosicion(0,2, 3,0):\n";
    A.imprimir();

    A.sustituirValor(3, 0, 99);
    cout << "\nsustituirValor(3,0, 99):\n";
    A.imprimir();

    cout << "\n=== Suma y Resta CSC ===\n";
    int matrix2[N][N] = {
        {0,0,3,0,0},
        {0,0,0,2,0},
        {0,0,0,0,0},
        {4,0,0,0,0},
        {0,0,0,0,0}
    };

    MatrizCSC MA, MB;
    MA.inicializar(matrix);
    MB.inicializar(matrix2);

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
