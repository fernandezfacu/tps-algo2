#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACIDAD_MINIMA 3
#define FACTOR_CARGA_MAXIMO 0.75

struct par {
	char *clave;
	void *elemento;
	struct par *siguiente;
};

struct hash {
	struct par **tabla;
	size_t capacidad;
	size_t ocupados;
	size_t (*funcion_de_hashing)(const char*);
};

//Función de hash utilizada en esta implementación del TDA.
size_t djb2(const char *str)
{
	int hash = 5381;
	int c;

	while((c = *(str++)))
		hash = ((hash << 5) + hash) + c;

	return (size_t)hash;
}

hash_t *hash_crear(size_t capacidad)
{
	if(capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash_t *hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	hash->funcion_de_hashing = djb2;
	hash->capacidad = capacidad;
	hash->ocupados = 0;

	hash->tabla = calloc(capacidad, sizeof(struct par *));
	if(!hash->tabla){
		free(hash);
		return NULL;
	}

	return hash;
}

/*
 * PRE: s debe apuntar a algo válido.
 * POST: Devuelve un puntero a char con el mismo contenido que s. 
 *       Si no se puede reservar memoria para este, se devuelve NULL.
 */ 
char *duplicar_string(const char *s)
{
	if (!s)
		return NULL;

	char *p = malloc(strlen(s) + 1);
	if (!p)
		return NULL;

	strcpy(p, s);
	return p;
}

/*
 * PRE: Recibe un dos punteros válidos.
 * POST: Ubica el par a_ubicar como úlitmo de los pares enlazados
 * (el primer par enlazado el primer parámetro de la función).
 */
struct par *par_ubicar(struct par *par, struct par *a_ubicar)
{
	if(!par){
		struct par *nuevo = a_ubicar;
		nuevo->siguiente = NULL;
		return nuevo;
	}
	par->siguiente = par_ubicar(par->siguiente, a_ubicar);
	return par; 
}

/*
 * PRE: Recibe un hash válido.
 * POST: Se rehashea el hash recibido.
 * 		 Devuelve el mismo hash pero con el doble de capacidad
 * 		 y sus elementos reordenados donde corresponda.
 */
hash_t *hash_rehash(hash_t *hash)
{
	struct par **tabla_nueva = calloc(hash->capacidad*2, sizeof(struct par *));
	if(!tabla_nueva)
		return NULL;

	for(size_t i = 0; i < hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual){
			struct par *siguiente = actual->siguiente;
			size_t pos_nueva = hash->funcion_de_hashing(actual->clave)%(hash->capacidad*2);
			tabla_nueva[pos_nueva] = par_ubicar(tabla_nueva[pos_nueva], actual);
			actual = siguiente;
		}
	}

	hash->capacidad = (hash->capacidad)*2;
	struct par **tabla_vieja = hash->tabla;
	hash->tabla = tabla_nueva;
	free(tabla_vieja);

	return hash;
}

/*
 * PRE: Recibe un hash válido 
 * POST: Devuelve su factor de carga.
 */
float factor_carga_hash(hash_t *hash)
{
	return (float)(hash->ocupados + 1)/(float)hash->capacidad;
}

/*
 * PRE: Recibe tres punteros válidos.
 * POST: Reserva la memoria para un nuevo par, asignándole la clave
 *       y el elemento recibidos. 
 *       En caso de error, se actualiza error a true y se devuelve NULL.
 */
struct par *par_crear(const char *clave, void *elemento, bool *error)
{
	struct par *nuevo_par = malloc(sizeof(struct par));
		if(!nuevo_par){
			*error = true;
			return NULL;
		}

	nuevo_par->clave = duplicar_string(clave);
	if(!nuevo_par->clave){
		*error = true;
		free(nuevo_par);
		return NULL;
	}
	nuevo_par->elemento = elemento;
	nuevo_par->siguiente = NULL;

	return nuevo_par;
}


/*
 * PRE: Recibe una clave válida.
 * POST: Devuelve el par recibido y se modifica el par que corresponda, 
 *       recorriendo desde el par hasta que:
 *
 *       Se encuentra un par que tenga la clave recibida; se reemplaza 
 *		 el elemento y se almacena un puntero al elemento reemplazado en 
 *		 *anterior, si anterior no es NULL.
 *	
 *       Si se llega a NULL, se inserta un nuevo par con la clave y el 
 *       elemento y se almacena NULL en *anterior, si este no es NULL.
 */
