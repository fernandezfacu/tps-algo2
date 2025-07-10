#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2m.h"

typedef struct cosa {
	int clave;
	int numeros[10];
} cosa;

cosa *crear_cosa(int clave)
{
	cosa *c = malloc(sizeof(cosa));
	if (c)
		c->clave = clave;
	return c;
}

void destruir_cosa(void *c)
{
	free(c);
}

int comparar_vectores(void *c1, void *c2)
{
	cosa *cosa1 = c1;
	cosa *cosa2 = c2;
	return cosa1->clave - cosa2->clave;
}

bool iterar_hasta_5(void *elemento, void *extra)
{
	extra = extra; 
	if (elemento) {
		if (((cosa *)elemento)->clave == 5)
			return false;
	}
	return true;
}

bool iterar(void *elemento, void *extra)
{
	return true;
}

int comparar_numeros(void *n1, void *n2)
{
	int *numero1 = n1;
	int *numero2 = n2;
	return *numero1 - *numero2;
}

void abbSeCreaVacio()
{
	abb_t *arbol = abb_crear(comparar_vectores);
	pa2m_afirmar(arbol != NULL, "Crear un árbol con un comparador válido no devuelve un árbol nulo.");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "Se crea un árbol de tamaño 0.");
	int numero = 10;
	pa2m_afirmar(!abb_buscar(arbol, &numero), "No encuentro un elemento en el árbol recién creado.");
	pa2m_afirmar(abb_vacio(arbol), "El árbol se crea vacío.");

	abb_destruir(arbol);
}

void abbInsertarAumentaEltamanio()
{
	cosa *c1 = crear_cosa(1);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c9 = crear_cosa(9);
	cosa *otroc7 = crear_cosa(7);
	cosa *auxiliar = crear_cosa(0);
	
	abb_t *arbol = abb_crear(comparar_vectores);

	pa2m_afirmar(abb_insertar(arbol, c5) == arbol, "Insertar un elemento devuelve el mismo árbol.");
	pa2m_afirmar(!abb_vacio(arbol), "El árbol ya no está vacío.");
	pa2m_afirmar(abb_tamanio(arbol) == 1, "El árbol tiene tamaño 1");
	auxiliar->clave = 5;
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c5, "Busco el elemento en el abb y lo encuentro.");
	
	pa2m_afirmar(abb_insertar(arbol, c1) == arbol, "Puedo insertar un elemento menor que el primero.");
	pa2m_afirmar(abb_tamanio(arbol) == 2, "El árbol tiene tamaño 2");
	auxiliar->clave = 1;
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c1, "Busco el elemento en el abb y lo encuentro.");
	
	pa2m_afirmar(abb_insertar(arbol, c7) == arbol, "Puedo insertar un elemento mayor que el primero.");
	pa2m_afirmar(abb_tamanio(arbol) == 3, "El árbol tiene tamaño 3");
	auxiliar->clave = 7;
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c7, "Busco el elemento en el abb y lo encuentro.");

	pa2m_afirmar(abb_insertar(arbol, otroc7) == arbol, "Puedo insertar un elemento con el mismo valor que otro.");
	pa2m_afirmar(abb_tamanio(arbol) == 4, "El árbol tiene tamaño 4");
	auxiliar->clave = 7;
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c7, "Busco el valor del elemento en el abb y encuentro el que inserté primero.");

	pa2m_afirmar(abb_insertar(arbol, c9) == arbol, "Puedo insertar un elemento mayor que los demás e insertarlo devuelve el mismo árbol.");
	pa2m_afirmar(abb_tamanio(arbol) == 5, "El árbol tiene tamaño 5");
	auxiliar->clave = 9;
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c9, "Busco el elemento en el abb y lo encuentro.");

	destruir_cosa(auxiliar);
	abb_destruir_todo(arbol, destruir_cosa);
}

void abbBuscarElementos()
{
	int numeros[5];
	for(int i = 0; i < 5; i++)
		numeros[i] = i+1;

	int numero_aux = 0;

	abb_t *arbol = abb_crear(comparar_numeros);
	for(int i = 0; i < 5; i++)
		abb_insertar(arbol, &numeros[i]);

	numero_aux = 1;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == &numeros[0], "Busco un elemento que está en el árbol y lo encuentro.");
	numero_aux = 2;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == &numeros[1], "Busco un elemento que está en el árbol y lo encuentro.");
	numero_aux = 3;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == &numeros[2], "Busco un elemento que está en el árbol y lo encuentro.");
	numero_aux = 4;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == &numeros[3], "Busco un elemento que está en el árbol y lo encuentro.");
	numero_aux = 5;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == &numeros[4], "Busco un elemento que está en el árbol y lo encuentro.");
	numero_aux = 10;
	pa2m_afirmar(abb_buscar(arbol, &numero_aux) == NULL, "Busco un elemento que no está en el árbol y devuelve NULL.");

	abb_destruir(arbol);
}

