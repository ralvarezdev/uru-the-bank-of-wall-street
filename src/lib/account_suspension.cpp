#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void suspenderCuentas(const std::string& fileName) {
    ifstream file(fileName);
    string line;

    // Guardar la primera línea
    getline(file, line);
    string primeraLinea = line;

    // Pedir el número de cuenta al usuario
    string numeroCuenta;
    cout << "Ingrese el número de cuenta: ";
    cin >> numeroCuenta;

    // Pedir la acción a realizar
    int accion;
    cout << "Seleccione la accion a realizar (1: activar / 2: suspender): ";
    cin >> accion;

    // Iterar sobre cada línea del archivo
    vector<string> lines;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> cells;

        // Dividir la línea en celdas usando la coma como delimitador
        while (getline(ss, cell, ',')) {
            cells.push_back(cell);
        }

        // Verificar si el número de cuenta coincide
        if (cells.size() >= 3 && cells[2] == numeroCuenta) {
            // Realizar la suspensión o activación de la cuenta según la acción seleccionada
            switch (accion) {
                case 1:
                    if (cells.size() >= 5 && cells[4] == "true") {
                        cells[4] = "false";
                        cout << "Cuenta " << numeroCuenta << " activada." << endl;
                    } else if (cells.size() >= 5 && cells[4] == "false") {
                        cout << "La cuenta " << numeroCuenta << " ya está activada." << endl;
                    } else {
                        cout << "Error: El estado de la cuenta es inválido." << endl;
                    }
                    break;
                case 2:
                    if (cells.size() >= 5 && cells[4] == "false") {
                        cells[4] = "true";
                        cout << "Cuenta " << numeroCuenta << " suspendida." << endl;
                    } else if (cells.size() >= 5 && cells[4] == "true") {
                        cout << "La cuenta " << numeroCuenta << " ya está suspendida." << endl;
                    } else {
                        cout << "Error: El estado de la cuenta es inválido." << endl;
                    }
                    break;
                default:
                    cout << "Error: Acción inválida." << endl;
                    break;
            }
        }

        // Reconstruir la línea con las celdas actualizadas
        stringstream rebuiltLine;
        for (const string& cell : cells) {
            rebuiltLine << cell << ",";
        }
        line = rebuiltLine.str().substr(0, rebuiltLine.str().size() - 1);
        lines.push_back(line);
    }

    file.close();

    // Agregar la primera línea al vector lines
    lines.insert(lines.begin(), primeraLinea);

    // Guardar los cambios en el archivo
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (const string& line : lines) {
            outFile << line << endl;
        }
        outFile.close();
    } else {
        cout << "Error: No se pudo abrir el archivo para guardar los cambios." << endl;
    }
}

int main() {
     suspenderCuentas("clients.csv");

    system("pause");
    return 0;

}
   