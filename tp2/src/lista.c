#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

nodo_t *nodo_crear(void *elemento, nodo_t *siguiente)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if(!nuevo_nodo)
		return NULL;
	
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = siguiente;

	return nuevo_nodo;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if(!lista)
		return NULL;

	nodo_t *nuevo_nodo = nodo_crear(elemento, NULL);
	if(!nuevo_nodo)
		return NULL;

	if(lista_vacia(lista))
		lista->nodo_inicio = nuevo_nodo;
	else
		lista->nodo_fin->siguiente = nuevo_nodo;
	
	lista->nodo_fin = nuevo_nodo;
	lista->cantidad++;
	return lista;
}

nodo_t *insertar_nodo_en_posicion_recursivamente(nodo_t *nodo, void *elemento, 
					size_t posicion)
{
	if(!nodo || posicion == 0) {
		nodo_t *nuevo_nodo = nodo_crear(elemento, nodo);
		if(!nuevo_nodo)
			return NULL;
		
		return nuevo_nodo;
	}

	nodo->siguiente = insertar_nodo_en_posicion_recursivamente(nodo->siguiente, 
		elemento, posicion - 1);

	return nodo;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if(!lista)
		return NULL;

	if(posicion >= lista_tamanio(lista))
		return lista_insertar(lista, elemento);

	lista->nodo_inicio = insertar_nodo_en_posicion_recursivamente(lista->nodo_inicio, 
		elemento, posicion); 

	lista->cantidad++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if(!lista || lista_vacia(lista))
		return NULL;

	void *elemento_a_quitar = lista->nodo_fin->elemento;
	nodo_t *nodo_a_quitar = lista->nodo_fin;

	nodo_t *nodo_actual = lista->nodo_inicio; 
	if(!(nodo_actual->siguiente)){
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		lista->cantidad--;
		free(nodo_a_quitar);
	} else {
		while(nodo_actual->siguiente != nodo_a_quitar) 
			nodo_actual = nodo_actual->siguiente;

		nodo_actual->siguiente = NULL;
		lista->nodo_fin = nodo_actual;
		lista->cantidad--;
		free(nodo_a_quitar);
	}

	return elemento_a_quitar;
}

void *lista_quitar_de_posicion_recursivamente(lista_t *lista, 
			nodo_t *nodo_anterior, nodo_t *nodo_actual, size_t posicion)
{
	void *candidato_a_quitar = nodo_actual->elemento;

	if(!nodo_anterior && posicion == 0) {
		lista->nodo_inicio = nodo_actual->siguiente;
		lista->cantidad--;

		free(nodo_actual);
		return candidato_a_quitar;
	}
	if(!nodo_actual->siguiente) {
		if(!nodo_anterior) {
			lista->nodo_inicio = NULL;
			lista->nodo_fin = NULL;
		}
		else {
			nodo_anterior->siguiente = NULL;
			lista->nodo_fin = nodo_anterior;
		}
		lista->cantidad--;

		free(nodo_actual);
		return candidato_a_quitar;
	}
	if(posicion == 0) {
		nodo_anterior->siguiente = nodo_actual->siguiente;
		lista->cantidad--;

		free(nodo_actual);
		return candidato_a_quitar;
	}

	return lista_quitar_de_posicion_recursivamente(lista, nodo_actual, 
		nodo_actual->siguiente, posicion - 1);
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if(!lista || lista_vacia(lista))
		return NULL;

	return lista_quitar_de_posicion_recursivamente(lista, 
						NULL, lista->nodo_inicio, posicion);
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if(!lista || lista_vacia(lista))
		return NULL;

	int contador = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while(nodo_actual && contador < posicion) {
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}

	if(!nodo_actual)
		return NULL;

	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if(!lista || !comparador)
		return NULL;

	bool encontrado = false;
	nodo_t *nodo_actual = lista->nodo_inicio;
	while(!encontrado && nodo_actual) {
		if(comparador(nodo_actual->elemento, contexto) == 0)
			encontrado = true;
		else nodo_actual = nodo_actual->siguiente;
	}

	if(encontrado)
		return nodo_actual->elemento;	
	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if(!lista || lista_vacia(lista))
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if(!lista || lista_vacia(lista))
		return NULL;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if(!lista || lista->cantidad == 0)
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if(!lista)
		return 0;

	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if(!lista)
		return;

	nodo_t *nodo_actual = lista->nodo_inicio;
	while(nodo_actual) {
		nodo_t *nodo_a_destruir = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
		if(funcion)
			funcion(nodo_a_destruir->elemento);
		free(nodo_a_destruir);
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if(!lista)
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if(!iterador)
		return NULL;
	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if(!iterador || !(iterador->corriente))
		return false;

	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if(!iterador || !(iterador->corriente))
		return false;

	if(iterador->corriente->siguiente){
		iterador->corriente = iterador->corriente->siguiente;
		return true;
	} else {
		iterador->corriente = NULL;
		return false;
	}

}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if(!iterador || !(iterador->corriente))
		return NULL;

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if(!lista || !funcion)
		return 0;

	size_t contador = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;
	bool sigue_iterando = true;

	while(nodo_actual && sigue_iterando) { 
		if(!funcion(nodo_actual->elemento, contexto))
			sigue_iterando = false;
		contador++;
		nodo_actual = nodo_actual->siguiente;
	}

	return contador;
}
