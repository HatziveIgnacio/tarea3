#include "heap.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo {
  void *data;
  int priority;
} heapElem;

typedef struct Heap {
  heapElem *heapArray;
  int size;
  int capac;
} Heap;

typedef struct { // Nodo que se guardara en el grafo
  char *nombre;
  int prioridad;
  bool completado;
  List *nodosAdj;
  Stack *accionT;
  // Agregue una variable char para ayudarme hacer la f(x) mostrar
  char *estado;
} Nodo;

typedef struct {    // stack de acciones realizadas para el deshacer acciones
  int numeroAccion; // 1: AgregarTarea // 2: establecerPrecedencia // 3:
                    // marcarComoCompletada
  Nodo tarea;       // la tarea que se realizo
} Accion;

int is_equal_string(void *key1, void *key2) {
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

int lower_than_prioridad(void *key1, void *key2) {
  Nodo *tarea1 = (Nodo *)key1;
  Nodo *tarea2 = (Nodo *)key2;
  if (tarea1->prioridad < tarea2->prioridad)
    return -1;
  return 0;
}

void menuTexto(int *opcion) {
  // GFKAJSKDLSJALDK

  printf("\nElija una opcion para continuar:\n");
  printf("\n1. Agregar tarea.\n");
  printf("2. establecer precedencia entre tareas.\n");
  printf("3. Mostrar tareas por hacer.\n");
  printf("4. Marcar tarea como completada.\n");
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

void agregarTarea(Map *grafo, Stack *acciones) {

  char nombre[50];
  int prioridad;
  Nodo *tarea = (Nodo *)malloc(sizeof(Nodo));
  tarea->nodosAdj = createList();
  tarea->completado = false;
  tarea->estado = (char *)malloc(sizeof(char));

  printf("Ingrese el nombre de la Tarea: ");
  scanf("%s", nombre);
  tarea->nombre = malloc(strlen(nombre) + 1);
  strcpy(tarea->nombre, nombre);

  printf("Ingrese la prioridad de la Tarea: ");
  scanf("%i", &prioridad);
  tarea->prioridad = prioridad * -1;
  // Se asigna la prioridad en negativo para ahorrar trabajo al usar el heap
  Accion *acc = malloc(sizeof(Accion));
  acc->tarea = *tarea;
  acc->numeroAccion = 1;
  stack_push(acciones, acc);
  // Establecer la función de ordenamiento por prioridad
  insertMap(grafo, tarea->nombre, tarea);

  printf("\nTarea agregada con éxito.\n");
} // LISTO(?)

void establecerPrecedencia(Map *grafo, Stack *acciones) {
  char tarea1[50];
  char tarea2[50];

  if (firstMap(grafo) == NULL) {
    puts("\nNo hay tareas guardadas");
    return;
  }

  printf("Ingrese el nombre de la primera tarea: ");
  scanf("%s", tarea1);

  if (!searchMap(grafo, tarea1)) {

    puts("La tarea no se encuentra guardada.");
    return;
  }

  printf("\nIngrese el nombre de la segunda tarea: ");
  scanf("%s", tarea2);
  if (searchMap(grafo, tarea2) == NULL) {

    puts("\nLa tarea no se encuentra guardada.");
    return;
  }

  Nodo *t1 = searchMap(grafo, tarea1);
  Nodo *t2 = searchMap(grafo, tarea2);

  if (searchMap(grafo, tarea1) != NULL) {

    pushBack(t1->nodosAdj, t2);
  }

  Accion *acc = malloc(sizeof(Accion));
  acc->tarea = *t1;
  acc->numeroAccion = 2;
  stack_push(acciones, acc);

} // LISTO(?)


void mostrarTareas(Map *grafo, Stack *acciones) {
  Heap *heap = createHeap();
  Map *mapaNodos = createMap(is_equal_string);
  List *listaOrdenada = createList();

    Nodo *aux = firstMap(grafo);

  if (aux == NULL) {
    puts("\nNo hay tareas guardadas");
    return;
  }

  while (aux != NULL) {
    strcpy(aux->estado, "0");
    
    if (firstList(aux->nodosAdj) == NULL) {
      strcpy(aux->estado, "1");
      heap_push(heap, aux, aux->prioridad);
    }

    aux = nextMap(grafo);
  }

  Nodo *tareaActual;
  while (heap_top(heap) != NULL) {
    tareaActual = heap_top(heap);
    
    if (strcmp(tareaActual->estado, "1") == 0) {
      strcpy(tareaActual->estado, "2");
      pushBack(listaOrdenada, tareaActual);
    }
    heap_pop(heap);
  }

  tareaActual = (Nodo *)firstList(listaOrdenada);
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");
  int cont = 1;

  while (tareaActual != NULL) {
    if(searchMap(mapaNodos, tareaActual->nombre));
    {
      if (!tareaActual->completado) {
        if (tareaActual->prioridad < 0) {
          tareaActual->prioridad *= -1;
        }
        printf("%d° %s (Prioridad: %d)\n", cont, tareaActual->nombre, tareaActual->prioridad);
        insertMap(mapaNodos, aux->nombre , aux);  
        if (tareaActual->prioridad > 0) {
          tareaActual->prioridad *= -1;
        }
        cont++;
      }
      tareaActual = nextList(listaOrdenada);
    }
  }
  
  tareaActual = firstMap(grafo);
  
  while (tareaActual != NULL) {
    if (strcmp(tareaActual->estado, "0") == 0) {
      strcpy(tareaActual->estado, "1");
      bool tienePrecedentes = false;
        
      Nodo *aux = firstList(tareaActual->nodosAdj);

      while (aux != NULL) {
        if (strcmp(aux->estado, "1") == 0) {
          tienePrecedentes = true;
          break;
        }
        aux = nextList(aux);
      }

      if (!tienePrecedentes) {
        heap_push(heap, tareaActual, tareaActual->prioridad);
        strcpy(tareaActual->estado, "1");
      }
    }
    tareaActual = nextMap(grafo);
  }
  
  tareaActual = (Nodo *)firstList(listaOrdenada);
  
  while (tareaActual != NULL) {
    if (!tareaActual->completado) {
      if (tareaActual->prioridad < 0) {
        tareaActual->prioridad *= -1;
      }
      
      printf("%d° %s (Prioridad: %d) - Precedente(s): ", cont, tareaActual->nombre, tareaActual->prioridad);

      Nodo *aux = firstList(tareaActual->nodosAdj);
      while (aux != NULL) {
        printf("%s ", aux->nombre);
        aux = nextList(aux);
      }
      printf("\n");
      if (tareaActual->prioridad > 0) {
        tareaActual->prioridad *= -1;
      }
      cont++;
    }
    tareaActual = nextList(listaOrdenada);
  }
}//SIN TERMINAR AUN

void marcarComoCompletada(Map *grafo, Stack *acciones) // LISTO
{
  char tarea[50];
  printf("Ingrese el nombre de la tarea a marcar como completada: ");
  scanf("%s", tarea);

  Nodo *nodo = searchMap(grafo, tarea);
  if (nodo != NULL) {
    nodo->completado = true;
    printf("La tarea %s se ha marcado completada.\n", tarea);
  } else {
    printf("La tarea '%s' no existe en el grafo.\n", tarea);
  }
  Accion *acc = malloc(sizeof(Accion));
  acc->tarea = *nodo;
  acc->numeroAccion = 3;
  stack_push(acciones, acc);
}

void deshacerUltima(Map *grafo, Stack *acciones) // LISTO
{
  Accion *accion = (Accion *)stack_top(acciones);
  Nodo *tarea = searchMap(grafo, accion->tarea.nombre);

  switch (accion->numeroAccion) {
  case 1:                           // Última acción fue agregar tarea
    eraseMap(grafo, tarea->nombre); // Eliminar tarea del grafo
    printf("Se ha deshecho la acción de agregar la tarea '%s'.\n",
           tarea->nombre);
    break;

  case 2:                     // Última acción fue establecer precedencia
    popBack(tarea->nodosAdj); // Eliminar el último nodo adyacente
    printf(
        "Se ha deshecho la acción de establecer precedencia entre tareas.\n");
    break;

  case 3:                      // Última acción fue marcar como completada
    tarea->completado = false; // Marcar la tarea como no completada
    printf(
        "Se ha deshecho la acción de marcar la tarea '%s' como completada.\n",
        tarea->nombre);
    break;

  default: // No existe última acción realizada
    printf("No se puede deshacer la última acción.\n");
    break;
  }

  stack_pop(acciones); // Eliminar la última acción de la pila
}

void importarArchivo(Map *grafo) {
  char nombre_archivo[100];
  printf("Ingrese el nombre del archivo de texto: ");
  scanf("%s", nombre_archivo);

  FILE *archivo = fopen(nombre_archivo, "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo.\n");
    return;
  }

  char line[1024];
  while (fgets(line, 1024, archivo)) {
    Nodo *tarea = (Nodo *)malloc(sizeof(Nodo));
    tarea->nodosAdj = createList();
    tarea->completado = false;
    tarea->estado = (char *)malloc(sizeof(char));

    char *linePtr = line;

    // nombre
    char *token = strsep(&linePtr, ",");
    tarea->nombre = malloc(strlen(token) + 1);
    strcpy(tarea->nombre, token);

    // prioridad
    token = strsep(&linePtr, ",");
    tarea->prioridad = atoi(token);

    // agregar precedentes
    token = strsep(&linePtr, ",");
    while (token != NULL) {
      token[strcspn(token, "\r\n")] = 0;
      char *precedente = malloc(strlen(token) + 1);
      strcpy(precedente, token);
      pushBack(tarea->nodosAdj, precedente);
      token = strsep(&linePtr, ",");
    }

    insertMap(grafo, tarea->nombre, tarea);
  }

  fclose(archivo);

  printf("\n******************************************\n");
  printf("Se ha importado correctamente el archivo.\n");
  printf("******************************************\n\n");
} // LISTO (??????????)
// 21:12 este realmente no si funciona, mas o menos me fije en el que hicieron
// en la tarea pasada y lo modifique
// 21:40 En teoria funciona, pero al mostrar tareas no las muestra, pero si
// utilizas una funcion, como ej, la marcar como completada y ingresas una tarea
// del archivo luego de importar la toma como que existe

int main() {

  int opcion;
  Stack *acciones = stack_create();
  Map *grafoTareas = createMap(is_equal_string);

  printf("~~~~~~BIENVENIDO AL MENU DE JUGADOR~~~~~~\n\n");
  while (1) {
    menuTexto(&opcion);

    switch (opcion) {
    case 1:
      printf("\n--------------------------------------------\n");
      agregarTarea(grafoTareas, acciones);
      printf("\n--------------------------------------------\n");

      break;
    case 2:
      printf("\n--------------------------------------------\n");
      establecerPrecedencia(grafoTareas, acciones);
      printf("\n--------------------------------------------\n");

      break;
    case 3:
      printf("\n--------------------------------------------\n");
      mostrarTareas(grafoTareas, acciones);
      printf("\n--------------------------------------------\n");

      break;
    case 4:
      printf("\n--------------------------------------------\n");
      marcarComoCompletada(grafoTareas, acciones);
      printf("\n--------------------------------------------\n");
      break;
    case 5:
      printf("\n--------------------------------------------\n");
      deshacerUltima(grafoTareas, acciones);
      printf("\n--------------------------------------------\n");
      break;
    case 6:
      printf("\n--------------------------------------------\n");
      importarArchivo(grafoTareas);
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