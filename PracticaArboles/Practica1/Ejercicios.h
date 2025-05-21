#include "ImplementacionEnlazada.h"
#include "ImplementacionVecPosicionesRelativas.h"
#include "ImplementacionVectorial.h"

//Ejercicio1
template <typename T>
int numNodos(const Abin<T>& A);
template <typename T>
int numNodos_rec (const Abin<T>& A, typename Abin<T>::nodo n);

//Ejercicio2
template <typename T>
int altura(const Abin<T>& A);
template <typename T>
int altura_rec(const Abin<T>& A, typename Abin<T>::nodo n);

//Ejercicio7
template <typename T>
bool esPseudoCompleto(Abin<T> A);
template <typename T>
bool esPseudoCompleto_rec(typename <T>::nodo n, Abin<T> A, int altura);