#include "gestor.h"
#include "cajas.h"
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_MINIMA 3
#define MAX_LINEA 100
#define FORMATO_LECTURA "%[^\n]\n%[^\n]\n%[^\n]\n"
#define CANTIDAD_LEIDOS 3
#define MAX_NOMBRE_POKEMON 50

struct gestor{
	hash_t *cajas;
	bool finalizado;
	hash_t *indices_pokemones;
};

/**
 * PRE: -
 * POST: Reserva la memoria para el gestor y lo devuelve.
 *       Devuelve NULL si hubo un error.
 */ 
gestor_t *gestor_crear()
{
	gestor_t *gestor = calloc(1, sizeof(gestor_t));
	if(!gestor)
		return NULL;
	gestor->cajas = hash_crear(CAPACIDAD_MINIMA);
	if(!gestor->cajas){
		free(gestor);
		return NULL;
	}
	gestor->indices_pokemones = hash_crear(CAPACIDAD_MINIMA);
	if(!gestor->indices_pokemones){
		hash_destruir(gestor->cajas);
		free(gestor);
		return NULL;
	}
	gestor->finalizado = false;
	return gestor;
}

/**
 * PRE: Recibe dos punteros válidos.
 * POST: Reserva la memoria correspondiente para un índice 
 *       y guarda este en el hash de índices con el nombre 
 *       del pokemon recibido como clave.
 * 		 Devuelve el gestor o NULL en caso de error.
 */ 
gestor_t *gestor_agregar_indice(gestor_t *gestor, const char *nombre_pokemon)
{
	lista_t *indice = lista_crear();
	if(!indice)
		return NULL;

	if(!hash_insertar(gestor->indices_pokemones, nombre_pokemon, indice, NULL))
		return NULL;

	return gestor;
}

/*
 * PRE: s debe apuntar a algo válido.
 * POST: Devuelve un puntero a char con el mismo contenido que s. 
 *       Si no se puede reservar memoria para este, se devuelve NULL.
 */ 
char *dupli_string(const char *s)
{
	if (!s)
		return NULL;

	char *p = malloc(strlen(s) + 1);
	if (!p)
		return NULL;

	strcpy(p, s);
	return p;
}

/**
 * PRE: Recibe dos punteros válidos, deben ser variables del tipo char*.
 * POST: Devuelve 0 si ambos nombres son iguales, un valor mayor a 0 si 
 *       el primero es mayor alfabeticamente al segundo y uno menor si 
 * 		 es el caso contrario.
 */ 
int comparar_nombres(void *nombre1, void *nombre2)
{
	return strcmp(nombre1, nombre2);
}

/**
 * PRE: Recibe tres punteros válidos.
 * POST: Guarda en el gestor los índices que correspondan si alguno de 
 * 		 los pokemones de la caja recibida no estaba en el gestor.
 * 		 Guarda en los índices del gestor los nombres de la caja en la que
 *       se encuentra cada pokemon.
 * 		 Devuelve el gestor o NULL en caso de error al reservar memoria.
 */ 
gestor_t *gestor_agregar_indices(gestor_t *gestor, char *nombre_caja, 
														caja_t *caja)
{
	int i = 0;
	bool hubo_error = false;
	while(i < caja_cantidad(caja) && !hubo_error) {
		pokemon_t *poke_aux = caja_obtener_pokemon(caja, i);
		const char *nombre_poke = pokemon_nombre(poke_aux);

		if(!hash_contiene(gestor->indices_pokemones, nombre_poke)){
			if(!gestor_agregar_indice(gestor, nombre_poke))
				hubo_error = true;
		} 
		if(!hubo_error){
			lista_t *indice = hash_obtener(gestor->indices_pokemones, 
															nombre_poke);
			if(!lista_buscar_elemento(indice, comparar_nombres, nombre_caja)){
				char *nombre = dupli_string(nombre_caja);
				if(!nombre)
					hubo_error = true;
				if(!lista_insertar_en_posicion(indice, nombre, 0))
					hubo_error = true;
			}
		}
		i++;
	}
	if(hubo_error)
		return NULL;

	return gestor;
}

