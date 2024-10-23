#include <iostream>
using namespace std;

enum Color { ROJO, NEGRO };

struct Nodo {
    int valor;
    Color color;
    Nodo *izquierda, *derecha, *padre;
    Nodo(int valor) : valor(valor), color(ROJO), izquierda(nullptr), derecha(nullptr), padre(nullptr) {}
};

class ArbolRojoNegro {
private:
    Nodo* raiz;

    void RotarIzquierda(Nodo* &nodo) {
        Nodo* hijoDerecho = nodo->derecha;
        nodo->derecha = hijoDerecho->izquierda;
        if (nodo->derecha != nullptr) nodo->derecha->padre = nodo;
        hijoDerecho->padre = nodo->padre;
        if (nodo->padre == nullptr) raiz = hijoDerecho;
        else if (nodo == nodo->padre->izquierda) nodo->padre->izquierda = hijoDerecho;
        else nodo->padre->derecha = hijoDerecho;
        hijoDerecho->izquierda = nodo;
        nodo->padre = hijoDerecho;
    }

    void RotarDerecha(Nodo* &nodo) {
        Nodo* hijoIzquierdo = nodo->izquierda;
        nodo->izquierda = hijoIzquierdo->derecha;
        if (nodo->izquierda != nullptr) nodo->izquierda->padre = nodo;
        hijoIzquierdo->padre = nodo->padre;
        if (nodo->padre == nullptr) raiz = hijoIzquierdo;
        else if (nodo == nodo->padre->derecha) nodo->padre->derecha = hijoIzquierdo;
        else nodo->padre->izquierda = hijoIzquierdo;
        hijoIzquierdo->derecha = nodo;
        nodo->padre = hijoIzquierdo;
    }

    void InsertarNodo(Nodo* &raiz, Nodo* nodo) {
        if (raiz == nullptr) raiz = nodo;
        else {
            Nodo* actual = raiz;
            while (true) {
                if (nodo->valor < actual->valor) {
                    if (actual->izquierda == nullptr) {
                        actual->izquierda = nodo;
                        nodo->padre = actual;
                        break;
                    } else {
                        actual = actual->izquierda;
                    }
                } else {
                    if (actual->derecha == nullptr) {
                        actual->derecha = nodo;
                        nodo->padre = actual;
                        break;
                    } else {
                        actual = actual->derecha;
                    }
                }
            }
            BalancearDespuesDeInsercion(nodo);
        }
    }

    void BalancearDespuesDeInsercion(Nodo* nodo) {
        Nodo *padre = nullptr, *abuelo = nullptr;
        while (nodo != raiz && nodo->color != NEGRO && nodo->padre->color == ROJO) {
            padre = nodo->padre;
            abuelo = padre->padre;
            if (padre == abuelo->izquierda) {
                Nodo* tio = abuelo->derecha;
                if (tio && tio->color == ROJO) {
                    abuelo->color = ROJO;
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    nodo = abuelo;
                } else {
                    if (nodo == padre->derecha) {
                        RotarIzquierda(padre);
                        nodo = padre;
                        padre = nodo->padre;
                    }
                    RotarDerecha(abuelo);
                    swap(padre->color, abuelo->color);
                    nodo = padre;
                }
            } else {
                Nodo* tio = abuelo->izquierda;
                if (tio && tio->color == ROJO) {
                    abuelo->color = ROJO;
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    nodo = abuelo;
                } else {
                    if (nodo == padre->izquierda) {
                        RotarDerecha(padre);
                        nodo = padre;
                        padre = nodo->padre;
                    }
                    RotarIzquierda(abuelo);
                    swap(padre->color, abuelo->color);
                    nodo = padre;
                }
            }
        }
        raiz->color = NEGRO;
    }

    Nodo* BuscarNodo(Nodo* nodo, int valor) const {
        while (nodo != nullptr) {
            if (valor == nodo->valor) return nodo;
            nodo = valor < nodo->valor ? nodo->izquierda : nodo->derecha;
        }
        return nullptr;
    }

    Nodo* ObtenerMinimo(Nodo* nodo) {
        while (nodo->izquierda != nullptr) nodo = nodo->izquierda;
        return nodo;
    }

