#include <iostream>
#include <string>
#include <stdlib.h>
#include <direct.h>
#include <fstream>
#include <ctime>
using namespace std;
   
struct Nodo {
    string contenido;
    int numero;
    int nivel;
    bool apuntando;
    bool primero;
    Nodo* compañero;
    Nodo* nodoDerecho;
    Nodo* nodoIzquierdo;
};

Nodo* crearNodo(string contenido, int numero) {
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->contenido = contenido;
    nuevoNodo->numero = numero;
    nuevoNodo->nivel = 1;
    nuevoNodo->apuntando = false;
    nuevoNodo->primero = false;
    nuevoNodo->compañero = nullptr;
    nuevoNodo->nodoIzquierdo = nullptr;
    nuevoNodo->nodoDerecho = nullptr;
    return nuevoNodo;
}

int obtenerNivel(Nodo* actual) {
    if (actual == nullptr) {
        return 0;
    }
    return actual->nivel;
}

Nodo* rotacionDerechaDerecha(Nodo* actual) {
    Nodo* temporal = actual->nodoIzquierdo;
    actual->nodoIzquierdo = temporal->nodoDerecho;
    temporal->nodoDerecho = actual;

    actual->nivel = (max(obtenerNivel(actual->nodoDerecho), obtenerNivel(actual->nodoIzquierdo)) + 1);
    temporal->nivel = (max(obtenerNivel(temporal->nodoDerecho), obtenerNivel(temporal->nodoIzquierdo)) + 1);
    return temporal;
}

Nodo* rotacionIzquierdaIzquierda(Nodo* actual) {
    Nodo* temporal = actual->nodoDerecho;
    actual->nodoDerecho = temporal->nodoIzquierdo;
    temporal->nodoIzquierdo = actual;

    actual->nivel = (max(obtenerNivel(actual->nodoDerecho), obtenerNivel(actual->nodoIzquierdo)) + 1);
    temporal->nivel = (max(obtenerNivel(temporal->nodoDerecho), obtenerNivel(temporal->nodoIzquierdo)) + 1);
    return temporal;
}

Nodo* rotacionSimpleDerecha(Nodo* actual) {
    Nodo* temporal;
    actual->nodoIzquierdo = rotacionIzquierdaIzquierda(actual->nodoIzquierdo);
    temporal = rotacionDerechaDerecha(actual);
    return temporal;
}

Nodo* rotacionSimpleIzquierda(Nodo* actual) {
    Nodo* temporal;
    actual->nodoDerecho = rotacionDerechaDerecha(actual->nodoDerecho);
    temporal = rotacionIzquierdaIzquierda(actual);
    return temporal;
}

Nodo* buscarLugar(Nodo* actual, Nodo* nuevo) {
    if (nuevo->numero < actual->numero) {
        if (actual->nodoIzquierdo == nullptr) {
            actual->nodoIzquierdo = nuevo;
        } else {
            actual->nodoIzquierdo = buscarLugar(actual->nodoIzquierdo, nuevo);

            if ((obtenerNivel(actual->nodoIzquierdo) - obtenerNivel(actual->nodoDerecho)) == 2) {
                if (nuevo->numero < actual->nodoIzquierdo->numero) {
                    actual = rotacionDerechaDerecha(actual);
                } else {
                    actual = rotacionSimpleDerecha(actual);
                }
            }
        }
    } else {
        if (actual->nodoDerecho == nullptr) {
            actual->nodoDerecho = nuevo;
        } else {
            actual->nodoDerecho = buscarLugar(actual->nodoDerecho, nuevo);

            if ((obtenerNivel(actual->nodoDerecho) - obtenerNivel(actual->nodoIzquierdo)) == 2) {
                if (nuevo->numero > actual->nodoDerecho->numero) {
                    actual = rotacionIzquierdaIzquierda(actual);
                } else {
                    actual = rotacionSimpleIzquierda(actual);
                }
            }
        }
    }
    if (actual->nodoIzquierdo == nullptr && actual->nodoDerecho != nullptr) {
        actual->nivel = actual->nodoDerecho->nivel + 1;
    } else if (actual->nodoDerecho == nullptr && actual->nodoIzquierdo != nullptr) {
        actual->nivel = actual->nodoIzquierdo->nivel + 1;
    } else {
        actual->nivel = (max(obtenerNivel(actual->nodoDerecho), obtenerNivel(actual->nodoIzquierdo)) + 1);
    }
    return actual;
}