void abbBorrarElementosDisminuyeEltamanio()
{
	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c8 = crear_cosa(8);
	cosa *c9 = crear_cosa(9);
	cosa *auxiliar = crear_cosa(0);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);

	pa2m_afirmar(abb_tamanio(arbol) == 7, "Se está por probar quitar elementos de un árbol de tamaño 7.");
	auxiliar->clave = 3;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c3, "Elimino un nodo hoja y me devuelve el eliminado");
	destruir_cosa(c3);
	pa2m_afirmar(abb_tamanio(arbol) == 6, "El tamaño del árbol es 6.");

	auxiliar->clave = 2;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c2, "Elimino un nodo con un hijo y me devuelve el eliminado");
	destruir_cosa(c2);
	pa2m_afirmar(abb_tamanio(arbol) == 5, "El tamaño del árbol es 5.");

	auxiliar->clave = 8;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c8, "Elimino un nodo con dos hijos y me devuelve el eliminado");
	destruir_cosa(c8);
	pa2m_afirmar(abb_tamanio(arbol) == 4, "El tamaño del árbol es 4.");

	auxiliar->clave = 5;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c5, "Elimino el nodo raíz y me devuelve el eliminado");
	destruir_cosa(c5);
	pa2m_afirmar(abb_tamanio(arbol) == 3, "El tamaño del árbol es 3.");

	auxiliar->clave = 10;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == NULL, "Intento eliminar un elemento que no está en el árbol y devuelve NULL");

	abb_quitar(arbol, c1);
	destruir_cosa(c1);
	abb_quitar(arbol, c7);
	destruir_cosa(c7);
	abb_quitar(arbol, c9);
	destruir_cosa(c9);
	pa2m_afirmar(abb_vacio(arbol), "Se eliminan los nodos restantes y el árbol queda vacío");

	free(auxiliar);
	abb_destruir_todo(arbol, destruir_cosa);
}

void abbBuscarAlInsertaryAlBorrar()
{
	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c8 = crear_cosa(8);
	cosa *c9 = crear_cosa(9);
	cosa *auxiliar = crear_cosa(0);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);

	pa2m_afirmar(abb_tamanio(arbol) == 7, "Se está por probar quitar elementos de un árbol de tamaño 7.");

	auxiliar->clave = 3;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c3, "Elimino un nodo hoja y me devuelve el eliminado");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el árbol");
	pa2m_afirmar(abb_insertar(arbol, c3) == arbol, "Puedo insertar de nuevo el elemento.");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c3, "El elemento se encuentra en el árbol");

	auxiliar->clave = 8;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c8, "Elimino un nodo con dos hijos y me devuelve el eliminado");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el árbol");
	pa2m_afirmar(abb_insertar(arbol, c8) == arbol, "Puedo insertar de nuevo el elemento.");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c8, "El elemento se encuentra en el árbol");

	auxiliar->clave = 5;
	pa2m_afirmar(abb_quitar(arbol, auxiliar) == c5, "Elimino el nodo raiz y me devuelve el eliminado");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == NULL, "El elemento ya no se encuentra en el árbol");
	pa2m_afirmar(abb_insertar(arbol, c5) == arbol, "Puedo insertar de nuevo el elemento.");
	pa2m_afirmar(abb_buscar(arbol, auxiliar) == c5, "El elemento se encuentra en el árbol");

	abb_quitar(arbol, c8);
	abb_quitar(arbol, c9);
	abb_quitar(arbol, c5);
	abb_quitar(arbol, c1);
	abb_quitar(arbol, c2);
	abb_quitar(arbol, c3);
	abb_quitar(arbol, c7);
	pa2m_afirmar(abb_vacio(arbol), "Elimino todos los elementos y el árbol queda vacío.");

	abb_insertar(arbol, c2);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c3);
	pa2m_afirmar(abb_tamanio(arbol) == 7, "Inserto todos los elementos nuevamente y el árbol tiene tamaño 7.");

	free(auxiliar);
	abb_destruir_todo(arbol, destruir_cosa);
}

