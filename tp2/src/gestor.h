#ifndef __GESTOR_H__
#define __GESTOR_H__

#include <stdbool.h>
#include <stddef.h>

typedef struct gestor gestor_t;

/**
 * Crea el gestor reservando la memoria necesaria para él
 * 
 * nombres_archivos son los nombres de los archivos con los que se crea el gestor
 * y cantidad es la cantidad de archivos.
 * 
 * Se cargan en el gestor las cajas que tengan los nombres recibidos
 * (cada caja queda inicializada con todos los pokemon del
 * archivo csv ORDENADOS ALFABETICAMENTE por nombre.). Se imprime por 
 * pantalla un mensaje de èxito por cada caja cargada.
 * 
 * Si algún archivo no existe, es inválido o no se puede reservar memoria, no se 
 * carga la caja y se imprime por pantalla un mensaje de error.
 * 
 * Devuelve un puntero al gestor creado o NULL en caso de no poder crearlo o si
 * no se pudo cargar ninguna caja.
 * 
 */
gestor_t *gestor_inicializar(char **nombres_archivos, int cantidad);

/**
 *
 * Imprime por pantalla el listado de cajas disponibles y
 * la cantidad de pokemones en cada caja.
 *
 * Devuelve la cantidad de cajas que fueron mostradas por pantalla o 
 * 0 en caso de error.
 * 
 */
size_t gestor_mostrar_inventario(gestor_t *gestor);

/**
 * 
 * Se carga en el gestor una caja con el nombre que devuelva
 * la función recibida.
 * 
 * Devuelve el gestor o NULL en caso de error (se considera
 * como error si se falló al reservar memoria, si el nombre del archivo 
 * no corresponde a un archivo existente, si ya existe una caja con ese
 * nombre o si el formato del archivo es incorrecto o si ya existe una caja 
 * con ese nombre).
 * 
 */
gestor_t *gestor_cargar_caja(gestor_t *gestor, 
								char* (*leer_texto)(char*));

/**
 * 
 * Se combinan dos cajas del gestor y se guarda en el gestor. 
 * Genera un archivo .csv con el contenido de la caja combinada.
 * Las cajas se combinan segun en texto que devuelve la funcion leer_texto, 
 * el cual debe tener el formato "caja1.csv\ncaja2.csv\nnombre_archivo.csv\n"
 * 
 * 
 * Devuelve el gestor o NULL en caso de error (se considera
 * como error si se falló al reservar memoria, si no hay cajas con los primeros
 * dos nombres o si ya existe una caja con el nombre del archivo).
 * 
 */
gestor_t *gestor_combinar_cajas(gestor_t *gestor, 
								char* (*leer_texto)(char*));

/**
 * 
 * Se muestra por pantalla el contenido de una caja con el nombre que devuelva
 * la función recibida.
 * 
 * Devuelve la cantidad de pokemones mostrados.
 * 
 */
int gestor_mostrar_caja(gestor_t *gestor, char* (*leer_texto)(char*)); 

/**
 * 
 * Se muestra por pantalla las cajas que contienen al pokemon con el nombre que devuelva
 * la función recibida.
 * 
 * Devuelve la cantidad de cajas mostradas.
 * 
 */
size_t gestor_buscar_caja(gestor_t *gestor, char* (*leer_texto)(char*));

/**
 * 
 * Devuelve la cantidad de cajas almacenadas en el gestor o 0 en caso de error.
 * 
 */
size_t gestor_cantidad(gestor_t *gestor);

/**
 * 
 * Devuelve true si la gestión finalizó o false en caso contrario o de error.
 * 
 */
bool gestor_finalizo(gestor_t *gestor);

/**
 * 
 * Finaliza la gestión.
 * 
 */
void gestor_salir(gestor_t *gestor);

/**
 * 
 * Destruye un gestor liberando toda la memoria asociada a él.
 * 
 */
void gestor_destruir(gestor_t *gestor);


#endif /* __GESTOR_H__ */