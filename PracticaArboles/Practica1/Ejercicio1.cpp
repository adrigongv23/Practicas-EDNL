//Implemente un subprograma que calcule el número de nodos de un árbol binario

#include "Ejercicios.h"

template <typename T>
int numNodos(const Abin<T>& A) {
    if(A.vacio())
        return 0;
    else 
        return numNodos_rec(A, A.raiz());
}

template <typename T>
int numNodos_rec(const Abin<T>& A, typename Abin<T>::nodo n){
    if(n == Abin<T>::NODO_NULO)
        return 0;
    else    
        return 1 + numNodos_rec(A, A.hijoDrcho(n)) + numNodos_rec(A, A.hijoIzqdo(n));
}