    void EliminarNodo(Nodo* nodo, int valor) {
        Nodo* nodoEliminar = BuscarNodo(nodo, valor);
        if (nodoEliminar == nullptr) return;

        Nodo* nodoReemplazo = nullptr;
        Nodo* hijoReemplazo = nullptr;

        if (nodoEliminar->izquierda == nullptr || nodoEliminar->derecha == nullptr)
            nodoReemplazo = nodoEliminar;
        else
            nodoReemplazo = ObtenerMinimo(nodoEliminar->derecha);

        if (nodoReemplazo->izquierda != nullptr)
            hijoReemplazo = nodoReemplazo->izquierda;
        else
            hijoReemplazo = nodoReemplazo->derecha;

        if (hijoReemplazo != nullptr)
            hijoReemplazo->padre = nodoReemplazo->padre;

        if (nodoReemplazo->padre == nullptr)
            raiz = hijoReemplazo;
        else if (nodoReemplazo == nodoReemplazo->padre->izquierda)
            nodoReemplazo->padre->izquierda = hijoReemplazo;
        else
            nodoReemplazo->padre->derecha = hijoReemplazo;

        if (nodoReemplazo != nodoEliminar)
            nodoEliminar->valor = nodoReemplazo->valor;

        if (nodoReemplazo->color == NEGRO && hijoReemplazo != nullptr)
            BalancearDespuesDeEliminacion(hijoReemplazo);

        delete nodoReemplazo;
    }

    void BalancearDespuesDeEliminacion(Nodo* nodo) {
        Nodo* hermano = nullptr;
        while (nodo != raiz && nodo->color == NEGRO) {
            if (nodo == nodo->padre->izquierda) {
                hermano = nodo->padre->derecha;
                if (hermano->color == ROJO) {
                    hermano->color = NEGRO;
                    nodo->padre->color = ROJO;
                    RotarIzquierda(nodo->padre);
                    hermano = nodo->padre->derecha;
                }
                if (hermano->izquierda->color == NEGRO && hermano->derecha->color == NEGRO) {
                    hermano->color = ROJO;
                    nodo = nodo->padre;
                } else {
                    if (hermano->derecha->color == NEGRO) {
                        hermano->izquierda->color = NEGRO;
                        hermano->color = ROJO;
                        RotarDerecha(hermano);
                        hermano = nodo->padre->derecha;
                    }
                    hermano->color = nodo->padre->color;
                    nodo->padre->color = NEGRO;
                    hermano->derecha->color = NEGRO;
                    RotarIzquierda(nodo->padre);
                    nodo = raiz;
                }
            } else {
                hermano = nodo->padre->izquierda;
                if (hermano->color == ROJO) {
                    hermano->color = NEGRO;
                    nodo->padre->color = ROJO;
                    RotarDerecha(nodo->padre);
                    hermano = nodo->padre->izquierda;
                }
                if (hermano->derecha->color == NEGRO && hermano->izquierda->color == NEGRO) {
                    hermano->color = ROJO;
                    nodo = nodo->padre;
                } else {
                    if (hermano->izquierda->color == NEGRO) {
                        hermano->derecha->color = NEGRO;
                        hermano->color = ROJO;
                        RotarIzquierda(hermano);
                        hermano = nodo->padre->izquierda;
                    }
                    hermano->color = nodo->padre->color;
                    nodo->padre->color = NEGRO;
                    hermano->izquierda->color = NEGRO;
                    RotarDerecha(nodo->padre);
                    nodo = raiz;
                }
            }
        }
        nodo->color = NEGRO;
    }

    void MostrarArbol(Nodo* nodo, string indent, bool last) const {
        if (nodo != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }
            string sColor = nodo->color == ROJO ? "ROJO" : "NEGRO";
            cout << nodo->valor << "(" << sColor << ")" << endl;
            MostrarArbol(nodo->izquierda, indent, false);
            MostrarArbol(nodo->derecha, indent, true);
        }
    }

public:
    ArbolRojoNegro() : raiz(nullptr) {}

    void Insertar(int valor) {
        Nodo* nuevo = new Nodo(valor);
        InsertarNodo(raiz, nuevo);
    }

    void Eliminar(int valor) {
        EliminarNodo(raiz, valor);
    }

    void Mostrar() const {
        if (raiz == nullptr) cout << "Árbol vacío." << endl;
        else MostrarArbol(raiz, "", true);
    }

    void Buscar(int valor) const {
        Nodo* resultado = BuscarNodo(raiz, valor);
        if (resultado == nullptr) cout << "Valor " << valor << " no encontrado." << endl;
        else cout << "Valor " << valor << " encontrado." << endl;
    }
};

void Menu() {
    ArbolRojoNegro arbol;
    int opcion, valor;

    do {
        cout << "\nMenu Árbol Rojo-Negro\n";
        cout << "1. Insertar Valor\n";
        cout << "2. Mostrar Árbol\n";
        cout << "3. Buscar Valor\n";
        cout << "4. Eliminar Valor\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese un valor para insertar: ";
                cin >> valor;
                arbol.Insertar(valor);
                break;
            case 2:
                arbol.Mostrar();
                break;
            case 3:
                cout << "Ingrese un valor para buscar: ";
                cin >> valor;
                arbol.Buscar(valor);
                break;
            case 4:
                cout << "Ingrese un valor para eliminar: ";
                cin >> valor;
                arbol.Eliminar(valor);
                break;
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida, intente de nuevo.\n";
        }
    } while (opcion != 5);
}

int main() {
    Menu();
    return 0;
}
