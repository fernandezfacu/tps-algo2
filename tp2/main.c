#include "src/gestor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ERROR -1
#define CANTIDAD_COMANDOS 6
#define MAX_LINEA 100
#define CARACTER_NULO 0
#define SALTO_LINEA "\n"


struct comando{
	char instruccion;
	const char *descripcion;
	void (*ejecutar)(struct gestor *);
};

/**
 * PRE: Recibe un puntero válido.
 * POST: Pide al usuario que ingrese texto y devuelve un
 * 	     string con lo ingresado o un puntero al 
 * 		 caracter nulo en caso de error.
 */
char* leer_linea(char *linea)
{
	printf("> ");
	char *leido = fgets(linea, MAX_LINEA, stdin);
	if(!leido)
		linea[0] = CARACTER_NULO;
	else linea[strlen(linea)-1] = CARACTER_NULO;
	return linea;
}

/**
 * PRE: Recibe un puntero válido.
 * POST: Pide al usuario que ingrese una letra y devuelve un
 * 	     caracter con lo ingresado o el caracter nulo
 * 		 si no se ingresó una letra o en caso de error.
 */
char leer_opcion(char opcion)
{
	char linea[MAX_LINEA];
	leer_linea(linea);

	if(strlen(linea) > 1)
		return 0;
	return linea[0];
}

/**
 * PRE: Recibe un puntero válido.
 * POST: Pide al usuario que ingrese texto tres veces y 
 * 		 devuelve un string con lo ingresado, con cada texto 
 * 		 separado por un '\n' o un puntero al caracter nulo 
 * 		 en caso de error.
 */
char* leer_texto(char *lineas)
{
	char linea1[MAX_LINEA];   
	leer_linea(linea1);
	char linea2[MAX_LINEA];
	leer_linea(linea2);
	char linea3[MAX_LINEA];
	leer_linea(linea3);

	strcat(linea1, SALTO_LINEA);
	strcat(linea1, linea2);
	strcat(linea1, SALTO_LINEA);
	strcat(linea1, linea3);
	strcat(linea1, SALTO_LINEA);

	strcpy(lineas, linea1);

	return lineas;
}

void mostrar_inventario(gestor_t *gestor)
{
	printf("CAJAS: \n\n");
	gestor_mostrar_inventario(gestor);
}

void cargar_caja(gestor_t *gestor)
{
	printf("Ingrese el nombre de la caja que quiere cargar: \n");
	if(!gestor_cargar_caja(gestor, leer_linea))
		printf("\nNo se pudo cargar la caja. Por favor chequee si el nombre de archivo o el formato del mismo es correcto (o si ya hay una caja con el nombre ingresado).\nPuede deberse también a un error en la reserva de memoria.\n");
	else printf("\nLa caja fue cargada exitosamente.\n");
}

void combinar_cajas(gestor_t *gestor)
{
	printf("Ingrese los nombre de las dos cajas que quiere combinar y posteriormente el nombre con el que quiere cargar la caja combinada:\n");
	if(!gestor_combinar_cajas(gestor, leer_texto))
		printf("\nHubo un problema al leer los datos ingresados. Asegurese de ingresar los nombres correctamente (o si ya hay una caja con el nombre últimamente ingresado).\nPuede deberse también a un error en la reserva de memoria.\n");
	else printf("\nLa caja fue combinada y cargada correctamente.\n");
}

void mostrar_caja(gestor_t *gestor)
{
	printf("Ingrese el nombre de la caja que quiere ver: \n");
	if(gestor_mostrar_caja(gestor, leer_linea) == 0)
		printf("\nLa caja no se encuentra cargada.\n");
}

void buscar_caja(gestor_t *gestor)
{
	printf("Ingrese el nombre del pokemon que quiere buscar: \n");
	if(gestor_buscar_caja(gestor, leer_linea) == 0)
		printf("\nEl pokemon ingresado no se encuentra en ninguna caja. \n");
}

void salir(gestor_t *gestor)
{
	printf("Saliendo...\n");
	gestor_salir(gestor);
}

bool ejecutar_accion(struct comando menu[], int cantidad_comandos, 
										char letra, struct gestor *gestor)
{
	letra = (char) toupper(letra);
	for(int i = 0; i < cantidad_comandos; i++){
		if(menu[i].instruccion == letra){
			menu[i].ejecutar(gestor);
			return true;
		}
	}
	
	return false;
}

void mostrar_menu(struct comando menu[], int cantidad_comandos)
{
	printf("Ingrese una de las siguientes instrucciones: \n");
	for(int i = 0; i < cantidad_comandos; i++)
		printf("\t%c - %s\n", menu[i].instruccion, menu[i].descripcion);
}

int main(int argc, char* argv[]) 
{
	struct comando menu[] = {
		{ 'I', "Mostrar Inventario", mostrar_inventario },
		{ 'C', "Cargar otra caja", cargar_caja },
		{ 'M', "Combinar cajas", combinar_cajas }, 
		{ 'D', "Mostrar caja", mostrar_caja },
		{ 'B', "Buscar caja", buscar_caja },
		{ 'Q', "Salir", salir }
	};
	
	struct gestor *gestor = gestor_inicializar(argv+1, argc-1);
	if(!gestor){
		printf("\nNo se pudo cargar ninguna caja o hubo un problema en la reserva de memoria.\n\n");
		return ERROR;
	} 
	
	while(!gestor_finalizo(gestor)){
		mostrar_menu(menu, CANTIDAD_COMANDOS);

		char opcion = leer_opcion(opcion);

		printf("\n");
		if(!ejecutar_accion(menu, CANTIDAD_COMANDOS, opcion, gestor))
			printf("Esta opción no existe\n");	
		printf("\n");
	}

	gestor_destruir(gestor);
	
	return 0;
}
