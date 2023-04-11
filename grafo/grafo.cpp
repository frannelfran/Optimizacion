 /*
 *  GRAFO.CPP - Plantilla para la implementación de la clase GRAFOS
 *
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos   : 2012-2021
 */

#include "grafo.h"

void GRAFO :: destroy() {
	for (unsigned i=0; i< n; i++) {
		LS[i].clear();
		// A[i].clear();
		if (dirigido == 1) {
            LP[i].clear();
        };
	}
	LS.clear();
	LP.clear();
	// A.clear();
}

void GRAFO :: build (char nombrefichero[85], int &errorapertura) {
    ElementoLista     dummy;
	ifstream textfile;
	textfile.open(nombrefichero);
	if (textfile.is_open()) {
		unsigned i, j, k;
		// leemos por conversion implicita el numero de nodos, arcos y el atributo dirigido
		textfile >> (unsigned &) n >> (unsigned &) m >> (unsigned &) dirigido;
		// los nodos internamente se numeran desde 0 a n-1
		// creamos las n listas de sucesores
		LS.resize(n);
        if(dirigido == 1) { // Si el grafo es dirigido creamos las n listas de predecesores
            LP.resize(n);
        }
        // leemos los m arcos
		for (k=0;k<m;k++) {
			textfile >> (unsigned &) i  >> (unsigned &) j >> (int &) dummy.c; // Valor de dummy.c
            dummy.j = j - 1; // Valor de dummy.j
            LS[i - 1].push_back(dummy); //situamos en la posición del nodo i a dummy mediante push_back
            // Creo un dummy2 de la estructura "ElementoLista" para almacenar los predecesores y la lista de adyacencia
            ElementoLista dummy2;
            dummy2.j = i - 1;
            dummy2.c = dummy.c;

            //pendiente de hacer un segundo push_back si es no dirigido. O no. 
            if(dirigido == 0 and i != j) {
                LS[j - 1].push_back(dummy2);
            }
            else if(dirigido == 1) {
                LP[j - 1].push_back(dummy2); // pendiente la construcción de LP, si es dirigido
            }
			//pendiente del valor a devolver en errorapertura
			//...
        }
    }
}

GRAFO::~GRAFO() {
	destroy();
}

GRAFO::GRAFO(char nombrefichero[85], int &errorapertura) {
	build (nombrefichero, errorapertura);
}

