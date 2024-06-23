#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

// Ukkonen's
struct Nodo {
    map<char, Nodo*> hijos;
    int inicio;
    int fin;
    Nodo *sufijoLink;
    Nodo(int inicio, int fin) : inicio(inicio), fin(fin), sufijoLink(nullptr) {}
};

class SuffixTree {
private:
    string texto;
    Nodo *raiz;
    Nodo *nodoActivo;
    int longitudActiva;
    int bordeActiva;
    int remanente;
    int longitudTexto;

    Nodo *ultimoNodoInterno;

    int edgeLength(Nodo *n) {
        return n->fin - n->inicio + 1;
    }

    bool walkDown(Nodo *nodoActual) {
        if (longitudActiva >= edgeLength(nodoActual)) {
            bordeActiva += edgeLength(nodoActual);
            longitudActiva -= edgeLength(nodoActual);
            nodoActivo = nodoActual;
            return true;
        }
        return false;
    }

    void extendSuffixTree(int pos) {
        remanente++;
        ultimoNodoInterno = nullptr;
        int finActual = pos;

        while (remanente > 0) {
            if (longitudActiva == 0) {
                bordeActiva = pos;
            }

            if (nodoActivo->hijos.find(texto[bordeActiva]) == nodoActivo->hijos.end()) {
                nodoActivo->hijos[texto[bordeActiva]] = new Nodo(pos, longitudTexto - 1);

                if (ultimoNodoInterno != nullptr) {
                    ultimoNodoInterno->sufijoLink = nodoActivo;
                    ultimoNodoInterno = nullptr;
                }
            } else {
                Nodo *siguiente = nodoActivo->hijos[texto[bordeActiva]];
                if (walkDown(siguiente)) {
                    continue;
                }

                if (texto[siguiente->inicio + longitudActiva] == texto[pos]) {
                    if (ultimoNodoInterno != nullptr && nodoActivo != raiz) {
                        ultimoNodoInterno->sufijoLink = nodoActivo;
                        ultimoNodoInterno = nullptr;
                    }
                    longitudActiva++;
                    break;
                }

                int puntoDivision = siguiente->inicio + longitudActiva - 1;
                Nodo *division = new Nodo(siguiente->inicio, puntoDivision);
                nodoActivo->hijos[texto[bordeActiva]] = division;
                division->hijos[texto[pos]] = new Nodo(pos, longitudTexto - 1);
                siguiente->inicio += longitudActiva;
                division->hijos[texto[siguiente->inicio]] = siguiente;

                if (ultimoNodoInterno != nullptr) {
                    ultimoNodoInterno->sufijoLink = division;
                }

                ultimoNodoInterno = division;
            }

            remanente--;

            if (nodoActivo == raiz && longitudActiva > 0) {
                longitudActiva--;
                bordeActiva = pos - remanente + 1;
            } else if (nodoActivo != raiz) {
                nodoActivo = nodoActivo->sufijoLink;
            }
        }
    }

    bool buscar(Nodo *nodo, const string &patron, int &indice) {
        if (nodo == nullptr) {
            return false;
        }

        if (nodo->inicio != -1) {
            for (int i = nodo->inicio; i <= nodo->fin; ++i) {
                if (indice == patron.size()) {
                    return true;
                }
                if (texto[i] != patron[indice]) {
                    return false;
                }
                indice++;
            }
        }

        if (indice == patron.size()) {
            return true;
        }

        if (nodo->hijos.find(patron[indice]) == nodo->hijos.end()) {
            return false;
        }

        return buscar(nodo->hijos[patron[indice]], patron, indice);
    }

public:
    SuffixTree(const string &texto) : texto(texto), longitudTexto(texto.size()) {
        raiz = new Nodo(-1, -1);
        nodoActivo = raiz;
        longitudActiva = 0;
        bordeActiva = -1;
        remanente = 0;

        for (int i = 0; i < longitudTexto; ++i) {
            extendSuffixTree(i);
        }
    }

    bool buscar(const string &patron) {
        int indice = 0;
        return buscar(raiz, patron, indice);
    }
};

int main() {
    string texto = "bananas";
    SuffixTree tree(texto);

    string patron = "bana";
    if (tree.buscar(patron)) {
        cout << "El patron '" << patron << "' se encontro en el texto." << endl;
    } else {
        cout << "El patron '" << patron << "' NO se encontro en el texto." << endl;
    }

    patron = "b";
    if (tree.buscar(patron)) {
        cout << "El patron '" << patron << "' se encontro en el texto." << endl;
    } else {
        cout << "El patron '" << patron << "' NO se encontro en el texto." << endl;
    }

    return 0;
}
