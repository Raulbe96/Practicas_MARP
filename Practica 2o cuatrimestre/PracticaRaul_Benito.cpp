#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include<queue>
#include <Windows.h>
#include <string>
using namespace std;

//Función sacada de internet para obtener la diferencia de tiempo en segundos
double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
}

/////////////////////////////////////////////////////////
//Funciones y estructurados utilizados en general por el programa
/////////////////////////////////////////////////////////
/*Tupla que indica:
	puesto: 1 si se escoge el cuadro, 0 si no.
	orientacion: 1 para vertical, 0 para horizontal.
	*/
typedef struct tElemento{
	bool puesto;
	bool orientacion;
};

/*
	cuadro: array que tuplas de tipo elemento.
	k: posicion del vector por la que vamos.
	hueco: la diferencia entre los cuadros que llevo y él tamaño de la pared.
	coste: el coste que llevo hasta ese momento.
	optimista: cota optimista del nodo
	*/
typedef struct tNodo{
	vector<tElemento> cuadro;
	int k;
	int hueco;
	int coste;
	int optimista;
};

/*
	id: identificador numérico del cuadro (utilizado para la cota optimista que requiere de ordenación
	a: dimensión a del cuadro (se contará cuando esté en vertical).
	b: dimensión b del cuadro (se contará cuando esté en horizontal).
	prest: prestigio del cuadro.
	*/
typedef struct tCuadro{
	int id;
	int a;
	int b;
	float prest;
};

/*
Identifica una solución
prestigio: prestigio de la solución
cuadros: vector que llevará estructurados de tipo tElemento
*/
typedef struct tSolucion{
	int prestigio;
	vector<tElemento> cuadros;
};

/*
Función de factibilidad:
Un nodo será factible cuando, sumadas las dimensiones más grandes de cada cuadro, ésta cubra completamente la pared
(da igual si la cubre exactamente o la sobrepasa), siempre y cuando no se haya sobrepasado antes de entrar en la función.
*/
bool factible(int hueco, int posicion, const vector<tCuadro> &cuadro){
	int i = posicion;
	int huecoLibre = hueco;
	//Voy quitando el hueco que me queda en función de los cuadros que me quedan.
	//Paro cuando compruebo todos los cuadros o cuando he sobrepasado el hueco.
	while (i < cuadro.size() && huecoLibre > 0){
		huecoLibre -= max(cuadro[i].a, cuadro[i].b);
		i++;
	}
	if (huecoLibre <= 0 && posicion < cuadro.size() && hueco >= 0) return true;
	else return false;
}

/*
Funciones para crear hijos del nodo actual.
- crearHijoNo: crea un nodo hijo en el que no se introduce el siguiente cuadro.
- crearHijoVer: crea un nodo hijo en el que se introduce el siguiente cuadro en vertical.
- crearHijoHor: crea un nodo hijo en el que se introduce el siguiente cuadro en horizontal.

*/
tNodo crearHijoNo(const tNodo &padre){
	tNodo hijo;

	hijo.coste = padre.coste;
	hijo.cuadro = padre.cuadro;
	hijo.k = padre.k + 1;

	hijo.cuadro[hijo.k].puesto = 0;
	hijo.hueco = padre.hueco;
	return hijo;
}

tNodo crearHijoVer(const tNodo &padre, int prest, int a){
	tNodo hijo;

	hijo.coste = padre.coste + prest;
	hijo.cuadro = padre.cuadro;
	hijo.k = padre.k + 1;

	hijo.cuadro[hijo.k].puesto = 1;
	hijo.cuadro[hijo.k].orientacion = 1;
	hijo.hueco = padre.hueco - a;
	return hijo;
}

