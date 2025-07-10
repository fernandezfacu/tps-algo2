#include "cajas.h"
#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINEA 50
#define MAX_NOMBRE_POKEMON 30
#define FORMATO_ESCRITURA_FILE "%s;%i;%i;%i\n"
#define FORMATO_ESCRITURA_STRING "%s;%i;%i;%i"

struct _caja_t {
	pokemon_t **pokemones;
	int cantidad;
};

caja_t *caja_crear()
{
	caja_t *caja = malloc(sizeof(caja_t));
	if(caja == NULL)
		return NULL;
	caja->pokemones = NULL;
	caja->cantidad = 0;

	return caja;
}

caja_t *caja_agregar_pokemon_desde_string(caja_t *caja, char *linea)
{
	pokemon_t *poke_aux = pokemon_crear_desde_string(linea);
	if(poke_aux == NULL){
		caja_destruir(caja);
		return NULL;
	}

	pokemon_t **nuevos_pokemones = realloc(
	        caja->pokemones, (size_t)(caja->cantidad+1)*sizeof(pokemon_t *));
	if(nuevos_pokemones == NULL){
		caja_destruir(caja);
		pokemon_destruir(poke_aux);
		return NULL;
	}
	caja->pokemones = nuevos_pokemones;	

	caja->pokemones[caja->cantidad] = poke_aux;
	caja->cantidad++;

	return caja;
}

caja_t *caja_llenar_desde_archivo(caja_t *caja, FILE* archivo)
{
	char linea[MAX_LINEA];
	char *leida = fgets(linea, MAX_LINEA, archivo);
	if(leida == NULL){
		caja_destruir(caja);
		return NULL;
	}
	while(leida) {
		caja = caja_agregar_pokemon_desde_string(caja, linea);
		if(caja == NULL)
			return NULL;
		leida = fgets(linea, MAX_LINEA, archivo);
	}
	return caja;
}

void caja_ordenar(caja_t *caja)
{
	if(caja == NULL)
		return;

	pokemon_t *poke_aux;
	int j;
	for(int i = 1; i < caja->cantidad; i++) {
		j = i;
		poke_aux = caja->pokemones[i];
		while(j > 0 && strcmp(pokemon_nombre(poke_aux), pokemon_nombre(caja->pokemones[j-1])) < 0) {
			caja->pokemones[j] = caja->pokemones[j-1];
			j--;
		}
		caja->pokemones[j] = poke_aux;
	}
}

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{
	if(nombre_archivo == NULL)
		return NULL;

	FILE *f_pokemones = fopen(nombre_archivo, "r");
	if(!f_pokemones)
		return NULL;

	caja_t *caja;
	caja = caja_crear();
	
	caja = caja_llenar_desde_archivo(caja, f_pokemones);
	fclose(f_pokemones);

	caja_ordenar(caja);

	return caja;
}

void pokemon_guardar_variables(pokemon_t *pokemon, char nombre[MAX_NOMBRE_POKEMON], int *nivel, int *ataque, int *defensa)
{
	strcpy(nombre, pokemon_nombre(pokemon));
	*nivel = pokemon_nivel(pokemon);
	*ataque = pokemon_ataque(pokemon);
	*defensa = pokemon_defensa(pokemon);
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
	if(nombre_archivo == NULL || caja == NULL)
		return 0;

	FILE* f_pokemones = fopen(nombre_archivo, "w");
	if(f_pokemones == NULL)
		return 0;

	char nombre[MAX_NOMBRE_POKEMON];
	int nivel, ataque, defensa;
	for(int i = 0; i < caja->cantidad; i++){	
		pokemon_guardar_variables(caja->pokemones[i], nombre, &nivel, &ataque, &defensa);
		fprintf(f_pokemones, FORMATO_ESCRITURA_FILE, nombre, nivel, ataque, defensa);
	}

	fclose(f_pokemones);

	return caja->cantidad;
}

caja_t *caja_agregar_pokemon(caja_t *caja, pokemon_t *pokemon)
{
	char nombre[MAX_NOMBRE_POKEMON];
	int nivel, ataque, defensa;
	pokemon_guardar_variables(pokemon, nombre, &nivel, &ataque, &defensa);
	
	char linea[MAX_LINEA];
	sprintf(linea, FORMATO_ESCRITURA_STRING, nombre, nivel, ataque, defensa);
	return caja_agregar_pokemon_desde_string(caja, linea);
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{
	if(caja1 == NULL || caja2 == NULL)
		return NULL;

	caja_t *caja_comb;
	caja_comb = caja_crear();
	
	int i = 0, j = 0;
	while(i < caja1->cantidad && j < caja2->cantidad) {
		if (strcmp(pokemon_nombre(caja1->pokemones[i]), pokemon_nombre(caja2->pokemones[j])) <= 0){
			caja_comb = caja_agregar_pokemon(caja_comb, caja1->pokemones[i]);
			i++;
		} else {
			caja_comb = caja_agregar_pokemon(caja_comb, caja2->pokemones[j]);
			j++;
		}
	}
	while(i < caja1->cantidad) {
		caja_comb = caja_agregar_pokemon(caja_comb, caja1->pokemones[i]);
		i++;
	}
	while(j < caja2->cantidad) {
		caja_comb = caja_agregar_pokemon(caja_comb, caja2->pokemones[j]);
		j++;
	}

	return caja_comb;
}

int caja_cantidad(caja_t *caja)
{
	if(caja == NULL)
		return 0;

	return caja->cantidad;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{
	if(caja == NULL || n < 0 || n >= caja->cantidad)
		return NULL;

	return caja->pokemones[n];
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{
	if(caja == NULL || funcion == NULL)
		return 0;

	int contador = 0;
	for(int i = 0; i < caja->cantidad; i++){
		(*funcion)(caja->pokemones[i]);
		contador++;
	}
	return contador;
}

void caja_destruir(caja_t *caja)
{
	if(caja == NULL)
		return; 

	for(int i = 0; i < caja->cantidad; i++)
		pokemon_destruir(caja->pokemones[i]);
	free(caja->pokemones);
	free(caja);
}