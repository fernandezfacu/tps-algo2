#include "cola.h"
#include "lista.h"

#define FRENTE 0

cola_t *cola_crear()
{
	cola_t *cola = (cola_t *) lista_crear();

	return cola;
}

cola_t *cola_encolar(cola_t *cola, void *elemento)
{
	lista_t *lista = (lista_t *) cola;

	return (cola_t *) lista_insertar(lista, elemento);
}

void *cola_desencolar(cola_t *cola)
{
	lista_t *lista = (lista_t *) cola;

	return lista_quitar_de_posicion(lista, FRENTE);
}

void *cola_frente(cola_t *cola)
{
	lista_t *lista = (lista_t *) cola;

	return lista_primero(lista);
}

size_t cola_tamanio(cola_t *cola)
{
	lista_t *lista = (lista_t *) cola;

	return lista_tamanio(lista);
}

bool cola_vacia(cola_t *cola)
{
	lista_t *lista = (lista_t *) cola;

	return lista_vacia(lista);
}

void cola_destruir(cola_t *cola)
{
	lista_t *lista = (lista_t *) cola;

	lista_destruir(lista);
}