tNodo crearHijoHor(const tNodo &padre, int prest, int b){
	tNodo hijo;

	hijo.coste = padre.coste + prest;
	hijo.cuadro = padre.cuadro;
	hijo.k = padre.k + 1;

	hijo.cuadro[hijo.k].puesto = 1;
	hijo.cuadro[hijo.k].orientacion = 0;
	hijo.hueco = padre.hueco - b;
	return hijo;
}

/////////////////////////////////////////////////////////
//Solo con poda de factible:
/////////////////////////////////////////////////////////


/*
Parte recursiva de la exploración solo con la función factible
*/
long int vuelta_atrasFact(const vector<tCuadro> &cuadro, int k,
	int pared, tSolucion &aux, tSolucion &mejorSolucion){
	long int nodosRecorridos = 1;

	if (pared == 0){
		if (aux.prestigio > mejorSolucion.prestigio){
			mejorSolucion.cuadros = aux.cuadros;
			mejorSolucion.prestigio = aux.prestigio;
		}
	}
	else if (k < cuadro.size() && factible(pared, k, cuadro)){
		//Pruebo a no meter el siguiente cuadro.
		nodosRecorridos += vuelta_atrasFact(cuadro, k + 1, pared, aux, mejorSolucion);
		//Pruebo a meter el siguiente cuadro
		aux.cuadros[k].puesto = true;
		aux.prestigio += cuadro[k].prest;

		//En vertical:
		aux.cuadros[k].orientacion = true;
		nodosRecorridos += vuelta_atrasFact(cuadro, k + 1, pared - cuadro[k].a, aux, mejorSolucion);

		//En horizontal:
		aux.cuadros[k].orientacion = false;
		nodosRecorridos += vuelta_atrasFact(cuadro, k + 1, pared - cuadro[k].b, aux, mejorSolucion);

		aux.prestigio -= cuadro[k].prest;
		aux.cuadros[k].puesto = false;
	}
	return nodosRecorridos;
}

//Descomentar para utilizar vuelta atrás,
//Comentar para utilizar esquema optimista-pesimista
/*
Llama a la función recursiva (la de arriba) y devuelve un par con:
-> un tSolucion que devolverá si los cuadros se han colocado o no y en qué posición.
-> un int con el número de nodos explorado.
*/
pair<tSolucion, long int> pared_factible(const vector<tCuadro> &cuadro, int pared){
	/*
	Creo el objeto donde estará la solución y lo relleno de 0s
	para que luego solo haya que modificarlos con el avance del algoritmo.
	El segundo false podría ser true o false dado que indica la orientación,
	y no nos interesa la orientación de un cuadro que no se ha insertado en la solución.
	aux será donde iré probando soluciones.
	mejorSolucion será donde vaya guardando provisionalmente la mejor solución.
	*/
	tSolucion aux;
	tSolucion mejorSolucion;
	for (int i = 0; i < cuadro.size(); i++)
	{
		aux.cuadros.push_back({ false, false });
		mejorSolucion.cuadros.push_back({ false, false });
	}
	mejorSolucion.prestigio = 0;
	aux.prestigio = 0;
	long int nodos = vuelta_atrasFact(cuadro, 0, pared, aux, mejorSolucion);
	return{ mejorSolucion, nodos };
}

