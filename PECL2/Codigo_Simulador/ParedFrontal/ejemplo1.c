#include <stdio.h>
#include <math.h>
#include <libplayerc/playerc.h>

// Defines
#define PI 3.1416
#define OFFSETY 0.105

int main(int argc, const char **argv){
	// Declaración de variables globales
	playerc_client_t *client;
	playerc_position2d_t *position2d;
	playerc_sonar_t *sonar;
	playerc_graphics2d_t *graficos;
	player_point_2d_t *puntos;

	double de, oe;
	double Kde, Koe;
	player_color_t color;
	// Declarar el resto de variables que sean necesaria
	double vl,dp;
	double posicionXAnterior,posicionYAnterior,anguloOAnterior, medidaSAnteriorI, medidaSAnteriorD;
	double posicionX, posicionY,anguloO, medidaSD, medidaSI;
	double distanciaAvanzada,va;
	puntos=(player_point_2d_t*)malloc(sizeof(player_point_2d_t)*(1));

	// Create a client and connect it to the server.
	client = playerc_client_create(NULL, "localhost", 6665);
	if (0 != playerc_client_connect(client))
		return 1;
	// Create and subscribe to a graphics2d device.
	graficos = playerc_graphics2d_create(client, 0);
	if (playerc_graphics2d_subscribe(graficos, PLAYER_OPEN_MODE))
		return 1;
	// Create and subscribe to a position2d device.
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
		return 1;
	// Create and subscribe to a sonar device
	sonar = playerc_sonar_create(client, 0);
	if (playerc_sonar_subscribe(sonar, PLAYER_OPEN_MODE) != 0)
		return 1;

	// Pedir por teclado las ganancias del controlador Kde y Koe
	printf("Inserte valor de Kde:\n");  
	scanf("%lf",&Kde);
	printf("Inserte valor de Koe:\n");
	scanf("%lf",&Koe);

	double velocidad(){
		double s2=fabs(cos(sonar->poses[2].pyaw)*sonar->scan[2]);
		double s3=fabs(cos(sonar->poses[3].pyaw)*sonar->scan[3]);	
		double med= ((s2+s3)/2);

		if(med>=3.0){
			return 0.8;
		}
		else if(med<3 && med>1.0){
			return 0.4;
		}
		else{
			return 0.0;
		}
	}

	// Leer la primera medida del sonar 0 y de la posición del robot (para almacenarlas como medidas “anteriores”
	// para la primera iteración del bucle de control)
	playerc_sonar_get_geom(sonar);
	playerc_client_read(client);
	posicionXAnterior=position2d->px;
	posicionYAnterior=position2d->py;
	anguloOAnterior=position2d->pa;
	medidaSAnteriorI=sonar->scan[0];
	medidaSAnteriorD=sonar->scan[5];

	//cambiamos color
	playerc_graphics2d_setcolor(graficos,color);

	//BUCLE DE CONTROL:
	while (1){
	playerc_client_read(client);
	//Leemos la posición actual del robot y la medida actual del sensor 0
	posicionX=position2d->px;
	posicionY=position2d->py;
	anguloO=position2d->pa;
	medidaSI=sonar->scan[0];
	medidaSD=-(sonar->scan[5]);
	printf("medida izq %lf medida der %lf\n",medidaSI,medidaSD);
	dp=((fabs(medidaSD)+medidaSI)/2);
	printf("distancia centro %lf\n",dp);

	vl=velocidad();

	if(medidaSI<fabs(medidaSD)){
		printf("voy leyendo de la izquierda\n");
		distanciaAvanzada=sqrt(((posicionX-posicionXAnterior)*(posicionX-posicionXAnterior))+((posicionY-posicionYAnterior)*(posicionY-posicionYAnterior)));
		printf("valor distancia avanzada %lf\n",distanciaAvanzada);
		oe=atan2((medidaSI-medidaSAnteriorI),distanciaAvanzada);
		printf("valor oe %lf\n",oe);
		de=(medidaSI+OFFSETY)*cos(oe)-dp;
		printf("valor de de %lf\n",de);
		//Calculamos la señal de control (velocidad angular)
		va=Kde*de+Koe*oe;
		printf("valor de va %lf\n",va);
		//Enviamos las velocidades al robot (angular y lineal)
		if(vl==0){
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}else{
			playerc_position2d_set_cmd_vel(position2d,vl,0.0,va,1);
		}
		//Dibujamos un punto verde en la posición actual del robot
		playerc_client_read(client);
		puntos->px=position2d->px;
	  	puntos->py=position2d->py;
	  	playerc_graphics2d_draw_points (graficos, puntos, 1);
		medidaSAnteriorI=medidaSI;
		medidaSAnteriorD=medidaSD;
	}else{
		printf("voy leyendo de la derecha\n");
		distanciaAvanzada=sqrt(((posicionX-posicionXAnterior)*(posicionX-posicionXAnterior))+((posicionY-posicionYAnterior)*(posicionY-posicionYAnterior)));
		printf("valor distancia avanzada %lf\n",distanciaAvanzada);
		oe=atan2((medidaSD-medidaSAnteriorD),distanciaAvanzada);
		printf("valor oe %lf\n",oe);
		de=(medidaSD+OFFSETY)*cos(oe)+dp;
		printf("valor de de %lf\n",de);
		//Calculamos la señal de control (velocidad angular)
		va=Kde*de+Koe*oe;
		printf("valor de va %lf\n",va);
		//Enviamos las velocidades al robot (angular y lineal)
		if(vl==0){
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}else{
			playerc_position2d_set_cmd_vel(position2d,vl,0.0,va,1);
		}
		//Dibujamos un punto verde en la posición actual del robot
		playerc_client_read(client);
		puntos->px=position2d->px;
	  	puntos->py=position2d->py;
	  	playerc_graphics2d_draw_points (graficos, puntos, 1);
		medidaSAnteriorD=medidaSD;
		medidaSAnteriorI=medidaSI;
	}
	//Calculamos los errores lateral y de orientación
	//actualizamos las variables para la proxima iteracion
	posicionXAnterior=posicionX;
	posicionYAnterior=posicionY;
	anguloOAnterior=anguloO;
	}
	// Shutdown
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_sonar_unsubscribe(sonar);
	playerc_sonar_destroy(sonar);
	playerc_graphics2d_unsubscribe(graficos);
	playerc_graphics2d_destroy(graficos);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);
	return 0;
}

