#include <stdio.h>	// libreria estandar
#include <stdlib.h> // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>   // para usar threads
#include <semaphore.h> // para usar semaforos
#include <unistd.h>

#define LIMITE 50

// creo estructura de semaforos
struct semaforos{
	sem_t sem_cortar;
	sem_t sem_mezclar;
    sem_t sem_salero;
    sem_t sem_sarten;
    sem_t sem_horno;
    sem_t sem_terminado;
};

// creo los pasos con los ingredientes
struct paso{
	char accion[LIMITE];
	char ingredientes[4][LIMITE];
};

// creo los parametros de los hilos
struct parametro{
	int equipo_param;
	struct semaforos semaforos_param;
	struct paso pasos_param[8];
};

// funcion para imprimir las acciones y los ingredientes de la accion
void *imprimirAccion(void *data, char *accionIn){
	struct parametro *mydata = data;
	// calculo la longitud del array de pasos
	int sizeArray = (int)(sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	// indice para recorrer array de pasos
	int i;
	for (i = 0; i < sizeArray; i++){
		// pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if (strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
			printf("\tEquipo %d - accion %s \n ", mydata->equipo_param, mydata->pasos_param[i].accion);
			// calculo la longitud del array de ingredientes
			int sizeArrayIngredientes = (int)(sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]));
			// indice para recorrer array de ingredientes
			int h;
			printf("\tEquipo %d -----------ingredientes : ----------\n", mydata->equipo_param);
			for (h = 0; h < sizeArrayIngredientes; h++){
				// consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
				if (strlen(mydata->pasos_param[i].ingredientes[h]) != 0)
				{
					printf("\tEquipo %d ingrediente  %d : %s \n", mydata->equipo_param, h, mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
    printf("############################## Fin acción #####################################\n");
}

// funcion para tomar de ejemplo
void *cortar(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "cortar";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_cortar);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_mezclar);

	pthread_exit(NULL);
}

void *mezclar(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "mezclar";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_mezclar);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (mezclar me habilita salar)
	sem_post(&mydata->semaforos_param.sem_salero);

	pthread_exit(NULL);
}

void *salar(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "salar";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_salero);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (salar me habilita cocinar en el sarten)
	sem_post(&mydata->semaforos_param.sem_sarten);

	pthread_exit(NULL);
}

void *cocinar(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "cocinar";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_sarten);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (cocinar me habilita hornear panes)
	sem_post(&mydata->semaforos_param.sem_horno);

	pthread_exit(NULL);
}

void *hornear(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "hornear";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_horno);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (hornear me habilita a terminar el sandwich)
	sem_post(&mydata->semaforos_param.sem_terminado);

	pthread_exit(NULL);
}

void *terminar(void *data){
	// creo el nombre de la accion de la funcion
	char *accion = "terminar";

	// creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_terminado);
	
	// llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata, accion);
	
	// uso sleep para simular que que pasa tiempo
	usleep(20000);

	// doy la señal a la siguiente accion (terminar me habilita a comenzar uno nuevo)
	sem_post(&mydata->semaforos_param.sem_cortar);

	pthread_exit(NULL);
}