/**
 * PRE: Recibe dos punteros válidos.
 * POST: Crea una caja con el nombre del archivo recibido. 
 *       Guarda la caja en el gestor y el nombre de la caja
 * 		 en los ìndices de los pokemones del gestor (crea nuevos
 * 		 si un pokemon no estaba en el gestor).
 * 		 Devuelve el gestor o NULL en caso de error (al reservar memoria o
 * 		 si el archivo con el nombre recibido no existe o tiene formato incorrecto).
 */ 
gestor_t *gestor_agregar_caja(gestor_t *gestor, char *nombre_archivo)
{
	caja_t *caja = caja_cargar_archivo(nombre_archivo);
	if(!caja)
		return NULL;

	if(!hash_insertar(gestor->cajas, nombre_archivo, caja, NULL))
		return NULL;

	if(!gestor_agregar_indices(gestor, nombre_archivo, caja))
		return NULL;

	return gestor;
}


gestor_t *gestor_inicializar(char **nombres_archivos, int cantidad)
{
	if(!nombres_archivos)
		return NULL;

	gestor_t *gestor = gestor_crear();
	if(!gestor)
		return NULL;

	for(int i = 0; i < cantidad; i++){
		if(!gestor_agregar_caja(gestor, nombres_archivos[i]))
			printf("\nNo se pudo cargar la caja %s. Por favor verifique si los nombres o los formatos de los archivos ingresados son correctos.\nPuede deberse también a un error en la reserva de memoria.\n", 
					nombres_archivos[i]);
		else printf("\nSe cargó la caja %s.\n", nombres_archivos[i]);
	}
	printf("\n");

	if(gestor_cantidad(gestor) == 0){
		gestor_destruir(gestor);
		return NULL;
	}
	return gestor;
}

/**
 * PRE: nombre y caja deben ser punteros válidos (caja debe apuntar
 * 		a algo de tipo caja_t).
 * POST: Imprime por pantalla los datos de la caja recibida.
 */ 
bool imprimir_caja(const char *nombre, void *caja, void *aux)
{
	caja_t *c = caja;
	printf("%s   -   %i pokemones\n", nombre, caja_cantidad(c));
	return true;
}

size_t gestor_mostrar_inventario(gestor_t *gestor)
{
	if(!gestor)
		return 0;
	return hash_con_cada_clave(gestor->cajas, imprimir_caja, NULL);
}

gestor_t *gestor_cargar_caja(gestor_t *gestor, 
										char* (*leer_texto)(char*))
{
	if(!gestor || !leer_texto)
		return NULL;

	char nombre_archivo[MAX_LINEA];
	leer_texto(nombre_archivo);

	if(hash_contiene(gestor->cajas, nombre_archivo)) 
		return NULL;
	
	if(!gestor_agregar_caja(gestor, nombre_archivo)) 
		return NULL;

	return gestor;
}

/**
 * 
 * PRE: Recibe tres punteros válidos. 
 * POST: Genera un archivo .csv con el contenido de la caja recibida, con el nombre
 *       recibido.
 * 		 Carga la caja en el gestor y actualiza el índice de pokemones como corresponda.
 * 		 Devuelve el gestor o NULL en caso de error.

 */
gestor_t *gestor_guardar_caja(gestor_t *gestor, caja_t *caja, char *nombre_archivo)
{
	caja_guardar_archivo(caja, nombre_archivo);

	if(!hash_insertar(gestor->cajas, nombre_archivo, caja, NULL))
		return NULL;

	if(!gestor_agregar_indices(gestor, nombre_archivo, caja))
		return NULL;
	
	return gestor;
}

