#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE_POKEMON 50
#define FORMATO_LECTURA "%[^;];%i;%i;%i"
#define CANTIDAD_LEIDOS 4

struct _pokemon_t {
	char nombre[MAX_NOMBRE_POKEMON];
	int nivel;
	int ataque;
	int defensa;
};


pokemon_t *pokemon_crear_desde_string(char *string)
{
	if(!string)
		return NULL;

	pokemon_t pokemon_aux;
	int leidos = sscanf(string, FORMATO_LECTURA, pokemon_aux.nombre, 
												 &pokemon_aux.nivel, 
								 				 &pokemon_aux.ataque, 
												 &pokemon_aux.defensa);
	if(leidos != CANTIDAD_LEIDOS) 
		return NULL;
	
	pokemon_t *pokemon = malloc(sizeof(pokemon_t));
	if(!pokemon)
		return NULL;

	*pokemon = pokemon_aux;

	return pokemon;
}

int pokemon_nivel(pokemon_t *pokemon)
{
	if(!pokemon)
		return 0;
	return pokemon->nivel;
}

int pokemon_ataque(pokemon_t *pokemon)
{
	if(!pokemon)
		return 0;
	return pokemon->ataque;
}

int pokemon_defensa(pokemon_t *pokemon)
{
	if(!pokemon)
		return 0;
	return pokemon->defensa;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(!pokemon)
		return NULL;
	return pokemon->nombre;
}

void pokemon_destruir(pokemon_t *pokemon)
{
	free(pokemon);
}