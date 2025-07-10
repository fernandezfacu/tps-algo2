#include "src/hash.h"
#include "pa2m.h"
#include <string.h>
#include <stdlib.h>

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

typedef struct cosa {
	char* clave;
	int numeros[10];
} cosa;

cosa *crear_cosa(const char *clave)
{
	cosa *c = (cosa *)malloc(sizeof(cosa));
	if(!c)
		return NULL;

	c->clave = dupli_string(clave);
	if(!c->clave)
		return NULL;
	return c;
}

void destruir_cosa(void *c)
{
	struct cosa *cosa = c;
	if(cosa->clave)
		free(cosa->clave);
	free(cosa);
}

void hashSeCreaVacio()
{
	hash_t *h1 = hash_crear(10);
	pa2m_afirmar(h1 != NULL, "Puedo crear un hash con capacidad mayor a 3 y me devuelve uno válido.");
	pa2m_afirmar(hash_cantidad(h1) == 0, "Se crea un hash vacío.");

	hash_t *h2 = hash_crear(1);
	pa2m_afirmar(h2 != NULL, "Puedo crear un hash con capacidad menor a 3 y me devuelve uno válido.");
	pa2m_afirmar(hash_cantidad(h2) == 0, "Se crea un hash vacío.");

	hash_destruir(h1);
	hash_destruir(h2);
}

void hash_insertarAumentaTamanioYBuscarEncuentra()
{
	hash_t *h = hash_crear(4);
	pa2m_afirmar(!hash_contiene(h, "Clave1"), "No encuentro una clave en un hash vacío.");

	void *anterior = NULL;
	int elementos[] = {10, 11, 22, 23, 1, 9, 100};
	
	pa2m_afirmar(hash_insertar(h, "Clave1", elementos+0, &anterior) == h, "Inserto una clave y devuelve el hash.");
	pa2m_afirmar(hash_contiene(h, "Clave1"), "Busco la clave insertada y la encuentro.");
	pa2m_afirmar(hash_insertar(h, "Clave2", elementos+1, &anterior) == h, "Inserto una clave y devuelve el hash.");
	pa2m_afirmar(hash_contiene(h, "Clave2"), "Busco la clave insertada y la encuentro.");
	pa2m_afirmar(hash_insertar(h, "Clave3", elementos+2, &anterior) == h, "Inserto una clave y devuelve el hash.");
	pa2m_afirmar(hash_contiene(h, "Clave3"), "Busco la clave insertada y la encuentro.");
	pa2m_afirmar(hash_insertar(h, "Clave4", elementos+3, &anterior) == h, "Inserto una clave y devuelve el hash.");
	pa2m_afirmar(hash_contiene(h, "Clave4"), "Busco la clave insertada y la encuentro.");
	pa2m_afirmar(!hash_contiene(h, "Clave5"), "No encuentro una clave que no fue insertada.");
	pa2m_afirmar(hash_cantidad(h) == 4, "El hash tiene 4 elementos almacenados. Hubo un rehash ya que las capacidad inicial era 4.");

	anterior = elementos+6;
	pa2m_afirmar(hash_insertar(h, "Clave4", elementos+4, &anterior) == h, "Inserto una clave repetida con otro valor y devuelve el hash.");
	pa2m_afirmar(anterior == elementos+3, "El elemento anterior reemplazado en el insertar es el valor anterior de la clave insertada.");
	pa2m_afirmar(hash_cantidad(h) == 4, "El hash sigue teniendo 4 elementos almacenados.");
	pa2m_afirmar(hash_insertar(h, "Clave6", elementos+2, &anterior) == h, "Inserto una clave y devuelve el hash.");
	pa2m_afirmar(anterior == NULL, "El elemento anterior reemplazado en el insertar es NULL (la clave insertada no estaba en el hash).");
	pa2m_afirmar(hash_cantidad(h) == 5, "El hash tiene 5 elementos almacenados.");
	
	hash_destruir(h);
}

void hash_quitarDisminuyeTamanio()
{
	hash_t *h = hash_crear(100);
	struct cosa *c1 = crear_cosa("Cosita1");
	struct cosa *c2 = crear_cosa("Cosita2");
	struct cosa *c3 = crear_cosa("Cosita3");
	struct cosa *c4 = crear_cosa("Cosita4");
	struct cosa *c5 = crear_cosa("Cosita5");
	hash_insertar(h, "Clave1", c1, NULL);
	hash_insertar(h, "Clave2", c2, NULL);
	hash_insertar(h, "Clave3", c3, NULL);
	hash_insertar(h, "Clave4", c4, NULL);
	hash_insertar(h, "Clave5", c5, NULL);

	pa2m_afirmar(hash_cantidad(h) == 5, "Inserto 5 elementos en un hash.");
	pa2m_afirmar(hash_quitar(h, "Clave6") == NULL, "Intento eliminar una clave que no se encuentra en el hash y devuelve NULL.");
	pa2m_afirmar(hash_cantidad(h) == 5, "La cantidad de elementos sigue siendo 5.");
	pa2m_afirmar(hash_quitar(h, "Clave4") == c4, "Elimino una clave y devuelve el elemento que corresponde.");
	destruir_cosa(c4);
	pa2m_afirmar(hash_cantidad(h) == 4, "La cantidad de elementos es 4.");
	pa2m_afirmar(hash_quitar(h, "Clave4") == NULL, "Intento eliminarla de nuevo y devuelve NULL.");

	hash_destruir_todo(h, destruir_cosa);
}

