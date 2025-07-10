#include "src/cajas.h"
#include "src/pokemon.h"
#include "src/gestor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa2m.h"

#define MAX_NOMBRES 30
#define MAX_LINEA 100

void probarCreacionPokemon()
{
	pokemon_t *pikachu = pokemon_crear_desde_string("Pikachu;1;2;3");
	pa2m_afirmar(pikachu != NULL, "Intento crear un pokemon con el texto 'Pikachu;1;2;3'");
	pa2m_afirmar(strcmp(pokemon_nombre(pikachu), "Pikachu") == 0, "El nombre es correcto.");
	pa2m_afirmar(pokemon_nivel(pikachu) == 1, "El nivel es correcto.");
	pa2m_afirmar(pokemon_ataque(pikachu) == 2, "El poder de ataque es correcto.");
	pa2m_afirmar(pokemon_defensa(pikachu) == 3, "El poder de defensa es correcto.");
	pokemon_destruir(pikachu);

	pokemon_t *poke = pokemon_crear_desde_string(";1;2;3");
	pa2m_afirmar(poke == NULL, "Intento crear un pokemon con el texto ';1;2;3' y me devuelve NULL");
	pokemon_t *poke2 = pokemon_crear_desde_string("Pika;1;2;");
	pa2m_afirmar(poke2 == NULL, "Intento crear un pokemon con el texto 'Pika;1;2;' y me devuelve NULL");
}

void probarCreacionCaja()
{
	caja_t *caja = caja_cargar_archivo("pokemones.csv"); 
	pa2m_afirmar(caja != NULL, "Puedo cargar la caja del archivo pokemones.csv");
	pa2m_afirmar(caja_cantidad(caja) == 6, "La caja tiene la misma cantidad de pokemones que el archivo.");
	pokemon_t *charmander = caja_obtener_pokemon(caja, 0);
	pa2m_afirmar(charmander != NULL, "Puedo obtener el primer pokemon de la caja.");
	pa2m_afirmar(strcmp(pokemon_nombre(charmander), "Charmander") == 0
				&& pokemon_nivel(charmander) == 6 
				&& pokemon_ataque(charmander) == 20
				&& pokemon_defensa(charmander) == 5, "Los datos del pokemon son correctos");

	pokemon_t *pikachu = caja_obtener_pokemon(caja, 2);
	pa2m_afirmar(pikachu != NULL, "Puedo obtener el tercer pokemon de la caja.");
	pa2m_afirmar(strcmp(pokemon_nombre(pikachu), "Pikachu") == 0
				&& pokemon_nivel(pikachu) == 8 
				&& pokemon_ataque(pikachu) == 30
				&& pokemon_defensa(pikachu) == 20, "Los datos del pokemon son correctos");
	
	pokemon_t *pokemon2 = caja_obtener_pokemon(caja, 10);
	pa2m_afirmar(pokemon2 == NULL, "No puedo obtener un pokemon en una posición mayor al tamaño de la caja.");

	pokemon_t *pokemon3 = caja_obtener_pokemon(caja, -1);
	pa2m_afirmar(pokemon3 == NULL, "No puedo obtener un pokemon en la posición -1.");

	caja_destruir(caja);
}

void probarGuardadoCaja(){

	caja_t *caja = caja_cargar_archivo("pokemones2.csv"); 
	pa2m_afirmar(caja_guardar_archivo(caja, "copia.csv") == caja_cantidad(caja), "Guardar caja devuelve la cantidad de pokemones en la caja.");
	caja_t *copia_caja = caja_cargar_archivo("copia.csv"); 
	pa2m_afirmar(copia_caja != NULL, "Puedo cargar la caja recién guardada.");
	pa2m_afirmar(caja_cantidad(copia_caja) == caja_cantidad(caja), "La cantidad de pokemones en la nueva caja es correcta.");
	pokemon_t *charizard = caja_obtener_pokemon(copia_caja, 0);
	pa2m_afirmar(charizard != NULL, "Puedo obtener el primer pokemon de la caja.");
	pa2m_afirmar(strcmp(pokemon_nombre(charizard), "Charizard") == 0
				&& pokemon_nivel(charizard) == 5 
				&& pokemon_ataque(charizard) == 20
				&& pokemon_defensa(charizard) == 30, "Los datos del pokemon son correctos");

	caja_destruir(copia_caja);
	caja_destruir(caja);
}

