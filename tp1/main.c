#include "src/cajas.h"
#include "src/pokemon.h"
#include <stdio.h>

void pokemon_mostrar(pokemon_t *pokemon)
{
	printf("%s-", pokemon_nombre(pokemon));
	printf("%i-", pokemon_nivel(pokemon));
	printf("%i-", pokemon_ataque(pokemon));
	printf("%i\n", pokemon_defensa(pokemon));
}

void caja_mostrar_contenido(caja_t *caja, int numero_caja)
{
	if(caja == NULL) {
		printf("La caja ingresada es inválida.\n");
		printf("---------------------------------------\n");
	} else {
		printf("Nº de Caja: %i.\n", numero_caja);
		printf("\n");
		printf("Nombre-Nivel-Ataque-Defensa\n");
		caja_recorrer(caja, *pokemon_mostrar);
		printf("La caja tiene %i pokemones.\n", caja_cantidad(caja));
		printf("---------------------------------------\n");
	}
}

int main(int argc, char* argv[]) 
{

	caja_t *caja1 = caja_cargar_archivo("pokemones.csv");
	caja_mostrar_contenido(caja1, 1);

	caja_t *caja2 = caja_cargar_archivo("pokemones2.csv");
	caja_mostrar_contenido(caja2, 2);

	caja_t *caja3 = caja_combinar(caja1, caja2);
	if(caja3 != NULL){
		printf("El segundo pokemon de la caja3 es %s.\n", pokemon_nombre(caja_obtener_pokemon(caja3, 1)));
		int cantidad_caja3 = caja_guardar_archivo(caja3, "pokemones3.csv");
		printf("La cantidad de pokemones guardados en el archivo desde caja3 fue de %i.\n", cantidad_caja3);
	}
	
	caja_destruir(caja1);
	caja_destruir(caja2);
	caja_destruir(caja3);

	return 0;
}
