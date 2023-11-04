#include <iostream>
#include <stdlib.h>

using namespace std;


int main() {
    double cantidad = 0;

    cout << "Ingrese la cantidad a transferir: ";
    cin >> cantidad;

    if (cantidad > 0) {
        cout << "Transferencia realizada de $" << cantidad << " al usuario" << endl;
    } else {
        cout << "Cantidad inválida" << endl;
    } 
    

 system("pause");
 return 0;
}