void GRAFO:: actualizar (char nombrefichero[85], int &errorapertura) {
    //Limpiamos la memoria dinamica asumida en la carga previa, como el destructor
    destroy();
    //Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
    build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido() {
    if(dirigido == 1) {
        return true;
    }
    return !true;
}

void GRAFO::Info_Grafo() {
    cout << "Numero de nodos: " << n << endl;
    cout << "Orden del grafo: " << n << endl;
    cout << "Numero de arcos: " << m << endl;
    cout << (Es_dirigido() == 1 ? "Es dirigido" : "No es dirigido") << endl;
    cout << endl; // Nueva línea
}

void Mostrar_Lista(vector<LA_nodo> L) {
    for(int i = 0; i < L.size(); i++) { // Recorrer la lista
        cout << "nodo " << i + 1 << ": {";
        for(ElementoLista dummy : L[i]) {
            if(L[i].at(L[i].size() - 1).j != dummy.j) {
                cout << dummy.j + 1 << ",";
                cout << "(" << dummy.c  << ")";
            }
            else {
                cout << dummy.j + 1;
                cout << "(" << dummy.c << ")";
            }
        }
        cout << "} "<< " " << endl;
    }
    cout << endl; // Nueva línea
}

void GRAFO :: Mostrar_Listas (int l) {
    if(l == 0) { // Mostrar lista de adyacencia
        cout << "--Lista de adyacencia--" << endl;
        Mostrar_Lista(LS);
    }
    if(l == 1) { // Mostrar lista de sucesores
        cout << "--Lista de sucesores--" << endl;
        Mostrar_Lista(LS);
    }
    if(l == -1) { // Mostrar lista de predecesores
        cout << "--Lista de predecesores--" << endl;
        Mostrar_Lista(LP);
    }
    cout << endl; // Nueva línea
}

void GRAFO::Mostrar_Matriz() //Muestra la matriz de adyacencia, tanto los nodos adyacentes como sus costes
{

}

void GRAFO::dfs_num(unsigned i, vector<LA_nodo>  L, vector<bool> &visitado, vector<unsigned> &prenum, unsigned &prenum_ind, vector<unsigned> &postnum, unsigned &postnum_ind) { //Recorrido en profundidad recursivo con recorridos enum y postnum
	visitado[i] = true;
    prenum[prenum_ind++]=i;//asignamos el orden de visita prenum que corresponde el nodo i
    for (unsigned j=0;j<L[i].size();j++)
             if (!visitado[L[i][j].j])
                {
                dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
                };
    postnum[postnum_ind++]=i;//asignamos el orden de visita posnum que corresponde al nodo i
}

void GRAFO::RecorridoProfundidad() 
{

}

void GRAFO::bfs_num(	unsigned i, //nodo desde el que realizamos el recorrido en amplitud
				vector<LA_nodo>  L, //lista que recorremos, LS o LP; por defecto LS
				vector<unsigned> &pred, //vector de predecesores en el recorrido
				vector<unsigned> &d) //vector de distancias a nodo i+1
//Recorrido en amplitud con la construcción de pred y d: usamos la cola
{
    vector<bool> visitado; //creamos e iniciamos el vector visitado
    visitado.resize(n, false);
    visitado[i] = true;

    pred.resize(n, 0); //creamos e inicializamos pred y d
    d.resize(n, 0);
    pred[i] = i;
    d[i] = 0;

    queue<unsigned> cola; //creamos e inicializamos la cola
    cola.push(i);//iniciamos el recorrido desde el nodo i+1

    while (!cola.empty()) { //al menos entra una vez al visitar el nodo i+1 y contin�a hasta que la cola se vac�e
        unsigned k = cola.front(); //cogemos el nodo k+1 de la cola
        cola.pop(); //lo sacamos de la cola
        //Hacemos el recorrido sobre L desde el nodo k+1
        for (unsigned j=0;j<L[k].size();j++) {
            //Recorremos todos los nodos u adyacentes al nodo k+1
            //Si el nodo u no está visitado
            int nodo_u = L[k].at(j).j;
            if(visitado[nodo_u] == true) {
                continue;
            }
            //Lo visitamos
            visitado[nodo_u] = true;
            //Lo metemos en la cola
            cola.push(nodo_u);
            //le asignamos el predecesor
            pred[nodo_u] = k;
            //le calculamos su etiqueta distancia
            d[nodo_u] = d[k] + 1;
        };
        //Hemos terminado pues la cola está vacía
    }
}

void GRAFO::RecorridoAmplitud() { //Construye un recorrido en amplitud desde un nodo inicial
   //creación e inicialización de variables y vectores
    int nodo_u;
    int nodo;
    vector<unsigned> pred;
    vector<unsigned> d;

    //solicitud al usuario del nodo inicial del recorrido en amplitud
    cout << "--Recorrido en amplitud--" << endl;
    cout << "Elije un nodo de partida: ";
    cin >> nodo_u;
    nodo_u -= 1;
    bfs_num(nodo_u, LS, pred, d);
    
    //mostrar en pantalla la etiqueta distancia
    cout << "Nodo según distancia al nodo inicial en numero de aristas" << endl;
    nodo = 1;
    for(auto element : d) {
        cout << "[" << nodo << "] " << element << " ";
        nodo++;
    }
    cout << endl;
    //mostrar en pantalla los predecesores
    cout << "Predecesores de cada nodo: " << endl;
    nodo = 1;
    for(auto element : pred) {
        cout << "[" << nodo << "] " << element + 1 << " ";
        nodo++;
    }
    cout << endl;
}