#ifndef AGEN_LIS_H
#define AGEN_LIS_H

#include <cassert>
#include <cstddef> // size_t
#include <cstdint> // SIZE_MAX
#include <utility> // swap

#include "listaenla.h"

template <typename T> class Agen {
    public:
        typedef size_t nodo;
        static const nodo NODO_NULO;

        explicit Agen(size_t maxNodos = 0);
 
        void insertarRaiz(const T& e);
        void insertarHijoIzqdo(nodo n, const T& e);
        void insertarHermDrcho(nodo n, const T& e);
        void eliminarHijoIzqdo(nodo n);
        void eliminarHermDrcho(nodo n);
        void eliminarRaiz();

        bool vacio() const;
        size_t tama() const;
        size_t tamaMax() const; // Requerido por la implementación

        const T& elemento(nodo n) const; // Lectura en Agen const
        T& elemento(nodo n); // Lectura/escritura en Agen no-const

        nodo raiz() const;
        nodo padre(nodo n) const;
        nodo hijoIzqdo(nodo n) const;
        nodo hermDrcho(nodo n) const;

        Agen(const Agen& A); // Ctor. de copia
        Agen& operator =(const Agen& A); // Asignación de árboles
        ~Agen(); // Destructor

    private:
        struct celda {
            T elto;
            nodo padre;
            Lista<nodo> hijos;
        };

        celda* nodos; // Vector de nodos
        size_t maxNodos, // Tamaño del vector
        numNodos; // Tamaño del árbol
        nodo libre; // Lista de celdas libres

        bool valido(nodo n) const;

}; // class Agen

// Definición del nodo nulo
template <typename T>
const typename Agen<T>::nodo Agen<T>::NODO_NULO{SIZE_MAX};

// Método privado
template <typename T>
inline bool Agen<T>::valido(nodo n) const { // Comprobar si n es un nodo del árbol.
    return !vacio() &&
    n < maxNodos && // Celda del vector
    (n == 0 || nodos[n].padre != NODO_NULO); // ocupada.
}

template <typename T>
Agen<T>::Agen(size_t maxNodos) : nodos{new celda[maxNodos]}, // Listas de hijos vacías.
    maxNodos{maxNodos}, numNodos{0} {
        if (maxNodos > 1) { // Crear la lista de celdas libres
            // enlazadas por el hijo izquierdo.
            libre = 1;

        for (nodo n = 1; n < maxNodos; ++n) {
            Lista<nodo>& Lh = nodos[n].hijos;
            Lh.insertar(n + 1, Lh.primera()); // Añadir nodo n+1
            #ifndef NDEBUG
                // Sólo para comprobar precondiciones.
                nodos[n].padre = NODO_NULO; // Marcar celda libre.
            #endif
        }
    }
}

template <typename T>
inline void Agen<T>::insertarRaiz(const T& e) {
    assert(maxNodos > 0);
    assert(vacio());

    nodos[0] = {e, NODO_NULO}; // Asigna lista de hijos vacía.
    numNodos = 1;
}

template <typename T>
void Agen<T>::insertarHijoIzqdo(nodo n, const T& e) {
    assert(tama() < tamaMax());
    assert(valido(n));

    // Sacar la primera celda de la lista de libres.
    nodo hizqdo = libre;
    libre = nodos[libre].hijos.elemento(nodos[libre].hijos.primera());

    // Añadir al vector el nuevo nodo...
    nodos[hizqdo] = {e, n};

    // e insertarlo al inicio de la lista de hijos de n.
    Lista<nodo>& Lh = nodos[n].hijos; // Lista de hijos.
    Lh.insertar(hizqdo, Lh.primera());
    ++numNodos;
}

template <typename T>
void Agen<T>::insertarHermDrcho(nodo n, const T& e) {
    nodo hedrcho;

    assert(tama() < tamaMax());
    assert(valido(n));
    assert(n != 0); // n no es la raíz.

    // Sacar la primera celda de la lista de libres.
    nodo hedrcho = libre;
    libre = nodos[libre].hijos.elemento(nodos[libre].hijos.primera());
 
    // Añadir al vector el nuevo nodo...
    nodos[hedrcho] = {e, nodos[n].padre};
 
    // e insertarlo en la lista de hijos del padre siguiendo a n.
    Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos; // Lista de hijos del padre.
    Lista<nodo>::posicion p = Lhp.primera();
    while (n != Lhp.elemento(p)) p = Lhp.siguiente(p);
        Lhp.insertar(hedrcho, Lhp.siguiente(p));
    ++numNodos;
}

