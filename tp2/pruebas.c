#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pa2m.h"
#include "src/pokemon.h"


void pruebas_lista_vacia()
{
	lista_t *lista = lista_crear();
	pa2m_afirmar(lista != NULL, "Crear lista devuelve una lista no nula");
	pa2m_afirmar(lista_vacia(lista), "Crear lista devuelve una lista vacía");
	pa2m_afirmar(lista_tamanio(lista) == 0, "Crear lista devuelve una lista de tamaño 0");
	pa2m_afirmar(lista_primero(lista) == NULL, "Crear lista devuelve una lista sin primer elemento");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "Crear lista devuelve una lista sin último elemento");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == NULL, "Pedir una posición arbitraria a una lista devuelve NULL");
	pa2m_afirmar(lista_quitar(lista) == NULL, "Quitar un elemento del final de una lista vacia devuelve NULL");
	pa2m_afirmar(lista_quitar_de_posicion(lista, 0) == NULL, "Quitar un elemento de una posición arbitraria a una lista vacia devuelve NULL");

	lista_destruir(lista);
}

void pruebas_insercion_al_final()
{
	lista_t *lista = lista_crear();

	int numeros[5];
	for(int i = 0; i < 5; i++)
		numeros[i] = i+1;


	pa2m_afirmar(lista_insertar(lista, &numeros[0]) == lista, "Insertar un elemento devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 1, "La lista tiene tamaño 1");
	pa2m_afirmar(lista_primero(lista) == &numeros[0], "El primer elemento es el mismo");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[0], "El último elemento es el correcto");

	pa2m_afirmar(lista_insertar(lista, &numeros[1]) == lista, "Insertar otro elemento devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 2, "La lista tiene tamaño 2");
	pa2m_afirmar(lista_primero(lista) == &numeros[0], "El primer elemento sigue siendo el mismo");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[1], "El último elemento es el correcto");

	pa2m_afirmar(lista_insertar(lista, &numeros[2]) == lista, "Insertar otro elemento devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 3, "La lista tiene tamaño 3");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == &numeros[1], "El segundo elemento es el correcto");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[2], "El último elemento es el correcto");

	lista_destruir(lista);
}

void pruebas_insercion_pos_arbitraria()
{
	lista_t *lista = lista_crear();

	int numeros[7];
	for(int i = 0; i < 7; i++)
		numeros[i] = i+1;
	for(int i = 0; i < 3; i++)
		lista_insertar(lista, &numeros[i]);
	
	pa2m_afirmar(lista_insertar_en_posicion(lista, &numeros[3], 0) == lista, "Insertar un elemento en la primera posición devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 4, "La lista tiene tamaño 4");
	pa2m_afirmar(lista_primero(lista) == &numeros[3], "Se agrega el elemento en la primera posición");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[2], "El último elemento sigue siendo el mismo");

	pa2m_afirmar(lista_insertar_en_posicion(lista, &numeros[4], 2) == lista, "Insertar un elemento en la tercera posición devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 5, "La lista tiene tamaño 5");
	pa2m_afirmar(lista_primero(lista) == &numeros[3], "El primer elemento sigue siendo el mismo");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[2], "El último elemento sigue siendo el mismo");

	pa2m_afirmar(lista_insertar_en_posicion(lista, &numeros[5], 100) == lista, "Insertar un elemento en una posición inexistente devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 6, "La lista tiene tamaño 6");
	pa2m_afirmar(lista_primero(lista) == &numeros[3], "El primer elemento sigue siendo el mismo");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[5], "El último elemento es el insertado");

	pa2m_afirmar(lista_insertar_en_posicion(lista, &numeros[6], 6) == lista, "Insertar un elemento en la sexta posición devuelve la misma lista");
	pa2m_afirmar(lista_tamanio(lista) == 7, "La lista tiene tamaño 7");
	pa2m_afirmar(lista_primero(lista) == &numeros[3], "El primer elemento sigue siendo el mismo");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[6], "El último elemento es el insertado");

	lista_destruir(lista);
}

