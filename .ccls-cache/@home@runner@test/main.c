#include "heap.h"
#include "stack.h"
#include "list.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *nombre;
  int prioridad;
  List *precedentes; //pueden ser varias las tareas precedentes, opino que usar una lista o un vector de strings seria bueno
  bool completado;
} tareaInfo;

typedef struct {    // stack de acciones realizadas para el deshacer acciones
  int numeroAccion; // que se realizo
  tareaInfo tarea;  // la tarea que se realizo
} accion;

void menuTexto(int *opcion) {

  printf("\nElija una opcion para continuar:\n");
  printf("\n1. Agregar tarea.\n");
  printf("2. establecer precedencia entre tareas.\n");
  printf("3. Mostrar tareas por hacer.\n");
  printf("4. Marcar tarea por como completada.\n");
  ;
  printf("5. Deshacer última acción.\n");
  printf("6. Importar datos desde un archivo de texto.\n");
  printf("0. Salir del menu.\n");
  /*
    Usar string para comprobar que lo ingresa el usuario es un numero valido
    para nuestro menu
  */
  char comprobar[2];
  while (1) {
    scanf("%s", comprobar);

    if (isdigit(comprobar[0]) && strlen(comprobar) == 1)
    // ver que sea un numero de un digito
    {

      *opcion = atoi(comprobar);
      // ya cuando se comprueba se actualiza opcion
      return;
    }
    printf("Entrada invalida, ingrese una opcion valida entre 0 y 9\n");
  }
}

void agregarTarea(Heap *arreglo)
{
  char nombre[50];
  char precedente[50];
  int prioridad;
  tareaInfo *tarea = malloc(sizeof(tareaInfo));

  printf("Ingrese el nombre de la Tarea: ");
  scanf("%s", nombre);
  tarea->nombre = malloc(strlen(nombre) + 1);
  strcpy(tarea->nombre, nombre);

  printf("Ingrese la prioridad de la Tarea: ");
  scanf("%i", &prioridad);
  tarea->prioridad = prioridad;

  printf("Tarea agregada con éxito.\n");
}

int main() {

  int opcion;
  Stack *acciones;
  Heap *arreglo = createHeap(arreglo);

  printf("~~~~~~BIENVENIDO AL MENU DE JUGADOR~~~~~~\n\n");
  while (1) {
    menuTexto(&opcion);

    switch (opcion) {
    case 1:
      printf("\n--------------------------------------------\n");
      agregarTarea(arreglo);
      printf("\n--------------------------------------------\n");

      break;
    case 2:
      printf("\n--------------------------------------------\n");
      // establecerPrecedencia(jugadores);
      printf("\n--------------------------------------------\n");
      break;
    case 3:
      // printf("¿Que item deasea agregar?"); // lo comento pq estoy probando lo
      // de la pila para el deshacer :)
      printf("\n--------------------------------------------\n");
      // mostrarPorHacer(jugadores);
      printf("\n--------------------------------------------\n");
      break;
    case 4:
      printf("\n--------------------------------------------\n");
      // marcarComoCompletada(jugadores);
      printf("\n--------------------------------------------\n");
      break;
    case 5:
      printf("\n--------------------------------------------\n");
      // deshacerUltima(jugadores);
      printf("\n--------------------------------------------\n");
      break;
    case 6:
      printf("\n--------------------------------------------\n");
      // importarArchivo(jugadores);
      printf("\n--------------------------------------------\n");
      break;
    case 0:

      exit(EXIT_SUCCESS);

    default:
      printf("Elija una opcion del menu para continuar:\n\n");
    }
  }

  return 0;
}