#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void buscarDatos(const string& datoBuscado) {
    ifstream archivo("clients.csv");
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string dato;


        
        while (getline(ss, dato, ',')) {
            if (dato == datoBuscado || linea == datoBuscado) {
                stringstream ss(linea);      
                string dato;        
                getline(ss, dato, ',');    
                getline(ss, dato, ','); 
                cout << "\n El titular de la cuenta es " << dato << endl;       
            }
        }
    }
  
    archivo.close();
}


int main() {
    string datoBuscado;
   
    
    cout << "Ingrese la cedula del titular: ";
    cin >> datoBuscado;
    
    buscarDatos(datoBuscado);

    system("pause");
    return 0;
}