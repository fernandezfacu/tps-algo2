#include "pila.h"
#include "lista.h"

#define TOPE 0

pila_t *pila_crear()
{
	pila_t *pila = (pila_t *)lista_crear();

	return pila;
}

pila_t *pila_apilar(pila_t *pila, void *elemento)
{
	lista_t *lista = (lista_t *) pila;

	return (pila_t *) lista_insertar_en_posicion(lista, elemento, TOPE);
}

void *pila_desapilar(pila_t *pila)
{
	lista_t *lista = (lista_t *) pila;

	return lista_quitar_de_posicion(lista, TOPE);
}

void *pila_tope(pila_t *pila)
{
	lista_t *lista = (lista_t *) pila;

	return lista_primero(lista);
}

size_t pila_tamanio(pila_t *pila)
{
	lista_t *lista = (lista_t *) pila;

	return lista_tamanio(lista);
}

bool pila_vacia(pila_t *pila)
{
	lista_t *lista = (lista_t *) pila;
		
	return lista_vacia(lista);
}

void pila_destruir(pila_t *pila)
{
	lista_t *lista = (lista_t *) pila;

	lista_destruir(lista);
}
