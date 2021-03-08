/*
 * Este programa en C muestra como se puede hacer crecer dinamicamente un
 * vector. En este caso es un vector de cadena de caracteres.
 *
 * Tomado:
 * https://stackoverflow.com/questions/21950388/dynamic-memory-realloc-string-array
 *
 * Autor: John Sanabria - john.sanabria@correounivalle.edu.co
 * Fecha: 2021-02-26
 */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 250

int main(int argc, char* argv[]) {
  char** buffer;
  char linea[MAXLINE] = "";
  int i = 0;
  int total = 0;
  // Inicializacion del vector de cadena de caracteres
  buffer = (char**)malloc(sizeof(char*));
  if (buffer == NULL) {
    fprintf(stderr, "Error en malloc inicial");
    exit(1);
  }

  // Ciclo que se detendra una vez se escriba la palabra salida. Condicion de
  // salida mas abajo
  while (1) {
    // Lee una cadena
    printf("Digite una linea ");
    fgets(linea, sizeof(linea), stdin);
    if (i != 0) {  // Despues de la 2da palabra se requiere reasignar memoria
      buffer = realloc(buffer, (i + 1) * sizeof(*buffer));
      if (buffer == NULL) {  // En caso de error al reasignar memoria
        fprintf(stderr, "Error en realloc (iteracion %d)", i);
        exit(1);
      }
    }
    // Se asigna espacio para la nueva posicion de memoria y se valida que
    // se haya podido hacer exitosamente
    buffer[i] = (char*)malloc(sizeof(char) * MAXLINE);
    if (buffer[i] == NULL) {
      fprintf(stderr, "Error en malloc (iteracion %d)", i);
      exit(1);
    }
    linea[strlen(linea) - 1] = '\x0';  // remueve '\n' al final de la var linea
    if (!strcmp(linea, "salida")) break;  // sale del ciclo
    strcpy(buffer[i], linea);             // se copia var linea en buffer[i]
    printf("%s\n", buffer[i]);

    i++;
  }  
}