void pruebas_obtencion_elementos()
{
	lista_t *lista = lista_crear();

	pokemon_t **pokemones = malloc(3*sizeof(pokemon_t *));
	pokemones[0] = pokemon_crear_desde_string("Pikachu;7;30;20");
	pokemones[1] = pokemon_crear_desde_string("Pity;9;12;18");
	pokemones[2] = pokemon_crear_desde_string("Charmander;4;20;5");
	lista_insertar(lista, pokemones[0]);
	lista_insertar(lista, pokemones[1]);
	lista_insertar(lista, pokemones[2]);
	
	
	pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == pokemones[0], "El primer elemento es un Pikachu");
	
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == pokemones[1], "El segundo elemento es un Pity");
	
	pa2m_afirmar(lista_elemento_en_posicion(lista, 2) == pokemones[2], "El último elemento es un Charmander");
	
	pa2m_afirmar(lista_elemento_en_posicion(lista, 200) == NULL, "No existe el elemento 200 en una lista de tamaño 3");

	pokemon_t *pokemon_a_destruir = lista_quitar_de_posicion(lista, 0); 
	pa2m_afirmar(lista_elemento_en_posicion(lista, 0) != pokemones[0], "Se elimina el primer elemento y ya no es un Pikachu");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == pokemones[1], "El primer elemento es ahora un Pity");
	pokemon_destruir(pokemon_a_destruir);

	lista_destruir_todo(lista, pokemon_destruir);
}

void pruebas_borrado_al_final()
{
	lista_t *lista = lista_crear();

	int numeros[6];
	for(int i = 0; i < 6; i++)
		numeros[i] = i+1;
	
	lista_insertar(lista, &numeros[0]);

	pa2m_afirmar(lista_quitar(lista) == &numeros[0], "Quitar un elemento del final de una lista de tamaño 1 devuelve el elemento eliminado");
	pa2m_afirmar(lista_tamanio(lista) == 0, "La lista tiene tamaño 0");

	for(int i = 0; i < 6; i++)
		lista_insertar(lista, &numeros[i]);
	for(int i = 0; i < 6; i++)
		lista_quitar(lista);
	pa2m_afirmar(lista_tamanio(lista) == 0, "Agrego muchos elementos y al eliminarlos del final la lista tiene tamaño 0");

	lista_destruir(lista);
}

void pruebas_borrado_pos_arbitraria()
{
	lista_t *lista = lista_crear();

	int numeros[6];
	for(int i = 0; i < 6; i++){
		numeros[i] = i+1;
		lista_insertar(lista, &numeros[i]);
	}

	pa2m_afirmar(lista_quitar_de_posicion(lista, 0) == &numeros[0], "Quitar un elemento de la primera posición de una lista de tamaño 6 devuelve el elemento eliminado");
	pa2m_afirmar(lista_tamanio(lista) == 5, "La lista ahora tiene tamaño 5");
	pa2m_afirmar(lista_primero(lista) == &numeros[1], "El primer elemento es un 2");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == &numeros[2], "El segundo elemento es un 3");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[5], "El último elemento sigue siendo un 6");

	pa2m_afirmar(lista_quitar_de_posicion(lista, 1) == &numeros[2], "Quitar un elemento de la segunda posición devuelve el elemento eliminado");
	pa2m_afirmar(lista_tamanio(lista) == 4, "La lista tiene tamaño 4");
	pa2m_afirmar(lista_primero(lista) == &numeros[1], "El primer elemento sigue siendo 2");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == &numeros[3], "El segundo elemento es un 4");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[5], "El último elemento sigue siendo un 6");

	pa2m_afirmar(lista_quitar_de_posicion(lista, 100) == &numeros[5], "Quitar un elemento de una posición inexistente elimina el último elemento");
	pa2m_afirmar(lista_tamanio(lista) == 3, "La lista tiene tamaño 3");
	pa2m_afirmar(lista_primero(lista) == &numeros[1], "El primer elemento sigue siendo un 2");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == &numeros[3], "El segundo elemento sigue siendo un 4");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[4], "El último es ahora un 5");
	
	pa2m_afirmar(lista_quitar_de_posicion(lista, 2) == &numeros[4], "Quitar un elemento de la última posición devuelve el elemento eliminado");
	pa2m_afirmar(lista_tamanio(lista) == 2, "La lista tiene tamaño 2");
	pa2m_afirmar(lista_primero(lista) == &numeros[1], "El primer elemento sigue siendo un 2");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == &numeros[3], "El segundo elemento sigue siendo un 4");
	pa2m_afirmar(lista_ultimo(lista) == &numeros[3], "El último es un 4");

	lista_quitar(lista);

	pa2m_afirmar(lista_quitar_de_posicion(lista, 2) == &numeros[1], "Quitar un elemento de una lista de tamaño 1 devuelve el elemento eliminado");
	pa2m_afirmar(lista_tamanio(lista) == 0, "La lista tiene tamaño 0");
	pa2m_afirmar(lista_primero(lista) == NULL, "No hay primer elemento");
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == NULL, "No hay segundo elemento");
	pa2m_afirmar(lista_ultimo(lista) == NULL, "No hay último elemento");

	lista_destruir(lista);
}

