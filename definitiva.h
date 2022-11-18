#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define compara " "
#define MSJ_ERROR "Error de sintaxis no se pueden introducir los operadores [|><&]\n"
#define Lmax 51
#define PROMPT ">ossh "
  
int CadIguales(char cadena1[Lmax],char cadena2[Lmax]); 
void Comando(char cadena[Lmax], int plano); 
void redirecEntrada(char cadena[Lmax]);
void redirecSalida(char cadena[Lmax]);
void tuberias(char* argumento1[Lmax], char* argumento2[Lmax]);
void CrearProceso(char* argumento[Lmax], int plano);