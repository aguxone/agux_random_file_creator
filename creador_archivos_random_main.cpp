#include <iostream>
#include <ctime> // Aporta la función time() y el tipo de objetos de tiempo time_t, struct tm, ctime(), strftime y otras cosas que dicen time.
#include <unistd.h>
#include <fstream>
#include <atomic>
using namespace std;

// Defino el tiempo actual y variable pointeada
// a un struct tm para modificar más fácilmente
// time_t tactual = time(NULL);
// struct tm* tactual_st = localtime(&tactual);

// // Defino un tiempo cada el cual vamos a re-crear los archivos y chekear esté todo ok
// time_t trerun;
// struct tm* trerun_st = localtime(&trerun);

// // Vamos a dejar guardados en variables los valores de seg, minutos y horas para que sea fácilmente accesibles y modificables después
// int secs = trerun_st->tm_sec;
// int mins = trerun_st->tm_min;
// int hours = trerun_st->tm_hour;

// int tiempo_pausa;

void lector_creador_archivos()
{
	fstream directorios; //Definimos una variable de tipo fstream.
	directorios.open("directorios.ini", ios::in);

	if (directorios.is_open()) //is_open es un método de fstream que justamente si está abierto y devuelve booleano true o false.
	{
		string line; //declaremos bien la variable line que queremos guardar en la memoria!
		while (getline(directorios, line))
		{
			cout << line << endl; //Podemos escribir tantas cosas como queramos. endl es una función de ioestream y lo que hace es bajar de línea como apretar enter, es una alternativa al famoso /n
			time_t tactual = time(NULL);
			string path = line + "\\archivito.txt";
			fstream path_archivo;
			cout <<path<< endl;
			path_archivo.open(path, ios::out);
			path_archivo.close();
			path_archivo.open(path, ios::app);
			path_archivo << ctime(&tactual)<<"\n";
			path_archivo.close(); // Igual que en python conviene cerrarlo para quitarlo de la memoria nomás.

		}
	}
	directorios.close();
}

void lector_creador_archivos_loop()
{

	while(1==1)
	{
		if (stop==0)
		{
			cout << "         " << stop << "          ";
			// tiempo_pausa=1;
			lector_creador_archivos();
			// cout<< "durmiendo...";
			sleep(tiempo_pausa);
			// cout<< "despertando";
		}
		else
		{
			break;
		}

	}
}

// void lector_creador_archivos_loop(int stopeador)
// {

// 	while(stopeador==0)
// 	{
// 			cout << "         " << stopeador << "          ";
// 			tiempo_pausa=1;
// 			lector_creador_archivos();
// 			// cout<< "durmiendo...";
// 			sleep(tiempo_pausa);
// 			// cout<< "despertando";
// 	}
// }

// void lector_creador_archivos_ejecutar()
// {
// 	// thread threadcito(lector_creador_archivos_loop);
// 	// threadcito.join();
// 	lector_creador_archivos_loop(stopeador);
// }