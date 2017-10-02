Se entregan dos archivo uno con error y otro sin error:
	En el archivo con error se ha usado la funcion set_odom para corrregir el error lineal y en el control p se ha eliminado la correcion angular.
	En el archivo sin error no se ha usado la funcion set_odom y si se tiene en cuenta el error angular en el control p.

PD: En realidad se podrian realizar los dos recorridos con el archivo con error pero se perderian alguna caracteristicas de las usadas en el otro como la correccion angular.