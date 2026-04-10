// Matriz dispersa 5x5 - formato CSR (Compressed Sparse Row)

#include <iostream>
using namespace std;

class MatrizCSR {
private:
    int valores[25];
    int ind_col[25];
    int ptr_fila[6];
    int nnz;

    /* buscarIndice(f, c):
     *   para k de ptr_fila[f] a ptr_fila[f+1]-1
     *     si ind_col[k] == c -> return k
     *   return -1 */
    int buscarIndice(int f, int c) const {
        if (f < 0 || f >= 5) return -1;
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
     *   para f de 0 a 4
     *     si ptr_fila[f] <= k < ptr_fila[f+1] -> return f
     *   return -1 */
    int filaDeIndice(int k) const {
        for (int f = 0; f < 5; f++)
            if (k >= ptr_fila[f] && k < ptr_fila[f + 1]) return f;
        return -1;
    }

    /* eliminarIndice(k):
     *   fila = filaDeIndice(k)
     *   para i de k a nnz-2: valores[i]=valores[i+1]; ind_col[i]=ind_col[i+1]
     *   nnz--
     *   para i de fila+1 a 5: ptr_fila[i]-- */
    void eliminarIndice(int k) {
        int fila = filaDeIndice(k);
        for (int i = k; i < nnz - 1; i++) {
            valores[i] = valores[i + 1];
            ind_col[i] = ind_col[i + 1];
        }
        nnz--;
        for (int i = fila + 1; i <= 5; i++) ptr_fila[i]--;
    }

public:
    MatrizCSR() : nnz(0) {
        for (int i = 0; i <= 5; i++) ptr_fila[i] = 0;
    }

    /* inicializar(m):
     *   ptr_fila[0] = 0
     *   para f de 0 a 4
     *     ptr_fila[f+1] = ptr_fila[f]
     *     para c de 0 a 4
     *       si m[f][c] != 0
     *         valores[nnz]=m[f][c]; ind_col[nnz]=c; nnz++; ptr_fila[f+1]++ */
    void inicializar(int m[5][5]) {
        nnz = 0;
        ptr_fila[0] = 0;
        for (int f = 0; f < 5; f++) {
            ptr_fila[f + 1] = ptr_fila[f];
            for (int c = 0; c < 5; c++) {
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
     *   si nnz >= 25 o (f,c) ya existe -> return false
     *   pos = ptr_fila[f+1]
     *   para k de ptr_fila[f] a ptr_fila[f+1]-1: si ind_col[k]>c -> pos=k; break
     *   desplazar derecha desde pos
     *   valores[pos]=v; ind_col[pos]=c; nnz++
     *   para i de f+1 a 5: ptr_fila[i]++ */
    bool insertarOrdenado(int v, int f, int c) {
        if (nnz >= 25 || buscarIndice(f, c) != -1) return false;
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
        for (int i = f + 1; i <= 5; i++) ptr_fila[i]++;
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

    /* sustituirPorPosicion(f, c, nv):
     *   k = buscarIndice(f, c)
     *   si k == -1 -> return false
     *   valores[k] = nv */
    bool sustituirPorPosicion(int f, int c, int nv) {
        int k = buscarIndice(f, c);
        if (k == -1) return false;
        valores[k] = nv;
        return true;
    }

    /* sumar(B) -> R:
     *   para f de 0 a 4
     *     ia=ptr_fila[f], ib=B.ptr_fila[f]
     *     mientras ia < ptr_fila[f+1] o ib < B.ptr_fila[f+1]
     *       si col_a < col_b -> R.insertar(valores[ia], f, col_a); ia++
     *       si col_b < col_a -> R.insertar(B.valores[ib], f, col_b); ib++
     *       sino suma = valores[ia]+B.valores[ib]; si != 0 -> R.insertar(suma,...); ia++; ib++ */
    MatrizCSR sumar(const MatrizCSR& B) const {
        MatrizCSR R;
        for (int f = 0; f < 5; f++) {
            int ia = ptr_fila[f],   ib = B.ptr_fila[f];
            int ea = ptr_fila[f+1], eb = B.ptr_fila[f+1];
            while (ia < ea || ib < eb) {
                int ca = (ia < ea) ? ind_col[ia] : 5;
                int cb = (ib < eb) ? B.ind_col[ib] : 5;
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
        for (int f = 0; f < 5; f++) {
            int ia = ptr_fila[f],   ib = B.ptr_fila[f];
            int ea = ptr_fila[f+1], eb = B.ptr_fila[f+1];
            while (ia < ea || ib < eb) {
                int ca = (ia < ea) ? ind_col[ia] : 5;
                int cb = (ib < eb) ? B.ind_col[ib] : 5;
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
        for (int i = 0; i <= 5; i++) cout << (i ? ", " : "") << ptr_fila[i];
        cout << "]\n";
    }

    void imprimirMatriz() const {
        for (int f = 0; f < 5; f++) {
            for (int c = 0; c < 5; c++) {
                int k = buscarIndice(f, c);
                cout << (c ? "  " : "") << (k != -1 ? valores[k] : 0);
            }
            cout << "\n";
        }
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

    A.sustituirPorPosicion(0, 2, 99);
    cout << "\nsustituirPorPosicion(0,2, 99):\n";
    A.imprimir();

    cout << "\n=== Suma y Resta CSR ===\n";
    int matrix2[5][5] = {
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
