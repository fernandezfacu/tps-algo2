#include "cajas.h"
#include "pokemon.h"
#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINEA 100
#define MAX_NOMBRE_POKEMON 30
#define FORMATO_ESCRITURA_FILE "%s;%i;%i;%i\n"
#define FORMATO_ESCRITURA_STRING "%s;%i;%i;%i"

struct _caja_t {
	abb_t *pokemones;
};

/* 
 * PRE: Recibe dos punteros válidos que deben apuntar a estructuras del tipo pokemon_t.
 * POST: Compara los nombres de los pokemones y devuelve un valor menor a 0 si el primero 
 *       va primero alfabéticamente, uno mayor a 0 si es el caso contrario o 0 si el nombre
 *       ese el mismo.
 */
int comparar_pokemones(void *p1, void *p2)
{
	pokemon_t *poke1 = p1;
	pokemon_t *poke2 = p2;
	return strcmp(pokemon_nombre(poke1), pokemon_nombre(poke2));
}

/*
 * PRE: -
 * POST: Reserva la memoria para una caja vacía y la devuelve.
 */
caja_t *caja_crear()
{
	caja_t *caja = malloc(sizeof(caja_t));
	if(!caja)
		return NULL;
	caja->pokemones = abb_crear(comparar_pokemones);
	if(!caja->pokemones){
		free(caja);
		return NULL;
	}

	return caja;
}

/*
 * PRE: Recibe una caja válida y un string válido, con el formato en el
 * 		enunciado de TP1.
 * POST: Agrega el pokemon a la caja y la devuelve. En caso de error al reservar 
 * 		 memoria, libera la memoria asociada a la caja y devuelve NULL.
 */
caja_t *caja_agregar_pokemon_desde_string(caja_t *caja, char *linea)
{
	pokemon_t *poke_aux = pokemon_crear_desde_string(linea);
	if(!poke_aux){
		caja_destruir(caja);
		return NULL;
	}

	abb_insertar(caja->pokemones, poke_aux);
	if(!caja->pokemones){
		pokemon_destruir(poke_aux);
		caja_destruir(caja);
		return NULL;
	}

	return caja;
}

/*
 * PRE: Recibe una caja vacía y válida y un archivo válido.
 * POST: Llena la caja con todos los pokemon del archivo csv y la devuelve.
 * 		 En caso de ser inválido EL FORMATO del archivo o de error al 
 *		 agregar un pokemon, libera la memoria asociada a la caja y devuelve NULL.
 */
caja_t *caja_llenar_desde_archivo(caja_t *caja, FILE* archivo)
{
	char linea[MAX_LINEA];
	char *leida = fgets(linea, MAX_LINEA, archivo);
	if(!leida){
		caja_destruir(caja);
		return NULL;
	}
	while(leida) {
		caja = caja_agregar_pokemon_desde_string(caja, linea);
		if(!caja)
			return NULL;
		leida = fgets(linea, MAX_LINEA, archivo);
	}
	return caja;
}

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{
	if(!nombre_archivo)
		return NULL;

	FILE *f_pokemones = fopen(nombre_archivo, "r");
	if(!f_pokemones)
		return NULL;

	caja_t *caja = caja_crear();
	caja_llenar_desde_archivo(caja, f_pokemones);
	if(!caja)
		return NULL;
	fclose(f_pokemones);

	return caja;
}

/*
 * PRE: Recibe dos punteros válidos. El primero debe apuntar a algo de tipo pokemon_t 
 *		y el segundo a algo del tipo FILE.
 * POST: Escribe en una linea del archivo los datos del pokemon recibido 
 *		 con el formato correspondiente.
 */
bool guardar_pokemon(void *pokemon, void *archivo)
{
	pokemon_t *poke = pokemon;
	FILE* f_pokemones = archivo;

	fprintf(f_pokemones, FORMATO_ESCRITURA_FILE, pokemon_nombre(poke), 
		pokemon_nivel(poke), pokemon_ataque(poke), pokemon_defensa(poke));

	return true;
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
	if(!nombre_archivo || !caja)
		return 0;

	FILE* f_pokemones = fopen(nombre_archivo, "w");
	if(!f_pokemones)
		return 0;

	int contador = (int) abb_con_cada_elemento(caja->pokemones, INORDEN, 
										guardar_pokemon, f_pokemones);
	fclose(f_pokemones);
	return contador;
}

