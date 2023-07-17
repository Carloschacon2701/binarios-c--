#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H
#include "Usuario.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <conio.h>
#include <cstring>

using namespace std;

struct Producto
{
    int id;
    int id_proveedor;
    int stock;
    float precio;
    char descripcion[45];
    int stock_min;
};

struct Proveedor
{
    int id;
    char nombre[45];
    char telefono[30];
};

class Administrador : public Usuario
{

public:
    Administrador(const char *clave) : Usuario(clave) {}
    void login()
    {
        char inputClave[50];
        cout << "Ingrese la clave del administrador: ";
        cin >> inputClave;

        if (strcmp(inputClave, clave) == 0)
        {
            cout << "Inicio de sesion exitoso como administrador." << endl;
            system("cls");
            actualizarRegistro();
        }
        else
        {
            cout << "Clave incorrecta. Inicio de sesion fallido." << endl;
        }
    }

    void desloguear()
    {
        cout << "Se cerro la sesion del administrador." << endl;
    }

    void actualizarRegistro()
    {
        int opcion;
        int sesionActiva = true;
        while (sesionActiva)
        {
            cout << "Ingrese la opcion deseada:" << endl;
            cout << "1. Ingresar nuevo registro" << endl;
            cout << "2. Modificar registro existente" << endl;
            cout << "3. Eliminar registro existente" << endl;
            cout << "4. Salir" << endl;
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion)
            {
            case 1:
                system("cls");
                ingresarRegistro();
                break;
            case 2:
                system("cls");
                modificarRegistro();
                break;
            case 3:
                system("cls");
                eliminarRegistro();
                break;
            case 4:
                system("cls");
                desloguear();
                sesionActiva = false;
                break;
            default:
                cout << "Opcion invalida." << endl;
                break;
            }
        }
    }

    void ingresarRegistro()
    {
        Producto producto;
        cout << "Ingrese los datos del nuevo producto:" << endl;
        cout << "ID: ";
        cin >> producto.id;
        cout << "ID Proveedor: ";
        cin >> producto.id_proveedor;
        cout << "Stock: ";
        cin >> producto.stock;
        cout << "Precio: ";
        cin >> producto.precio;
        cout << "Descripcion: ";
        cin.ignore();
        cin.getline(producto.descripcion, 45);
        cout << "Stock minimo: ";
        cin >> producto.stock_min;

        verificarProveedor(producto.id_proveedor);

        ofstream archivo("productos.dat", ios::binary | ios::app);
        if (!archivo)
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            return;
        }

        archivo.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));
        archivo.close();

        cout << "Producto agregado correctamente." << endl;
    }

    void verificarProveedor(int idProveedor)
    {
        Proveedor proveedor;
        bool encontrado = false;

        proveedor.id = idProveedor;

        ifstream archivoProveedores("proveedores.dat", ios::binary);
        if (!archivoProveedores)
        {
            cout << "No se encontro el archivo de proveedores. Se creará uno nuevo." << endl;
            ofstream nuevoArchivoProveedores("proveedores.dat", ios::binary);
            if (!nuevoArchivoProveedores)
            {
                cout << "No se pudo crear el archivo de proveedores." << endl;
                return;
            }
            nuevoArchivoProveedores.close();
        }

        while (archivoProveedores.read(reinterpret_cast<char *>(&proveedor), sizeof(Proveedor)))
        {
            if (proveedor.id == idProveedor)
            {
                encontrado = true;
                break;
            }
        }

        archivoProveedores.close();

        if (!encontrado)
        {
            cout << "No se encontro el proveedor con el ID especificado." << endl;
            cout << "Se creará un nuevo registro del proveedor." << endl;
            cout << "Ingrese el nombre del proveedor: ";
            cin.ignore();
            cin.getline(proveedor.nombre, 50);

            cout << "Ingrese el telefono del proveedor: ";
            cin.getline(proveedor.telefono, 15);

            ofstream archivoProveedores("proveedores.dat", ios::binary | ios::app);
            if (!archivoProveedores)
            {
                cout << "No se pudo abrir el archivo de proveedores." << endl;
                return;
            }

            archivoProveedores.write(reinterpret_cast<const char *>(&proveedor), sizeof(Proveedor));
            archivoProveedores.close();

            cout << "Registro del proveedor ingresado correctamente." << endl;
        }
    }

    void modificarRegistro()
    {
        int id;
        cout << "Ingrese el ID del producto a modificar: ";
        cin >> id;

        fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
        if (!archivo)
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            return;
        }

        Producto producto;
        bool encontrado = false;

        while (!archivo.eof())
        {
            archivo.read(reinterpret_cast<char *>(&producto), sizeof(Producto));
            if (archivo.eof())
            {
                break;
            }

            if (producto.id == id)
            {
                cout << "Ingrese los nuevos datos del producto:" << endl;
                cout << "ID Proveedor: ";
                cin >> producto.id_proveedor;
                cout << "Stock: ";
                cin >> producto.stock;
                cout << "Precio: ";
                cin >> producto.precio;
                cout << "Descripcion: ";
                cin.ignore();
                cin.getline(producto.descripcion, 45);
                cout << "Stock minimo: ";
                cin >> producto.stock_min;

                archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
                archivo.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));
                encontrado = true;
                break;
            }
        }

        archivo.close();

        if (encontrado)
        {
            cout << "Registro modificado correctamente." << endl;
        }
        else
        {
            cout << "No se encontro el registro con el ID especificado." << endl;
        }
    }

    void eliminarRegistro()
    {
        int id;
        cout << "Ingrese el ID del producto a eliminar: ";
        cin >> id;

        ifstream archivoEntrada("productos.dat", ios::binary);
        if (!archivoEntrada)
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            return;
        }

        ofstream archivoTemporal("temp.dat", ios::binary);
        if (!archivoTemporal)
        {
            cout << "No se pudo crear el archivo temporal." << endl;
            archivoEntrada.close();
            return;
        }

        Producto producto;
        bool encontrado = false;

        while (!archivoEntrada.eof())
        {
            archivoEntrada.read(reinterpret_cast<char *>(&producto), sizeof(Producto));
            if (archivoEntrada.eof())
            {
                break;
            }

            if (producto.id != id)
            {
                archivoTemporal.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));
            }
            else
            {
                encontrado = true;
            }
        }

        archivoEntrada.close();
        archivoTemporal.close();

        if (encontrado)
        {
            remove("productos.dat");
            rename("temp.dat", "productos.dat");
            cout << "Registro eliminado correctamente." << endl;
        }
        else
        {
            remove("temp.dat");
            cout << "No se encontro el registro con el ID especificado." << endl;
        }
    }
};

#endif