struct par *par_insertar_o_reemplazar(struct par *par, const char *clave, 
			void *elemento, void **anterior, bool *error, bool *se_reemplazo)
{
	if(!par) {
		if(anterior)
			*anterior = NULL;
		return par_crear(clave, elemento, error);

	} else if(strcmp(par->clave, clave) == 0) {
		if(anterior)
			*anterior = par->elemento;
		par->elemento = elemento;
		*se_reemplazo = true;

	} else par->siguiente = par_insertar_o_reemplazar(par->siguiente, 
						clave, elemento, anterior, error, se_reemplazo);

	return par;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if(!hash || !clave)
		return NULL;

	if(factor_carga_hash(hash) > FACTOR_CARGA_MAXIMO){
		if(!hash_rehash(hash))
			return NULL;
	}

	size_t pos = hash->funcion_de_hashing(clave)%hash->capacidad;
	bool error = false;
	bool se_reemplazo = false; 
	hash->tabla[pos] = par_insertar_o_reemplazar(hash->tabla[pos], clave, 
								elemento, anterior, &error, &se_reemplazo);

	if(error)
		return NULL;

	if(!se_reemplazo)
		hash->ocupados++;

	
	return hash;
}

/*
 * PRE: Recibe una clave válida.
 * POST: Se busca el elemento que tenga la clave pasada en la cadena 
 *       de pars que tiene al par pasado como primero.
 *       Si no se encuentra, se devuelve NULL.
 *       Se actualiza el booleano a true si un elemento es eliminado.
 */
struct par *par_quitar(struct par *actual, const char *clave, 
						void **elemento_extraido, bool *se_extrajo)
{
	if(!actual)
		return NULL;
	
	if(strcmp(actual->clave, clave) == 0){
		*se_extrajo = true;
		*elemento_extraido = actual->elemento;

		struct par *siguiente = actual->siguiente;
		free(actual->clave);
		free(actual);
		return siguiente;
	}

	actual->siguiente = par_quitar(actual->siguiente, 
							clave, elemento_extraido, se_extrajo);

	return actual;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if(!hash || !clave)
		return false;

	size_t pos = hash->funcion_de_hashing(clave)%hash->capacidad;
	void *elemento_extraido = NULL;
	bool se_extrajo = false;
	hash->tabla[pos] = par_quitar(hash->tabla[pos], 
						clave, &elemento_extraido, &se_extrajo);

	if(!se_extrajo)
		return NULL;

	hash->ocupados--;
	return elemento_extraido;
}

struct aux_busqueda{
	void *elemento;
	bool encontrado;
};

/*
 * PRE: Recibe tres punteros válidos.
 * POST: Busca dentro del hash la clave recibida. Si la encuentra, guarda en 
 *       aux el elemento asociado a la clave y actualiza el campo encontrado a true.
 */
void hash_buscar(hash_t *hash, const char *clave, struct aux_busqueda *aux)
{
	size_t pos = hash->funcion_de_hashing(clave)%hash->capacidad;

	struct par *actual = hash->tabla[pos];
	while(actual && !aux->encontrado){
		if(strcmp(actual->clave, clave) == 0){
			aux->elemento = actual->elemento;
			aux->encontrado = true;
		}

		actual = actual->siguiente;
	}
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if(!hash || !clave)
		return false;

	struct aux_busqueda aux = {NULL, false};
	hash_buscar(hash, clave, &aux);

	return aux.elemento;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if(!hash || !clave)
		return false;

	struct aux_busqueda aux = {NULL, false};
	hash_buscar(hash, clave, &aux);

	return aux.encontrado;
}

size_t hash_cantidad(hash_t *hash)
{
	return (!hash)? 0: hash->ocupados;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if(!hash)
		return;

	for(size_t i = 0; i < hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual){
			struct par *siguiente = actual->siguiente;
			if(destructor)
				destructor(actual->elemento);
			free(actual->clave);
			free(actual);

			actual = siguiente;
		}
	}
	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	if(!hash || !f)
		return 0;

	size_t contador = 0;
	bool sigue_iterando = true;

	for(size_t i = 0; i < hash->capacidad; i++){
		struct par *actual = hash->tabla[i];
		while(actual && sigue_iterando){
			if(!f(actual->clave, actual->elemento, aux))
				sigue_iterando = false;
			contador++;
			actual = actual->siguiente;
		}
	}

	return contador;
}
