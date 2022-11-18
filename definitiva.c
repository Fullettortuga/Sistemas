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
char *cadHelp = "help"; 
 

int CadIguales(char cadena1[],char cadena2[]) //Función que compara dos cadenas

{
int i=0;
int igual=0;

while ((cadena1[i]==cadena2[i])&&(cadena1[i]!='\0')&&(cadena2[i]!='\0')) i++;	//Mientras que los caracteres sean iguales y no se llegue al final de la cadena
if ((cadena1[i]==cadena2[i])&&(cadena1[i]=='\0')&&(cadena2[i]=='\0')){
	printf("#################\n");
	printf("That's all folks!\n");
	printf("#################\n");
	exit(0);
} 		
return(igual);
}

//Función que ejecuta el comando introducido por el usuario
void Comando(char cadena[Lmax], int plano) 

{ 
int i,j,k,flag_tuberia;		 //variables para el bucle
char argumentoInd[Lmax][Lmax]; //matriz que contendra los argumentos 				
char argumentoInd2[Lmax][Lmax]; 
char redirec[Lmax]; 
char entrada[Lmax];
char *argumento[Lmax];  //puntero que contendra los argumentos
char *argumento2[Lmax];
int ejecutar=0; 
argumento[0] = NULL;	 //inicializamos el puntero a NULL				
argumento2[0] = NULL;
flag_tuberia = 0; 

k = 0;  //inicializamos la variable k a 0
i = 0; //inicializamos la variable i a 0
		/* Recorremos la cadena a la vez que la almacenamos en un array de dos dimensiones,es decir, 
		almacena cadenas de cadenas de carácteres*/

	if(*cadena == *cadHelp){
		printf("################## GTIDIC - UDL - OSSH ####################\n"
                "#                                                        #\n"
                "# Type program names and arguments, and hit enter.       #\n"
                "# Use the man command for information on other programs. #\n"
                "#                                                        #\n"
                "##########################################################\n");	
		
	}

	
	while(cadena[i] != '\0' && cadena[i] != '|' && cadena[i] != '>'){ 
			
				for(j=0; cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '|'  && cadena[i] != '>' && cadena[i] != '<';j++) {  
							argumentoInd[k][j] = cadena[i];
							i++;}

				/* Verificamos si hemos salido del bucle por encontrar un espacio en blanco. 
				Si fuese el caso, hay que evitar caer en un bucle infinito. Aumentamos la i en una unidad.*/

				if (cadena[i] == ' ') i++; 
			
				argumentoInd[k][j] = '\0'; //Asignaremos el "finalizador" de cadena a cada comando leído.
				
				argumento[k] = argumentoInd[k]; //Ahora le pasaremos la cadena finalizada al puntero para que la ejecute.
				k++;
				if (cadena[i] == '<') {  // Si encontramos un '<', significa que hay que redirigir la entrada estándar.
					i++;
					if (cadena[i] != ' ') ejecutar=1; // Si no hay un espacio en blanco, significa que no hay un fichero de entrada.
					else { i++;
						for(j=0; cadena[i] != '\0' && cadena[i] != ' ' && cadena [i] != '|' && cadena [i] != '>'; j++){ 
							entrada[j] = cadena[i]; 
							i++; 
								}
						entrada[j] = '\0'; 
						if (cadena[i] != '\0') i++;
						redirecEntrada(entrada); // Llamamos a la función que redirige la entrada estandar.
						}
					}//Fin del condicional 
				
				} // Fin del bucle principal - WHILE-

	argumento[k] = NULL; 


	if (cadena[i] == '|') { // Si encontramos una tuberia, significa que hay que ejecutar otro comando
		k=0;
		i++;
		if (cadena[i] != ' ') ejecutar=1; // Si no hay un espacio en blanco, significa que no hay un comando
		else { 
		i++;
		flag_tuberia = 1; 
		while(cadena[i] != '\0' && cadena[i] != '>'){   
				for(j=0; cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '>';j++) {    
							argumentoInd2[k][j] = cadena[i]; 
							i++;}
				if (cadena[i] == ' ' ) i++; 
		
				argumentoInd2[k][j] = '\0';
				argumento2[k] = argumentoInd2[k];
				k++;
			}
		argumento2[k] = NULL;
		}		
	}


	
	if (cadena[i] == '>') {  // Si encontramos un '>', significa que hay que redirigir la salida estándar.
				i++;
				if (cadena[i] != ' ') ejecutar=1; // Si no hay espacio en blanco, significa que no hay un fichero de salida.
				else {
				i++;
				for(j=0; cadena[i] != '\0';j++) {
					redirec[j] = cadena[i];
					i++;
					}

				redirec[j] = '\0';
				redirecSalida(redirec);	
				}
	}

	
	if(ejecutar == 0) { // Si no hay errores, ejecutamos el comando.
				if (flag_tuberia==0) CrearProceso(argumento,plano); // Si no hay tuberia, ejecutamos el comando.
		
				else tuberias(argumento,argumento2);
			}
	else printf( MSJ_ERROR ); // Si hay errores, mostramos el mensaje de error.
		
}

void redirecEntrada(char cadena[Lmax]) //Función que redirige a la pantalla estandar.

{ 
  char *cadenaPtr; 
  int fd;   
  cadenaPtr = cadena; //Puntero a la cadena.
  fd = open (cadenaPtr,O_RDONLY);  //Cerramos la salida estándard asignamos el fichero a la salida.
  close (0); 	
  dup(fd);	
  

} 


void redirecSalida(char cadena[Lmax]) //Función que redirige a la pantalla estandar

{ 
  char *cadenaPtr;
  cadenaPtr = cadena; 
  close (1); 	
  open (cadenaPtr,O_CREAT | O_WRONLY,0777); 

} 

// Función que crea las tuberias y ejecuta los procesos.

void tuberias(char* argumento1[Lmax], char* argumento2[Lmax]) 

{  
  int fd[2],estado; 
	pid_t hijo; 
	hijo=fork();
	

	if (hijo==-1) printf("ERROR Creacion de proceso");  //Si no se ha podido crear el proceso, mostramos el mensaje de error.
	else if (hijo==0) { //Si se ha creado el proceso, ejecutamos el comando.
		pipe(fd); //Creamos la tuberia.
		close(fd[0]); //Cerramos la entrada de la tuberia.
		dup2(fd[1],1); //Duplicamos la salida de la tuberia.
		close(fd[1]); //Cerramos la salida de la tuberia.
		execvp(argumento1[0],argumento1); //Ejecutamos el comando.
		}
	else { //Si se ha creado el proceso, ejecutamos el comando.
		wait(&estado); //Esperamos a que termine el proceso.
		close(fd[1]); //Cerramos la salida de la tuberia.
		dup2(fd[0],0); //Duplicamos la entrada de la tuberia.
		close(fd[0]); //Cerramos la entrada de la tuberia.
		execvp(argumento2[0],argumento2); //Ejecutamos el comando.
		}
	} // Fin Función Tuberías  

void CrearProceso(char* argumento[Lmax],int plano) //Creamos los procesos.

{	
	int estado=0;  
	pid_t hijo; 
	hijo=fork(); //Creamos el proceso hijo.

if (hijo==-1) printf("ERROR Creacion de proceso");  
else if (hijo==0) { 
		
			execvp(argumento[0],argumento); // Recubrimiento del proceso hijo.
			perror("JASHELL");  
			exit(estado);
		   }
else  { 
		if (plano == 0) hijo=wait(&estado); 
		}

} // Fin función CrearProceso 