//Utilizando solo la poda por factibilidad sin utilizar la vuelta atrás.
//Comentar para utilizar vuelta atrás,
//Descomentar para utilizar esquema optimista-pesimista
/*
pair<tSolucion, long int> pared_factible(const vector<tCuadro> &cuadro, int pared){
	long int nodosExplorados = 0;
	queue<tNodo> cola;

	tNodo nodoX;
	tNodo nodoY;
	tSolucion mejorSol;

	nodoY.coste = 0;
	nodoY.hueco = pared;
	nodoY.k = -1;
	nodoY.cuadro = vector<tElemento>(cuadro.size(), { false, false });
	cola.push(nodoY);
	int costeMejor = 0;
	while (!cola.empty()){
		nodoY = cola.front();
		cola.pop();
		nodosExplorados++;
		//Si tenemos más cuadros que introducir:
		if (nodoY.k + 1 < cuadro.size()){
			//Este bucle se hará tres veces por nodo, una por cada hijo, y en cada iteración creará un tipo de hijo distinto.
			for (int i = 0; i < 3; i++){
				if (i == 0) nodoX = crearHijoNo(nodoY);
				else if (i == 1)
					nodoX = crearHijoVer(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].a);
				else
					nodoX = crearHijoHor(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].b);

				//nodoX es solucion:
				if (nodoX.hueco == 0){
					if (nodoX.coste > costeMejor){
						mejorSol.cuadros = nodoX.cuadro;
						mejorSol.prestigio = nodoX.coste;
						costeMejor = nodoX.coste;
					}
				}
				//Si nodoX no es solución
				else{
					if (factible(nodoX.hueco, nodoX.k, cuadro)){
						cola.push(nodoX);
					}
				}
			}
		}
	}
	return{ mejorSol, nodosExplorados };
}
*/


/////////////////////////////////////////////////////////
//Con cota optimista ingenua:
/////////////////////////////////////////////////////////

/*
Se implementa la sobrecarga del operador.
Se utiliza para el compare de la cola de prioridad.
*/
struct ComparaCotas {
	bool operator()(tNodo const& p1, tNodo const& p2)
	{
		return p1.optimista < p2.optimista;
	}
};

/*
Cota optimista ingenua.
Compruebo, csin modificar el tamaño del hueco de pared que me queda, si puedo o no meter cada uno de los cuadros
que me quedan individualmente.
Esto es una cota optimista muy ingenua de coste lineal.
No necesita de ninguna ordenación previa.
*/
float cota_optimista_ing(int coste, int hueco, int k, const vector<tCuadro> &cuadro){
	float cota = coste;
	int aux;
	for (int i = k; i < cuadro.size(); i++){
		//Cojo la dimensión de menor tamaño para ver si entra.
		if (min(cuadro[i].a, cuadro[i].b) <= hueco)
			cota += cuadro[i].prest;
	}
	return cota;
}

/*
Recibe:
-> cuadro: vector de cuadros. No necesita estar ordenado.
-> pared: tamaño de la pared que queremos ocupar.
Devuelve un par(tSolucion, int), donde:
-> el tSolucion devuelve la solución (qué cuadros se han mpuesto, cómo y el prestigio total).
-> el int devuelve el número de nodos que se han evaluado.
*/
pair<tSolucion, long int> pared_conOptimistaIng(const vector<tCuadro> &cuadro, int pared){
	long int nodosExplorados = 0;
	priority_queue<tNodo, vector<tNodo>, ComparaCotas> cola;

	tNodo nodoX;
	tNodo nodoY;
	tSolucion mejorSol;

	nodoY.coste = 0;
	nodoY.hueco = pared;
	nodoY.k = -1;
	nodoY.optimista = 1;
	nodoY.cuadro = vector<tElemento>(cuadro.size(), { false, false });
	cola.push(nodoY);
	int costeMejor = 0;
	while (!cola.empty() && cola.top().optimista > costeMejor){
		nodoY = cola.top();
		cola.pop();
		nodosExplorados++;
		//Si tenemos más cuadros que introducir:
		if (nodoY.k + 1 < cuadro.size()){
			//Este bucle se hará tres veces por nodo, una por cada hijo, y en cada iteración creará un tipo de hijo distinto.
			for (int i = 0; i < 3; i++){
				if (i == 0) nodoX = crearHijoNo(nodoY);
				else if (i == 1)
					nodoX = crearHijoVer(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].a);
				else
					nodoX = crearHijoHor(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].b);

				nodoX.optimista = cota_optimista_ing(nodoX.coste, nodoX.hueco, nodoX.k, cuadro);

				//nodoX es solucion:
				if (nodoX.hueco == 0){
					if (nodoX.coste > costeMejor){
						mejorSol.cuadros = nodoX.cuadro;
						mejorSol.prestigio = nodoX.coste;
						costeMejor = nodoX.coste;
					}
				}

				//Si nodoX no es solución
				else{
					if (factible(nodoX.hueco, nodoX.k, cuadro) && nodoX.optimista > costeMejor){
						cola.push(nodoX);
					}
				}
			}
		}
	}
	return{ mejorSol, nodosExplorados };
}

