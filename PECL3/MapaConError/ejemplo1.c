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
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <libplayerc/playerc.h>

#define pi 3.141592

int main(int argc, const char **argv){
	//especial para crear el mapa una estructura para la informacion se ha eligido de 11x11 para que se pueda usar se empieze donde se empieze
  	struct MATRIX{
 		int casillaNueva;	//marca para ver si esta visitada o no
		char marca;		//marca inicio camino o fin
		int veces;		//veces pasado
		int formato;		//tipo de casilla
  	}MAPA[11][11];

  	//contador de casilllas para termianr tiene un maximo de 25
  	int contadorCasillas=25;

  	//funcion para imprimir los tipos de casillas segun su nuero
  	void mostrarEsquema(){
		printf("\n");
		printf("El mapa esta representado mediante la informacion de la tabla en la que el simbolo + representa un hueco libre y el simbolo - o | representa una pared, se ha de tener en cuenta que el numero de la casilla del mapa corresponede con los dibujos\n");
		printf("++++   ----   ++++   ++++\n");
		printf("+  +   +  +   +  |   +  +\n");
		printf("+  +   +  +   +  |   +  +\n");
		printf("++++0  ++++1  ++++2  ----3\n\n");

		printf("++++   ----   ----   ----\n");
		printf("|  +   +  |   +  +   |  +\n");
		printf("|  +   +  |   +  +   |  +\n");
		printf("++++4  ++++5  ----6  ++++7\n\n");

		printf("++++   ++++   ++++   ----\n");
		printf("+  |   |  |   |  +   +  |\n");	
		printf("+  |   |  |   |  +   +  |\n");
		printf("----8  ++++9  ----10 ----11\n\n");

		printf("++++   ----   ----   ----\n");
		printf("|  |   |  +   |  |   |  |\n");
		printf("|  |   |  +   |  |   |  |\n");
		printf("----12 ----13 ++++14 ----15\n\n");
  	}

  	//funcion que nos da el valor del tipo de casilla en funcion del angulo para poder represetnarlo con la misma orientacion
	int representacionMapa(double orientacion, int valor){
		if(orientacion>=(-0.79) && orientacion<0.79){		//si estoy mirando a 0
			switch(valor){
				case 15: return 15;
					break;
				case 14: return 11;
					break;
				case 13: return 14;
					break;
				case 12: return 13;
					break;
				case 11: return 12;
					break;
				case 10: return 7;
					break;
				case 9: return 6;
					break;
				case 8: return 10;
					break;
				case 7: return 5;
					break;
				case 6: return 9;
					break;
				case 5: return 8;
					break;
				case 4: return 1;
					break;
				case 3: return 4;
					break;
				case 2: return 3;
					break;
				case 1: return 2;
					break;
				default:return 0;
					break;
			}	
		}else if(orientacion>=0.79 && orientacion<2.36){		//si estoy mirando a 90 es tal cual ya que es la que se ha elegido como base
			return valor;
		}else if(orientacion<(-0.79) && orientacion>=(-2.36)){	//si estoy mirando a -90
			switch(valor){
				case 15: return 15;
					break;
				case 14: return 12;
					break;
				case 13: return 11;
					break;
				case 12: return 14;
					break;
				case 11: return 13;
					break;
				case 10: return 5;
					break;
				case 9: return 9;
					break;
				case 8: return 7;
					break;
				case 7: return 8;
					break;
				case 6: return 6;
					break;
				case 5: return 10;
					break;
				case 4: return 2;
					break;
				case 3: return 1;
					break;
				case 2: return 4;
					break;
				case 1: return 3;
					break;
				default:return 0;
					break;
			}
		}else{							//si estoy mirando a 180
			switch(valor){
				case 15: return 15;
					break;
				case 14: return 13;
					break;
				case 13: return 12;
					break;
				case 12: return 11;
					break;
				case 11: return 14;
					break;
				case 10: return 8;
					break;
				case 9: return 6;
					break;
				case 8: return 5;
					break;
				case 7: return 10;
					break;
				case 6: return 9;
					break;
				case 5: return 7;
					break;
				case 4: return 3;
					break;
				case 3: return 2;
					break;
				case 2: return 1;
					break;
				case 1: return 4;
					break;
				default:return 0;
					break;
			}
		}
	}	

	//funcion que nos da el valor redondo ya que los movimiento son al centro de las casilla evitando asi el error
	int redondear(double z){
	  	if(z>4.5){
			return 5;	
		}else if(z<=4.5 && z>3.5){
			return 4;
		}else if(z<=3.5 && z>2.5){
			return 3;
		}else if(z<=2.5 && z>1.5){
			return 2;
		}else if(z<=1.5 && z>0.5){
			return 1;
		}else if(z<=0.5 && z>-0.5){
			return 0;
		}else if(z<=-0.5 && z>-1.5){
			return -1;
		}else if(z<=-1.5 && z>-2.5){
			return -2;
		}else if(z<=-2.5 && z>-3.5){
			return -3;
		}else if(z<=-3.5 && z>-4.5){
			return -4;
		}else{
			return -5;	
		}
	}
	  
	int i;
	playerc_client_t *client;			//conexion con el cliente
	playerc_position2d_t *position2d;		//subcripcion dispositivos necesario uno por componente
	playerc_sonar_t *sonar;				//suscribimos el sonar	
	playerc_laser_t *laser;				//nos suscribimos al laser


	//METODOS Y FUNCIONES DEL SONAR
	//variable y array globales
	double resultadosSonar[3][24];
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
		for (n=0; n < 3; n++){		//media	
			mx=mx+resultadosSonar[n][num*2];
		}
		return (mx/3);
	}
	double sx(int num){
		int n;
		double aux=0.0;	
		double Mx=0.0;
		Mx=mediax(num);
		for (n=0; n < 3; n++){		//sumatorio 
			aux=aux+pow((resultadosSonar[n][num*2]-Mx), 2);
		}
		aux=aux/2;				//division del sumatorio
		return sqrt(aux);			//raiz
	}
	double mediay(int num){
		int n;
		double my=0.0;
		for (n=0; n < 3; n++){		//media	
			my=my+resultadosSonar[n][(num+(num+1))];
		}
		return my/3;
	}
	double sy(int num){
		int n;
		double aux=0.0;	
		double My=0.0;	
		My=mediay(num);
		for (n=0; n < 3; n++){		//sumatorio 
			aux=aux+pow((resultadosSonar[n][(num+(num+1))]-My), 2);
		}
		aux=aux/2;				//division del sumatorio
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
		for (n=0; n < 3; n++){		//media	
			mx=(resultadosSonar[n][num*2]-Mx);
			my=(resultadosSonar[n][(num+(num+1))]-My);
			total=total+(mx*my);
		}
		return (total/2);
	}
	int r(int num){
		return (vectorSxy[num]/(vectorSx[num]*vectorSy[num]));
	}

	//funcion para cañidad de la medida de la pared
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

	//calculo distancia con la medida de los sensores
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
	
	//funcion para sacar las parades que tenemos al rededor y devolver el numero de la casilla
	int paredesAlrededor(){
		double front=frontal();
		double tras=trasero();
		double izq=globalY(0,sonar->scan[0]);
		double der=globalY(5,sonar->scan[5]);
		double calidadFrontal=calidadPerson(2,3);
		double calidadTrasera=calidadPerson(6,7);
		double calidadDerecha=calidadPerson(4,5);
		double calidadIzquierda=calidadPerson(0,1);
		//printf("Las medidas de los sensores son Font:%f Tras:%f Der:%f e Izq:%f\n",front,tras,der,izq);
		printf("Grado confianza frontal %f trasero %f derecho %f e izquierdo %f\n",calidadFrontal, calidadTrasera, calidadDerecha, calidadIzquierda);
		if(front<0.55 && tras<0.4 && izq<0.7 && der<0.7){
			//cuatro 15
			return 15;
		}else if(front<0.55 && izq<0.7 && der<0.7){
			//delante y lados 14
			return 14;
		}else if(front<0.55 && tras<0.4 && izq<0.7){
			//delante detras e izq 13
			return 13;
		}else if(tras<0.4 && izq<0.7 && der<0.7){
			//detras y lados 12
			return 12;
		}else if(front<0.55 && tras<0.4 && der<0.7){
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
		}else if(front<0.55 && izq<0.7){
			//7 esquina superiror izq
			return 7;
		}else if(front<0.55 && tras<0.4){
			//delante y detras 6
			return 6;
		}else if(front<0.55 && der<0.7){
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
		}else if(front<0.55){
			//delante 1
			return 1;
		}else{
			//nada 0
			return 0;
		}	
	}

	//funcion rellenar blancos
	void rellenarBlancos(){
		int AA,BB, aux;
		for(AA=1; AA < 10; AA++){
			for(BB=1;BB<10;BB++){
				if(MAPA[BB][AA].casillaNueva==1){	
					if((MAPA[BB][AA-1].casillaNueva==0 && MAPA[BB+1][AA].casillaNueva==0) || (MAPA[BB][AA+1].casillaNueva==0 && MAPA[BB+1][AA].casillaNueva==0)){		//superior derecho
						aux=MAPA[BB+1][AA].formato;
						switch (aux){
							case 4: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 7: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 9: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 10: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 12: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 13: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 14: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							default:MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=0; 
								break;
						}
					}else if((MAPA[BB][AA-1].casillaNueva==0 && MAPA[BB-1][AA].casillaNueva==0) || (MAPA[BB][AA+1].casillaNueva==0 && MAPA[BB-1][AA].casillaNueva==0)){	//superior izq
						aux=MAPA[BB-1][AA].formato;
						switch (aux){
							case 2: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 5: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 8: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 9: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 11: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 12: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							case 14: MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=15;
								break;
							default:MAPA[BB][AA].casillaNueva=0; MAPA[BB][AA].formato=0; 
								break;
						}
					}
				}
			}
		printf("\n\n");
		}
	}

	//Metodo para realizar todos los calculos del sonar
	void recalcular(){
	  	int zz;
	  	double s1,s2,s3,s4,s5,s6,s7,s0;
	  	for (zz=0; zz < 3; zz++) {
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
			//printf("0: %f  1: %f  2: %f  3: %f  4: %f  5: %f  6: %f  7: %f\n",s0,s1,s2,s3,s4,s5,s6,s7);
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
			for(ao=0;ao<10;ao++){
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
	//funcion para avanzar con contol P
	void avanzar (double avance){
		//Calculamos donde estamos
		playerc_client_read(client);
		double orientacion=position2d->pa;
		double ejex=position2d->px;
		double ejey=position2d->py;
		double avanzarlaser=0.0;
		double frontallaser=0.0;
		double Iw, Ix, Iy, de, oe, V, W;
		//printf("la orientacion es %f en la posicion en x es %f y la posicion en y es %f\n",orientacion,ejex,ejey);
		//obtenemos la nueva ubicacion en la que hemos de terminar
		//Calculamos la posicion donde tendremos que movernos
		ejex=(cos(orientacion)*avance)+ejex;
		ejey=(sin(orientacion)*avance)+ejey;
		//printf("la posicion final tiene que ser %f en x y %f en y\n",ejex,ejey);
		//redondeamos el movimiento ya que se pide movimientos precisos por eso se redondea a la unidad
		if(ejex>4.5){
			ejex=5;	
		}else if(ejex<=4.5 && ejex>3.5){
			ejex=4;
		}else if(ejex<=3.5 && ejex>2.53){
			ejex=3;
		}else if(ejex<=2.5 && ejex>1.5){
			ejex=2;
		}else if(ejex<=1.5 && ejex>0.5){
			ejex=1;
		}else if(ejex<=0.5 && ejex>-0.5){
			ejex=0;
		}else if(ejex<=-0.5 && ejex>-1.5){
			ejex=-1;
		}else if(ejex<=-1.5 && ejex>-2.5){
			ejex=-2;
		}else if(ejex<=-2.5 && ejex>-3.5){
			ejex=-3;
		}else if(ejex<=-3.5 && ejex>-4.5){
			ejex=-4;
		}else{
			ejex=-5;	
		}

		if(ejey>4.5){
			ejey=5;	
		}else if(ejey<=4.5 && ejey>3.5){
			ejey=4;
		}else if(ejey<=3.5 && ejey>2.5){
			ejey=3;
		}else if(ejey<=2.5 && ejey>1.5){
			ejey=2;
		}else if(ejey<=1.5 && ejey>0.5){
			ejey=1;
		}else if(ejey<=0.5 && ejey>-0.5){
			ejey=0;
		}else if(ejey<=-0.5 && ejey>-1.5){
			ejey=-1;
		}else if(ejey<=-1.5 && ejey>-2.5){
			ejey=-2;
		}else if(ejey<=-2.5 && ejey>-3.5){
			ejey=-3;
		}else if(ejey<=-3.5 && ejey>-4.5){
			ejey=-4;
		}else{
			ejey=-5;	
		}	

		//mostramos posicion destion redondeada
		//printf("la posicion final tiene que ser %f en x y %f en y\n",ejex,ejey);

		//odometria
		//printf("distancia frontal %lf\n",laser->scan[180][0]);	
		avanzarlaser=laser->scan[180][0]-1;

		//Movernos mediante control P
		playerc_client_read(client);
		//Leemos la posición actual del robot antes de entrar para tener una referencia
		Iw=position2d->pa;
		Ix=position2d->px;
		Iy=position2d->py;
		//Calculamos los errores lateral y de orientación 
		de=laser->scan[180][0]-avanzarlaser;
		frontallaser=laser->scan[180][0];
		
		//mientras no estemos lo sufucientemente cerca respecto al error de la distancia continuamos
		while(fabs(de)>0.12 && frontallaser>0.54){	//fabs(de)>0.07  avanzarlaser-laser->scan[180][0]<0.9
			playerc_client_read(client);
			//Leemos la posición actual del robot
			Iw=position2d->pa;
			Ix=position2d->px;
			Iy=position2d->py;
			//Calculamos los errores lateral y de orientación 
			oe=atan2((ejey-Iy),(ejex-Ix))-Iw;
			if(oe>pi){
				oe=oe-pi*2;
			}else if(oe<(-pi)){
				oe=oe+pi*2;
			}
			//Calculamos las señales de control (velocidades V y W)
			V=0.35*de;
			//limitacion de la velocidad pero parece que no hace falta con los valores actuales de Koe y Kde
			if(V>0.8){
				V=0.8;
			}else if(V<0.45){
				V=0.45;
			}
			W=2.0*oe;
			//Enviamos las velocidades al robot (angular y lineal)
			playerc_position2d_set_cmd_vel(position2d,V,0.0,0,1);
			playerc_client_read(client);
			frontallaser=laser->scan[180][0];
			de=frontallaser-avanzarlaser;			
			//printf("frontal laser %lf\n", frontallaser);
		}
		playerc_position2d_set_cmd_vel(position2d,0.0,0.0,0.0,1);
		playerc_position2d_set_odom(position2d,ejex,ejey,position2d->pa);
	}

	//funcion para girar en parado
	void girar(int giro){
		//Calculamos donde estamos
		playerc_client_read(client);
		double orientacion=position2d->pa;
		//printf("la orientacion es %f \n",orientacion);
		//calculamos el angulo final
		if(giro<0){	//giro a la derecha
			if(position2d->pa<0){	//lado negativo
				orientacion=orientacion+DTOR(giro);
				if(orientacion<DTOR(-181)){	//pasamos de 180
					orientacion=orientacion+DTOR(360);	
				}
			}else{
				orientacion=orientacion+DTOR(giro);		
			}
		}else{		//giro a la izquierda
			if(position2d->pa<0){	//lado negativo
				orientacion=orientacion+DTOR(giro);
			}else{
				orientacion=orientacion+DTOR(giro);
				if(orientacion>DTOR(181)){
					orientacion=orientacion-DTOR(360);	
				}		
			}
		}
		//nos movemos segun el tipo de angulo al que queramos ir (Redondeo para que sean exactos los giros)
		//printf("la orientacion es %f \n",orientacion);
		if(orientacion>=(-0.79) && orientacion<0.79){
			//printf("orientacion final %f \n",DTOR(0));
			//Nos movemos a 0
			while((position2d->pa>0.009) || (position2d->pa<-0.009)){
				playerc_client_read(client);
				if(position2d->pa>0){			//giro horario
					if(position2d->pa>0.25){	//giro rapido
						if(fabs(position2d->pa)-0.0>1.59){
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-60),1);
						}else{
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-40),1);
						}
					}else{						//giro lento
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-6),1);
					}
				}else{							//giro antihorario
					if(position2d->pa>-0.25){	//giro lento
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(6),1);
					}else{						//giro rapido
						if(fabs(position2d->pa)-0.0<-1.59){
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(60),1);
						}else{
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(40),1);
						}
					}			
				}
				//printf("orientacion %f \n",position2d->pa);
				playerc_client_read(client);
			}
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}else if(orientacion>=0.79 && orientacion<2.36){
			//printf("orientacion final %f \n",DTOR(90));
			//Nos movemos a 90
			while((position2d->pa<1.56) || (position2d->pa>1.58)){
				playerc_client_read(client);
				if(fabs(position2d->pa)>1.57){
					if(fabs(position2d->pa)>1.82 || position2d->pa<-1.57 ){
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-40),1);
					}else{
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-6),1);
					}
				}else{
					if(fabs(position2d->pa)<1.30 || (position2d->pa>-1.57 && position2d->pa<0)){
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(40),1);
					}else{
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(6),1);
					}			
				}
				//printf("orientacion %f \n",position2d->pa);
				playerc_client_read(client);
			}
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}else if(orientacion<(-0.79) && orientacion>=(-2.36)){
			//printf("orientacion final %f \n",DTOR(-90));
			//Nos movemos a -90
			while((position2d->pa>-1.56) || (position2d->pa<-1.58)){
				playerc_client_read(client);
				if(fabs(position2d->pa)<1.57){
					if(fabs(position2d->pa)<1.30 || position2d->pa>0){
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-40),1);
					}else{
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-6),1);
					}
				}else{
					if(fabs(position2d->pa)>1.82 || position2d->pa>1.57){
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(40),1);
					}else{
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(6),1);
					}			
				}
				//printf("orientacion %f \n",position2d->pa);
				playerc_client_read(client);
			}
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}else{
			//printf("orientacion final %f \n",DTOR(180));	
			//Nos movemos 180
			while((position2d->pa<3.132) && (position2d->pa>-3.132)){
				playerc_client_read(client);
				if(position2d->pa<0){
					if(position2d->pa<-2.88){
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-6),1);
					}else{
						if(3.14-fabs(position2d->pa)>1.59){
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-60),1);
						}else{
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(-40),1);
						}
					}
				}else{
					if(position2d->pa<2.88){
						if(3.14-fabs(position2d->pa)>1.59){
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(60),1);
						}else{
							playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(40),1);
						}
					}else{
						playerc_position2d_set_cmd_vel(position2d,0,0.0,DTOR(6),1);
					}			
				}
				//printf("orientacion %f \n",position2d->pa);
				playerc_client_read(client);
			}
			playerc_position2d_set_cmd_vel(position2d,0,0.0,0,1);
		}
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

	// Create and subscribe to a laser device.
	laser = playerc_laser_create(client, 0);
	if (playerc_laser_subscribe(laser, PLAYER_OPEN_MODE))
		return -1;

	playerc_client_read(client);
	playerc_client_read(client);

	//obtenemos las mediciones y mostramos la informacion general
	playerc_sonar_get_geom(sonar);
	playerc_laser_get_geom(laser);

	//infoSensores();
	//recalcular();
	//mostrarXY();  
	//mostrarSXYR();

	//MAIN PRINCIPAL DE LOS MOVIMIENTOS
	int eleccion;
	int auxX, auxY;
	int primeraVez;
	int AA,BB;
	int vecesMax=0;

	//inicializamos la matrix con la informacion de inicio/vacia
	for(AA=0; AA < 11; AA++){
		for(BB=0;BB<11;BB++){
			MAPA[AA][BB].casillaNueva=1;
			MAPA[AA][BB].marca=' ';
			MAPA[AA][BB].veces=0;
			MAPA[AA][BB].formato=99;
	 	}
	}
	  
	primeraVez=0;	//marca para identificar el inicio
	//mientras el numero de casillas no sea 0 continuamos
	while (contadorCasillas>0 && vecesMax<4){
		playerc_client_read(client);
		printf("Contador de casillas restantes para concluir %d\n",contadorCasillas);
		recalcular();
		//comprobamos la situacion de las paredes
		eleccion=paredesAlrededor();
		//operamos con el mapa despues de ver donde andamos
		auxX=redondear(position2d->px);
		auxY=redondear(position2d->py);
		printf("Casilla del mapa %d %d del tipo %d ",auxX, auxY, eleccion);
		//comprobamos si ya hemos pasado si no es asi continuamos y restamos el contador de casillas
		if(MAPA[5+auxX][5-auxY].casillaNueva==1){
			printf("casilla nueva");
			MAPA[5+auxX][5-auxY].casillaNueva=0;
			contadorCasillas--;
			//comprobamos si es la primera vez y por tanto inicio
			if(primeraVez==0){		//si somos la primera rellenar especial	
				printf(", casilla inicial\n");
				MAPA[5+auxX][5-auxY].formato=representacionMapa(position2d->pa,eleccion);
				MAPA[5+auxX][5-auxY].marca='X';
				MAPA[5+auxX][5-auxY].veces=MAPA[5+auxX][5-auxY].veces+1;
				vecesMax=MAPA[5+auxX][5-auxY].veces;
				primeraVez=primeraVez+1;
			}else{	//de no ser la primera casilla realizar este apartado
				printf(", casilla normal\n");
				MAPA[5+auxX][5-auxY].formato=representacionMapa(position2d->pa,eleccion);
				MAPA[5+auxX][5-auxY].marca='*';
				MAPA[5+auxX][5-auxY].veces=MAPA[5+auxX][5-auxY].veces+1;
				vecesMax=MAPA[5+auxX][5-auxY].veces;
			}
		}else{		//si la casilla ya se ha visitado continuar
			printf("casilla ya transitada\n");
			MAPA[5+auxX][5-auxY].veces=MAPA[5+auxX][5-auxY].veces+1;
			vecesMax=MAPA[5+auxX][5-auxY].veces;
		}
		//en funcion del tipo de casilla una vez documentada realizar las acciones pertinenetes 	
		switch(eleccion){
			case 15:
				//printf("Ha ingresado el numero 15\n");
				avanzar (0.1);
				break;
			case 14:
				//printf("Ha ingresado el numero 14\n");
				girar(180);
				avanzar (1.0);
				break;
			case 13:
				//printf("Ha ingresado el numero 13\n");
				girar(-90);
				avanzar (1.0);
				break;
			case 12:
				//printf("Ha ingresado el numero 12\n");
				avanzar (1.0);
				break;
			case 11:
				//printf("Ha ingresado el numero 11\n");
				girar(90);
				avanzar (1.0);
				break;
			case 10:
				//printf("Ha ingresado el numero 10\n");
				girar(-90);
				avanzar (1.0);
				break;
			case 9:
				//printf("Ha ingresado el numero 9\n");
				avanzar (1.0);
				break;
			case 8:
				//printf("Ha ingresado el numero 8\n");
				avanzar (1.0);
				break;
			case 7:
				//printf("Ha ingresado el numero 7\n");
				girar(-90);
				avanzar (1.0);
				break;
			case 6:
				//printf("Ha ingresado el numero 6\n");
				girar(90);
				avanzar (1.0);
				break;
			case 5:
				//printf("Ha ingresado el numero 5\n");
				girar(90);
				avanzar (1.0);
				break;
			case 4:
				//printf("Ha ingresado el numero 4\n");
				girar(-90);
				avanzar (1.0);
				break;
			case 3:
				//printf("Ha ingresado el numero 3\n");
				avanzar (1.0);
				break;
			case 2:
				//printf("Ha ingresado el numero 2\n");
				avanzar (1.0);
				break;
			case 1:
				//printf("Ha ingresado el numero 1\n");
				girar(-90);
				avanzar (1.0);
				break;
			default:
				//printf("Ha ingresado el numero 0\n");
				girar(180);
				avanzar (1.0);
				break;
		}
		//repetir hasta trerminar
	} 

	//mostramos la forma de representacion
	mostrarEsquema();

	//motramos las casillas de referencia 
	printf("Representacionde las casillas del mapa en numero\n");
	for(AA=0; AA < 11; AA++){
		for(BB=0;BB<11;BB++){
			if(MAPA[BB][AA].formato<16){
				printf("  %d  ",MAPA[BB][AA].formato);
			}else{
				printf("     ");
			}	
		}
		printf("\n\n");
	}

	//mostramos el mapa con los lugares visto inicio y fin
	printf("Representacion de los lugares visitados inicio (X) y resto (*)");
	for(AA=0; AA < 11; AA++){
		for(BB=0;BB<11;BB++){
			printf("  %c  ",MAPA[BB][AA].marca);
		}
		printf("\n\n");
	}

	rellenarBlancos();
	
	//motramos las casillas de referencia 
	printf("Representacionde las casillas del mapa en numero con los huecos rellenos\n");
	for(AA=0; AA < 11; AA++){
		for(BB=0;BB<11;BB++){
			if(MAPA[BB][AA].formato<16){
				printf("  %d  ",MAPA[BB][AA].formato);
			}else{
				printf("     ");
			}	
		}
		printf("\n\n");
	}
	//FIN

	// Shutdown desconexion y cerrado
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);
	playerc_sonar_unsubscribe(sonar);
	playerc_sonar_destroy(sonar);
	playerc_laser_unsubscribe(laser);
	playerc_laser_destroy(laser);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);

	return 0;
}
