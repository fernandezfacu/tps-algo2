#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if(!comparador)
		return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));
	if(arbol != NULL)
		arbol->comparador = comparador;

	return arbol;
}

nodo_abb_t *nodo_crear(void *elemento)
{
	nodo_abb_t *nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
	if(!nuevo_nodo)
		return NULL;

	nuevo_nodo->elemento = elemento;
	return nuevo_nodo;
}

nodo_abb_t *abb_insertar_rec(nodo_abb_t *raiz, abb_comparador comparador, 
						nodo_abb_t *nodo_a_insertar)
{
	if(!raiz) 
		return nodo_a_insertar;
	

	if(comparador(nodo_a_insertar->elemento, raiz->elemento) > 0)
		raiz->derecha = abb_insertar_rec(raiz->derecha, comparador, 
						nodo_a_insertar);
	else 
		raiz->izquierda = abb_insertar_rec(raiz->izquierda, comparador, 
						nodo_a_insertar);
	
	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if(!arbol)
		return NULL;

	nodo_abb_t *nuevo_nodo = nodo_crear(elemento);
		if(!nuevo_nodo)
			return NULL;

	arbol->nodo_raiz = abb_insertar_rec(arbol->nodo_raiz, 
						arbol->comparador, nuevo_nodo);
	arbol->tamanio++;

	return arbol;
}

nodo_abb_t *extraer_maximo(nodo_abb_t *raiz, void **predecesor){
	
	if(!raiz->derecha){
		*predecesor = raiz->elemento;
		nodo_abb_t *izquierda = raiz->izquierda;
		free(raiz);
		return izquierda;
	}
	raiz->derecha = extraer_maximo(raiz->derecha, predecesor);
	return raiz;
}

nodo_abb_t *abb_quitar_rec(nodo_abb_t *raiz, abb_comparador comparador, 
				void *elemento, void **elemento_extraido, bool *se_extrajo)
{
	if(!raiz)
		return NULL;
	
	if(comparador(elemento, raiz->elemento) > 0)
		raiz->derecha = abb_quitar_rec(raiz->derecha, comparador, 
						elemento, elemento_extraido, se_extrajo);
	else if(comparador(elemento, raiz->elemento) < 0)
		raiz->izquierda = abb_quitar_rec(raiz->izquierda, comparador, 
						elemento, elemento_extraido, se_extrajo);
	else {
		*elemento_extraido = raiz->elemento;
		*se_extrajo = true;

		if(!raiz->derecha || !raiz->izquierda){
			nodo_abb_t *hijo_no_nulo = raiz->derecha;
			if(!hijo_no_nulo)
				hijo_no_nulo = raiz->izquierda;

			free(raiz);
			return hijo_no_nulo;
		} else {
			void *predecesor = NULL;
			raiz->izquierda = extraer_maximo(raiz->izquierda, &predecesor);
			raiz->elemento = predecesor;
			return raiz;
		}
	}
	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if(!arbol)
		return NULL;

	void *elemento_extraido = NULL;
	bool se_extrajo = false;
	arbol->nodo_raiz = abb_quitar_rec(arbol->nodo_raiz, 
				arbol->comparador, elemento, &elemento_extraido, &se_extrajo);

	if(!se_extrajo)
		return NULL;

	arbol->tamanio--;
	return elemento_extraido;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if(!arbol)
		return NULL;

	nodo_abb_t *nodo = arbol->nodo_raiz;

	while(nodo != NULL) {
		if(arbol->comparador(elemento, nodo->elemento) > 0)
			nodo = nodo->derecha;
		else if(arbol->comparador(elemento, nodo->elemento) < 0)
			nodo = nodo->izquierda;
		else return nodo->elemento;
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	if(!arbol || !(arbol->nodo_raiz))
		return true;

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	return (!arbol)? 0: arbol->tamanio;
}

size_t abb_iterar_inorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *), 
							void *aux, bool *seguir_iterando)
{
	if(!raiz || !(*seguir_iterando))
		return 0;

	size_t contador = 0; 

	contador += abb_iterar_inorden(raiz->izquierda, funcion, aux, seguir_iterando);
	if(!(*seguir_iterando))
		return contador;
	if(!funcion(raiz->elemento, aux))
		*seguir_iterando = false;
	contador++;
	contador += abb_iterar_inorden(raiz->derecha, funcion, aux, seguir_iterando);

	return contador;
}

size_t abb_iterar_preorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *), 
							void *aux, bool *seguir_iterando)
{
	if(!raiz || !(*seguir_iterando))
		return 0;

	size_t contador = 0; 

	if(!funcion(raiz->elemento, aux))
		*seguir_iterando = false;
	contador++;
	contador += abb_iterar_preorden(raiz->izquierda, funcion, aux, seguir_iterando);
	contador += abb_iterar_preorden(raiz->derecha, funcion, aux, seguir_iterando);

	return contador;
}

size_t abb_iterar_postorden(nodo_abb_t *raiz, bool (*funcion)(void *, void *), 
							void *aux, bool *seguir_iterando)
{
	if(!raiz || !(*seguir_iterando))
		return 0;

	size_t contador = 0; 

	contador += abb_iterar_postorden(raiz->izquierda, funcion, aux, seguir_iterando);
	contador += abb_iterar_postorden(raiz->derecha, funcion, aux, seguir_iterando);
	if(!(*seguir_iterando))
		return contador;
	if(!funcion(raiz->elemento, aux))
		*seguir_iterando = false;
	contador++;

	return contador;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if(!arbol || !funcion)
		return 0;
	
	bool seguir_iterando = true;
	if(recorrido == INORDEN)
		return abb_iterar_inorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando);
	else if(recorrido == PREORDEN)
		return abb_iterar_preorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando);
	return abb_iterar_postorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando);
}

struct arreglo{
	void **array;
	size_t cantidad;
	size_t tamanio;
};

bool sigo_almacenando(void *elemento, void *arreglo)
{
	struct arreglo *arreglo_aux = arreglo;

	if(arreglo_aux->cantidad < arreglo_aux->tamanio){
		arreglo_aux->array[arreglo_aux->cantidad] = elemento;
		arreglo_aux->cantidad++;
	}
	if(arreglo_aux->cantidad < arreglo_aux->tamanio)
		return true;
	return false;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if(!array || tamanio_array == 0)
		return 0;

	struct arreglo arreglo = {array, 0, tamanio_array};

	return abb_con_cada_elemento(arbol, recorrido, sigo_almacenando, &arreglo);
}

void abb_destruir_todo_rec(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if(!raiz)
		return;

	abb_destruir_todo_rec(raiz->izquierda, destructor);
	abb_destruir_todo_rec(raiz->derecha, destructor);

	if(destructor != NULL)
		destructor(raiz->elemento);
	free(raiz);
}

void abb_destruir(abb_t *arbol)
{
	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if(!arbol)
		return;

	abb_destruir_todo_rec(arbol->nodo_raiz, destructor);

	free(arbol);
}