Nodo* buscarNodo(Nodo* inicial) {
    Nodo *aEnviar = nullptr;

    if (inicial != nullptr) {
        if (!inicial->apuntando) {
            aEnviar = inicial;
        }
        if (aEnviar == nullptr) {
            if (inicial->nodoIzquierdo != nullptr) {
                if (!inicial->nodoIzquierdo->apuntando) {
                    aEnviar = inicial->nodoIzquierdo;
                } else {
                    aEnviar = buscarNodo(inicial->nodoIzquierdo);
                }
            }
        }
        if (aEnviar == nullptr) {
            if (inicial->nodoDerecho != nullptr) {
                if (!inicial->nodoDerecho->apuntando) {
                    aEnviar = inicial->nodoDerecho;
                } else {
                    aEnviar = buscarNodo(inicial->nodoDerecho);
                }
            }
        }
    }
    return aEnviar;
}

class Campo {
    private:
        string nombre;
        string tipo;
        Nodo *valorInicial;
        int numeroDeDatos, orden;

    public:
        Campo() {}
        Campo(string nombre, string tipo, int orden) {
            this->nombre = nombre;
            this->tipo = tipo;
            this->valorInicial = nullptr;
            this->numeroDeDatos = 0;
            this->orden = orden;
        }   
        void guardarValor(string contenido) {
            int sumaLetras = 0;
            for (int i = 0; i < contenido.size(); i++) {
                sumaLetras += contenido[i];
            }
            
            if (valorInicial == nullptr) {
                valorInicial = crearNodo(contenido, sumaLetras);
            } else {
                Nodo *nuevo = crearNodo(contenido, sumaLetras);
                valorInicial = buscarLugar(valorInicial, nuevo);
            }
            numeroDeDatos++;
        }
        string getNombre() {
            return this->nombre;
        }
        string getTipo() {
            return this->tipo;
        }
        Nodo* getNodo() {
            return buscarNodo(valorInicial);
        }
        Nodo* getNodoInicial() {
            return this->valorInicial;
        }
        /*
        NodoLista* getNodosEspecificos(string valor) {
            
            NodoLista *lista = crearNodoLista();
            lista = buscarNodosEspecificos(valorInicial, lista, valorInicial);
            return lista;
        }
        */
        int getNumeroDeDatos() {
            return this->numeroDeDatos;
        }
        int getOrden() {
            return this->orden;
        }
};

int funcionHash(string grupo, int size) {
    int numerohash = 0;

    int sumaLetras = 0;
    for (int i = 0; i < grupo.size(); i++) {
        sumaLetras += grupo[i];
    }
    sumaLetras = sumaLetras % size;
    return sumaLetras;
}

class Conjunto {
    private:
        string nombre = "";
        Campo* campos = NULL;
        int size = 0;

    public:
        Conjunto() {}
        void setDatos(string nombre, int size) {
            this->nombre = nombre;
            this->campos = new Campo[size];
            this->size = size;
        }
        void agregarCampo(string nombre, string tipo, int orden) {
            int ubicacion = funcionHash(nombre, this->size);

            for (int i = 0; i < size; i++) {
                if (campos[ubicacion].getNombre() == "") {
                    campos[ubicacion] = Campo(nombre, tipo, orden);
                    break;
                } else {
                    if (ubicacion != (size - 1)) {
                        ubicacion++;
                    } else {
                        ubicacion = 0;
                    }
                }
            }
        }
        string getNombre() {
            return this->nombre;
        }
        Campo* getCampos() {
            return this->campos;
        }
        int getSize() {
            return this->size;
        }
};

int buscarGrupo(Conjunto gruposActuales[], int size, string grupo) {
    int ubicacion = funcionHash(grupo, size);

    for (int i = 0; i < size; i++) {
        if (gruposActuales[ubicacion].getNombre() == grupo) {
            return ubicacion;
        } else {
            if (ubicacion != (size - 1)) {
                ubicacion++;
            } else {
                ubicacion = 0;
            }
        }
    }
    return -1;   
}

int guardarGrupo(Conjunto gruposActuales[], int size, string grupo) {
    int ubicacion = funcionHash(grupo, size);

    for (int i = 0; i < size; i++) {
        if (gruposActuales[ubicacion].getNombre() == "") {
            return ubicacion;
        } else {
            if (ubicacion != (size - 1)) {
                ubicacion++;
            } else {
                ubicacion = 0;
            }
        }
    }
    return -1;   
}




















void escribirArchivoLog(string accion) {
    ofstream archivoEscribir;
    ifstream archivoLeer;
    string texto;

    time_t tiempo;
    tiempo = time(NULL);
    struct tm *fecha;
    fecha = localtime(&tiempo);

    string fechaAMostrar = "   " + to_string(fecha->tm_hour) + ":" + to_string(fecha->tm_min) + " " + to_string(fecha->tm_mday) + "/" + to_string(fecha->tm_mon + 1) + "/" + to_string(fecha->tm_year + 1900);
    archivoLeer.open("Sistema.log", ios::in);

    if (!archivoLeer.fail()) {
        string textoTotal = accion + fechaAMostrar + "+";

        while(!archivoLeer.eof()) {
            getline(archivoLeer, texto);
            textoTotal += (texto + "+");
        }
        archivoLeer.close();
        remove("Sistema.log");

        archivoEscribir.open("Sistema.log", ios::out);

        if (!archivoEscribir.fail()) {
            string textoAEscribir = "";
        
            for (int i = 0; i < textoTotal.size(); i++) {
                if (textoTotal[i] != '+') {
                    textoAEscribir += textoTotal[i];
                } else {
                    archivoEscribir << textoAEscribir << endl;
                    textoAEscribir = "";
                }
            }
        }
    } else {
        archivoEscribir.open("Sistema.log", ios::out);
        accion = accion + fechaAMostrar;

        if (!archivoEscribir.fail()) {
            archivoEscribir << accion << endl;
        }
    }
}

string obtenerGrupo(string comando, int limite) {
    string grupo = "";

    for (int i = limite; i < comando.size(); i++) {
        if (comando[i] != ' ') {
            grupo += comando[i];
        } else {
            break;
        }
    }
    return grupo;
}

void crearGrupoDeContactos(string comando, Conjunto gruposActuales[], int size) {
    string grupo = obtenerGrupo(comando, 14), campo = "", tipo = "";
    int contador = 14 + grupo.size() + 9, valor = guardarGrupo(gruposActuales, size, grupo), indiceArreglo = 0, contador_2 = contador, orden = 0, index = 0;
    bool grupoRepetido = true;
    int indiceConjunto = buscarGrupo(gruposActuales, size, grupo);

    if (indiceConjunto == -1) {
        grupoRepetido = false;
    }

    if (!grupoRepetido) {
        while (contador_2 < comando.size()) {
            if (comando[contador_2] == ',') {
                indiceArreglo++;
            } else if (comando[contador_2] == ')') {
                indiceArreglo++;
                break;
            }
            contador_2++;
        }
        gruposActuales[valor].setDatos(grupo, indiceArreglo);
        
        while (contador < comando.size()) {

            if (comando[contador] != ' ') {
                campo += comando[contador];
                contador++;
            } else {
                contador++;

                for (int j = contador; j < comando.size(); j++) {
                    if (comando[j] != ',' && comando[j] != ')') {
                        tipo += comando[j];
                    } else {
                        contador = j;
                        break;
                    }
                }
                if (comando[contador] == ',') {
                    gruposActuales[valor].agregarCampo(campo, tipo, orden);
                    campo = ""; tipo = "";
                    contador += 2;
                    orden++;
                } else {
                    gruposActuales[valor].agregarCampo(campo, tipo, orden);
                    break;
                }
            }
        }
        escribirArchivoLog("Se creo el Grupo " + grupo + " con " + to_string(indiceArreglo) + " campo(s): ");
    } else {
        cout << endl;
        cout << "+================================================+" << endl;
        cout << "| Error, el grupo ya Existe o hubo otro Problema |" << endl;
        cout << "+================================================+" << endl;
        escribirArchivoLog("Se intento Crear el Grupo: " + grupo + ", pero el grupo ya Existe");
    }
}

void insertarContactos(string comando, Conjunto gruposActuales[], int size) {
    string grupo = obtenerGrupo(comando, 15), valor = "";
    int contador = (15 + grupo.size() + 9), ubicacion = buscarGrupo(gruposActuales, size, grupo), indice = 0, orden = 0, contadorValoresCopiados = 0;
    
    if (ubicacion != -1) {
        Campo* campos = gruposActuales[ubicacion].getCampos();
        int numeroDeCampos = gruposActuales[ubicacion].getSize();
        string* valoresIngresados = new string[numeroDeCampos];

        while (contador < (comando.size() - 2)) {
            if (comando[contador] != ',') {
                valor += comando[contador];
            } else {
                contador++;

                for (int i = 0; i < numeroDeCampos; i++) {
                    if (campos[i].getOrden() == orden) {
                        indice = i;
                        orden++;
                        break;
                    }
                } 
                campos[indice].guardarValor(valor);
                valoresIngresados[contadorValoresCopiados] = valor;
                indice++; contadorValoresCopiados++;
                valor = "";            
            }
            contador++;
        }

        for (int i = 0; i < numeroDeCampos; i++) {
            if (campos[i].getOrden() == orden) {
                indice = i;
                break;
            }
        }
        campos[indice].guardarValor(valor);
        valoresIngresados[contadorValoresCopiados] = valor;

        Nodo* primero = nullptr;
        for (int i = 0; i < numeroDeCampos; i++) {
            Nodo* nodoApuntador = nullptr;
            Nodo* nodoApuntado = nullptr;
            int numeroDeIndice = 0;
            int ind_1 = i, ind_2 = i + 1;

            if (i == (numeroDeCampos - 1)) { ind_2 = 0; }

            while(true) {
                if (campos[numeroDeIndice].getOrden() == ind_1) {
                    Nodo* v = campos[numeroDeIndice].getNodoInicial();
                    nodoApuntador = campos[numeroDeIndice].getNodo();
                    break;
                }
                numeroDeIndice++;
            }
            numeroDeIndice = 0;
            while(true) {
                if (campos[numeroDeIndice].getOrden() == ind_2) {
                    nodoApuntado = campos[numeroDeIndice].getNodo();
                    break;
                }
                numeroDeIndice++;
            }

            if (i != (numeroDeCampos - 1)) {
                nodoApuntador->compañero = nodoApuntado;
                nodoApuntador->apuntando = true;
            } else {
                nodoApuntador->compañero = primero;
                nodoApuntador->apuntando = true;
            }

            if (i == 0) {
                primero = nodoApuntador;
                nodoApuntador->primero = true;
            }
        } 
        escribirArchivoLog("Se ingreso un Contacto al Grupo: " + grupo);
    } else {
        cout << endl;
        cout << "+===========================+" << endl;
        cout << "| Error, el grupo no Existe |" << endl;
        cout << "+===========================+" << endl;
        escribirArchivoLog("Se intento ingresar un Contacto, pero el grupo " + grupo + " no Existe");
    }
}

void imprimirContactos(Nodo* valorActual, string valor, int numeroDeCampos) {
    if (valorActual->contenido == valor) {
        Nodo *aMostrar = valorActual;

        while (true) {
            if (!aMostrar->primero) {
                aMostrar = aMostrar->compañero;
            } else {
                break;
            }
        }

        for (int i = 0; i < numeroDeCampos; i++) {
            if (i == (numeroDeCampos - 1)) {
                cout << " " << aMostrar->contenido << endl;
            } else {
                cout << " " << aMostrar->contenido << ", ";
            }
            aMostrar = aMostrar->compañero;
        }
        
    }
    if (valorActual->nodoIzquierdo != nullptr) {
        imprimirContactos(valorActual->nodoIzquierdo, valor, numeroDeCampos);
    }
    if (valorActual->nodoDerecho != nullptr) {
        imprimirContactos(valorActual->nodoDerecho, valor, numeroDeCampos);
    }
}

void buscarContactos(string comando, Conjunto gruposActuales[], int size) {
    string grupo = obtenerGrupo(comando, 16), campo = "", valor = "";
    int contador = 16 + grupo.size() + 15;

    while (contador < comando.size()) {
        if (comando[contador] != '=') {
            campo += comando[contador];
            contador++;
        } else {
            contador++;

            for (int i = contador; i < (comando.size() - 1); i++) {
                valor += comando[i];
            }
            break;
        }
    }

    int indiceGrupo = buscarGrupo(gruposActuales, size, grupo);

    if (indiceGrupo != -1) {
        Campo* campos = gruposActuales[indiceGrupo].getCampos();
        int numeroDeCampos = gruposActuales[indiceGrupo].getSize();
        int indiceCampo = funcionHash(campo, numeroDeCampos); 
        Nodo* valorInicial = campos[indiceCampo].getNodoInicial();

        cout << endl;
        cout << "------------------------------------" << endl;
        cout << "Contacto(s):" << endl;
        cout << endl;
        
        if (valorInicial != nullptr) {
            imprimirContactos(valorInicial, valor, numeroDeCampos);
        }
        cout << "------------------------------------" << endl;
        escribirArchivoLog("Se busco en el Grupo: " + grupo + ", en el campo: " + campo + " el valor: " + valor);
    } else {
        cout << "+===========================+" << endl;
        cout << "| Error, el grupo no Existe |" << endl;
        cout << "+===========================+" << endl;
        escribirArchivoLog("Se busco en el Grupo: " + grupo + ", pero el Grupo no Existe ");
    }
}

void opcionesDeLaTerminal(Conjunto gruposActuales[], int size) {
    bool salir = false;
    int seleccion = 0;
    string comando;

    while(!salir) {
    
        cout << endl;
        cout << "Terminal:" << endl;
        cout << "   1. Crear" << endl;
        cout << "   2. Insertar" << endl;
        cout << "   3. Buscar" << endl;
        cout << "   4. Atras" << endl;
        cin >> seleccion;

        switch (seleccion) {
            case 1: 
                cout << endl;
                cout << "-----------------------------------------------------------------------------" << endl;
                cout << "Ingrese comando para crear un Nuevo Grupo:"<<endl;
                cout << "Escribir: "; getline(cin, comando); getline(cin, comando);
                cout << "-----------------------------------------------------------------------------" << endl;
                crearGrupoDeContactos(comando, gruposActuales, size);
                salir = true;
                break;
            case 2: 
                cout << endl;
                cout << "-----------------------------------------------------------------------------" << endl;
                cout << "Ingrese comando para insertar Valores:"<<endl;
                cout << "Escribir: "; getline(cin, comando); getline(cin, comando);
                cout << "-----------------------------------------------------------------------------" << endl;
                insertarContactos(comando, gruposActuales, size);
                salir = true;
                break;
            case 3: 
                cout<<endl;
                cout << "-----------------------------------------------------------------------------" << endl;
                cout << "Ingrese comando de Busqueda:" << endl;
                cout << "Escribir: "; getline(cin,comando); getline(cin,comando);
                cout << "-----------------------------------------------------------------------------" << endl;
                buscarContactos(comando, gruposActuales, size);
                salir = true;
                break;
            case 4: salir = true;
                break;
            default:
                cout << endl;
                cout << "+==============================+" << endl;
                cout << "| Error, Numero Fuera de Rango |" << endl;
                cout << "+==============================+" << endl;
                break;
        }
    }
}

void reportes(Conjunto gruposActuales[], int size) {
    int* cantidadDeDatosPorGrupo = new int[size];
    int* cantidadDeContactosPorGrupo = new int[size];
    int contadorValores = 0, indice = 0, numeroDeString = 0, numeroDeInteger = 0, numeroDeChar = 0, numeroDeDate = 0;

    for (int i = 0; i < size; i++) {
        cantidadDeDatosPorGrupo[i] = 0;
        cantidadDeContactosPorGrupo[i] = 0;
    }

    cout << endl;
    cout << endl;
    cout << "+-----------------REPORTES-----------------+" << endl;
    cout << " Cantidad de Datos por Grupo:" << endl;
    cout << endl;

    for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() != "") {
            string nombreGrupo = gruposActuales[i].getNombre();
            int numeroDeCampos = gruposActuales[i].getSize();
            Campo* campos = gruposActuales[i].getCampos();
            int numeroDeDatos = campos->getNumeroDeDatos();


            int totalGrupo = numeroDeDatos * numeroDeCampos; 
            cantidadDeDatosPorGrupo[i] = totalGrupo;
            cantidadDeContactosPorGrupo[i] = numeroDeDatos;
            cout << " - "<< nombreGrupo << ": " << totalGrupo << " dato(s)"<< endl;
            contadorValores = 0;
        }
    }
    cout << "+------------------------------------------+" << endl;
    int totalSistema = 0;

    for (int i = 0; i < size; i++) {
        totalSistema += cantidadDeDatosPorGrupo[i];
    }
    cout << " Todos de Datos del Sistema: " << endl;
    cout << endl;
    cout << " - Total: " << totalSistema << " dato(s)" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << " Cantidad de Contactos con el mismo Tipo:" << endl;
    cout << endl;
    for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() != "") {
            bool stringRegistrado = false, integerRegistrado = false, charRegistrado = false, dateRegistrado = false;
            int numeroDeCampos = gruposActuales[i].getSize();
            Campo* campos = gruposActuales[i].getCampos();

            for (int j = 0; j < numeroDeCampos; j++) {
                if (campos[j].getTipo() == "STRING") {
                    if (!stringRegistrado) {
                        stringRegistrado = true;
                    }
                } else if (campos[j].getTipo() == "INTEGER") {
                    if (!integerRegistrado) {
                        integerRegistrado = true;
                    }
                } else if (campos[j].getTipo() == "CHAR") {
                    if (!charRegistrado) {
                        charRegistrado = true;
                    }
                } else if (campos[j].getTipo() == "DATE") {
                    if (!dateRegistrado) {
                        dateRegistrado = true;
                    }
                }
            }   
            int numeroDeDatos = campos[0].getNumeroDeDatos();

            if (stringRegistrado) {
                numeroDeString += numeroDeDatos;
            }
            if (integerRegistrado) {
                numeroDeInteger += numeroDeDatos;
            }
            if (charRegistrado) {
                numeroDeChar += numeroDeDatos;
            }
            if (dateRegistrado) {
                numeroDeDate += numeroDeDatos;
            }
        }
    }
    cout << " - STRING: " << numeroDeString << " contacto(s)" << endl;
    cout << " - INTEGER: " << numeroDeInteger << " contacto(s)" << endl;
    cout << " - CHAR: " << numeroDeChar << " contacto(s)" << endl;
    cout << " - DATE: " << numeroDeDate << " contacto(s)" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << " Cantidad de Contactos por Grupo:" << endl;
    cout << endl;
    for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() != "") {
            string nombreGrupo = gruposActuales[i].getNombre();
            cout << " - "<< nombreGrupo << ": " << cantidadDeContactosPorGrupo[i] << " contacto(s)"<< endl;
        }
    }
    cout << "+------------------------------------------+" << endl;
    cout << endl;
}

int graficarNodos(Nodo* actual, string& valores, string& referencias, int numeroDeReferencia, int contadorNodo) {
    valores += "    " + to_string(contadorNodo) + " [label=\"" + actual->contenido + "\"];\n";
    referencias += "    " + to_string(numeroDeReferencia) + " -> " + to_string(contadorNodo) + ";\n";
    numeroDeReferencia = contadorNodo;
    contadorNodo++;

    if (actual->nodoIzquierdo != nullptr) {
        contadorNodo = graficarNodos(actual->nodoIzquierdo, valores, referencias, numeroDeReferencia, contadorNodo);
    } 
    if (actual->nodoDerecho != nullptr) {
        contadorNodo = graficarNodos(actual->nodoDerecho, valores, referencias, numeroDeReferencia, contadorNodo);
    }
    return contadorNodo;
}

void graficarEspecifico(Conjunto gruposActuales[], int size, string grupoABuscar, bool mostrarNodos) {
    string referencias = "", campoDescripcion = "", valores = "";
    int contadorNodo = 2;
    bool crear = false;

    string code = "digraph G {\n"
    "    node [shape=circle, style=filled, fillcolor=lightblue, fontcolor=black];\n"
    "    graph [rankdir = \"LR\"];\n"
    "    \n";

    int indiceConjunto = buscarGrupo(gruposActuales, size, grupoABuscar);

    if (indiceConjunto != -1) {
        crear = true;
    }

    /*for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() == grupoABuscar) {
            indice = i;
            crear = true;
            break;
        }
    }*/
    
    if (crear) {
        Campo* campos = gruposActuales[indiceConjunto].getCampos(); //
        int numeroDeCampos = gruposActuales[indiceConjunto].getSize(); //
        code += "    node0 [label = \"<f0> " + gruposActuales[indiceConjunto].getNombre() + " \" shape = \"record\"];\n";

        for (int j = 0; j < numeroDeCampos; j++) {
            Nodo* valorInicial = campos[j].getNodoInicial();
            
            if (j == 0) {
                campoDescripcion += "    node1 [label = \"<f" + to_string(j) + "> " + campos[j].getNombre();
            } else {
                campoDescripcion += "| <f" + to_string(j) + "> " + campos[j].getNombre();
            }
            referencias += "    \"node0\":f0 -> \"node1\":f" + to_string(j) + ";\n";

            if (valorInicial != nullptr && mostrarNodos) {
                valores += "    " + to_string(contadorNodo) + " [label=\"" + valorInicial->contenido + "\"];\n";
                referencias += "    \"node1\":f"  + to_string(j) + " -> " + to_string(contadorNodo) + ";\n";
                int numeroReferencia = contadorNodo;
                contadorNodo++;

                if (valorInicial->nodoIzquierdo != nullptr) {
                    contadorNodo = graficarNodos(valorInicial->nodoIzquierdo, valores, referencias, numeroReferencia, contadorNodo);
                } 
                if (valorInicial->nodoDerecho != nullptr) {
                    contadorNodo = graficarNodos(valorInicial->nodoDerecho, valores, referencias, numeroReferencia, contadorNodo);
                }
            }
        }
        campoDescripcion += " \" shape = \"record\"];\n";
    
        code += campoDescripcion;
        code +=  "\n";
        code += valores;
        code +=  "\n";
        code += referencias;
        code += "}";

        ofstream file("gra.dot");
        if (file.is_open()) {
            file << code << endl;
            file.close();
            system("dot -Tpng gra.dot -o graf.png");
            cout << endl;
            cout << "---------------------" << endl;
            cout << " Se genero la Grafica" << endl;
            cout << "---------------------" << endl;
        }
        escribirArchivoLog("Se genero la grafica del grupo: " + grupoABuscar);
    } else {
        cout << endl;
        cout << "+===============================================+" << endl;
        cout << "| Error, no se encontro un grupo con ese Nombre |" << endl;
        cout << "+===============================================+" << endl;
    }
}

void graficarTodo(Conjunto gruposActuales[], int size) {
    string referencias = "", campoDescripcion = "", valores = "";
    int campoNumero = 0, contadorNodo = 0, indice = 0;
    bool crear = false;

    string code = "digraph G {\n"
    "    node [shape=circle, style=filled, fillcolor=lightblue, fontcolor=black];\n"
    "    graph [rankdir = \"LR\"];\n"
    "    \n";

    for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() != "") {
            Campo* campos = gruposActuales[i].getCampos(); //
            int numeroDeCampos = gruposActuales[i].getSize(); //
            crear = true;

            if (indice == 0) {
                code += "    node" + to_string(contadorNodo) + " [label = \"<f" + to_string(indice) + "> " + gruposActuales[i].getNombre();
                contadorNodo++;
            } else  {
                code += "| <f" + to_string(indice) + "> " + gruposActuales[i].getNombre();
            }
            indice++;

            for (int j = 0; j < numeroDeCampos; j++) {
                Nodo* valorInicial = campos[j].getNodoInicial();
                
                if (j == 0) {
                    campoDescripcion += "    node" + to_string(contadorNodo) + " [label = \"<f" + to_string(j) + "> " + campos[j].getNombre();
                    
                    for (int k = 0; k < numeroDeCampos; k++) {
                        referencias += "    \"node0\":f" + to_string(indice - 1) + " -> \"node" + to_string(contadorNodo) + "\":f" + to_string(k) + ";\n";
                    }
                    campoNumero = contadorNodo;
                    contadorNodo++; 
                } else {
                    campoDescripcion += "| <f" + to_string(j) + "> " + campos[j].getNombre();
                }

                /*if (valorInicial != nullptr) {
                    valores += "    " + to_string(contadorNodo) + " [label=\"" + valorInicial->contenido + "\"];\n";
                    referencias += "    \"node" + to_string(campoNumero) + "\":f"  + to_string(j) + " -> " + to_string(contadorNodo) + ";\n";
                    int numeroReferencia = contadorNodo;
                    contadorNodo++;

                    if (valorInicial->nodoIzquierdo != nullptr) {
                        contadorNodo = graficarNodos(valorInicial->nodoIzquierdo, valores, referencias, numeroReferencia, contadorNodo);
                    } 
                    if (valorInicial->nodoDerecho != nullptr) {
                        contadorNodo = graficarNodos(valorInicial->nodoDerecho, valores, referencias, numeroReferencia, contadorNodo);
                    }
                }*/
            }
            campoDescripcion += " \" shape = \"record\"];\n";
        }
    }
    code += " \" shape = \"record\"];\n";
    code += campoDescripcion;
    code +=  "\n";
    code += valores;
    code +=  "\n";
    code += referencias;
    code += "}";

    if (crear) {
        ofstream file("gra.dot");
        if (file.is_open()) {
            file << code << endl;
            file.close();
            system("dot -Tpng gra.dot -o graf.png");
            cout << endl;
            cout << "---------------------" << endl;
            cout << " Se genero la Grafica" << endl;
            cout << "---------------------" << endl;
        }
        escribirArchivoLog("Se genero la grafica de todo el Sistema");
    } else {
        cout << endl;
        cout << "+===================================+" << endl;
        cout << "| Error, no hay grupos para Mostrar |" << endl;
        cout << "+===================================+" << endl;
    }
}

void menuGrafica(Conjunto gruposActuales[], int size) {
    bool salir = false;
    int seleccion = 0;
    string grupoABuscar;

    while(!salir) {
        cout << endl;
        cout << "Grafica:" << endl;
        cout << "   1. Visualizar todas la Estructuras del Sistema" << endl;
        cout << "   2. Visualizar una Estructura del Sistema" << endl;
        cout << "   3. Visualizar una Estructura del Sistema con sus Datos" << endl;
        cout << "   4. Atras" << endl;
        cin >> seleccion;

        switch (seleccion) {
            case 1: 
                graficarTodo(gruposActuales, size);
                salir = true;
                break;
            case 2: 
                cout<<endl;
                cout << "------------------------------------------------" << endl;
                cout << "Ingrese el grupo a Graficar: "; getline(cin, grupoABuscar); getline(cin, grupoABuscar);
                cout << "------------------------------------------------" << endl;
                graficarEspecifico(gruposActuales, size, grupoABuscar, false);
                salir = true;
                break;
            case 3: 
                cout<<endl;
                cout << "------------------------------------------------" << endl;
                cout << "Ingrese el grupo a Graficar: "; getline(cin, grupoABuscar); getline(cin, grupoABuscar);
                cout << "------------------------------------------------" << endl;
                graficarEspecifico(gruposActuales, size, grupoABuscar, true);
                salir = true;
                break;
            case 4: salir = true;
                break;
            default:
                cout << endl;
                cout << "+==============================+" << endl;
                cout << "| Error, Numero Fuera de Rango |" << endl;
                cout << "+==============================+" << endl;
                break;
        }
    }
}

void exportarArchivos(Nodo* valorActual, string nombreDeLaCarpeta, int limiteCampos) {
    Nodo *nodoAExportar = valorActual;

    while (true) {
        if (!nodoAExportar->primero) {
            nodoAExportar = nodoAExportar->compañero;
        } else {
            break;
        }
    }
    string contacto = "";
    string nombreDelArchivo = nodoAExportar->contenido;

    for (int i = 0; i < limiteCampos; i++) {
        if (i == (limiteCampos - 1)) {
            contacto += nodoAExportar->contenido;
        } else {
            contacto += nodoAExportar->contenido + ", ";
        }
        nodoAExportar = nodoAExportar->compañero;
    }    

    ofstream archivo;
    archivo.open(nombreDeLaCarpeta + "\\" + nombreDelArchivo +".txt", ios::out);

    if (!archivo.fail()) {
        archivo << contacto << endl;
    } 

    if (valorActual->nodoIzquierdo != nullptr) {
        exportarArchivos(valorActual->nodoIzquierdo, nombreDeLaCarpeta, limiteCampos);
    }
    if (valorActual->nodoDerecho != nullptr) {
        exportarArchivos(valorActual->nodoDerecho, nombreDeLaCarpeta, limiteCampos);
    }
}

void exportarCarpeta(Conjunto gruposActuales[], int size) {
    string nombreDeLaCarpeta = "";
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Escriba el Nombre del Grupo a Exportar: "; getline(cin, nombreDeLaCarpeta); getline(cin, nombreDeLaCarpeta);
    cout << "---------------------------------------------------" << endl;
    bool grupoCorrecto = false;
    int indiceConjunto = buscarGrupo(gruposActuales, size, nombreDeLaCarpeta);

    if (indiceConjunto != -1) {
        grupoCorrecto = true;
    }

    if (grupoCorrecto) {
        string comando = "rmdir " + nombreDeLaCarpeta + " /s /q";
        system(comando.c_str());

        if (mkdir(nombreDeLaCarpeta.c_str()) == 0) {
            Campo* campos = gruposActuales[indiceConjunto].getCampos();
            int limiteCampos = gruposActuales[indiceConjunto].getSize();
            Nodo *valorInicial = campos[0].getNodoInicial();

            if (valorInicial != nullptr) {
                exportarArchivos(valorInicial, nombreDeLaCarpeta, limiteCampos);
            }
            escribirArchivoLog("Se Exporto el Grupo: " + nombreDeLaCarpeta);
        } else {
            cout << endl;
            cout << "+=======================================+" << endl;
            cout << "| Error, no se pudo crear el Directorio |" << endl;
            cout << "+=======================================+" << endl;
        }
    } else {
        cout << endl;
        cout << "+===============================================+" << endl;
        cout << "| Error, no se encontro un grupo con ese Nombre |" << endl;
        cout << "+===============================================+" << endl;
    }
}

Conjunto* verificarReHashing(Conjunto gruposActuales[], int& size) {
    int contador = 0;
    
    for (int i = 0; i < size; i++) {
        if (gruposActuales[i].getNombre() != "") {
            contador++;
        }
    }
    
    double porcentaje = (double) contador/size;

    if (porcentaje >= 0.6) {
        Conjunto *temporal = new Conjunto[contador];
        int contadorNuevo = 0;

        for (int i = 0; i < size; i++) {
            if (gruposActuales[i].getNombre() != "") {
                temporal[contadorNuevo] = gruposActuales[i];
                contadorNuevo++;
            }
        }
        size = size + 3;
        gruposActuales = new Conjunto[size];

        for (int i = 0; i < contador; i++) {
            int indice = guardarGrupo(gruposActuales, size, temporal[i].getNombre());
            gruposActuales[indice] = temporal[i];
        }
        cout << endl;
        cout << "-----------------------------------------------------------------" << endl;
        cout << "Se llego al 60% de la capacidad de la Tabla se aumento el tamanio" << endl;
        cout << "en 3 unidades, el nuevo tamanio es de: " << size << " unidades" << endl;
        cout << "-----------------------------------------------------------------" << endl;
    }
    return gruposActuales;
}

int main() {
    //Conjunto gruposActuales[5] = {Conjunto()};
    Conjunto *gruposActuales = new Conjunto[5];
    int size = 5, opcion = 0;
    bool salir = false;

    while(!salir) {
        cout << endl;
        cout << "Menu Principal" << endl;
        cout << "   1. Terminal del Lenguaje" << endl;
        cout << "   2. Menu de Reportes" << endl;
        cout << "   3. Graficas de las Estructuras" << endl;
        cout << "   4. Exportar de Contactos" << endl;
        cout << "   5. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: opcionesDeLaTerminal(gruposActuales, size);
                break;
            case 2: reportes(gruposActuales, size);
                escribirArchivoLog("Se Visualizo el Menu de Reportes");
                break;
            case 3: menuGrafica(gruposActuales, size);
                break;
            case 4: exportarCarpeta(gruposActuales, size);
                break;
            case 5: salir = true;
                break;
            default:
                cout << endl;
                cout << "+==============================+" << endl;
                cout << "| Error, Numero Fuera de Rango |" << endl;
                cout << "+==============================+" << endl;
                break;
        }
        gruposActuales = verificarReHashing(gruposActuales, size);
    }
}