/*
 * PRE: Recibe una caja y un pokemon válidos.
 * POST: Guarda en la caja el pokemon recibido y la devuelve.
 */
caja_t *caja_agregar_pokemon(caja_t *caja, pokemon_t *pokemon)
{
	char linea[MAX_LINEA];
	sprintf(linea, FORMATO_ESCRITURA_STRING, 
				pokemon_nombre(pokemon), pokemon_nivel(pokemon), 
				pokemon_ataque(pokemon), pokemon_defensa(pokemon));
	return caja_agregar_pokemon_desde_string(caja, linea);
}

/*
 * PRE: Recibe dos punteros válidos, el primero debe apuntar a una variable de tipo
 *      pokemon_t y el segundo a una del tipo caja_t.
 * POST: Guarda el pokemon recibido en la estructura y devuelve false
 *       si el campo recorridos es igual al campo hasta dentro de la estructura.
 *       Caso contrario, se aumenta recorridos y se devuelve true.
 */
bool insertar_en_abb(void *p, void *c)
{
	caja_t *caja = c;
	pokemon_t *pokemon = p;
	caja_agregar_pokemon(caja, pokemon);

	return true;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{
	if(!caja1 || !caja2)
		return NULL;

	caja_t *caja_comb = caja_crear();
	if(!caja_comb)
		return NULL;

	abb_con_cada_elemento(caja1->pokemones, PREORDEN, insertar_en_abb, caja_comb);
	abb_con_cada_elemento(caja2->pokemones, PREORDEN, insertar_en_abb, caja_comb);

	return caja_comb;
}

int caja_cantidad(caja_t *caja)
{
	if(!caja)
		return 0;

	return (int) abb_tamanio(caja->pokemones);
}

struct aux_busqueda{
	pokemon_t *pokemon;
	int recorridos;
	int hasta;
};

/*
 * PRE: Recibe dos punteros válidos, el primero debe apuntar a una variable de tipo
 *      pokemon_t y el segundo a una del tipo struct aux_busqueda.
 * POST: Guarda el pokemon recibido en la estructura y devuelve false
 *       si el campo recorridos es igual al campo hasta dentro de la estructura.
 *       Caso contrario, se aumenta recorridos y se devuelve true.
 */
bool se_recorrio_hasta(void *pokemon, void *aux){

	struct aux_busqueda *aux_busqueda = aux;
	if(aux_busqueda->recorridos == aux_busqueda->hasta){
		aux_busqueda->pokemon = pokemon;
		return false;
	}
	aux_busqueda->recorridos++;
	return true;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{
	if(!caja|| n < 0 || n >= caja_cantidad(caja))
		return NULL;

	struct aux_busqueda aux = {NULL, 0, n};
	abb_con_cada_elemento(caja->pokemones, INORDEN, 
							se_recorrio_hasta, &aux);
	return aux.pokemon;
}

struct aux_funcion{
	void (*f)(pokemon_t *);
};

/*
 * PRE: Recibe dos punteros válidos, el primero debe apuntar a una variable de tipo
 *      pokemon_t y el segundo a una del tipo struct aux_funcion.
 * POST: Aplica la función del aux_funcion al pokemon recibido
 *       y devuelve true.
 */
bool aplicar_funcion(void *pokemon, void *aux){

	struct aux_funcion *aux_f = aux;
	pokemon_t *poke = pokemon;
	aux_f->f(poke);
	return true;
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{
	if(!caja|| !funcion)
		return 0;

	struct aux_funcion aux = {funcion};
	return (int) abb_con_cada_elemento(caja->pokemones, INORDEN, 
										aplicar_funcion, &aux);
}

/* 
 * PRE: Recibe un puntero a que debe apuntar a algo del tipo pokemon_t.
 * POST: Aplica la funciòn destructora al pokemon para liberar toda
 *       la memoria asociada a este.
 */
void destructor_pokemon(void *pokemon)
{
	pokemon_destruir(pokemon);
}

void caja_destruir(caja_t *caja)
{
	if(!caja)
		return;

	abb_destruir_todo(caja->pokemones, destructor_pokemon);
	free(caja);
}