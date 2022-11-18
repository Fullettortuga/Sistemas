/*Alumnos: Claudia Rodríguez y Carlos Martínez
Activitat #03: OSSH v1.0 (Mòdul 2)*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "definitiva.h"

#define compara " "
#define MSJ_ERROR "Error de sintaxis no se pueden introducir los operadores [|><&]\n"
#define Lmax 51
#define PROMPT ">ossh "

/* La estrategia es la siguiente: El bucle principal del porgrama lee una línea de la entrada estándar,
la divide en palabras y las almacena en un array de cadenas, mientras no se introduzca la palabra "EXIT" el bucle
seguirá funcionando.
Se utilizan PIPES para el redireccionamiento de la E/S.*/


//Inicio de la funcion main
int main(int argc, char *argv[])
{ 
char cadena[Lmax];   //Cadena que se introducira por teclado.
char cadFin[]="EXIT";//Cadena que se introducira para finalizar el programa.

int fin=0, help = 0, i,segplano=0, guardaStdout = dup(1), guardaStdin = dup(0);  

while(fin==0)  //Bucle que se repetira hasta que se introduzca la cadena "EXIT".
	{
        close(1); // Cerramos la salida que tenga, o si se hubiese guardado datos en un fichero, se cerrará el fichero. 
                 
        dup(guardaStdout); //Restauramos la salida estándar
        close(0); 
        
        dup(guardaStdin);  //Restauramos la entrada estándar.
	printf(PROMPT);  //Mostramos el prompt.
	scanf("\n%[^\n]",cadena);  // Escanea la cadena entera hasta que pulsamos 'Intro'.
	segplano=0;  
	for(i=0;cadena[i] != '\0'; i++){   //Bucle que recorre la cadena.
		if(cadena[i] == '&') {  //Si se encuentra el caracter & se cambia por \0.
			cadena[i] = '\0'; 
			segplano = 1; 
		}
	}
   
	fin=CadIguales(cadena,cadFin); // Verificamos si la cadena es "EXIT
	Comando(cadena,segplano); 
	}	
exit(0); // Hemos introducido la cadena "EXIT" y por tanto finalizamos el programa.
	
return(0);
} //fin de la funcion main