void abbPruebasNULL()
{
	abb_t *arbol_comparador_nulo = abb_crear(NULL);
	pa2m_afirmar(arbol_comparador_nulo == NULL, "No se puede crear un árbol con comparador nulo.");

	void *elemento = NULL;
	pa2m_afirmar(abb_insertar(NULL, elemento) == NULL, "No puedo insertar en un abb inválido.");
	pa2m_afirmar(abb_quitar(NULL, elemento) == NULL, "No puedo quitar un elemento de un abb inválido.");
	pa2m_afirmar(abb_buscar(NULL, elemento) == NULL, "No puedo buscar un elemento de un abb inválido.");
	pa2m_afirmar(abb_vacio(NULL), "Un abb inválido es vacío.");
	pa2m_afirmar(abb_tamanio(NULL) == 0, "El tamaño de un abb inválido es 0.");
	pa2m_afirmar(abb_con_cada_elemento(NULL, INORDEN, iterar, elemento) == 0, "No se itera sobre un abb inválido.");

	cosa *c1 = crear_cosa(1);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);

	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c7);
	pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, NULL, elemento) == 0, "No se itera con una función inválida.");
	pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, iterar, NULL) == 3, "Puedo iterar aunque el puntero extra sea NULL");

	void *array1[10];
	pa2m_afirmar(abb_recorrer(NULL, INORDEN, array1, 10) == 0, "No se recorre un abb inválido.");
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, NULL, 0) == 0, "No se recorre mandando un array inválido.");
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, array1, 0) == 0, "No se recorre mandando un array de tamaño 0.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeIteraCantidadDeVecesAdecuadaINORDEN()
{
	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c8 = crear_cosa(8);
	cosa *c9 = crear_cosa(9);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);

	pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, iterar, NULL) == abb_tamanio(arbol), "Itero sobre todo el arbol y me devuelve la cantidad de elementos que tiene el mismo.");
	pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, iterar_hasta_5, NULL) == 4, "Itero hasta un elemento y me devuelve la cantidad esperada.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeIteraCantidadDeVecesAdecuadaPREORDEN()
{
	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c8 = crear_cosa(8);
	cosa *c9 = crear_cosa(9);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);

	pa2m_afirmar(abb_con_cada_elemento(arbol, PREORDEN, iterar, NULL) == abb_tamanio(arbol), "Itero sobre todo el arbol y me devuelve la cantidad de elementos que tiene el mismo.");
	pa2m_afirmar(abb_con_cada_elemento(arbol, PREORDEN, iterar_hasta_5, NULL) == 1, "Itero hasta un elemento y me devuelve la cantidad esperada.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeIteraCantidadDeVecesAdecuadaPOSTORDEN()
{
	cosa *c1 = crear_cosa(1);
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	cosa *c8 = crear_cosa(8);
	cosa *c9 = crear_cosa(9);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c8);
	abb_insertar(arbol, c1);
	abb_insertar(arbol, c9);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);

	pa2m_afirmar(abb_con_cada_elemento(arbol, POSTORDEN, iterar, NULL) == abb_tamanio(arbol), "Itero sobre todo el arbol y me devuelve la cantidad de elementos que tiene el mismo.");
	pa2m_afirmar(abb_con_cada_elemento(arbol, POSTORDEN, iterar_hasta_5, NULL) == 7, "Itero hasta un elemento y me devuelve la cantidad esperada.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeAlmacenaCorrectamenteINORDEN()
{
	cosa *c2 = crear_cosa(2);
	cosa *c5 = crear_cosa(5);
	cosa *c7 = crear_cosa(7);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c7);

	void *array1[10];
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, array1, 10) == abb_tamanio(arbol), 
		"Recorro todo el arbol y devuelve la cantidad de elementos que tiene.");
	pa2m_afirmar(((cosa *)array1[0])->clave == 2 && ((cosa *)array1[1])->clave == 5 && ((cosa *)array1[2])->clave == 7, 
		"Se recorrió y almacenó elementos en el orden correcto.");
	void *array2[2];
	pa2m_afirmar(abb_recorrer(arbol, INORDEN, array2, 2) == 2, 
		"Recorro menos elementos de los que hay en el árbol y devuelve la canidad esperada.");
	pa2m_afirmar(((cosa *)array2[0])->clave == 2 && ((cosa *)array2[1])->clave == 5, 
		"Se recorrió y almacenó elementos en el orden correcto.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeAlmacenaCorrectamentePREORDEN()
{
	cosa *c1 = crear_cosa(1);
	cosa *c4 = crear_cosa(4);
	cosa *c10 = crear_cosa(10);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c4);
	abb_insertar(arbol, c10);
	abb_insertar(arbol, c1);

	void *array1[10];
	pa2m_afirmar(abb_recorrer(arbol, PREORDEN, array1, 10) == abb_tamanio(arbol), 
		"Recorro todo el arbol y devuelve la cantidad de elementos que tiene.");
	pa2m_afirmar(((cosa *)array1[0])->clave == 4 && ((cosa *)array1[1])->clave == 1 && ((cosa *)array1[2])->clave == 10, 
		"Se recorrió y almacenó elementos en el orden correcto.");
	void *array2[1];
	pa2m_afirmar(abb_recorrer(arbol, PREORDEN, array2, 1) == 1, 
		"Recorro menos elementos de los que hay en el árbol y devuelve la canidad esperada.");
	pa2m_afirmar(((cosa *)array2[0])->clave == 4, 
		"Se recorrió y almacenó elementos en el orden correcto.");

	abb_destruir_todo(arbol, destruir_cosa);
}