void probarCombinadoCaja()
{
	caja_t *caja1 = caja_cargar_archivo("pokemones.csv"); 
	caja_t *caja2 = caja_cargar_archivo("pokemones2.csv"); 
	caja_t *caja3 = caja_combinar(caja1, caja2);
	pa2m_afirmar(caja3 != NULL, "Puedo combinar dos cajas válidas");
	pa2m_afirmar(caja_cantidad(caja3) == caja_cantidad(caja1) + caja_cantidad(caja2), "En la caja combinada hay tantos pokemones como en las otras dos cajas sumadas.");
	pokemon_t *charizard = caja_obtener_pokemon(caja3, 0);
	pa2m_afirmar(charizard != NULL, "Puedo obtener el primer pokemon de la caja combinada.");
	pa2m_afirmar(strcmp(pokemon_nombre(charizard), "Charizard") == 0
				&& pokemon_nivel(charizard) == 5 
				&& pokemon_ataque(charizard) == 20
				&& pokemon_defensa(charizard) == 30, "Los datos del pokemon son correctos");

	caja_destruir(caja1);
	caja_destruir(caja2);
	caja_destruir(caja3);
}

void pokemon_funcion(pokemon_t *pokemon){
	return;
}

void probarRecorridoCaja()
{
	caja_t *caja1 = caja_cargar_archivo("pokemones.csv"); 
	pa2m_afirmar(caja_recorrer(caja1, pokemon_funcion) == caja_cantidad(caja1), "Recorro una caja y me devuelve la cantidad de pokemones en la caja.");

	caja_destruir(caja1);
}

void pruebasNULLDevuelvenErrorCajas()
{
	pokemon_t *pokemon = pokemon_crear_desde_string(NULL);
	pa2m_afirmar(pokemon == NULL, "No puedo crear un pokemon con un string inválido.");

	pa2m_afirmar(pokemon_nombre(pokemon) == NULL, "No puedo obtener el nombre de un pokemon inválido.");
	pa2m_afirmar(pokemon_nivel(pokemon) == 0, "El nivel de un pokemon inválido es 0.");
	pa2m_afirmar(pokemon_ataque(pokemon) == 0, "El poder de ataque de un pokemon inválido es 0.");
	pa2m_afirmar(pokemon_defensa(pokemon) == 0, "El poder de defensa de un pokemon inválido es 0.");

	pa2m_afirmar(caja_cargar_archivo("archivo") == NULL, "No puedo crear una caja con un archivo inválido");
	pa2m_afirmar(caja_guardar_archivo(NULL, "pokemones.csv") == 0, "Guardar una caja inválida devuelve 0.");

	caja_t *caja1 = caja_cargar_archivo("pokemones.csv"); 
	pa2m_afirmar(caja_guardar_archivo(caja1, NULL) == 0, "Guardar una caja con un nombre de archivo inválido devuelve 0.");

	pa2m_afirmar(caja_combinar(caja1, NULL) == NULL, "No puedo combinar una caja con otra inválida.");
	pa2m_afirmar(caja_combinar(NULL, caja1) == NULL, "No puedo combinar una caja inválida con otra.");
	pa2m_afirmar(caja_combinar(NULL, NULL) == NULL, "No puedo combinar dos cajas inválidas.");
	
	pa2m_afirmar(caja_recorrer(NULL, pokemon_funcion) == 0, "Recorrer una caja inválida devuelve 0.");
	pa2m_afirmar(caja_recorrer(caja1, NULL) == 0, "Recorrer una caja con una función inválida devuelve 0.");

	caja_destruir(caja1);
}

/*
 * PRE: s debe apuntar a algo válido.
 * POST: Devuelve un puntero a char con el mismo contenido que s. 
 *       Si no se puede reservar memoria para este, se devuelve NULL.
 */ 
char *duplicate_string(const char *s)
{
	if (!s)
		return NULL;

	char *p = malloc(strlen(s) + 1);
	if (!p)
		return NULL;

	strcpy(p, s);
	return p;
}

