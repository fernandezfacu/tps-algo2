#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if(!comparador)
		return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));
	if(arbol)
		arbol->comparador = comparador;

	return arbol;
}

/* 
 * PRE: -
 * POST: Reserva la memoria para un nodo, lo inicializa 
 *       asignándole el elemento recibido y lo devuelve.
 *       En caso de error al reservar memoria, devuelve NULL. 
 */
nodo_abb_t *nodo_abb_crear(void *elemento)
{
	nodo_abb_t *nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
	if(!nuevo_nodo)
		return NULL;

	nuevo_nodo->elemento = elemento;
	return nuevo_nodo;
}

/*
 * PRE: Recibe un nodo a insertar válido.
 * POST: Inserta el nodo a insertar recibido en el abb a partir de su raíz
 *       y devuelve la raíz con el nodo insertado.        
 */     
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

	nodo_abb_t *nuevo_nodo = nodo_abb_crear(elemento);
		if(!nuevo_nodo)
			return NULL;

	arbol->nodo_raiz = abb_insertar_rec(arbol->nodo_raiz, 
						arbol->comparador, nuevo_nodo);
	arbol->tamanio++;

	return arbol;
}

/*
 * PRE: -
 * POST: Se guarda como predecesor al elemento del nodo que se encuentra más 
 *       a la derecha del abb (el que tiene el mayor valor) que tiene como raíz 
 *		 al nodo recibido. Se elimina del abb a este nodo.
 *       Se devuelve la raiz con el nodo correspondiente ya eliminado.
 */
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

/*
 * PRE: El puntero se_extrajo debe ser válido.
 * POST: Elimina del abb el elemento recibido si este se encuentra en el mismo.
 *       Si se encuentra el elemento, se lo guarda como elemento_extraido y se 
 *       modifica el booleano a true. Si no, no se hace nada.
 *       Devuelve la raíz del abb con el elemento eliminado (si no se elimino, 
 *       no se modifica nada).
 */
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

/*
 * PRE: Recibe un puntero a bool y una función válidos.
 * POST: Recorre el arbol que tiene como raíz el nodo recibido e invoca la funcion con cada elemento 
 *       almacenado en el mismo como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 *       función. Si la función devuelve false, se finaliza el recorrido aun si quedan elementos por recorrer. 
 *       Si devuelve true se sigue recorriendo mientras queden elementos.
 *       El tipo de recorrido realizado es INORDEN.
 *       Devuelve la cantidad de veces que fue invocada la función.
 */
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

/*
 * PRE: Recibe un puntero a bool y una función válidos.
 * POST: Recorre el arbol que tiene como raíz el nodo recibido e invoca la funcion con cada elemento 
 *       almacenado en el mismo como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 *       función. Si la función devuelve false, se finaliza el recorrido aun si quedan elementos por recorrer. 
 *       Si devuelve true se sigue recorriendo mientras queden elementos.
 *       El tipo de recorrido realizado es PREORDEN.
 *       Devuelve la cantidad de veces que fue invocada la función.
 */
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

/*
 * PRE: Recibe un puntero a bool y una función válidos.
 * POST: Recorre el arbol que tiene como raíz el nodo recibido e invoca la funcion con cada elemento 
 *       almacenado en el mismo como primer parámetro. El puntero aux se pasa como segundo parámetro a la
 *       función. Si la función devuelve false, se finaliza el recorrido aun si quedan elementos por recorrer. 
 *       Si devuelve true se sigue recorriendo mientras queden elementos.
 *       El tipo de recorrido realizado es POSTORDEN.
 *       Devuelve la cantidad de veces que fue invocada la función.
 */
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

/*
 * PRE: Recibe un puntero válido que debe apuntar a una estructura
 *      del tipo struct arreglo.
 * POST: Inserta el elemento en la siguiente posición vacía en caso de
 *       ser posible.
 *       Si queda espacio en el arreglo luego de insertar, devuelve true.
 *       En caso contrario, devuelve false.
 */
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

/*
 * PRE: -
 * POST: Libera toda la memoria asociada a un abb a partir de su raíz.
 */
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