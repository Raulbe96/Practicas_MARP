#include <iostream>
#include <fstream>
#include "grafoDirigido.h"
#include <Windows.h>
using namespace std;


//Función sacada de internet para obtener la diferencia de tiempo en segundos
double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
}


//Insertar arista en el grafo
void insertarArista(grafoDir & grafo){
	int a, b;
	cout << "¿De que vertice a que vertice ira?" << endl;
	cin >> a >> b;
	grafo.insArista(a, b);
}

void insertarArista(grafoDir &grafo, int a, int b){
	grafo.insArista(a, b);
}

//Hace las comprobaciones necesarias:
//1. ¿Es cíclico?
//2. Devuelve el orden topológico si es cíclico o la lista de vértices que forman una componente conexa si no.
void procesarGrafo(const grafoDir &grafo){
	//¿Es cíclico?
	bool ciclico = grafo.esCiclico();
	if (ciclico)
		cout << "El grafo es ciclico" << endl;
	else
		cout << "El grafo no es ciclico" << endl;


	//Si no es cíclico, devuelve el orden topológico.
	//Devuelve esta lista en v.
	if (!ciclico){
		vector<int> v;
		grafo.topologico(v);
		cout << "Orden topologico: ";
		for (int i = 0; i < v.size(); i++){
			cout << v[i] << " ";
		}
		cout << endl;
	}
	else{
		vector<vector<int>> conjuntos;
		conjuntos = grafo.fuertementeConexas();
		for (int i = 0; i < conjuntos.size(); i++){
			vector<int> conj = conjuntos[i];
			cout << "Componente fuertemente conexa numero " << i+1 << ":";
			for (int j = 0; j < conj.size(); j++){
				cout << " " << conj[j];
			}
			cout << endl;
		}
	}
}

int main(){
	int metodo;
	setlocale(LC_ALL, "spanish");
	system("chcp 1252");
	cout << "¿Como quiere introducir el grafo?:\n1. A través del archivo casos.txt\n2. Por consola" << endl;
	cin >> metodo;
	if (metodo == 1){

		int num;
		fstream in("casos.txt");
		auto cinbuf = cin.rdbuf(in.rdbuf());
		int rep = 1000;
		int count = 0;
		cin >> num;

		int n, a, b;
		for (int i = 0; i < num; i++){
			cin >> n >> a >> b;
			cout << "Caso " << i + 1 << endl;
			grafoDir grafo = grafoDir(n);
			while (a != -1 && b != -1){
				insertarArista(grafo, a, b);
				cin >> a >> b;
			}

			procesarGrafo(grafo);


			/*A la hora de medir el tiempo para la gráfica, se utilizó el siguiente bloque de código ligeramente diferente:
			había un bucle que repetía la función procesarGrafo las suficientes veces para que diera un tiempo de más de 10 milisegundos.
			habían también un par de operaciones que ajustaban las repeticiones de procesarGrafo para que devolviera el tiempo que tardaba
			en ejecutarlo una sola vez.
			*/
		/*LARGE_INTEGER inicio, fin;
		int milis;
		double secs;
		int k = 0;
		QueryPerformanceCounter(&inicio);

		procesarGrafo(grafo);

		QueryPerformanceCounter(&fin);
		//Tiempo en segundos
		secs = performancecounter_diff(&fin, &inicio);
		milis = secs * 1000;
		cout << "El algoritmo ha tardado " << milis << " milisegundos con " << count*(20)<< " vertices" << endl;
		count++;
		if (milis >= 10)
		rep = 3;*/

		}
		std::cin.rdbuf(cinbuf);
		in.close();
		system("pause");
	}

	else{
		int n;
		cout << "Elementos del grafo: ";
		cin >> n;
		grafoDir grafo = grafoDir(n);
		int opcion = 0;
		while (opcion != 3){
			cout << "¿Que quieres hacer?\n1.Insertar arista\n2.Procesar el grafo\n3.Salir" << endl;
			cin >> opcion;
			if (opcion <1 || opcion > 3)
				cout << "Debes elegir entre las opciones anteriores." << endl;
			else if (opcion == 1)
				insertarArista(grafo);
			else if (opcion == 2)
				procesarGrafo(grafo);
		}
	}
	return 0;
}