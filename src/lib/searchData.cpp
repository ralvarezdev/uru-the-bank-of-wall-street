#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void buscarDatos(const string& datoBuscado) {
    ifstream archivo("clients.csv");
    string linea;
    int contador = 0;

    cout << "\nLos datos coinciden con la(s) cuenta(s): "<< endl;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string dato;


        
        while (getline(ss, dato, ',')) {
            if (dato == datoBuscado || linea == datoBuscado) {
                cout << "\n" << linea << endl;
                contador++;

            }
        }
    }
    cout << "\nSe han encontrado " << contador << " coincidencias." << endl;
    archivo.close();
}


int main() {
    string datoBuscado;
   
    
    cout << "Ingrese el dato que desea buscar: ";
    cin >> datoBuscado;
    
    buscarDatos(datoBuscado);

    system("pause");
    return 0;
}