int es_el_mismo_numero(void *numero_1, void *numero_2)
{
	if(*(int*)(numero_1) == *(int*)(numero_2))
		return 0;
	else return -1;
}

void pruebas_busqueda()
{
	lista_t *lista = lista_crear();

	int numeros[7];
	for(int i = 0; i < 6; i++){
		numeros[i] = i+1;
		lista_insertar(lista, &numeros[i]);
	}
	numeros[6] = 1;

	pa2m_afirmar(lista_buscar_elemento(lista, es_el_mismo_numero, &numeros[3]) == &numeros[3], "El 4 está en la lista y se devuelve el mismo");

	pa2m_afirmar(lista_buscar_elemento(lista, es_el_mismo_numero, &numeros[0]) == &numeros[0], "El 1 se encuentra dos veces en la lista. Devuelve el primero encontrado");
	pa2m_afirmar(lista_buscar_elemento(lista, es_el_mismo_numero, &numeros[0]) != &numeros[6], "El 1 se encuentra dos veces en la lista. No devuelve el que aparece segundo");

	lista_quitar_de_posicion(lista, 2);
	pa2m_afirmar(lista_buscar_elemento(lista, es_el_mismo_numero, &numeros[2]) == NULL, "El 3 fue eliminado de la lista y no se encuentra al mismo");

	lista_destruir(lista);
}

bool es_el_mismo_que(void *numero_1, void *contexto)
{
	if(*(int*)(numero_1) == *(int*)(contexto))
		return false;
	else return true;
}

void pruebas_iterador_interno()
{
	lista_t *lista = lista_crear();

	int numeros[7];
	for(int i = 0; i < 7; i++){
		numeros[i] = i+1;
		lista_insertar(lista, &numeros[i]);
	}

	pa2m_afirmar(lista_con_cada_elemento(lista, NULL, lista_primero(lista)) == 0, "No puedo iterar sin función de iteración");
	pa2m_afirmar(lista_con_cada_elemento(lista, es_el_mismo_que, lista_primero(lista)) == 1, "Busco el primer elemento de la lista y devuelve que iteré sobre un elemento");
	pa2m_afirmar(lista_con_cada_elemento(lista, es_el_mismo_que, lista_elemento_en_posicion(lista, 3)) == 4, "Busco el cuarto elemento de la lista y devuelve que iteré sobre cuatro elementos");
	pa2m_afirmar(lista_con_cada_elemento(lista, es_el_mismo_que, lista_ultimo(lista)) == lista_tamanio(lista), "Busco el último elemento de la lista y devuelve que iteré sobre todos los elementos de la lista");

	lista_destruir(lista);
}

