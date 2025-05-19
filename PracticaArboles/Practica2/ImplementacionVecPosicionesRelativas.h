#ifndef ABIN_VEC1_H
#define ABIN_VEC1_H
#include <cassert>
#include <cstddef> // size_t
#include <cstdint> // SIZE_MAX
#include <utility> // swap

template <typename T> class Abin {
    public:
        typedef size_t nodo;
        static const nodo NODO_NULO;

        explicit Abin(size_t maxNodos = 0, const T& e_nulo = T());

        void insertarRaiz(const T& e);
        void insertarHijoIzqdo(nodo n, const T& e);
        void insertarHijoDrcho(nodo n, const T& e);
        void eliminarHijoIzqdo(nodo n);
        void eliminarHijoDrcho(nodo n);
        void eliminarRaiz();

        bool vacio() const;
        size_t tama() const;
        size_t tamaMax() const; // Requerido por la implementación

        const T& elemento(nodo n) const; // Lec. en Abin const
        T& elemento(nodo n); // Lec/Esc. en Abin no-const

        nodo raiz() const;
        nodo padre(nodo n) const;
        nodo hijoIzqdo(nodo n) const;
        nodo hijoDrcho(nodo n) const;

        Abin(const Abin& A); // Ctor. de copia
        Abin& operator =(const Abin& A); // Asignación de árboles
        ~Abin(); // Destructor

    private:
        T* nodos; // Vector de elementos
        size_t maxNodos, // Tamaño del vector
        numNodos; // Tamaño del árbol
        T ELTO_NULO; // Marca celdas libres

        bool valido(nodo n) const;

}; 

// Método privado
template <typename T>
inline bool Abin<T>::valido(nodo n) const { 
    // Comprobar si n es un nodo del árbol.
    return n < maxNodos && // Celda del vector
    !(nodos[n] == ELTO_NULO); // ocupada.
}

// Definición del nodo nulo
template <typename T>
const typename Abin<T>::nodo Abin<T>::NODO_NULO{SIZE_MAX};

template <typename T>
Abin<T>::Abin(size_t maxNodos, const T& e_nulo) : nodos{new T[maxNodos]}, maxNodos{maxNodos}, numNodos{0}, ELTO_NULO{e_nulo} {
    // Marcar libres todas las celdas.
    for (nodo n = 0; n < maxNodos; ++n)
        nodos[n] = ELTO_NULO;
}

template <typename T>
inline void Abin<T>::insertarRaiz(const T& e) {
    assert(maxNodos > 0);
    assert(vacio());
    nodos[0] = e;
    numNodos = 1;
}

template <typename T>
inline void Abin<T>::insertarHijoIzqdo(nodo n, const T& e) {
    assert(valido(n));
    assert(2*n+1 < maxNodos); // Hijo izqdo. cabe en el árbol.
    assert(nodos[2*n+1] == ELTO_NULO); // n no tiene hijo izqdo.

    nodos[2*n+1] = e;
    ++numNodos;
}

template <typename T>
inline void Abin<T>::insertarHijoDrcho(nodo n, const T& e) {
    assert(valido(n));
    assert(2*n+2 < maxNodos); // Hijo drcho. cabe en el árbol.
    assert(nodos[2*n+2] == ELTO_NULO); // n no tiene hijo drcho.

    nodos[2*n+2] = e;
    ++numNodos;
}

template <typename T>
inline void Abin<T>::eliminarHijoIzqdo(nodo n) {
    assert(valido(n));
    assert(valido(2*n+1));
    assert(!valido(4*n+3) && !valido(4*n+4));

    nodos[2*n+1] = ELTO_NULO;
    --numNodos;
}

template <typename T>
inline void Abin<T>::eliminarHijoDrcho(nodo n) {
    assert(valido(n));
    assert(valido(2*n+2));
    assert(!valido(4*n+5) && !valido(4*n+6));
    
    nodos[2*n+2] = ELTO_NULO;
    --numNodos;
}

template <typename T>
inline void Abin<T>::eliminarRaiz() {
    assert(numNodos == 1);
    numNodos = 0;
}

template <typename T>
inline bool Abin<T>::vacio() const { return numNodos == 0; }

template <typename T>
inline size_t Abin<T>::tama() const { return numNodos; }

template <typename T>
inline size_t Abin<T>::tamaMax() const { return maxNodos; }

template <typename T>
inline const T& Abin<T>::elemento(nodo n) const {
    assert(valido(n));
    return nodos[n];
}

template <typename T>
inline T& Abin<T>::elemento(nodo n) {
    assert(valido(n));
    return nodos[n];

}
template <typename T>
inline typename Abin<T>::nodo Abin<T>::raiz() const { return vacio() ? NODO_NULO : 0; }

template <typename T>
inline typename Abin<T>::nodo Abin<T>::padre(nodo n) const {
    assert(valido(n));
    return n ? (n-1)/2 : NODO_NULO;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoIzqdo(nodo n) const {
    assert(valido(n));
    return valido(2*n+1) ? 2*n+1 : NODO_NULO;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoDrcho(nodo n) const {
    assert(valido(n));
    return valido(2*n+2) ? 2*n+2 : NODO_NULO;
}

template <typename T>
Abin<T>::Abin(const Abin& A) : Abin{A.maxNodos, A.ELTO_NULO} {
    if (!A.vacio()) {
        for (nodo n = 0; n < maxNodos; ++n)
            nodos[n] = A.nodos[n];
        numNodos = A.numNodos;
    }
}

template <typename T>
inline Abin<T>& Abin<T>::operator =(const Abin& A) {
    Abin B{A};
    std::swap(nodos, B.nodos);
    std::swap(maxNodos, B.maxNodos);
    std::swap(numNodos, B.numNodos);
    std::swap(ELTO_NULO, B.ELTO_NULO);
    return *this;
}

template <typename T>
inline Abin<T>::~Abin() { delete[] nodos; }
#endif // ABIN_VEC1_H
