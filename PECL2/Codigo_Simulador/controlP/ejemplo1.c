#include <stdio.h>
#include <math.h>
#include <libplayerc/playerc.h>
// Defines
#define      PI     3.1416



int main(int argc, const char **argv)
{

  // Declaración de variables globales
  playerc_client_t *client;
  playerc_position2d_t *position2d;
  playerc_graphics2d_t *graficos;
  player_point_2d_t *puntos;
  
  double xp, yp, de, oe;
  double Kde, Koe, V, W;
  player_color_t color;  

  //  Declarar el resto de variables que sean necesarias
  double Ix, Iy, Iw;
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
  //   Pedir   por   teclado   el   punto   de   destino   (xp,   yp)   y   las   ganancias   de   los controladores (Kde y Koe)
  printf("Inserte la posicion final X:\n");  
  scanf("%lf",&xp);
  printf("Inserte la posicion final Y:\n");
  scanf("%lf",&yp);

  printf("Inserte valor de Kde:\n");  
  scanf("%lf",&Kde);
  printf("Inserte valor de Koe:\n");
  scanf("%lf",&Koe);

  // Dibujar el punto de destino
  puntos->px=xp;
  puntos->py=yp;
  playerc_graphics2d_draw_points (graficos, puntos, 1);

  //BUCLE DE CONTROL:
  while (1)
  {
	playerc_client_read(client);
        //Leemos la posición actual del robot
	Iw=position2d->pa;
	Ix=position2d->px;
	Iy=position2d->py;
	
        //Calculamos los errores lateral y de orientación 
	de=sqrt(((Ix-xp)*(Ix-xp))+((Iy-yp)*(Iy-yp)));
	oe=atan2((yp-Iy),(xp-Ix))-Iw;
	if(oe>PI){
		oe=oe-PI*2;
	}else if(oe<(-PI)){
		oe=oe+PI*2;
	}
        //Calculamos las señales de control (velocidades V y W)
	V=Kde*de;
	W=Koe*oe;

        //Enviamos las velocidades al robot (angular y lineal)
	playerc_position2d_set_cmd_vel(position2d,V,0.0,W,1);
	//playerc_position2d_set_cmd_pose(position2d,xp,yp,DTOR(45),1);      

	//Dibujamos un punto en la posición actual del robot
	playerc_client_read(client);
	puntos->px=position2d->px;
  	puntos->py=position2d->py;
  	playerc_graphics2d_draw_points (graficos, puntos, 1);	

        //Si llegamos cerca de los bordes del mapa, terminamos el programa
	if (position2d->px>20 | position2d->px<-20 | position2d->py>20 | position2d->py< -20)
		break;
  }
  // Shutdown
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_graphics2d_unsubscribe(graficos);
  playerc_graphics2d_destroy(graficos);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);
  return 0;
}