void hash_obtenerDevuelveInsertados()
{
	hash_t *h = hash_crear(3);
	int elementos[] = {10, 11, 22, 23, 1};
	hash_insertar(h, "Clave1", elementos+0, NULL);
	hash_insertar(h, "Clave2", elementos+1, NULL);
	hash_insertar(h, "Clave4", elementos+4, NULL);
	hash_insertar(h, "Clave4", elementos+3, NULL);
	pa2m_afirmar(hash_cantidad(h) == 3, "Se está por buscar elementos en un hash con 4 elementos.");

	pa2m_afirmar(hash_obtener(h, "Clave1") == elementos+0, "Busco un elemento y obtengo su valor.");
	pa2m_afirmar(hash_obtener(h, "Clave2") == elementos+1, "Busco un elemento y obtengo su valor.");
	pa2m_afirmar(hash_obtener(h, "Clave4") == elementos+3, "Busco un elemento y obtengo su valor.");
	pa2m_afirmar(hash_obtener(h, "Clave7") == NULL, "Busco una clave que no se encuentra en el hash y obtengo NULL.");

	hash_quitar(h, "Clave4");
	pa2m_afirmar(hash_obtener(h, "Clave4") == NULL, "Elimino una clave y ya no puedo obtener su valor.");

	hash_destruir(h);
}

bool iterar(const char *clave, void *valor, void *aux)
{
	return true;
}

bool iterar_hasta_10(const char *clave, void *valor, void *aux)
{
	if(*((int *)valor) == 10)
		return false;

	return true;
}

void hashSeIteraCantidadDeVecesAdecuada()
{
	hash_t *h = hash_crear(100);
	pa2m_afirmar(hash_con_cada_clave(h, iterar, NULL) == 0, "Iterar sobre un hash vacìo devuelve 0.");

	int elementos[] = {10, 11, 22, 23, 1};
	hash_insertar(h, "Clave1", elementos+0, NULL);
	hash_insertar(h, "Clave2", elementos+1, NULL);
	hash_insertar(h, "Clave3", elementos+2, NULL);
	hash_insertar(h, "Clave4", elementos+3, NULL);
	hash_insertar(h, "Clave5", elementos+4, NULL);

	pa2m_afirmar(hash_con_cada_clave(h, iterar, NULL) == hash_cantidad(h), "Itero el hash en su totalidad y me devuelve la cantidad de elementos en el hash.");
	pa2m_afirmar(hash_con_cada_clave(h, iterar_hasta_10, NULL) != hash_cantidad(h), "Itero deteniéndome antes de visitar todos elementos y no se recorre todo el hash.");

	hash_destruir(h);
}

void hashConNullEsValido()
{
	hash_t *h = hash_crear(100);
	int elementos[] = {10, 11, 22, 23, 1};
	void *anterior = elementos+0;

	pa2m_afirmar(hash_insertar(NULL, "Clave1", elementos+0, anterior) == NULL, "Insertar en un hash nulo devuelve NULL.");
	pa2m_afirmar(hash_insertar(h, NULL, elementos+0, anterior) == NULL, "Insertar una clave nula devuelve NULL.");
	pa2m_afirmar(hash_insertar(h, "Clave1", NULL, anterior) == h, "No hay problema con insertar un elemento nulo.");

	pa2m_afirmar(hash_quitar(NULL, "Clave1") == NULL, "Se intenta quitar una clave de un hash nulo y devuelve NULL.");
	pa2m_afirmar(hash_quitar(h, NULL) == NULL, "Se intenta quitar una clave nula y devuelve NULL.");

	pa2m_afirmar(hash_obtener(NULL, "Clave1") == NULL, "Se intenta obtener un elemento de un hash nulo y devuelve NULL.");
	pa2m_afirmar(hash_obtener(h, NULL) == NULL, "Se intenta obtener un elemento con una clave nula y devuelve NULL.");

	pa2m_afirmar(!hash_contiene(NULL, "Clave1"), "Se intenta ver si un hash nulo contiene una clave y devuelve false.");
	pa2m_afirmar(!hash_contiene(h, NULL), "Se intenta ver si un hash contiene una clave nula y devuelve false.");

	pa2m_afirmar(hash_cantidad(NULL) == 0, "Un hash nulo tiene 0 elementos.");

	pa2m_afirmar(hash_con_cada_clave(NULL, iterar, anterior) == 0, "Iterar sobre un hash nulo devuelve 0.");
	pa2m_afirmar(hash_con_cada_clave(h, NULL, anterior) == 0, "Iterar con una función nula devuelve 0.");
	pa2m_afirmar(hash_con_cada_clave(h, iterar, NULL) == hash_cantidad(h), "No hay problema en interar con un auxiliar nulo.");

	hash_destruir(h);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creación");
	hashSeCreaVacio();

	pa2m_nuevo_grupo("Pruebas de inserción - búsqueda");
	hash_insertarAumentaTamanioYBuscarEncuentra();

	pa2m_nuevo_grupo("Pruebas de obtención");
	hash_obtenerDevuelveInsertados();
	
	pa2m_nuevo_grupo("Pruebas de eliminación");
	hash_quitarDisminuyeTamanio();

	pa2m_nuevo_grupo("Pruebas de iteración");
	hashSeIteraCantidadDeVecesAdecuada();

	pa2m_nuevo_grupo("Pruebas con NULL");
	hashConNullEsValido();

	return pa2m_mostrar_reporte();
}