void probarInicializacionGestor()
{
	char* nombres_archivos[MAX_NOMBRES];	
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 0);
	pa2m_afirmar(gestor == NULL, "No puedo crear un gestor sin nombres de archivos.");

	nombres_archivos[0] = duplicate_string("poke.csv");
	gestor = gestor_inicializar(nombres_archivos, 1); 
	pa2m_afirmar(gestor == NULL, "No puedo crear un gestor con un nombre de archivo inexistente.");

	nombres_archivos[1] = duplicate_string("pokemones.csv");
	gestor = gestor_inicializar(nombres_archivos, 2); 
	pa2m_afirmar(gestor != NULL, "Puedo crear un gestor con un nombre de un archivo inexistende y otro válido.");
	pa2m_afirmar(!gestor_finalizo(gestor), "La gestión está iniciada.");
	pa2m_afirmar(gestor_cantidad(gestor) == 1, "El gestor tiene una caja cargada.");
	gestor_destruir(gestor);

	nombres_archivos[2] = duplicate_string("pokemones2.csv");
	gestor = gestor_inicializar(nombres_archivos, 3);
	pa2m_afirmar(gestor != NULL, "Puedo crear un gestor con un nombre de un archivo inexistente y dos válidos.");
	pa2m_afirmar(!gestor_finalizo(gestor), "La gestión está iniciada.");
	pa2m_afirmar(gestor_cantidad(gestor) == 2, "El gestor tiene dos cajas cargadas.");
	gestor_destruir(gestor);

	for(int i = 0; i < 3; i++)
		free(nombres_archivos[i]);
}

void probarMuestraInventarioGestor()
{
	pa2m_afirmar(1, "Creo un gestor válido");
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 1);
	pa2m_afirmar(gestor_mostrar_inventario(gestor) == 1, "Mostrar el inventario de un gestor con una caja cargada me devuelve 1.");
	gestor_destruir(gestor);

	nombres_archivos[1] = duplicate_string("pokemones2.csv");
	gestor = gestor_inicializar(nombres_archivos, 2);
	pa2m_afirmar(gestor_mostrar_inventario(gestor) == 2, "Mostrar el inventario de un gestor con dos cajas cargadas me devuelve 2.");

	gestor_destruir(gestor);

	for(int i = 0; i < 2; i++)
		free(nombres_archivos[i]);
}

char* devolver_nombre_archivo_existente(char *linea)
{
	strcpy(linea, "pokemones2.csv");
	return linea;
}

char* devolver_nombre_archivo_inexistente(char *linea)
{
	strcpy(linea, "poketete.csv");
	return linea;
}



void probarCargaCajasGestor()
{
	
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 1);
	pa2m_afirmar(1, "Se creo un gestor con una caja cargada.");
	pa2m_afirmar(gestor_cargar_caja(gestor, devolver_nombre_archivo_existente) != NULL, "Puedo cargar una caja si la función lee un nombre de un archivo existente.");
	pa2m_afirmar(gestor_cantidad(gestor) == 2, "EL gestor ahora tiene dos cajas cargadas");
	pa2m_afirmar(gestor_mostrar_caja(gestor, devolver_nombre_archivo_existente) == 4, "Mostrar la caja cargada me devuelve la cantidad de pokemones que tiene la caja.");

	pa2m_afirmar(gestor_cargar_caja(gestor, devolver_nombre_archivo_existente) == NULL, "No puedo volver a cargar una caja ya cargada en el gestor.");
	pa2m_afirmar(gestor_cargar_caja(gestor, devolver_nombre_archivo_inexistente) == NULL, "No puedo cargar una caja si la función lee un nombre de un archivo inexistente.");
	
	gestor_destruir(gestor);
	for(int i = 0; i < 1; i++)
		free(nombres_archivos[i]);
}

char* devolver_primer_nombre_archivo_inexistente(char *linea)
{
	strcpy(linea, "poketete.csv\npokemones2.csv\npoke3.csv");
	return linea;
}

char* devolver_segundo_nombre_archivo_inexistente(char *linea)
{
	strcpy(linea, "pokemones.csv\npoketete.csv\npoke3.csv");
	return linea;
}

