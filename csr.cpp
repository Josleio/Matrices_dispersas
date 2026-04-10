// Matriz dispersa 5x5 - formato CSR (Compressed Sparse Row)

#include <iostream>
using namespace std;

const int N       = 5;
const int MAX_NNZ = N * N;

class MatrizCSR {
private:
    int valores[MAX_NNZ];
    int ind_col[MAX_NNZ];
    int ptr_fila[N + 1];
    int nnz;

    /* buscarIndice(f, c):
     *   para k de ptr_fila[f] a ptr_fila[f+1]-1
     *     si ind_col[k] == c -> return k
     *   return -1 */
    int buscarIndice(int f, int c) const {
        if (f < 0 || f >= N) return -1;
        for (int k = ptr_fila[f]; k < ptr_fila[f + 1]; k++)
            if (ind_col[k] == c) return k;
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

    /* filaDeIndice(k):
     *   para f de 0 a N-1
     *     si ptr_fila[f] <= k < ptr_fila[f+1] -> return f
     *   return -1 */
    int filaDeIndice(int k) const {
        for (int f = 0; f < N; f++)
            if (k >= ptr_fila[f] && k < ptr_fila[f + 1]) return f;
        return -1;
    }

    /* eliminarIndice(k):
     *   fila = filaDeIndice(k)
     *   para i de k a nnz-2: valores[i]=valores[i+1]; ind_col[i]=ind_col[i+1]
     *   nnz--
     *   para f de fila+1 a N: ptr_fila[f]-- */
    void eliminarIndice(int k) {
        int fila = filaDeIndice(k);
        for (int i = k; i < nnz - 1; i++) {
            valores[i] = valores[i + 1];
            ind_col[i] = ind_col[i + 1];
        }
        nnz--;
        for (int f = fila + 1; f <= N; f++) ptr_fila[f]--;
    }

public:
    MatrizCSR() : nnz(0) {
        for (int i = 0; i <= N; i++) ptr_fila[i] = 0;
    }

    /* inicializar(m):
     *   ptr_fila[0] = 0
     *   para f de 0 a N-1
     *     ptr_fila[f+1] = ptr_fila[f]
     *     para c de 0 a N-1
     *       si m[f][c] != 0
     *         valores[nnz]=m[f][c]; ind_col[nnz]=c; nnz++; ptr_fila[f+1]++ */
    void inicializar(int m[N][N]) {
        nnz = 0;
        ptr_fila[0] = 0;
        for (int f = 0; f < N; f++) {
            ptr_fila[f + 1] = ptr_fila[f];
            for (int c = 0; c < N; c++) {
                if (m[f][c] != 0) {
                    valores[nnz] = m[f][c];
                    ind_col[nnz] = c;
                    nnz++;
                    ptr_fila[f + 1]++;
                }
            }
        }
    }

    /* insertarOrdenado(v, f, c):
     *   si nnz >= MAX_NNZ o (f,c) ya existe -> return false
     *   pos = ptr_fila[f+1]
     *   para k de ptr_fila[f] a ptr_fila[f+1]-1: si ind_col[k]>c -> pos=k; break
     *   desplazar derecha desde pos
     *   valores[pos]=v; ind_col[pos]=c; nnz++
     *   para i de f+1 a N: ptr_fila[i]++ */
    bool insertarOrdenado(int v, int f, int c) {
        if (nnz >= MAX_NNZ || buscarIndice(f, c) != -1) return false;
        int pos = ptr_fila[f + 1];
        for (int k = ptr_fila[f]; k < ptr_fila[f + 1]; k++) {
            if (ind_col[k] > c) { pos = k; break; }
        }
        for (int k = nnz; k > pos; k--) {
            valores[k] = valores[k - 1];
            ind_col[k] = ind_col[k - 1];
        }
        valores[pos] = v;
        ind_col[pos] = c;
        nnz++;
        for (int i = f + 1; i <= N; i++) ptr_fila[i]++;
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
     *   para f de 0 a N-1
     *     ia=ptr_fila[f], ib=B.ptr_fila[f]
     *     mientras ia < ptr_fila[f+1] o ib < B.ptr_fila[f+1]
     *       si col_a < col_b -> R.insertar(valores[ia], f, col_a); ia++
     *       si col_b < col_a -> R.insertar(B.valores[ib], f, col_b); ib++
     *       sino suma = valores[ia]+B.valores[ib]; si != 0 -> R.insertar(suma,...); ia++; ib++ */
    MatrizCSR sumar(const MatrizCSR& B) const {
        MatrizCSR R;
        for (int f = 0; f < N; f++) {
            int ia = ptr_fila[f],   ib = B.ptr_fila[f];
            int ea = ptr_fila[f+1], eb = B.ptr_fila[f+1];
            while (ia < ea || ib < eb) {
                int ca = (ia < ea) ? ind_col[ia] : N;
                int cb = (ib < eb) ? B.ind_col[ib] : N;
                if (ca < cb)      { R.insertarOrdenado(valores[ia],   f, ca); ia++; }
                else if (cb < ca) { R.insertarOrdenado(B.valores[ib], f, cb); ib++; }
                else {
                    int s = valores[ia] + B.valores[ib];
                    if (s != 0) R.insertarOrdenado(s, f, ca);
                    ia++; ib++;
                }
            }
        }
        return R;
    }

    /* restar(B) -> R:
     *   igual que sumar pero con diff = valores[ia] - B.valores[ib]
     *   y para solo-B: insertar -B.valores[ib] */
    MatrizCSR restar(const MatrizCSR& B) const {
        MatrizCSR R;
        for (int f = 0; f < N; f++) {
            int ia = ptr_fila[f],   ib = B.ptr_fila[f];
            int ea = ptr_fila[f+1], eb = B.ptr_fila[f+1];
            while (ia < ea || ib < eb) {
                int ca = (ia < ea) ? ind_col[ia] : N;
                int cb = (ib < eb) ? B.ind_col[ib] : N;
                if (ca < cb)      { R.insertarOrdenado(valores[ia],    f, ca); ia++; }
                else if (cb < ca) { R.insertarOrdenado(-B.valores[ib], f, cb); ib++; }
                else {
                    int d = valores[ia] - B.valores[ib];
                    if (d != 0) R.insertarOrdenado(d, f, ca);
                    ia++; ib++;
                }
            }
        }
        return R;
    }

    void imprimir() const {
        cout << "valores   : [";
        for (int i = 0; i < nnz; i++) cout << (i ? ", " : "") << valores[i];
        cout << "]\n";
        cout << "ind_col   : [";
        for (int i = 0; i < nnz; i++) cout << (i ? ", " : "") << ind_col[i];
        cout << "]\n";
        cout << "ptr_fila  : [";
        for (int i = 0; i <= N; i++) cout << (i ? ", " : "") << ptr_fila[i];
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

    MatrizCSR A;
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

    cout << "\n=== Suma y Resta CSR ===\n";
    int matrix2[N][N] = {
        {0,0,3,0,0},
        {0,0,0,2,0},
        {0,0,0,0,0},
        {4,0,0,0,0},
        {0,0,0,0,0}
    };

    MatrizCSR MA, MB;
    MA.inicializar(matrix);
    MB.inicializar(matrix2);

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
