#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum Opcion
{
    Depositar,
    Retirar,
    Transferir,
    Salir
};

string usuario;

void depositar(double cantidad)
{
    cout << "Deposito realizado de $" << cantidad << "." << endl;
}

void retirar(double cantidad)
{
    cout << "Retiro realizado de $" << cantidad << "." << endl;
}

void buscarDatos(const string &datoBuscado)
{
    ifstream archivo("clients.csv");
    string linea;

    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        string dato;

        while (getline(ss, dato, ','))
        {
            if (dato == datoBuscado)
            {
                stringstream ss(linea);
                string dato;
                getline(ss, dato, ',');
                getline(ss, dato, ',');
                cout << "\n Bienvenido, " << dato << endl;
                usuario = datoBuscado;
            }
        }
    }

    archivo.close();
}

void transferir(const string &datoBuscado)
{
    ifstream archivo("clients.csv");
    string linea;
    int opcion; // added semicolon here

    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        string dato;

        while (getline(ss, dato, ','))
        {
            if (dato == datoBuscado || linea == datoBuscado)
            {
                if (datoBuscado != usuario)
                {
                    stringstream ss(linea);
                    string dato;

                    getline(ss, dato, ',');
                    getline(ss, dato, ',');

                    cout << "\nEsta seguro de transferir a " << dato << "?" << endl;
                    cout << "\n1. Si" << endl
                         << "2. No" << endl;
                    cin >> opcion;

                    if (opcion == 1)
                    {
                        cout << "Ingrese la cantidad a transferir: ";
                        double cantidadTransferir;
                        cin >> cantidadTransferir;
                        if (cantidadTransferir > 0)
                        {
                            cout << "Transferencia realizada de $" << cantidadTransferir << " a " << dato << endl;
                        }
                        else
                        {
                            cout << "Transferencia inválida" << endl;
                        }
                    }
                    else if (opcion == 2)
                    {
                        cout << "Transferencia cancelada" << endl;
                    }
                }
                else
                {
                    cout << "\nNo puedes transferir a ti mismo" << endl;
                }
            }
        }
    }

    archivo.close();
}

int main()
{
    string datoBuscado;
    int Opcion;

    cout << "Ingrese la cedula del titular: ";
    cin >> datoBuscado;

    buscarDatos(datoBuscado);

    cout << "\nIndique la operacion que desea realizar " << endl;
    cout << "\n1. Depositos" << endl;
    cout << "2. Retiros" << endl;
    cout << "3. Tranferencias" << endl;
    cout << "4. Salir" << endl;
    cin >> Opcion;

    switch (Opcion)
    {
    case 1:
        cout << "Ingrese la cantidad a depositar: ";
        double cantidadDepositar;
        cin >> cantidadDepositar;
        depositar(cantidadDepositar);
        break;
    case 2:
        cout << "Ingrese la cantidad a retirar: ";
        double cantidadRetirar;
        cin >> cantidadRetirar;
        retirar(cantidadRetirar);
        break;
    case 3:

        cout << "Ingrese la cedula del destinatario: ";
        cin >> datoBuscado;
        transferir(datoBuscado);

        break;
    case 4:
        cout << "Saliendo..." << endl;
        break;
    }

    system("pause");
    return 0;
}
