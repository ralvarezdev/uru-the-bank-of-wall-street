#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    ifstream archivo("clients.csv");
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string dato;

        while (getline(ss, dato, ',')) {
            cout << dato << " ";
        }

        cout << endl;
    }

    archivo.close();
    system("pause");
    return 0;
}