void *ejecutarReceta(void *i){

	// variables semaforos
    sem_t sem_cortar;
	sem_t sem_mezclar;
    sem_t sem_salero;
    sem_t sem_sarten;
    sem_t sem_horno;
    sem_t sem_terminado;

	// variables hilos
	pthread_t p1;

	// numero del equipo (casteo el puntero a un int)
	int p = *((int *)i);

	printf("Ejecutando equipo %d \n", p);

	// reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	// seteo los valores al struct

	// seteo numero de grupo
	pthread_data->equipo_param = p;

	// seteo semaforos
    pthread_data->semaforos_param.sem_cortar = sem_cortar;
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
    pthread_data->semaforos_param.sem_salero = sem_salero;
    pthread_data->semaforos_param.sem_sarten = sem_sarten;
    pthread_data->semaforos_param.sem_horno = sem_horno;
    pthread_data->semaforos_param.sem_terminado = sem_terminado;
    
	// seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");

	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
	strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");

    strcpy(pthread_data->pasos_param[2].accion, "salar");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "Carne");
	strcpy(pthread_data->pasos_param[2].ingredientes[1], "Sal");
	strcpy(pthread_data->pasos_param[2].ingredientes[2], "Ajo");
	strcpy(pthread_data->pasos_param[2].ingredientes[3], "Perejil");

    strcpy(pthread_data->pasos_param[3].accion, "cocinar");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "milanesa");

    strcpy(pthread_data->pasos_param[4].accion, "hornear");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "Pan");

    strcpy(pthread_data->pasos_param[5].accion, "terminar");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "Sandwich");

	// inicializo los semaforos
    sem_init(&(pthread_data->semaforos_param.sem_cortar), 0, 1);
	sem_init(&(pthread_data->semaforos_param.sem_mezclar), 0, 0);
    sem_init(&(pthread_data->semaforos_param.sem_salero), 0, 0);
    sem_init(&(pthread_data->semaforos_param.sem_sarten), 0, 0);
    sem_init(&(pthread_data->semaforos_param.sem_horno), 0, 0);
    sem_init(&(pthread_data->semaforos_param.sem_terminado), 0, 0);

	// creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos
	int rc;
	rc = pthread_create(&p1,NULL,cortar,pthread_data); // hilo para cortar

    int rm;
	rm = pthread_create(&p1,NULL,mezclar,pthread_data); // hilo para mezclar

    int rs;
	rs = pthread_create(&p1,NULL,salar,pthread_data); // hilo para salar

    int rcocinar;
	rcocinar = pthread_create(&p1,NULL,cocinar,pthread_data); // hilo para cocinar en sarten

    int rh;
	rh = pthread_create(&p1,NULL,hornear,pthread_data); // hilo para hornear

    int rt;
	rt = pthread_create(&p1,NULL,terminar,pthread_data); // hilo para informar que se termino el sandwich

	// join de todos los hilos
	pthread_join(p1, NULL);
	// crear join de demas hilos

	// valido que cada hilo se alla creado bien
	if (rc){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

    if (rm){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

    if (rs){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

    if (rcocinar){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

    if (rh){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

    if (rt){
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

	// destruccion de los semaforos
    sem_destroy(&sem_cortar);
	sem_destroy(&sem_mezclar);
    sem_destroy(&sem_salero);
    sem_destroy(&sem_sarten);
    sem_destroy(&sem_horno);
    sem_destroy(&sem_terminado);


	// salida del hilo
	pthread_exit(NULL);
}

int main(){
	// creo los nombres de los equipos
	int rc;
	int *equipoNombre1 = malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 = malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 = malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 = malloc(sizeof(*equipoNombre4));

	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	// creo las variables los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;

	// inicializo los hilos de los equipos
	rc = pthread_create(&equipo1,		// identificador unico
						NULL,			// atributos del thread
						ejecutarReceta, // funcion a ejecutar
						equipoNombre1);

	rc = pthread_create(&equipo2,		// identificador unico
						NULL,			// atributos del thread
						ejecutarReceta, // funcion a ejecutar
						equipoNombre2);
	rc = pthread_create(&equipo3,		// identificador unico
						NULL,			// atributos del thread
						ejecutarReceta, // funcion a ejecutar
						equipoNombre3);
	rc = pthread_create(&equipo4,		// identificador unico
						NULL,			// atributos del thread
						ejecutarReceta, // funcion a ejecutar
						equipoNombre4);

	if (rc)
	{
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	}

	// join de todos los hilos
	pthread_join(equipo1, NULL);
	pthread_join(equipo2, NULL);
	pthread_join(equipo3, NULL);
	pthread_join(equipo4, NULL);

	pthread_exit(NULL);
}

// Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
// Para ejecutar:   ./ejecutable
