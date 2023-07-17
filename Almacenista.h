#include "Usuario.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <conio.h>
#include <cstring>
#ifndef ALMACENISTA_H
#define ALMACENISTA_H

using namespace std;

class Almacenista : public Usuario
{
public:
    Almacenista(const char *clave) : Usuario(clave) {}
    void login()
    {
        char inputClave[50];
        cout << "Ingrese la clave del personal de almacen: ";
        cin >> inputClave;

        if (strcmp(inputClave, clave) == 0)
        {
            cout << "Inicio de sesion exitoso como personal de almacen." << endl;
            system("cls");
            actualizarArchivoProductos();
        }
        else
        {
            cout << "Clave incorrecta. Inicio de sesion fallido." << endl;
        }
    }

    void desloguear()
    {
        cout << "Se cerro la sesion del personal de almacen." << endl;
    }

    void actualizarArchivoProductos()
    {
        char nombreArchivo[100];
        cout << "Ingrese el nombre del archivo DAT para actualizar los productos: ";
        cin.ignore();
        cin.getline(nombreArchivo, 100);

        fstream archivoProductos(nombreArchivo, ios::binary | ios::in | ios::out);
        if (!archivoProductos)
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            return;
        }

        int sesionActiva = true;
        int opcion;

        while (sesionActiva)
        {
            cout << "Ingrese la opcion deseada:" << endl;
            cout << "1. Modificar un producto existente" << endl;
            cout << "2. Agregar un nuevo producto" << endl;
            cout << "3. Salir" << endl;
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion)
            {
            case 1:
                system("cls");
                modificarProducto(archivoProductos);
                break;
            case 2:
                system("cls");
                agregarProducto(archivoProductos);
                break;
            case 3:
                system("cls");
                cout << "Se cerro la sesion del personal de almacen." << endl;
                sesionActiva = false;
                break;
            default:
                cout << "Opcion invalida." << endl;
                break;
            }
        }

        archivoProductos.close();
    }

    void modificarProducto(fstream &archivoProductos)
    {
        int id;
        cout << "Ingrese el ID del producto a modificar: ";
        cin >> id;

        Producto producto;
        bool encontrado = false;

        while (archivoProductos.read(reinterpret_cast<char *>(&producto), sizeof(Producto)))
        {
            if (producto.id == id)
            {
                encontrado = true;

                cout << "Datos del producto:" << endl;
                cout << "ID: " << producto.id << endl;
                cout << "ID Proveedor: " << producto.id_proveedor << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "Precio: " << producto.precio << endl;
                cout << "Descripcion: " << producto.descripcion << endl;
                cout << "Stock minimo: " << producto.stock_min << endl;

                cout << "Ingrese los nuevos datos del producto:" << endl;
                cout << "Stock: ";
                cin >> producto.stock;
                cout << "Precio: ";
                cin >> producto.precio;
                cout << "Descripcion: ";
                cin.ignore();
                cin.getline(producto.descripcion, 45);

                archivoProductos.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
                archivoProductos.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));

                cout << "Producto modificado correctamente." << endl;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "No se encontro el producto con el ID especificado." << endl;
        }
    }

    void agregarProducto(fstream &archivoProductos)
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

        archivoProductos.seekp(0, ios::end);
        archivoProductos.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));

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
};

#endif