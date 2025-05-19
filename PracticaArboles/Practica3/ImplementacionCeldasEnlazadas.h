#ifndef AGEN_H
#define AGEN_H

#include <cassert>
#include <cstddef> // size_t
#include <utility> // swap

template <typename T> class Agen {
    struct celda; // Declaración adelantada privada
    public:
        typedef celda* nodo;
        static const nodo NODO_NULO;

        Agen();

        void insertarRaiz(const T& e);
        void insertarHijoIzqdo(nodo n, const T& e);
        void insertarHermDrcho(nodo n, const T& e);
        void eliminarHijoIzqdo(nodo n);
        void eliminarHermDrcho(nodo n);
        void eliminarRaiz();

        bool vacio() const;
        size_t tama() const;
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
            nodo padre, hizq, heder;
        };

        nodo r; // Raíz del árbol
        size_t numNodos; // Tamaño del árbol

        nodo copiar(nodo n);
        void destruir(nodo& n);
}; // class Agen

/* Definición del nodo nulo */
template <typename T>
const typename Agen<T>::nodo Agen<T>::NODO_NULO{nullptr};

/* Métodos privados */
// Destruye un nodo y todos sus descendientes
template <typename T>
void Agen<T>::destruir(nodo& n) {
    nodo hedrcho;
    if (n != NODO_NULO) {
        while (n->hizq != NODO_NULO){ 
            // Destruir hijos
            hedrcho = n->hizq->heder;
            destruir(n->hizq);
            n->hizq = hedrcho;
        }

        delete(n);
        n = NODO_NULO;
    }
}

// Devuelve una copia de un nodo y todos sus descendientes
template <typename T>
typename Agen<T>::nodo Agen<T>::copiar(nodo n) {
    nodo m = NODO_NULO;
    if (n != NODO_NULO) {
        Agen A; // Contiene los nodos copiados.

        // Si la copia no se completa, A es destruido.
        A.r = new celda{n->elto}; // Copiar n en raíz.

        if (n->hizq != NODO_NULO) { // n tiene descendientes.
            A.r->hizq = copiar(n->hizq); // Copiar primer subárbol.
            A.r->hizq->padre = A.r;

            // Copiar el resto de subárboles.
            nodo hijo = A.r->hizq; // Último subárbol copiado.
            nodo hedrcho = n->hizq->heder; // Siguiente subárbol a copiar

            while (hedrcho != NODO_NULO) {
                hijo = hijo->heder = copiar(hedrcho);
                hijo->padre = A.r;
                hedrcho = hedrcho->heder;
            }
        }

        m = A.r; A.r = NODO_NULO; // Evita destruir la copia.
    }

    return m;
}

/* Métodos públicos */

template <typename T>
inline Agen<T>::Agen() : r{NODO_NULO}, numNodos{0} {}

template <typename T>
inline void Agen<T>::insertarRaiz(const T& e) {
    assert(vacio());
    r = new celda{e};
    numNodos = 1;
}

template <typename T>
inline void Agen<T>::insertarHijoIzqdo(nodo n, const T& e) {
    assert(n != NODO_NULO);

    nodo hizqdo = n->hizq; // Hijo izqdo. actual.
    n->hizq = new celda{e, n};
    n->hizq->heder = hizqdo; // El actual hijo izqdo. se convierte en hermano drcho. del nuevo hijo.
    ++numNodos;
}

template <typename T>
inline void Agen<T>::insertarHermDrcho(nodo n, const T& e) {
    assert(n != NODO_NULO);
    assert(n != r); // n no es la raíz.

    nodo hedrcho = n->heder; // Hermano drcho. actual
    n->heder = new celda{e, n->padre};
    n->heder->heder = hedrcho; // El actual hermano drcho. se convierte en hermano drcho. del nuevo.
    ++numNodos;
}

template <typename T>
inline void Agen<T>::eliminarHijoIzqdo(nodo n) {
    nodo hizqdo;
    assert(n != NODO_NULO);
    hizqdo = n->hizq;

    assert(hizqdo != NODO_NULO); // Existe hijo izqdo.
    assert(hizqdo->hizq == NODO_NULO); // y es hoja.

     // El hermano drcho. pasa a ser el nuevo hijo izqdo.
    n->hizq = hizqdo->heder;
    delete hizqdo;
    --numNodos;
}

template <typename T>
inline void Agen<T>::eliminarHermDrcho(nodo n) {
    nodo hedrcho;
    assert(n != NODO_NULO);
    hedrcho = n->heder;

    assert(hedrcho != NODO_NULO); // Existe hermano drcho.
    assert(hedrcho->hizq == NODO_NULO); // y es hoja.

    // El hermano del hermano se convierte en el nuevo hermano drcho. de n.
    n->heder = hedrcho->heder;
    delete hedrcho;
    --numNodos;
}

template <typename T>
inline void Agen<T>::eliminarRaiz() {
    assert(numNodos == 1);
    delete(r);
    r = NODO_NULO;
    numNodos = 0;
}

template <typename T> 
inline bool Agen<T>::vacio() const { return numNodos == 0; }

template <typename T>
inline size_t Agen<T>::tama() const { return numNodos; }

template <typename T>
inline const T& Agen<T>::elemento(nodo n) const {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T>
inline T& Agen<T>::elemento(nodo n) {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::raiz() const { return r; }

template <typename T> inline
typename Agen<T>::nodo Agen<T>::padre(nodo n) const {
    assert(n != NODO_NULO);
    return n->padre;
}

template <typename T> inline
typename Agen<T>::nodo Agen<T>::hijoIzqdo(nodo n) const {
    assert(n != NODO_NULO);
    return n->hizq;
}

template <typename T> inline
typename Agen<T>::nodo Agen<T>::hermDrcho(nodo n) const {
    assert(n != NODO_NULO);
    return n->heder;
}

template <typename T>
inline Agen<T>::Agen(const Agen& A) : Agen{} {
    r = copiar(A.r); // Copiar raíz y descendientes.
    numNodos = A.numNodos;
}

template <typename T>
inline Agen<T>& Agen<T>::operator =(const Agen& A) {
    Agen B{A};
    std::swap(r, B.r);
    std::swap(numNodos, B.numNodos);
    return *this;
}

template <typename T>
inline Agen<T>::~Agen() {
    destruir(r); // Vaciar el árbol.
}



#endif // AGEN_H
