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
  playerc_sonar_t *sonar;		//suscribimos el sonar	

  double aleatorioSonar(){
	double aux;
	aux=(100-(rand() % 3)/100);
	printf("%f\n",aux);
	return ((100-(rand() % 3)/100)); 
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

  // Create and subscribe to a sonar device.
  sonar = playerc_sonar_create(client, 0);
  if (playerc_sonar_subscribe(sonar, PLAYER_OPEN_MODE))
    return -1;

  //obtenemos las mediciones y mostramos la informacion general
  playerc_sonar_get_geom(sonar);
  printf("pose count %d y scan_count %d\n",sonar->pose_count, sonar->scan_count);

  //mostramos la informacion especifica de cada uno de los sensores
   printf("En el sensor 0: px es %f,py es %f,pyaw es %f\n",sonar->poses[0].px, sonar->poses[0].py, sonar->poses[0].pyaw);
  printf("En el sensor 1: px es %f,py es %f, pyaw es %f\n",sonar->poses[1].px, sonar->poses[1].py, sonar->poses[1].pyaw);
  printf("En el sensor 2: px es %f,py es %f, pyaw es %f\n",sonar->poses[2].px, sonar->poses[2].py, sonar->poses[2].pyaw);
  printf("En el sensor 3: px es %f,py es %f, pyaw es %f\n",sonar->poses[3].px, sonar->poses[3].py, sonar->poses[3].pyaw);
  printf("En el sensor 4: px es %f,py es %f, pyaw es %f\n",sonar->poses[4].px, sonar->poses[4].py, sonar->poses[4].pyaw);
  printf("En el sensor 5: px es %f,py es %f, pyaw es %f\n",sonar->poses[5].px, sonar->poses[5].py, sonar->poses[5].pyaw);
  printf("En el sensor 6: px es %f,py es %f, pyaw es %f\n",sonar->poses[6].px, sonar->poses[6].py, sonar->poses[6].pyaw);
  printf("En el sensor 7: px es %f,py es %f, pyaw es %f\n",sonar->poses[7].px, sonar->poses[7].py, sonar->poses[7].pyaw);


  playerc_client_read(client);
  playerc_client_read(client);

  int zz;
  float s1,s2,s3,s4,s5,s6,s7,s0;
  for (zz=0; zz < 100; zz++) {
	playerc_client_read(client);
	s0=sonar->scan[0]*aleatorioSonar();
	s1=sonar->scan[1]*aleatorioSonar();
	s2=sonar->scan[2]*aleatorioSonar();
	s3=sonar->scan[3]*aleatorioSonar();
	s4=sonar->scan[4]*aleatorioSonar();
	s5=sonar->scan[5]*aleatorioSonar();
	s6=sonar->scan[6]*aleatorioSonar();
	s7=sonar->scan[7]*aleatorioSonar();
	printf("0: %f  1: %f  2: %f  3: %f  4: %f  5: %f  6: %f  7: %f\n",s0,s1,s2,s3,s4,s5,s6,s7);
  }

  playerc_client_read(client);	 
  

  //playerc_position2d_set_odom(position2d,0.0,0.0,1.57);

  //Main princical
  avanzar(1);
  girar(-90);	//nos referimos a la posicion absoltua respecto a la inicial (0 derecha 90 arriba 180 izq 270 abajo) a la que queremos ir (- giro izq) (+ giro der)
  avanzar(3);
  girar(90);
  avanzar(1);
  girar(-90);
  avanzar(1);
  girar(90);
  avanzar(1);
  girar(-90);
  avanzar(1);  

  // Shutdown desconexion y cerrado
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_sonar_unsubscribe(sonar);
  playerc_sonar_destroy(sonar);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}