void pruebas_iterador_externo()
{
	lista_t *lista = lista_crear();

	lista_iterador_t *iterador_lista_vacia = lista_iterador_crear(lista);
	pa2m_afirmar(iterador_lista_vacia != NULL, "Puedo crear un iterador con una lista vacía");
	pa2m_afirmar(!lista_iterador_elemento_actual(iterador_lista_vacia), "Elemento actual del iterador de lista es NULL");
	pa2m_afirmar(!lista_iterador_tiene_siguiente(iterador_lista_vacia), "Iterador de lista vacía no tiene siguiente");
	pa2m_afirmar(!lista_iterador_avanzar(iterador_lista_vacia), "No puedo avanzar en un iterador de lista vacía");
	pa2m_afirmar(!lista_iterador_elemento_actual(iterador_lista_vacia), "Elemento actual del iterador de lista vacía sigue siendo NULL luego de avanzar");
	pa2m_afirmar(!lista_iterador_tiene_siguiente(iterador_lista_vacia), "Iterador de lista vacía no tiene siguiente luego de avanzar");
	
	lista_iterador_destruir(iterador_lista_vacia);

	int numeros[7];
	for(int i = 0; i < 7; i++){
		numeros[i] = i+1;
		lista_insertar(lista, &numeros[i]);
	}

	lista_iterador_t *iterador = lista_iterador_crear(lista);
	pa2m_afirmar(iterador != NULL, "Puedo crear un iterador con una lista no vacía");
	pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &numeros[0], "Elemento actual del iterador de lista es el primero de la lista");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador), "Iterador de lista de tamaño mayor a 1 tiene siguiente");
	pa2m_afirmar(lista_iterador_avanzar(iterador), "Puedo avanzar en un iterador de lista de tamaño mayor a 1");

	lista_iterador_destruir(iterador);


	lista_iterador_t *it = NULL;
	size_t contador = 0;

	for (it = lista_iterador_crear(lista);
	     lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)) {
		contador++;
	}

	pa2m_afirmar(contador == lista_tamanio(lista), "Se iteran la misma cantidad de elementos que los de la lista");
	pa2m_afirmar(!lista_iterador_elemento_actual(it), "Elemento actual del iterador una lista de tamaño 7 es el NULL luego de avanzar 7 veces");

	lista_iterador_destruir(it);

	lista_destruir(lista);
}

void casos_nullEsError()
{
	void *elemento = NULL;
	void *contexto = NULL;

	pa2m_afirmar(lista_tamanio(NULL) == 0, "Lista nula tiene tamaño 0");
	pa2m_afirmar(lista_vacia(NULL), "Lista nula es vacía");
	
	pa2m_afirmar(!lista_primero(NULL), "Lista nula no tiene primer elemento");
	pa2m_afirmar(!lista_ultimo(NULL), "Lista nula no tiene último elemento");
	pa2m_afirmar(!lista_elemento_en_posicion(NULL, 7), "Lista nula no tiene elemento en cualquier posición");
	pa2m_afirmar(!lista_quitar(NULL), "No puedo quitar un elemento de una lista nula");
	pa2m_afirmar(!lista_quitar_de_posicion(NULL, 0), "No puedo quitar un elemento de la primera posición de una lista nula");
	pa2m_afirmar(!lista_quitar_de_posicion(NULL, 7), "No puedo quitar un elemento de cualquiier posición de una lista nula");
	pa2m_afirmar(!lista_insertar(NULL, elemento), "No puedo insertar un elemento al final de una lista nula");
	pa2m_afirmar(!lista_insertar_en_posicion(NULL, elemento, 0), "No puedo insertar un elemento en la primera posición de una lista nula");
	pa2m_afirmar(!lista_insertar_en_posicion(NULL, elemento, 7), "No puedo insertar un elemento en cualquier posición de una lista nula");
	pa2m_afirmar(lista_con_cada_elemento(NULL, es_el_mismo_que, contexto) == 0, "Recorrer una lista nula con iterador interno recorre 0 elementos");
	pa2m_afirmar(!lista_iterador_crear(NULL), "Un iterador de una lista nula es nulo");
}