/////////////////////////////////////////////////////////
//Con una mejor cota optimista
/////////////////////////////////////////////////////////

/*
Compare para ordenar el vector de cuadros en función de su valor/dimensión_menor.
*/
bool mas_valioso(const tCuadro &c1, const tCuadro &c2){
	float valor1, valor2;
	valor1 = c1.prest / min(c1.a, c1.b);
	valor2 = c2.prest / min(c2.a, c2.b);
	return valor1 > valor2;
}

/*
Cota optimista:
Para utilizar esta cota, es necesario tener ordenados, de mayor a menor, los cuadros en función de
su proporción (valor / dimension_menor).
Se meten los cuadros con mayor proporción tamaño-valor hasta que se ocupe toda la pared,
el último, si no entra, se considera que se puede partir.
Coste de tiempo lineal, dado que los datos ya estarán ordenados.
*/
float cota_optimista_mejor(int coste, int pared, int k, const vector<tCuadro> &cuadro)
{
	float cota = coste;
	int hueco = pared;
	int i = k;
	//Considero que introduzco todos los cuadros con mayor proporción tamaño-valor que quepan, haciendo desaparecer el hueco.
	//Salgo cuando se han introducido todos los cuadros restantes o cuando no entra el siguiente.
	while (i < cuadro.size() && hueco - min(cuadro[i].a, cuadro[i].b) > 0){
		hueco -= min(cuadro[i].a, cuadro[i].b);
		cota += cuadro[i].prest;
		i++;
	}

	//Si he salido porque no entraba el siguiente cuadro a valorar, lo "parto"
	if (hueco > 0 && i < cuadro.size()){
		cota += (cuadro[i].prest / min(cuadro[i].a, cuadro[i].b) * hueco);
	}
	return cota;
}

/*
Recibe y devuelve lo mismo que pared_conOptimistaIng, dado que solo las diferencia la función de cota
y, importante, que necesita que los cuadros estén ordenados de mayor a menor en función de su proporción
valor/dimensión_menor
*/
pair<tSolucion, long int> pared_conOptimistaBuena(const vector<tCuadro> &cuadro, int pared){
	long int nodosExplorados = 0;
	priority_queue<tNodo, vector<tNodo>, ComparaCotas> cola;

	tNodo nodoX;
	tNodo nodoY;
	tSolucion mejorSol;

	nodoY.coste = 0;
	nodoY.hueco = pared;
	nodoY.k = -1;
	nodoY.optimista = 1;
	nodoY.cuadro = vector<tElemento>(cuadro.size(), { false, false });
	cola.push(nodoY);

	int costeMejor = 0;
	while (!cola.empty() && cola.top().optimista > costeMejor){
		nodoY = cola.top();
		cola.pop();
		nodosExplorados++;
		if (nodoY.k + 1 < cuadro.size()){
			for (int i = 0; i < 3; i++){
				if (i == 0) nodoX = crearHijoNo(nodoY);
				else if (i == 1)
					nodoX = crearHijoVer(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].a);
				else
					nodoX = crearHijoHor(nodoY, cuadro[nodoY.k + 1].prest, cuadro[nodoY.k + 1].b);

				nodoX.optimista = cota_optimista_mejor(nodoX.coste, nodoX.hueco, nodoX.k, cuadro);

				//nodoX es solucion:
				if (nodoX.hueco == 0){
					if (nodoX.coste > costeMejor){
						mejorSol.cuadros = nodoX.cuadro;
						mejorSol.prestigio = nodoX.coste;
						costeMejor = nodoX.coste;
					}
				}
				//Si nodoX no es solución
				else{
					if (factible(nodoX.hueco, nodoX.k, cuadro) && nodoX.optimista >= costeMejor){
						cola.push(nodoX);
					}
				}
			}
		}
	}
	return{ mejorSol, nodosExplorados };
}

