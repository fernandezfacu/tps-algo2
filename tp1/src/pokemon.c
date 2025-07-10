#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE_POKEMON 30
#define FORMATO_LECTURA "%[^;];%i;%i;%i"
#define CANTIDAD_LEIDOS 4

struct _pokemon_t {
	char nombre[MAX_NOMBRE_POKEMON];
	int nivel;
	int poder_ataque;
	int poder_defensa;
};


pokemon_t *pokemon_crear_desde_string(char *string)
{
	if(!string)
		return NULL;

	char nombre[MAX_NOMBRE_POKEMON];
	int nivel, ataque, defensa;
	int leidos = sscanf(string, FORMATO_LECTURA, nombre, &nivel, &ataque, &defensa);
	if(leidos != CANTIDAD_LEIDOS) 
		return NULL;
	
	pokemon_t *pokemon = malloc(sizeof(pokemon_t));
	if(pokemon == NULL)
		return NULL;

	strcpy(pokemon->nombre, nombre);
	pokemon->nivel = nivel;
	pokemon->poder_ataque = ataque;
	pokemon->poder_defensa = defensa;
	return pokemon;
}

int pokemon_nivel(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return 0;
	return pokemon->nivel;
}

int pokemon_ataque(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return 0;
	return pokemon->poder_ataque;
}

int pokemon_defensa(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return 0;
	return pokemon->poder_defensa;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return NULL;
	return pokemon->nombre;
}

void pokemon_destruir(pokemon_t *pokemon)
{
	free(pokemon);//no es necesario chequear como puse en el tp enviado
}

// ERRORRES EN CORRECCIÓN:





