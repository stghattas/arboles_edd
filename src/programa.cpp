#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int MAX_HERMANOS = 10; // Máximo número de hermanos por persona

class Persona {
public:
    string nombre;
    int edad;
    Persona* padre;
    Persona* madre;
    Persona* hermanos[MAX_HERMANOS];
    int numHermanos;

    Persona(string n, int e) : nombre(n), edad(e), padre(nullptr), madre(nullptr), numHermanos(0) {
        for (int i = 0; i < MAX_HERMANOS; i++) {
            hermanos[i] = nullptr;
        }
    }

    void agregarHermano(Persona* hermano) {
        if (numHermanos < MAX_HERMANOS) {
            hermanos[numHermanos++] = hermano;
        }
    }
};

class ArbolGenealogico {
private:
    Persona* raiz;

public:
    ArbolGenealogico() : raiz(nullptr) {}

    void agregarPersona(const string& nombre, int edad, const string& padre, const string& madre) {
        Persona* nuevaPersona = new Persona(nombre, edad);

        // Si es la primera persona, la establecemos como raíz
        if (raiz == nullptr) {
            raiz = nuevaPersona;
        }

        // Asignar padres
        if (!padre.empty()) {
            // Si el padre ya existe, lo buscamos
            Persona* padrePersona = buscarPersona(padre);
            if (padrePersona == nullptr) {
                padrePersona = new Persona(padre, 0); // Edad no se conoce
            }
            nuevaPersona->padre = padrePersona;
            padrePersona->agregarHermano(nuevaPersona);
        }

        if (!madre.empty()) {
            // Si la madre ya existe, la buscamos
            Persona* madrePersona = buscarPersona(madre);
            if (madrePersona == nullptr) {
                madrePersona = new Persona(madre, 0); // Edad no se conoce
            }
            nuevaPersona->madre = madrePersona;
            madrePersona->agregarHermano(nuevaPersona);
        }
    }

    Persona* buscarPersona(const string& nombre) {
        // Buscamos en el árbol a la persona por su nombre
        return buscarEnArbol(raiz, nombre);
    }

    Persona* buscarEnArbol(Persona* persona, const string& nombre) {
        if (persona == nullptr) return nullptr;
        if (persona->nombre == nombre) return persona;

        // Buscar en los hermanos
        for (int i = 0; i < persona->numHermanos; i++) {
            Persona* encontrado = buscarEnArbol(persona->hermanos[i], nombre);
            if (encontrado != nullptr) return encontrado;
        }

        return nullptr;
    }

    void mostrarArbol(Persona* persona, int nivel = 0) {
        if (persona) {
            cout << string(nivel * 4, ' ') << persona->nombre << " (" << persona->edad << " anios)" << endl;
            for (int i = 0; i < persona->numHermanos; i++) {
                mostrarArbol(persona->hermanos[i], nivel + 1);
            }
        }
    }

    void mostrar() {
        mostrarArbol(raiz);
    }
};

void cargarDesdeCSV(ArbolGenealogico& arbol, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;

    if (archivo.is_open()) {
        // Saltar la cabecera
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string nombre, edadStr, padre, madre;

            getline(ss, nombre, ',');
            getline(ss, edadStr, ',');
            getline(ss, padre, ',');
            getline(ss, madre, ',');

            int edad = stoi(edadStr);
            arbol.agregarPersona(nombre, edad, padre, madre);
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}

int main() {
    ArbolGenealogico arbol;
    cargarDesdeCSV(arbol, "./bin/family.csv");
    arbol.mostrar();

    return 0;
}