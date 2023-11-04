#include <iostream>

using namespace std;

enum class Opcion {
    Depositar,
    Retirar
};

void depositar(double cantidad) {
    cout << "Depósito realizado de $" << cantidad << "." << endl;
}

void retirar(double cantidad) {
    cout << "Retiro realizado de $" << cantidad << "." << endl;
}

int main() {
    Opcion opcion;

    cout << "Seleccione una opción: (0) Depositar, (1) Retirar" << endl;
    int opcionInt;
    cin >> opcionInt;
    opcion = static_cast<Opcion>(opcionInt);

    switch (opcion) {
        case Opcion::Depositar:
            cout << "Ingrese la cantidad a depositar: ";
            double cantidadDepositar;
            cin >> cantidadDepositar;
            depositar(cantidadDepositar);
            break;
        case Opcion::Retirar:
            cout << "Ingrese la cantidad a retirar: ";
            double cantidadRetirar;
            cin >> cantidadRetirar;
            retirar(cantidadRetirar);
            break;
        default:
            cout << "Opción inválida." << endl;
            break;
    }

    return 0;
}