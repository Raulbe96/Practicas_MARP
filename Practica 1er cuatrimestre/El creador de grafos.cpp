#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main(){
	ofstream puente;
	puente.open("grafos.txt");
	puente << 1000 << endl;
	int a, b;
	int vertices;
	puente << 1 << endl;
	puente << -1 << " " << -1 << endl;
	for (int i = 1; i <= 1000; i++){
		vertices = i*20;
		puente << vertices << endl;
		for (int j = 0; j < vertices * 2.5; j++){
			a = rand() % vertices;
			b = a;
			while (b == a)
				b = rand() % vertices;
			puente << a << " " << b << endl;
		}
		puente << -1 << " " << -1 << endl;
	}
	/*for (int i = 2; i <= 6; i++){
		vertices = i;
		puente << vertices << endl;
		for (int j = 0; j < vertices + (i / 3); j++){
			a = rand() % vertices;
			b = a;
			while (b == a)
				b = rand() % vertices;
			puente << a << " " << b << endl;
		}
		puente << -1 << " " << -1 << endl;
	}

	for (int i = 7; i <= 19; i++){
		vertices = i;
		puente << vertices << endl;
		int maxV = vertices *2.15;
		for (int j = 0; j < maxV; j++){
			a = rand() % vertices;
			b = a;
			while (b == a)
				b = rand() % vertices;
			puente << a << " " << b << endl;
		}
		puente << -1 << " " << -1 << endl;
	}

	for (int i = 20; i <= 1000; i++){
		vertices = i;
		puente << vertices << endl;
		for (int j = 0; j < vertices * 2.98; j++){
			a = rand() % vertices;
			b = a;
			while (b == a)
				b = rand() % vertices;
			puente << a << " " << b << endl;
		}
		puente << -1 << " " << -1 << endl;
	}
	for (int i = 1001; i <= 5000; i++){
		vertices = i;
		puente << vertices << endl;
		for (int j = 0; j < vertices * 3.52; j++){
			a = rand() % vertices;
			b = a;
			while (b == a)
				b = rand() % vertices;
			puente << a << " " << b << endl;
		}
		puente << -1 << " " << -1 << endl;
	}*/
	puente.close();
	return 0;

}