#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){
	ofstream archivo;
	archivo.open("datos.txt");
	int num;
	archivo << 10 << " " << 100 << endl;
	for (int i = 0; i < 30; i++){
		archivo << (rand() % 20)+1 << " ";
		archivo << (rand() % 10)+1 << " ";
		archivo << (rand() % 150)+1 << endl;
	}
	archivo.close();
	return 0;



}