/*
Copyright (c) 2002, Andrew Howard
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Player Project nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <math.h>

#include <libplayerc/playerc.h>

#define pi 3.141592

int
main(int argc, const char **argv)
{
  int i;
  playerc_client_t *client;		//conexion con el cliente
  playerc_position2d_t *position2d;	//subcripcion dispositivos necesario uno por componente
  playerc_laser_t *laser;		//suscribimos el laser	

	//punteros para el laser
  double min_angle1=-0.79;
  double min_angle2;
  double max_angle1=0.79;
  double max_angle2;
  double resolution1=0.006;
  double resolution2;  
  double range_res1=1;
  double range_res2;
  unsigned char intensity1=1;
  unsigned char intensity2;
  double scanning_frecuency1=10;
  double scanning_frecuency2;

  void mostrarInfoLaser(){
  	//mostramos la informacion especifica de cada uno de los sensores
  	printf("Laser geometry in robot: posicion %lf orientacion %lf tamaño %lf\n",laser->pose[0],laser->pose[1],laser->pose[2]);
  	printf("Robot pose: x %lf y %lf angulo %lf\n",laser->robot_pose[0],laser->robot_pose[1],laser->robot_pose[2]);
  	printf("Otros: intesidad de datos %d numero puntos scan %d inicio angulo escaner %lf resolucion angulo %lf rango resolition %lf maximo rango sensor %lf frecuencia escaneo %lf\n",laser->intensity_on,laser->scan_count,laser->scan_start,laser->scan_res,laser->range_res,laser->max_range,laser->scanning_frequency);
  
  	//Mostramos el resto de elementos de la structura concretamente los que soon del tipo *
  	printf("ID del scan %d Laser ID %d Minimo rango derecho %lf Minimo rango Izq %lf\n", laser->scan_id, laser->laser_id, laser->min_right, laser->min_left);
  }

  void distanciasLaser(){
	int zzz;
	//bucle para mostrar las distancias
  	for (zzz=0;zzz<laser->scan_count;zzz++){ 
		printf("Scan %d metros %lf Scan radianes %lf Punto X %lf Punto Y %lf \n",zzz, laser->scan[zzz][0], laser->scan[zzz][1],laser->point[zzz].px,laser->point[zzz].py);
  	}
  }  


  void avanzar (int avance){
	double vx=0.3;	//la velocidad en x  avance recto solo la y no vale
	double difx,dify,n1,n2;
	int auxiliar=1;
	//Calculamos donde estamos
	playerc_client_read(client);
	double orientacion=position2d->pa;
	double ejex=position2d->px;
	double ejey=position2d->py;
	printf("la orientacion es %f en la posicion en x es %f y la posicion en y es %f\n",orientacion,ejex,ejey);
	//obtenemos la nueva ubicacion en la que hemos de terminar
	//Calculamos la posicion donde tendremos que movernos
	ejex=(cos(orientacion)*avance)+ejex;
	ejey=(sin(orientacion)*avance)+ejey;
	printf("la posicion final tiene que ser %f en x y %f en y\n",ejex,ejey);
	//Comprobar si hemos llegado y parar
	auxiliar=1;
	playerc_client_read(client);
	if(position2d->px<=ejex && position2d->py<=ejey){//cuadrante 1
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,vx,0.0,DTOR(0.0),1);
		while(position2d->px<(ejex-0.01) || position2d->py<(ejey-0.01)){
			playerc_client_read(client);
			n1=abs(abs(ejex)-abs(position2d->px));
			n2=abs(abs(ejey)-abs(position2d->py));
			if(((n1<(1.0)) && (n2<(1.0))) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0.2,0.0,DTOR(0.0),1);
				auxiliar=2;
			}
			if(((n1<(0.5)) && (n2<(0.5))) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0.1,0.0,DTOR(0.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}else if(position2d->px<=ejex && position2d->py>=ejey){//cuadrante 4
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,vx,0.0,DTOR(0.0),1);
		while(position2d->px<(ejex-0.01) || position2d->py>(ejey+0.01)){
			playerc_client_read(client);
			n1=abs(abs(ejex)-abs(position2d->px));
			n2=abs(abs(ejey)-abs(position2d->py));
			if(((n1<(1.0)) && (n2<(1.0))) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0.2,0.0,DTOR(0.0),1);
				auxiliar=2;
			}
			if(((n1<(0.5)) && (n2<(0.5))) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0.1,0.0,DTOR(0.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}else if(position2d->px>=ejex && position2d->py<=ejey){//cuadrante 2
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,vx,0.0,DTOR(0.0),1);
		while(position2d->px>(ejex+0.01) || position2d->py<(ejey-0.01)){
			playerc_client_read(client);
			n1=abs(abs(ejex)-abs(position2d->px));
			n2=abs(abs(ejey)-abs(position2d->py));
			if(((n1<(1.0)) && (n2<(1.0))) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0.2,0.0,DTOR(0.0),1);
				auxiliar=2;
			}
			if(((n1<(0.5)) && (n2<(0.5))) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0.1,0.0,DTOR(0.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}else{//cuadrante 3
		difx=abs(abs(ejex)-abs(position2d->px));
		dify=abs(abs(ejey)-abs(position2d->py));
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,vx,0.0,DTOR(0.0),1);
		while(position2d->px>(ejex+0.01) || position2d->py>(ejey+0.01)){
			playerc_client_read(client);
			n1=abs(abs(ejex)-abs(position2d->px));
			n2=abs(abs(ejey)-abs(position2d->py));
			if(((n1<(1.0)) && (n2<(1.0))) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0.2,0.0,DTOR(0.0),1);
				auxiliar=2;
			}
			if(((n1<(0.5)) && (n2<(0.5))) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0.1,0.0,DTOR(0.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}	
	//Parar de movernos
	playerc_position2d_set_cmd_vel(position2d,0.0,0.0,DTOR(0.0),1);			
  }


  void girar(int giro){
	double aux=0.0;
	double aux1=0.0;
	double aux2=0.0;
	double n1=0.0;
	int auxiliar=1;
	//Calculamos donde estamos
	playerc_client_read(client);
	double orientacion=position2d->pa;
	printf("la orientacion es %f \n",orientacion);
	auxiliar=1;
	if(giro<0){	//girar izq
		//obtenemos la nueva ubicacion en la que hemos de terminar
		//Calculamos la posicion donde tendremos que movernos
		if(position2d->pa<0){		//lado negativo como comienzo
			aux=((giro*pi)/180);
			orientacion=(position2d->pa)-aux;	
		}else{				//lado positivo de comienzo
			aux=((giro*pi)/180);
			orientacion=(position2d->pa)-aux;
			if(orientacion>pi){	//si superamos pi cambio signo
				aux=orientacion-pi;
				orientacion=pi-aux;
				orientacion=-orientacion;
			}
		}
		printf("la orientacion final tiene que ser %f\n",orientacion);
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,0.0,0.0,DTOR(15.0),1);
		playerc_client_read(client);
		while(position2d->pa>(orientacion+0.02) || position2d->pa<(orientacion-0.02)){
			playerc_client_read(client);
			n1=abs(abs(orientacion)-abs(position2d->pa));
			if((n1<0.8) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(7.0),1);
				auxiliar=2;
			}
			if((n1<0.2) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(1.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}else{	//girar derecha
		//obtenemos la nueva ubicacion en la que hemos de terminar
		//Calculamos la posicion donde tendremos que movernos
		//obtenemos la nueva ubicacion en la que hemos de terminar
		//Calculamos la posicion donde tendremos que movernos
		if(position2d->pa<0){		//lado negativo como comienzo
			aux=((giro*pi)/180);
			orientacion=(position2d->pa)-aux;
			if(orientacion<(-pi)){	//si superamos pi cambio signo
				aux=orientacion+pi;
				orientacion=pi+aux;
			}	
		}else{				//lado positivo de comienzo
			aux=((giro*pi)/180);
			orientacion=(position2d->pa)-aux;	
		}
		printf("la orientacion final tiene que ser %f\n",orientacion);
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,0.0,0.0,DTOR(-12.0),1);
		playerc_client_read(client);
		while(position2d->pa>(orientacion+0.02) || position2d->pa<(orientacion-0.02)){
			playerc_client_read(client);
			n1=abs(abs(orientacion)-abs(position2d->pa));
			if((n1<0.8) && auxiliar==1){
				playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-7.0),1);
				auxiliar=2;
			}
			if((n1<0.2) && auxiliar==2){
				playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-1.0),1);
				auxiliar=0;
			}
			playerc_client_read(client);
			printf("position2d : %f %f %f %d\n",position2d->px, position2d->py, position2d->pa, position2d->stall);
		}
	}	
	//Parar de movernos
	playerc_position2d_set_cmd_vel(position2d,0.0,0.0,DTOR(0.0),1);	
  }

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);	//conexion al player en este caso local si no cambiar
  if (0 != playerc_client_connect(client))
    return -1;

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
    return -1;
  //habilitamos los motores
  playerc_position2d_enable(position2d,1);

  // Create and subscribe to a laser device.
  laser = playerc_laser_create (client, 0);
  if (playerc_laser_subscribe(laser, PLAYER_OPEN_MODE))
    return -1;

  //obtenemos las mediciones y mostramos la informacion general
  playerc_client_read(client);
  playerc_laser_get_geom(laser);
  playerc_client_read(client);

  mostrarInfoLaser();
  distanciasLaser();			

  //comprobamos la configuracion original
  playerc_laser_get_config(laser,&min_angle2,&max_angle2,&resolution2,&range_res2,&intensity2,&scanning_frecuency2);
  printf("Original min angle %f max angle %f resolucion %f resolucion del rango %f intensidad %u frecuencia escanep %f\n",min_angle2, max_angle2, resolution2, range_res2, intensity2, scanning_frecuency2);

  //añadimos la nueva configuracion
  playerc_laser_set_config(laser,min_angle1,max_angle1,resolution1,range_res1,intensity1,scanning_frecuency1);
  
  //verificamos que se ha cambiado
  mostrarInfoLaser();
  
  //comprobamos el cambio
  playerc_laser_get_config(laser,&min_angle2,&max_angle2,&resolution2,&range_res2,&intensity2,&scanning_frecuency2);

  //imprimimos las nuevas variables que han de tener la nueva medida
  printf("Nuevo min angle %f max angle %f resolucion %f resolucion del rango %f intensidad %u frecuencia escanep %f\n",min_angle2, max_angle2, resolution2, range_res2, intensity2, scanning_frecuency2);

  playerc_client_read(client);

  //Main princical  

  // Shutdown desconexion y cerrado
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_laser_unsubscribe(laser);
  playerc_laser_destroy(laser);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}