void pruebas_pila()
{
	pila_t *pila = pila_crear();

	pa2m_afirmar(pila != NULL, "Crear pila devuelve una pila no nula");
	pa2m_afirmar(pila_vacia(pila), "Crear pila devuelve una pila vacía");
	pa2m_afirmar(pila_tamanio(pila) == 0, "Crear pila devuelve una pila de tamaño 0");
	pa2m_afirmar(pila_tope(pila) == NULL, "Crear pila devuelve una pila sin tope");

	int numeros[7];
	for(int i = 0; i < 7; i++)
		numeros[i] = i+1;
	

	pa2m_afirmar(pila_apilar(pila, &numeros[0]) == pila, "Apilar un elemento devuelve la misma pila");
	pa2m_afirmar(pila_tamanio(pila) == 1, "La pila tiene tamaño 1");
	pa2m_afirmar(pila_tope(pila) == (void *) &numeros[0], "El tope es el elemento apilado");

	for(int i = 1; i < 7; i++)
		pila_apilar(pila, &numeros[i]);

	pa2m_afirmar(pila_tamanio(pila) == 7, "Apilo seis elementos más y la pila tiene tamaño 7");
	pa2m_afirmar(pila_tope(pila) == (void *) &numeros[6], "El tope es el último elemento apilado");
	
	pa2m_afirmar(pila_desapilar(pila) == (void *) &numeros[6], "Desapilo un elemento y me devuelve el mismo");
	pa2m_afirmar(pila_tamanio(pila) == 6, "La pila tiene tamaño 6");
	pa2m_afirmar(pila_tope(pila) == (void *) &numeros[5], "El tope es el anteúltimo elemento apilado");

	pila_destruir(pila);
}

void pruebas_cola()
{
	cola_t *cola = cola_crear();

	pa2m_afirmar(cola != NULL, "Crear cola devuelve una cola no nula");
	pa2m_afirmar(cola_vacia(cola), "Crear cola devuelve una cola vacía");
	pa2m_afirmar(cola_tamanio(cola) == 0, "Crear cola devuelve una cola de tamaño 0");
	pa2m_afirmar(cola_frente(cola) == NULL, "Crear cola devuelve una cola sin tope");

	int numeros[7];
	for(int i = 0; i < 7; i++)
		numeros[i] = i+1;
	

	pa2m_afirmar(cola_encolar(cola, &numeros[0]) == cola, "Encolar un elemento devuelve la misma cola");
	pa2m_afirmar(cola_tamanio(cola) == 1, "La cola tiene tamaño 1");
	pa2m_afirmar(cola_frente(cola) == (void *) &numeros[0], "El frente es el elemento encolado (el único de la cola)");

	for(int i = 1; i < 7; i++)
		cola_encolar(cola, &numeros[i]);

	pa2m_afirmar(cola_tamanio(cola) == 7, "Encolo seis elementos más y la cola tiene tamaño 7");
	pa2m_afirmar(cola_frente(cola) == (void *) &numeros[0], "El frente sigue siendo el mismo");
	
	pa2m_afirmar(cola_desencolar(cola) == (void *) &numeros[0], "Desencolo un elemento y me devuelve el mismo");
	pa2m_afirmar(cola_tamanio(cola) == 6, "La cola tiene tamaño 6");
	pa2m_afirmar(cola_frente(cola) == (void *) &numeros[1], "El frente es el segundo elemento encolado");

	cola_destruir(cola);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de lista vacía");
	pruebas_lista_vacia();

	pa2m_nuevo_grupo("Pruebas de inserción de lista");
	pruebas_insercion_al_final();
	
	pa2m_nuevo_grupo("Pruebas de inserción en posición de lista");
	pruebas_insercion_pos_arbitraria();

	pa2m_nuevo_grupo("Pruebas de obtener elementos");
	pruebas_obtencion_elementos();

	pa2m_nuevo_grupo("Pruebas de eliminación de lista");
	pruebas_borrado_al_final();
	pruebas_borrado_pos_arbitraria();

	pa2m_nuevo_grupo("Pruebas de búsqueda de elemento");
	pruebas_busqueda();

	pa2m_nuevo_grupo("Pruebas de iterador interno");
	pruebas_iterador_interno();

	pa2m_nuevo_grupo("Pruebas de iterador externo");
	pruebas_iterador_externo();
	
	pa2m_nuevo_grupo("Pruebas con NULL");
	casos_nullEsError();

	pa2m_nuevo_grupo("Pruebas de pila");
	pruebas_pila();
	
	pa2m_nuevo_grupo("Pruebas de cola");
	pruebas_cola();

	return pa2m_mostrar_reporte();
}