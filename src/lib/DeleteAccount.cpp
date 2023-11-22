#include <iostream>
#include <fstream>
#include <sstream>

struct Usuario {
    std::string ci;
    std::string nombre;
    std::string numeroCuenta;
    std::string tipoCuenta;
    bool cuentaSuspendida;
};

void eliminarUsuario(const std::string& archivoCSV, const std::string& ci) {
    std::ifstream file(archivoCSV);
    std::ofstream outfile("temp.csv");
    Usuario usuario;

    if (file.is_open() && outfile.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;

            std::getline(ss, token, ',');
            usuario.ci = token;
            std::getline(ss, token, ',');
            usuario.nombre = token;
            std::getline(ss, token, ',');
            usuario.numeroCuenta = token;
            std::getline(ss, token, ',');
            usuario.tipoCuenta = token;
            std::getline(ss, token, ',');
            usuario.cuentaSuspendida = (token == "true");

            if (usuario.ci != ci) {
                outfile << usuario.ci << "," << usuario.nombre << "," << usuario.numeroCuenta << "," << usuario.tipoCuenta << "," << (usuario.cuentaSuspendida ? "true" : "false") << std::endl;
            }
        }

        file.close();
        outfile.close();

        if (std::remove(archivoCSV.c_str()) == 0) {
            if (std::rename("temp.csv", archivoCSV.c_str()) == 0) {
                std::cout << "Usuario eliminado correctamente." << std::endl;
            } else {
                std::cout << "Error al renombrar el archivo temporal." << std::endl;
            }
        } else {
            std::cout << "Error al eliminar el archivo original." << std::endl;
        }
    } else {
        std::cout << "Error al abrir el archivo." << std::endl;
    }
}

int main() {
    std::string archivoCSV = "clients.csv";
    std::string ci;

    std::cout << "Ingrese la cédula del usuario que desea eliminar: ";
    std::cin >> ci;

    eliminarUsuario(archivoCSV, ci);
    system("pause");
    return 0;
}