gestor_t *gestor_combinar_cajas(gestor_t *gestor, char* (*leer_texto)(char*))
{
	if(!gestor || !leer_texto)
		return NULL;

	char lineas[3*MAX_LINEA];
	leer_texto(lineas);

	char linea1[MAX_LINEA];
	char linea2[MAX_LINEA];
	char linea3[MAX_LINEA];

	int leidos = sscanf(lineas, FORMATO_LECTURA, linea1, linea2, linea3);
	if(leidos != CANTIDAD_LEIDOS){
		printf("\nHubo un problema al leer los datos ingresados. Asegurese de ingresar los nombres correctamente.\n");
		return NULL;
	}

	caja_t *caja1 = hash_obtener(gestor->cajas, linea1);
	if(!caja1)
		return NULL;
	
	caja_t *caja2 = hash_obtener(gestor->cajas, linea2);
	if(!caja2)
		return NULL;
	
	if(hash_contiene(gestor->cajas, linea3))
		return NULL;
	
	caja_t *caja_comb = caja_combinar(caja1, caja2);
	if(!caja_comb)
		return NULL;
	
	if(!gestor_guardar_caja(gestor, caja_comb, linea3)) {
		free(caja_comb);
		return NULL;
	}
	return gestor;
}

/**
 * PRE: Recibe un puntero válido.
 * POST: Imprime por pantalla los datos del pokemon recibido.
 * 
 * 
 */
void pokemon_mostrar(pokemon_t *pokemon)
{
	printf("%s-", pokemon_nombre(pokemon));
	printf("%i-", pokemon_nivel(pokemon));
	printf("%i-", pokemon_ataque(pokemon));
	printf("%i\n", pokemon_defensa(pokemon));
}

int gestor_mostrar_caja(gestor_t *gestor, char* (*leer_texto)(char*))
{
	if(!gestor || !leer_texto)
		return 0;

	char linea[MAX_LINEA];
	leer_texto(linea);

	caja_t *caja = hash_obtener(gestor->cajas, linea);
	if(!caja) 
		return 0;
	
	printf("\ncaja: %s\n\n", linea);
	return caja_recorrer(caja, pokemon_mostrar);
}

/**
 * PRE: El puntero nombre_caja debe ser válido y debe ser de tipo char*
 * POST: Imprime por pantalla el nombre recibido y devuelve true.
 */
bool cajas_mostrar(void *nombre_caja, void *aux)
{
	char *nombre = nombre_caja;
	printf("%s\n", nombre);

	return true;
}

size_t gestor_buscar_caja(gestor_t *gestor, char* (*leer_texto)(char*))
{
	if(!gestor || !leer_texto)
		return 0;

	char nombre_pokemon[MAX_LINEA];
	leer_texto(nombre_pokemon);

	lista_t *indice = hash_obtener(gestor->indices_pokemones, nombre_pokemon);
	if(!indice)
		return 0;

	printf("\nEl pokemon se encuentra en la(s) caja(s):\n");
	size_t cantidad = lista_con_cada_elemento(indice, cajas_mostrar, NULL);
	printf("\nCantidad de cajas en la que se encuentra el pokemon: %zu\n", cantidad);

	return cantidad;
}

size_t gestor_cantidad(gestor_t *gestor)
{
	if(!gestor)
		return 0;
	return hash_cantidad(gestor->cajas);
}

bool gestor_finalizo(gestor_t *gestor)
{
	if(!gestor)
		return true;
	return gestor->finalizado;
}

void gestor_salir(gestor_t *gestor)
{
	if(!gestor)
		return;
	gestor->finalizado = true;
}

/* 
 * PRE: Recibe un puntero a que debe apuntar a algo del tipo caja_t.
 * POST: Aplica la función destructora a la caja para liberar toda
 *       la memoria asociada a esta.
 */
void destructor_caja(void *caja)
{
	caja_destruir(caja);
}

/* 
 * PRE: Recibe un puntero a que debe apuntar a algo del tipo char*.
 * POST: Libera la memoria asociada a este.
 */
void destructor_string(void *s)
{
	free(s);
}

/* 
 * PRE: Recibe un puntero a que debe apuntar a algo del tipo lista_t, 
 *		que como elementos debe tener strings unicamente.
 * POST: Aplica la función destructora a la lista para liberar toda
 *       la memoria asociada a esta.
 */
void destructor_indice(void *lista)
{
	if(!lista)
		return;
	lista_destruir_todo(lista, destructor_string);
}

void gestor_destruir(gestor_t *gestor)
{
	if(!gestor)
		return;
	hash_destruir_todo(gestor->cajas, destructor_caja);
	hash_destruir_todo(gestor->indices_pokemones, destructor_indice);
	free(gestor);
}

