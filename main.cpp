#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <conio.h>
#include "Administrador.h"
#include "Almacenista.h"
#include "Vendedor.h"
using namespace std;

int main()
{
    Administrador administrador("123");
    Almacenista almacenista("123");
    Vendedor vendedor("123");

    int opcion;

    while (true)
    {
        cout << "Menu de opciones:" << endl;
        cout << "1. Iniciar sesion como administrador" << endl;
        cout << "2. Iniciar sesion como almacenista" << endl;
        cout << "3. Iniciar sesion como vendedor" << endl;
        cout << "4. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            system("cls");
            administrador.login();
            break;
        case 2:
            system("cls");
            almacenista.login();
            break;
        case 3:
            system("cls");
            vendedor.login();
            break;
        case 4:
            return 0;
        default:
            cout << "Opcion inválida." << endl;
            break;
        }
    }

    return 0;
}