int main(){


	int n, pared;
	ifstream archivo;
	int a, b;
	float v;
	setlocale(LC_ALL, "spanish");
	system("chcp 1252");

	archivo.open("datos.txt");
	archivo >> n >> pared;

	//Introducimos los datos de los cuadros en un vector cuadros.
	vector<tCuadro> cuadros = vector<tCuadro>();
	for (int i = 0; i < n; i++){
		archivo >> a >> b >> v;
		cuadros.push_back({ i, a, b, v });
	}
	archivo.close();

	//Creo las estructuras donde se almacenarán las soluciones y sus valores.
	char or;
	pair<tSolucion, long int> opt;
	int rep = 3;


	/////////////////////////////////////
	//Solución considerando solo la función de factibilidad:
	cout << "Solucion considerando solo factible" << endl;

	//Para medir el tiempo
	LARGE_INTEGER inicio, fin;
	double milis1;
	double secs;
	QueryPerformanceCounter(&inicio);

	opt = pared_factible(cuadros, pared);

	QueryPerformanceCounter(&fin);
	//Tiempo en segundos
	secs = performancecounter_diff(&fin, &inicio);
	milis1 = secs * 1000;

	//////////////////////////
	tSolucion optimista1;
	long int nodos1;
	optimista1 = opt.first;
	nodos1 = opt.second;
	for (int i = 0; i < cuadros.size(); i++){
		or = (optimista1.cuadros[i].puesto == 1 ? (optimista1.cuadros[i].orientacion == 1 ? 'V' : 'H') : 'N');
		cout << optimista1.cuadros[i].puesto << "-" << or << " ";
	}

	cout << "\nPrestigio: " << optimista1.prestigio << endl;
	cout << "Nodos explorados: " << nodos1 << endl;
	cout << "El algoritmo ha tardado " << milis1 << " milisegundos con " << n << " cuadros" << endl;
	cout << "De media, ha tardado " << (milis1 / nodos1) << " milisegundos por nodo" << endl;
	//////////////////////////////////////////
	//Considerando cota optimista ingenua:	

	cout << "\nSolucion considerando cota optimista ingenua:" << endl;

	double milis2;
	QueryPerformanceCounter(&inicio);

	opt = pared_conOptimistaIng(cuadros, pared);

	QueryPerformanceCounter(&fin);
	//Tiempo en segundos
	secs = performancecounter_diff(&fin, &inicio);
	milis2 = secs * 1000;

	tSolucion optimista2;
	long int nodos2;
	optimista2 = opt.first;
	nodos2 = opt.second;
	for (int i = 0; i < cuadros.size(); i++){
		or = (optimista2.cuadros[i].puesto == 1 ? (optimista2.cuadros[i].orientacion == 1 ? 'V' : 'H') : 'N');
		cout << optimista2.cuadros[i].puesto << "-" << or << " ";
	}
	cout << "\nPrestigio: " << optimista2.prestigio << endl;
	cout << "Nodos explorados: " << nodos2 << endl;
	cout << "El algoritmo ha tardado " << milis2 << " milisegundos con " << n << " cuadros" << endl;
	cout << "De media, ha tardado " << (milis2 / nodos2) << " milisegundos por nodo" << endl;
	//////////////////////////////////////////
	//Considerando una mejor cota optimista:	

	//Primero, ordenamos los cuadros para calcular el coste optimista
	//ordenReal llevará la posición del vector ordenado en que estará la posición real del vector inicial

	sort(cuadros.begin(), cuadros.end(), mas_valioso);
	vector<int> ordenReal = vector<int>(n, 0);
	for (int i = 0; i < cuadros.size(); i++){
		ordenReal[cuadros[i].id] = i;
	}

	cout << "\nSolucion considerando una mejor cota optimista" << endl;

	double milis3;
	QueryPerformanceCounter(&inicio);

	opt = pared_conOptimistaBuena(cuadros, pared);

	QueryPerformanceCounter(&fin);
	//Tiempo en segundos
	secs = performancecounter_diff(&fin, &inicio);
	milis3 = secs * 1000;


	tSolucion optimista3;
	long int nodos3;
	optimista3 = opt.first;
	nodos3 = opt.second;

	for (int i = 0; i < ordenReal.size(); i++){
		or = (optimista3.cuadros[ordenReal[i]].puesto == 1 ? (optimista3.cuadros[ordenReal[i]].orientacion == 1 ? 'V' : 'H') : 'N');
		cout << optimista3.cuadros[ordenReal[i]].puesto << "-" << or << " ";
	}
	cout << "\nPrestigio: " << optimista3.prestigio << endl;
	cout << "Nodos explorados: " << nodos3 << endl;
	cout << "El algoritmo ha tardado " << milis3 << " milisegundos con " << n << " cuadros" << endl;
	cout << "De media, ha tardado " << (milis3 / nodos3) << " milisegundos por nodo" << endl;

	//Guardamos los resultados en el archivo resultados
	ofstream puente;

	puente.open("resultados.txt");

	puente << "Solucion considerando solo factible" << endl;
	for (int i = 0; i < cuadros.size(); i++){
		or = (optimista1.cuadros[i].puesto == 1 ? (optimista1.cuadros[i].orientacion == 1 ? 'V' : 'H') : 'N');
		puente << optimista1.cuadros[i].puesto << "-" << or << " ";
	}
	puente << "\nPrestigio: " << optimista1.prestigio << endl;
	puente << "Nodos explorados: " << nodos1 << endl;
	puente << "El algoritmo ha tardado " << milis1 << " milisegundos con " << n << " cuadros" << endl;
	puente << "De media, ha tardado " << (milis1 / nodos1) << " milisegundos por nodo" << endl;


	puente << "\nSolucion considerando cota optimista ingenua:" << endl;
	for (int i = 0; i < cuadros.size(); i++){
		or = (optimista2.cuadros[i].puesto == 1 ? (optimista2.cuadros[i].orientacion == 1 ? 'V' : 'H') : 'N');
		puente << optimista2.cuadros[i].puesto << "-" << or << " ";
	}
	puente << "\nPrestigio: " << optimista2.prestigio << endl;
	puente << "Nodos explorados: " << nodos2 << endl;
	puente << "El algoritmo ha tardado " << milis2 << " milisegundos con " << n << " cuadros" << endl;
	puente << "De media, ha tardado " << (milis2 / nodos2) << " milisegundos por nodo" << endl;



	puente << "\nSolucion considerando una mejor cota optimista" << endl;
	for (int i = 0; i < ordenReal.size(); i++){
		if (optimista3.cuadros[ordenReal[i]].puesto == 1){
			if (optimista3.cuadros[ordenReal[i]].orientacion == 1)
				or = 'V';
			else or = 'H';
		}
		else or = 'N';
		puente << optimista3.cuadros[ordenReal[i]].puesto << "-" << or << " ";
	}
	puente << "\nPrestigio: " << optimista3.prestigio << endl;
	puente << "Nodos explorados: " << nodos3 << endl;
	puente << "El algoritmo ha tardado " << milis3 << " milisegundos con " << n << " cuadros" << endl;
	puente << "De media, ha tardado " << (milis3 / nodos3) << " milisegundos por cada nodo" << endl;

	puente.close();
	system("pause");
	return 0;
}