char* devolver_nombres_archivos_existentes(char *linea)
{
	strcpy(linea, "pokemones.csv\npokemones2.csv\npoke3.csv");
	return linea;
}

char* devolver_nombre_archivo_existente2(char *linea)
{
	strcpy(linea, "poke3.csv");
	return linea;
}

void probarCombinacionCajasGestor()
{
	pa2m_afirmar(1, "Creo un gestor válido con dos cajas cargadas");
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	nombres_archivos[1] = duplicate_string("pokemones2.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 2);

	pa2m_afirmar(gestor_combinar_cajas(gestor, devolver_primer_nombre_archivo_inexistente) == NULL, "No puedo combinar cajas si la función lee primero un nombre de un archivo inexistente.");
	pa2m_afirmar(gestor_cantidad(gestor) == 2, "El gestor sigue teniendo dos cajas cargadas.");
	pa2m_afirmar(gestor_combinar_cajas(gestor, devolver_segundo_nombre_archivo_inexistente) == NULL, "No puedo combinar cajas si la función lee segundo un nombre de un archivo inexistente.");
	pa2m_afirmar(gestor_cantidad(gestor) == 2, "El gestor sigue teniendo dos cajas cargadas.");
	pa2m_afirmar(gestor_combinar_cajas(gestor, devolver_nombres_archivos_existentes) != NULL, "Puedo combinar cajas si la función lee nombres de archivos existentes y el tercero no está cargado.");
	pa2m_afirmar(gestor_cantidad(gestor) == 3, "El gestor ahora tiene tres cajas cargadas.");
	pa2m_afirmar(gestor_mostrar_caja(gestor, devolver_nombre_archivo_existente2) == 10, "Mostrar la caja combinadaa me devuelve la suma de la cantidad de pokemones que tienen las otras dos cajas.");
	pa2m_afirmar(gestor_combinar_cajas(gestor, devolver_nombres_archivos_existentes) == NULL, "No puedo combinar cajas si la función lee tercero un nombre de una caja que está cargada.");
	pa2m_afirmar(gestor_cantidad(gestor) == 3, "El gestor sigue teniendo tres cajas cargadas.");

	gestor_destruir(gestor);
	for(int i = 0; i < 2; i++)
		free(nombres_archivos[i]);
}

char* devolver_nombre_pokemon(char *linea)
{
	strcpy(linea, "Charmander");
	return linea;
}

char* devolver_nombre_pokemon2(char *linea)
{
	strcpy(linea, "Pity");
	return linea;
}

char* devolver_nombre_pokemon3(char *linea)
{
	strcpy(linea, "Pikachu");
	return linea;
}

char* devolver_nombre_pokemon4(char *linea)
{
	strcpy(linea, "Squirtle");
	return linea;
}

void probarBusquedaCajasGestor()
{
	pa2m_afirmar(1, "Creo un gestor válido con dos cajas.");
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	nombres_archivos[1] = duplicate_string("pokemones2.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 2);

	pa2m_afirmar(gestor_buscar_caja(gestor, devolver_nombre_pokemon) == 1, "Buscar un pokemon que se encuentra en una caja devuelve 1.");
	pa2m_afirmar(gestor_buscar_caja(gestor, devolver_nombre_pokemon2) == 1, "Buscar un pokemon que se encuentra repetido en una caja devuelve 1.");
	pa2m_afirmar(gestor_buscar_caja(gestor, devolver_nombre_pokemon3) == 2, "Buscar un pokemon que se encuentra dos cajas devuelve 2.");
	pa2m_afirmar(gestor_buscar_caja(gestor, devolver_nombre_pokemon4) == 0, "Buscar un pokemon que no se encuentra en ninguna caja devuelve 0.");

	gestor_destruir(gestor);
	for(int i = 0; i < 2; i++)
		free(nombres_archivos[i]);
}

void probarFinalizacion()
{
	pa2m_afirmar(1, "Creo un gestor válido.");
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	nombres_archivos[1] = duplicate_string("pokemones2.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 2);
	pa2m_afirmar(!gestor_finalizo(gestor), "La gestión no figura como finalizada");
	gestor_salir(gestor);
	pa2m_afirmar(gestor_finalizo(gestor), "Finalizo la gestión y figura como finalizada");

	gestor_destruir(gestor);
	for(int i = 0; i < 2; i++)
		free(nombres_archivos[i]);
}



void pruebasNULLDevuelvenErrorGestor()
{
	pa2m_afirmar(gestor_inicializar(NULL, 100) == NULL, "No puedo crear un gestor sin un vector de nombres de archivos válido.");
	pa2m_afirmar(gestor_mostrar_inventario(NULL) == 0, "Mostrar el inventario de un gestor inválido devuelve 0.");
	pa2m_afirmar(gestor_cargar_caja(NULL, devolver_nombre_archivo_existente) == NULL, "No puedo cargar una caja en un gestor inválido,");
	pa2m_afirmar(gestor_combinar_cajas(NULL, devolver_nombre_archivo_existente) == NULL, "No puedo combinar cajas de un gestor inválido,");
	pa2m_afirmar(gestor_mostrar_caja(NULL, devolver_nombre_archivo_existente) == 0, "Mostrar el contenido de una caja de un gestor inválido devuelve 0.");
	pa2m_afirmar(gestor_buscar_caja(NULL, devolver_nombre_pokemon) == 0, "Buscar un pokemon en un gestor inválido devuelve 0.");
	pa2m_afirmar(gestor_cantidad(NULL) == 0, "Un gestor inválido tiene 0 cajas.");
	pa2m_afirmar(gestor_finalizo(NULL) == true, "La gestión de un gestor inválido está finalizada");

	pa2m_afirmar(1, "Creo un gestor válido");
	char* nombres_archivos[MAX_NOMBRES];	
	nombres_archivos[0] = duplicate_string("pokemones.csv");
	nombres_archivos[1] = duplicate_string("pokemones2.csv");
	gestor_t *gestor = gestor_inicializar(nombres_archivos, 2);
	pa2m_afirmar(gestor_cargar_caja(gestor, NULL) == NULL, "No puedo cargar una caja con una función de lectura inválida.");
	pa2m_afirmar(gestor_combinar_cajas(gestor, NULL) == NULL, "No puedo combinar cajas con una función de lectura inválida.");
	pa2m_afirmar(gestor_mostrar_caja(gestor, NULL) == 0, "No puedo mostrar el contenido de una caja con una función de lectura inválida.");
	pa2m_afirmar(gestor_buscar_caja(gestor, NULL) == 0, "No puedo buscar un pokemon con una función de lectura inválida.");


	gestor_destruir(gestor);
	for(int i = 0; i < 2; i++)
		free(nombres_archivos[i]);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas TDA CAJA");

	pa2m_nuevo_grupo("Pruebas de creación de pokemon");
	probarCreacionPokemon();
	
	pa2m_nuevo_grupo("Pruebas de creación de cajas");
	probarCreacionCaja();

	pa2m_nuevo_grupo("Pruebas de guardado de cajas");
	probarGuardadoCaja();

	pa2m_nuevo_grupo("Pruebas de combinación de cajas");
	probarCombinadoCaja();

	pa2m_nuevo_grupo("Pruebas de recorrido de cajas");
	probarRecorridoCaja();

	pa2m_nuevo_grupo("Pruebas con NULL pokemones y cajas");
	pruebasNULLDevuelvenErrorCajas();

	pa2m_nuevo_grupo("Pruebas TDA GESTOR");

	pa2m_nuevo_grupo("Pruebas de inicialización de gestor");
	probarInicializacionGestor();

	pa2m_nuevo_grupo("Pruebas de mostrar inventario");
	probarMuestraInventarioGestor();

	pa2m_nuevo_grupo("Pruebas de cargar cajas");
	probarCargaCajasGestor();

	pa2m_nuevo_grupo("Pruebas de combinar cajas");
	probarCombinacionCajasGestor();

	pa2m_nuevo_grupo("Pruebas de buscar cajas");
	probarBusquedaCajasGestor();

	pa2m_nuevo_grupo("Pruebas de finalización de gestión");
	probarFinalizacion();

	pa2m_nuevo_grupo("Pruebas con NULL pokemones y gestor");
	pruebasNULLDevuelvenErrorGestor();

	return pa2m_mostrar_reporte();
}