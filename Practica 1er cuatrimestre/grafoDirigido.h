#ifndef GRAFOD
#define GRAFOD

#include <iostream>
#include <vector>
#include <map>
using namespace std;


class grafoDir{
private:
	//nVert es el n�mero de v�rtices que tiene el grafo
	int nVert;
	//ady ser� la lista de adyacencia. Cada elemento del vector tendr�, a su vez, un vector de aristas en la que se indican los v�rtices destino.
	vector<vector<int>> ady;

protected:

	bool existe(const vector<int> & v, int a) const{
		int i = 0;
		while (i < v.size()){
			if (v[i] == a)
				return true;
			i++;
		}
		return false;
	}

	bool existe(const vector<vector<int>> & v, int a) const{
		return (nVert >= a);
	}
	
	//Segunda parte del algoritmo ciclico
	//Devuelve true si encuentra un ciclo (un nodo ya visitado o recorrido)
	//Devuelve false si no encuentra ning�n ciclo.
	bool esCiclicoR(int ver, vector<int> &recorrido) const{
		vector<int>::const_iterator ini = ady[ver].cbegin();
		vector<int>::const_iterator fin = ady[ver].cend();
		while (ini != fin){
			int v = *ini;
			if (recorrido[v] == 0){
				recorrido[v] = 1;
				if (esCiclicoR(v, recorrido))
					return true;
			}
			else if (recorrido[v] == 1)
				return true;
			recorrido[v] = 2;
			ini++;
		}
		return false;
	}

	//Segunda parte de topol�gico. Devuelve en lista el orden topol�gico en que se recorrer�n los v�rtices.
	void topologicoR(int ver, vector<int> &lista, vector<int> &recorrido) const{
		vector<int>::const_iterator ini = ady[ver].cbegin();
		vector<int>::const_iterator fin = ady[ver].cend();
		while (ini != fin){
			int v = *ini;
			if (recorrido[v] == 0){
				recorrido[v] = 1;
				topologicoR(v, lista, recorrido);
			}

			ini++;
		}
		recorrido[ver] = 2;
		//Al ser el orden topol�gico el inverso del recorrido,
		//inserto los v�rtices directamente en orden inverso, es decir, al principio.
		lista.insert(lista.begin(), ver);
	}

	//Crea el grafo inverso al que se ha dado.
	grafoDir alReves() const{
		grafoDir reves = grafoDir(nVert);
		for (int i = nVert - 1; i >= 0; i--){
			for (int j = 0; j < ady[i].size(); j++){
				reves.insArista(ady[i][j], i);
			}
		}
		return reves;
	}

	//Segunda parte de la segunda parte del algoritmo de kosaraju-Sharir
	void marcar(vector<bool> &recorrido, int cuenta, int v, vector<vector<int>> &conjuntos) const{
		//Si hay una componente conexa m�s que vectores en el vector conjuntos, se crear� una nueva
		if (cuenta + 1 > conjuntos.size())
			conjuntos.push_back(vector<int>());
		//Se introduce el v�rtice en la lista
		conjuntos[cuenta].push_back(v);
		recorrido[v] = true;
		int aux;
		for (int i = 0; i < ady[v].size(); i++){
			aux = ady[v][i];
			if (recorrido[aux] == false){
				recorrido[aux] = true;
				marcar(recorrido, cuenta, aux, conjuntos);
			}
		}
	}

public:
	grafoDir(int n){
		nVert = n;
		for (int i = 0; i < n; i++){
			ady.push_back(vector<int>());
		}
	}

	~grafoDir(){
		ady.clear();
	}

	//inserta una arista en el grafo
	//a es el v�rtice origen
	//b es el v�rtice destino
	//Si la arista ya exist�a, no vuelve a crearla, pero no lo comunica al usuario.
	void insArista(int a, int b){
		if (existe(ady, a) && !existe(ady[a], b))
			ady[a].push_back(b);
	}


	//Primera parte del algoritmo que, haciendo un recorrido en profundidad del grafo, buscar� ciclos.
	bool esCiclico() const{
		vector<int> recorrido(nVert, 0);
		//recorrido: 0=blanco; 1=gris; 2=negro.
		int i = 0;
		while (i < nVert){
			if (recorrido[i] == 0){
				recorrido[i] = 1;
				if (esCiclicoR(i, recorrido))
					return true;
				recorrido[i] = 2;
			}
			i++;
		}
		return false;
	}

	//Primera parte del algoritmo que almlacenar�, en el vector lista, la lista de v�rtices en orden topol�gico.
	//Hace un recorrido en profundidad
	void topologico(vector<int> &lista) const{
		//recorrido: marca los nodos con: 0=blanco; 1=gris; 2=negro.
		vector<int> recorrido(nVert, 0);
		int i = 0;
		while (i < nVert){
			if (recorrido[i] == 0){
				recorrido[i] = 1;
				topologicoR(i, lista, recorrido);
			}
			i++;
		}
	}

	//Primera parte del algoritmo que devuelve los v�rtices
	//que forman las distintas componentes conexas del grafo
	/*Sigue el algoritmo de Kosaraju-Sharir con alguna modificaci�n para que,
	en lugar de devolver una lista con el n�mero de componente conexa en el que est�,
	devuelve una lista con cada componente conexa y sus v�rtices.
	*/
	//Dado que los v�rtices sabr�n qu� componente conexa ocupan, era suficiente con agruparlo en un vector de vectores.
	vector<vector<int>>fuertementeConexas() const{
		//Primera  parte del algoritmo de Kosaraju-Sharir

		//Se crea el grafo inverso al estudiado
		//(LAs aristas con sentido x-y ahora lo tendr�n y-x).
		grafoDir reves = alReves();
		//Hago su orden topol�gico
		vector<int> lista;
		vector<bool> recorrido = vector<bool>(nVert, false);
		reves.topologico(lista);

		//En "conjuntos" se guardar�n los v�rtices que forman cada componente conexa.
		vector<vector<int>> conjuntos;
		int cuenta = 0;
		int v;
		//Segunda parte del alogoritmo de Kosaraju-Sharir (1� parte)
		for (int i = 0; i < nVert; i++){
			v = lista[i];
			if (recorrido[v] == false){
				marcar(recorrido, cuenta, v, conjuntos);
				cuenta++;
			}
		}

	return conjuntos;
	}

};

#endif