template <typename T>
void Agen<T>::eliminarHijoIzqdo(nodo n) {
    assert(valido(n));

    Lista<nodo>& Lh = nodos[n].hijos; // Lista de hijos.
    assert(!Lh.vacia()); // n no es hoja.
    hizqdo = Lh.elemento(Lh.primera());
    assert(nodos[hizqdo].hijos.vacia()); // Hijo izqdo. de n es hoja.
 
    // Eliminar primer nodo de la lista de hijos de n.
    Lh.eliminar(lh.primera());
 
    // Añadir hizqdo al inicio de la lista de libres.
    nodos[hizqdo].hijos.insertar(libre, nodos[hizqdo].hijos.primera());
    libre = hizqdo;
    #ifndef NDEBUG
        nodos[hizqdo].padre = NODO_NULO; // Marcar celda libre.
    #endif

    --numNodos;
}

template <typename T>
void Agen<T>::eliminarHermDrcho(nodo n) {
    assert(valido(n));
    assert(n != 0); // n no es la raíz.
 
    // Buscar hermano drcho. de n en la lista de hijos del padre.
    Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos;
    Lista<nodo>::posicion p = Lhp.primera();
    while (n != Lhp.elemento(p)) p = Lhp.siguiente(p);
        p = Lhp.siguiente(p);

    assert(p != Lhp.fin()); // n tiene hermano drcho.
    nodo hdrcho = Lhp.elemento(p);

    assert(nodos[hdrcho].hijos.vacia()); // Herm. drcho de n es hoja.
    
    // Eliminar hermano drcho. de n en la lista de hijos del padre.
    Lhp.eliminar(p);
 
    // Añadir hdrcho al inicio de la lista de libres.
    nodos[hdrcho].hijos.insertar(libre, nodos[hdrcho].hijos.primera());
    libre = hdrcho;
    #ifndef NDEBUG
        nodos[hdrcho].padre = NODO_NULO; // Marcar celda libre.
    #endif
    --numNodos;
}

template <typename T>
inline void Agen<T>::eliminarRaiz() { 
    assert(numNodos == 1);
    numNodos = 0;
}

template <typename T>
inline bool Agen<T>::vacio() const { return numNodos == 0; }

template <typename T>
inline size_t Agen<T>::tama() const { return numNodos; }

template <typename T>
inline size_t Agen<T>::tamaMax() const { return maxNodos; }

template <typename T>
inline const T& Agen<T>::elemento(nodo n) const {
    assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline T& Agen<T>::elemento(nodo n) {
    assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::raiz() const { return vacio() ? NODO_NULO : 0; }

template <typename T>
inline typename Agen<T>::nodo Agen<T>::padre(nodo n) const {
    assert(valido(n));
    return nodos[n].padre;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::hijoIzqdo(nodo n) const {
    assert(valido(n));
    Lista<nodo>& Lh = nodos[n].hijos;
    return Lh.vacia() ? // ¿n es hoja?
    NODO_NULO : Lh.elemento(Lh.primera());
}

template <typename T>
typename Agen<T>::nodo Agen<T>::hermDrcho(nodo n) const {
    assert(valido(n));
    if (n == 0) // n es la raíz.
        return NODO_NULO;
    else {
        // Buscar hermano drcho. de n en la lista de hijos del padre.
        Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos;
        Lista<nodo>::posicion p = Lhp.primera();

        while (n != Lhp.elemento(p)) p = Lhp.siguiente(p);
            p = Lhp.siguiente(p);

        return p == Lhp.fin() ? // ¿n tiene hermano drcho.?
        NODO_NULO : Lhp.elemento(p);
    }
}

template <typename T>
Agen<T>::Agen(const Agen& A) : Agen{A.maxNodos} {
    if (!A.vacio()) {
        for (nodo n = 0; n < maxNodos; ++n)
            nodos[n] = A.nodos[n];
        numNodos = A.numNodos;
        libre = A.libre;
    }
}

template <typename T>
inline Agen<T>& Agen<T>::operator =(const Agen& A) {
    Agen B{A};
    std::swap(nodos, B.nodos);
    std::swap(maxNodos, B.maxNodos);
    std::swap(numNodos, B.numNodos);
    std::swap(libre, B.libre);

    return *this;
}

template <typename T>
inline Agen<T>::~Agen() {
 delete[] nodos; // También destruye las listas de hijos.
}
#endif // AGEN_LIS_H