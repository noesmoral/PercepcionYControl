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
#include <stdlib.h>
#include <time.h>


#include <libplayerc/playerc.h>

#define pi 3.141592

int
main(int argc, const char **argv)
{
  int i;
  playerc_client_t *client;		//conexion con el cliente
  playerc_position2d_t *position2d;	//subcripcion dispositivos necesario uno por componente
  playerc_sonar_t *sonar;		//suscribimos el sonar	


	//METODOS Y FUNCIONES DEL SONAR
	//variable y array globales
  double resultadosSonar[20][24];
  double vectorSx[8];
  double vectorSy[8];
  double vectorSxy[8];
  double vectorR[8];
  
	//para el random
  time_t t;	
  srand((unsigned)time(&t));


	//aleatorio para añadir error a la medida
  double aleatorioSonar(){
	double aux;
	aux=rand()%4;//es posible que tengamos que aumentarlo para que sea mayor
	aux=((100-aux)/100);
	return aux; 
  }

	//cuantas para las medidas
  double mediax(int num){
	int n;
	double mx=0.0;
	for (n=0; n < 20; n++){		//media	
		mx=mx+resultadosSonar[n][num*2];
	}
	return (mx/20);
  }
  double sx(int num){
	int n;
	double aux=0.0;	
	double Mx=0.0;
	Mx=mediax(num);
	for (n=0; n < 20; n++){		//sumatorio 
		aux=aux+pow((resultadosSonar[n][num*2]-Mx), 2);
	}
	aux=aux/19;				//division del sumatorio
	return sqrt(aux);			//raiz
  }
  double mediay(int num){
	int n;
	double my=0.0;
	for (n=0; n < 20; n++){		//media	
		my=my+resultadosSonar[n][(num+(num+1))];
	}
	return my/20;
  }
  double sy(int num){
	int n;
	double aux=0.0;	
	double My=0.0;	
	My=mediay(num);
	for (n=0; n < 20; n++){		//sumatorio 
		aux=aux+pow((resultadosSonar[n][(num+(num+1))]-My), 2);
	}
	aux=aux/19;				//division del sumatorio
	return sqrt(aux);			//raiz
  }
  double sxy(int num){
  	int n;
	double my=0.0;
	double My=0.0;
	double mx=0.0;
	double Mx=0.0;
	double aux=0.0;
	double total=0.0;
	Mx=mediax(num);
	My=mediay(num);
	for (n=0; n < 20; n++){		//media	
		mx=(resultadosSonar[n][num*2]-Mx);
		my=(resultadosSonar[n][(num+(num+1))]-My);
		total=total+(mx*my);
	}
	return (total/19);
  }

  int r(int num){
	return (vectorSxy[num]/(vectorSx[num]*vectorSy[num]));
  }

  int calidadPerson(int a ,int b){
	int res,res1;
  	double aux=fabs(vectorR[a]);
	if(aux>=0 && aux<0.25){
		res=50;
	}else if(aux>=0.25 && aux<0.5){
		res=30;
	}else if(aux>=0.50 && aux<0.75){
		res=15;
	}else{
		res=0;
	}
	double aux1=fabs(vectorR[b]);
	if(aux1>=0 && aux1<0.25){
		res1=50;
	}else if(aux1>=0.25 && aux1<0.5){
		res1=30;
	}else if(aux1>=0.50 && aux1<0.75){
		res1=15;
	}else{
		res1=0;
	}
	return res+res1;
  }

	//saca los valores de la distancia respecto al robot
  double globalX(int x,double s){
	return (fabs(sonar->poses[x].px)+(fabs(cos(fabs(sonar->poses[x].pyaw))*s)));	
  }
  double globalY(int x,double s){
  	return (fabs(sonar->poses[x].py)+(fabs(sin(fabs(sonar->poses[x].pyaw))*s)));	
  }

	//calculo posicion
  double trasero(){
	double s7;
	double s6;
	s6=fabs(cos(sonar->poses[6].pyaw)*sonar->scan[6]);
	s7=fabs(cos(sonar->poses[7].pyaw)*sonar->scan[7]);	
	return ((s6+s7)/2);
  }
  double frontal(){
	double s2;
	double s3;
	s2=fabs(cos(sonar->poses[2].pyaw)*sonar->scan[2]);
	s3=fabs(cos(sonar->poses[3].pyaw)*sonar->scan[3]);	
	return ((s2+s3)/2);
  }
  int paredesAlrededor(){
	double front=frontal();
	double tras=trasero();
	double izq=globalY(0,sonar->scan[0]);
	double der=globalY(5,sonar->scan[5]);
	double calidadFrontal=calidadPerson(2,3);
	double calidadTrasera=calidadPerson(6,7);
	double calidadDerecha=calidadPerson(4,5);
	double calidadIzquierda=calidadPerson(0,1);
	printf("Las medidas de los sensores son Font:%f Tras:%f Der:%f e Izq:%f\n",front,tras,der,izq);
	printf("Grado confianza frontal %f trasero %f derecho %f e izquierdo %f\n",calidadFrontal, calidadTrasera, calidadDerecha, calidadIzquierda);
	if(front<0.5 && tras<0.4 && izq<0.7 && der<0.7){
		//cuatro 15
		return 15;
	}else if(front<0.5 && izq<0.7 && der<0.7){
		//delante y lados 14
		return 14;
	}else if(front<0.5 && tras<0.4 && izq<0.7){
		//delante detras e izq 13
		return 13;
	}else if(tras<0.4 && izq<0.7 && der<0.7){
		//detras y lados 12
		return 12;
	}else if(front<0.5 && tras<0.4 && der<0.7){
		//delante detras y derehca 11
		return 11;
	}else if(tras<0.4 && izq<0.7){
		//esquina inferior izq 10
		return 10;
	}else if(izq<0.7 && der<0.7){
		//lados 9
		return 9;
	}else if(tras<0.4 && der<0.7){
		//esquina inferior derecha 8
		return 8;
	}else if(front<0.5 && izq<0.7){
		//7 esquina superiror izq
		return 7;
	}else if(front<0.4 && tras<0.4){
		//delante y detras 6
		return 6;
	}else if(front<0.5 && der<0.7){
		//esquina superior derecha 5
		return 5;
	}else if(izq<0.7){
		//izq 4
		return 4;
	}else if(tras<0.4){
		//detras 3
		return 3;
	}else if(der<0.7){
		//derecha 2
		return 2;
	}else if(front<0.5){
		//delante 1
		return 1;
	}else{
		//nada 0
		return 0;
	}	
  }

	//Metodo para realizar todos los calculos del sonar
  void recalcular(){
  	int zz;
  	double s1,s2,s3,s4,s5,s6,s7,s0;
  	for (zz=0; zz < 20; zz++) {
		playerc_client_read(client);
		s0=sonar->scan[0]*aleatorioSonar();
		resultadosSonar[zz][0]=globalX(0,s0);	//ejex MEDIDA DESDE EL SONAR A LA PARED NO DESDE EL CENTRO DEL ROBOT
		resultadosSonar[zz][1]=globalY(0,s0);	//ejey MEDIDA DESDE EL SONAR A LA PARED NO DESDE EL CENTRO DEL ROBOT	
		resultadosSonar[zz][16]=s0;		//distancia medida por el sensor
		s1=sonar->scan[1]*aleatorioSonar();
		resultadosSonar[zz][2]=globalX(1,s1);
		resultadosSonar[zz][3]=globalY(1,s1);
		resultadosSonar[zz][17]=s1;
		s2=sonar->scan[2]*aleatorioSonar();
		resultadosSonar[zz][4]=globalX(2,s2);
		resultadosSonar[zz][5]=globalY(2,s2);
		resultadosSonar[zz][18]=s2;
		s3=sonar->scan[3]*aleatorioSonar();
		resultadosSonar[zz][6]=globalX(3,s3);
		resultadosSonar[zz][7]=globalY(3,s3);
		resultadosSonar[zz][19]=s3;
		s4=sonar->scan[4]*aleatorioSonar();
		resultadosSonar[zz][8]=globalX(4,s4);
		resultadosSonar[zz][9]=globalY(4,s4);
		resultadosSonar[zz][20]=s4;
		s5=sonar->scan[5]*aleatorioSonar();
		resultadosSonar[zz][10]=globalX(5,s5);
		resultadosSonar[zz][11]=globalY(5,s5);
		resultadosSonar[zz][21]=s5;
		s6=sonar->scan[6]*aleatorioSonar();
		resultadosSonar[zz][12]=globalX(6,s6);
		resultadosSonar[zz][13]=globalY(6,s6);
		resultadosSonar[zz][22]=s6;
		s7=sonar->scan[7]*aleatorioSonar();
		resultadosSonar[zz][14]=globalX(7,s7);
		resultadosSonar[zz][15]=globalY(7,s7);
		resultadosSonar[zz][23]=s7;
		printf("0: %f  1: %f  2: %f  3: %f  4: %f  5: %f  6: %f  7: %f\n",s0,s1,s2,s3,s4,s5,s6,s7);
  	} 
		//CALCULA sx sy y xy para cada sensor
  	for(zz=0;zz<8;zz++){
		vectorSx[zz]=sx(zz);
		vectorSy[zz]=sy(zz);
		vectorSxy[zz]=sxy(zz);
	  }
		//CALCULA r para cada sensor
	  for(zz=0;zz<8;zz++){
		vectorR[zz]=r(zz);
	  }
  }

	//MOSTRAMOS los valores de x e y de los sensores
  void mostrarXY(){
  	int ao,zz;
  	for(zz=0; zz < 8; zz++){
		for(ao=0;ao<20;ao++){
			printf("Sensor %d valor x %f valor y %f\n ",zz,resultadosSonar[ao][zz*2], resultadosSonar[ao][(zz+(zz+1))]);
  		}
  	}
  }

	//MOSTRAMOS sxy y r
  void mostrarSXYR(){
	int zz;  	
	for(zz=0;zz<8;zz++){
		printf("Sensor %d valor sxy %f valor r %f\n",zz , vectorSxy[zz], vectorR[zz]);
  	}
  }

	//mostramos la informacion especifica de cada uno de los sensores
  void infoSensores(){
	printf("pose count %d y scan_count %d\n",sonar->pose_count, sonar->scan_count);
  	printf("En el sensor 0: px es %f,py es %f, pyaw es %f\n",sonar->poses[0].px, sonar->poses[0].py, sonar->poses[0].pyaw);
  	printf("En el sensor 1: px es %f,py es %f, pyaw es %f\n",sonar->poses[1].px, sonar->poses[1].py, sonar->poses[1].pyaw);
  	printf("En el sensor 2: px es %f,py es %f, pyaw es %f\n",sonar->poses[2].px, sonar->poses[2].py, sonar->poses[2].pyaw);
  	printf("En el sensor 3: px es %f,py es %f, pyaw es %f\n",sonar->poses[3].px, sonar->poses[3].py, sonar->poses[3].pyaw);
  	printf("En el sensor 4: px es %f,py es %f, pyaw es %f\n",sonar->poses[4].px, sonar->poses[4].py, sonar->poses[4].pyaw);
  	printf("En el sensor 5: px es %f,py es %f, pyaw es %f\n",sonar->poses[5].px, sonar->poses[5].py, sonar->poses[5].pyaw);
  	printf("En el sensor 6: px es %f,py es %f, pyaw es %f\n",sonar->poses[6].px, sonar->poses[6].py, sonar->poses[6].pyaw);
  	printf("En el sensor 7: px es %f,py es %f, pyaw es %f\n",sonar->poses[7].px, sonar->poses[7].py, sonar->poses[7].pyaw);
  }
	//FIN SONAR

	//METODOS PRINCICPALES
  void avanzar (double avance){
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
			n1=fabs(fabs(ejex)-fabs(position2d->px));
			n2=fabs(fabs(ejey)-fabs(position2d->py));
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
			n1=fabs(fabs(ejex)-fabs(position2d->px));
			n2=fabs(fabs(ejey)-fabs(position2d->py));
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
			n1=fabs(fabs(ejex)-fabs(position2d->px));
			n2=fabs(fabs(ejey)-fabs(position2d->py));
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
		difx=fabs(fabs(ejex)-fabs(position2d->px));
		dify=fabs(fabs(ejey)-fabs(position2d->py));
		//Le ponemos que empiece a moverse
		playerc_position2d_set_cmd_vel(position2d,vx,0.0,DTOR(0.0),1);
		while(position2d->px>(ejex+0.01) || position2d->py>(ejey+0.01)){
			playerc_client_read(client);
			n1=fabs(fabs(ejex)-fabs(position2d->px));
			n2=fabs(fabs(ejey)-fabs(position2d->py));
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
	giro=-giro;
	if(giro<0){	//girar izq
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
			n1=fabs(fabs(orientacion)-fabs(position2d->pa));
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
			n1=fabs(fabs(orientacion)-fabs(position2d->pa));
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
	//FIN METODOS PRINCIPALES

	//INICIO DEL PROGRAMA PRINCIPAL
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
  //printf("pose count %d y scan_count %d\n",sonar->pose_count, sonar->scan_count);

  playerc_client_read(client);
  playerc_client_read(client);

  infoSensores();
  recalcular();
  mostrarXY();  
  mostrarSXYR();

  //MAIN PRINCIPAL DE LOS MOVIMIENTOS
  int eleccion;
  while (position2d->px>-5.1){
	recalcular();
	eleccion=paredesAlrededor();
	printf("%d\n",eleccion);
	switch(eleccion){
		case 15:
			printf("Ha ingresado el numero 15\n");
			avanzar (0.1);
			break;
		case 14:
			printf("Ha ingresado el numero 14\n");
			girar(-180);
			avanzar (1.0);
			break;
		case 13:
			printf("Ha ingresado el numero 13\n");
			girar(-90);
			avanzar (1.0);
			break;
		case 12:
			printf("Ha ingresado el numero 12\n");
			avanzar (1.0);
			break;
		case 11:
			printf("Ha ingresado el numero 11\n");
			girar(90);
			avanzar (1.0);
			break;
		case 10:
			printf("Ha ingresado el numero 10\n");
			girar(-90);
			avanzar (1.0);
			break;
		case 9:
			printf("Ha ingresado el numero 9\n");
			avanzar (1.0);
			break;
		case 8:
			printf("Ha ingresado el numero 8\n");
			avanzar (1.0);
			break;
		case 7:
			printf("Ha ingresado el numero 7\n");
			girar(-90);
			avanzar (1.0);
			break;
		case 6:
			printf("Ha ingresado el numero 6\n");
			girar(90);
			avanzar (1.0);
			break;
		case 5:
			printf("Ha ingresado el numero 5\n");
			girar(90);
			avanzar (1.0);
			break;
		case 4:
			printf("Ha ingresado el numero 4\n");
			girar(-90);
			avanzar (1.0);
			break;
		case 3:
			printf("Ha ingresado el numero 3\n");
			avanzar (1.0);
			break;
		case 2:
			printf("Ha ingresado el numero 2\n");
			avanzar (1.0);
			break;
		case 1:
			printf("Ha ingresado el numero 1\n");
			girar(-90);
			avanzar (1.0);
			break;
		default:
			printf("Ha ingresado el numero 0\n");
			avanzar (1.0);
			break;
	}	
  } 
  //FIN


  // Shutdown desconexion y cerrado
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_sonar_unsubscribe(sonar);
  playerc_sonar_destroy(sonar);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}