void abbSeAlmacenaCorrectamentePOSTORDEN()
{
	cosa *c2 = crear_cosa(2);
	cosa *c3 = crear_cosa(3);
	cosa *c5 = crear_cosa(5);
	cosa *c6 = crear_cosa(6);
	cosa *c7 = crear_cosa(7);
	
	abb_t *arbol = abb_crear(comparar_vectores);
	abb_insertar(arbol, c5);
	abb_insertar(arbol, c2);
	abb_insertar(arbol, c7);
	abb_insertar(arbol, c3);
	abb_insertar(arbol, c6);

	void *array1[5];
	pa2m_afirmar(abb_recorrer(arbol, POSTORDEN, array1, 5) == abb_tamanio(arbol), 
		"Recorro todo el arbol y devuelve la cantidad de elementos que tiene.");
	pa2m_afirmar(((cosa *)array1[0])->clave == 3 && ((cosa *)array1[1])->clave == 2 && ((cosa *)array1[2])->clave == 6 && 
		((cosa *)array1[3])->clave == 7 && ((cosa *)array1[4])->clave == 5, "Se recorrió y almacenó elementos en el orden correcto.");
	void *array2[3];
	pa2m_afirmar(abb_recorrer(arbol, POSTORDEN, array2, 3) == 3, 
		"Recorro menos elementos de los que hay en el árbol y devuelve la canidad esperada.");
	pa2m_afirmar(((cosa *)array2[0])->clave == 3 && ((cosa *)array2[1])->clave == 2 && ((cosa *)array1[2])->clave == 6,
		"Se recorrió y almacenó elementos en el orden correcto.");

	abb_destruir_todo(arbol, destruir_cosa);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creación de abb");
	abbSeCreaVacio();

	pa2m_nuevo_grupo("Pruebas de inserción");
	abbInsertarAumentaEltamanio();

	pa2m_nuevo_grupo("Pruebas de búsqueda");
	abbBuscarElementos();

	pa2m_nuevo_grupo("Pruebas de borrado");
	abbBorrarElementosDisminuyeEltamanio();
	
	pa2m_nuevo_grupo("Pruebas mixtas (insertar-buscar/borrar-buscar)");
	abbBuscarAlInsertaryAlBorrar();

	pa2m_nuevo_grupo("Pruebas con NULL");
	abbPruebasNULL();

	pa2m_nuevo_grupo("Pruebas recorridos e iterador");

	pa2m_nuevo_grupo("Iterador inorden");
	abbSeIteraCantidadDeVecesAdecuadaINORDEN();
	pa2m_nuevo_grupo("Iterador preorden");
	abbSeIteraCantidadDeVecesAdecuadaPREORDEN();
	pa2m_nuevo_grupo("Iterador postorden");
	abbSeIteraCantidadDeVecesAdecuadaPOSTORDEN();

	pa2m_nuevo_grupo("Recorrido inorden");
	abbSeAlmacenaCorrectamenteINORDEN();
	pa2m_nuevo_grupo("Recorrido preorden");
	abbSeAlmacenaCorrectamentePREORDEN();
	pa2m_nuevo_grupo("Recorrido postorden");
	abbSeAlmacenaCorrectamentePOSTORDEN();

	return pa2m_mostrar_reporte();
}
