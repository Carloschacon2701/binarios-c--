#include "Usuario.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <conio.h>
#ifndef VENDEDOR_H
#define VENDEDOR_H

using namespace std;

struct Cliente
{
    int id;
    char nombre[45];
    char direccion[70];
    char telefono[18];
};

class Vendedor : public Usuario
{
public:
    Vendedor(const char *clave) : Usuario(clave) {}
    void login()
    {
        char inputClave[50];
        cout << "Ingrese la clave del vendedor: ";
        cin >> inputClave;

        if (strcmp(inputClave, clave) == 0)
        {
            cout << "Inicio de sesion exitoso como vendedor." << endl;
            system("cls");
            procesarVentas();
        }
        else
        {
            cout << "Clave incorrecta. Inicio de sesion fallido." << endl;
        }
    }
    void desloguear()
    {
        cout << "Se cerro la sesion del vendedor." << endl;
    }

    void procesarVentas()
    {
        int opcion;
        bool cajaAbierta = true;

        while (cajaAbierta)
        {
            cout << "Menu de opciones:" << endl;
            cout << "1. Abrir caja" << endl;
            cout << "2. Procesar compra" << endl;
            cout << "3. Cerrar caja" << endl;
            cout << "4. Salir" << endl;
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion)
            {
            case 1:
                system("cls");
                abrirCaja();
                break;
            case 2:
                system("cls");
                procesarCompra();
                break;
            case 3:
                system("cls");
                cerrarCaja(cajaAbierta);
                break;
            case 4:
                system("cls");
                desloguear();
                cajaAbierta = false;
                break;
            default:
                cout << "Opcion invalida." << endl;
                break;
            }
        }
    }

    void abrirCaja()
    {
        cout << "Se abrio la caja." << endl;
    }

    void cerrarCaja(bool &cajaAbierta)
    {
        emitirListado();
        cout << "Se cerro la caja." << endl;
        cajaAbierta = false;
    }

    void procesarCompra()
    {
        int idCliente;
        cout << "Ingrese el ID del cliente: ";
        cin >> idCliente;

        Cliente cliente;
        bool clienteExistente = false;

        ifstream archivoClientes("clientes.dat", ios::binary);
        if (!archivoClientes)
        {
            cout << "No se pudo abrir el archivo de clientes." << endl;
            return;
        }

        while (archivoClientes.read(reinterpret_cast<char *>(&cliente), sizeof(Cliente)))
        {
            if (cliente.id == idCliente)
            {
                clienteExistente = true;
                break;
            }
        }

        archivoClientes.close();

        if (!clienteExistente)
        {
            cout << "El cliente no existe. Registre los datos:" << endl;
            cliente.id = idCliente;

            cout << "Nombre: ";
            cin.ignore();
            cin.getline(cliente.nombre, 45);

            cout << "Direccion: ";
            cin.getline(cliente.direccion, 70);

            cout << "Telefono: ";
            cin.getline(cliente.telefono, 18);

            ofstream archivoClientes("clientes.dat", ios::binary | ios::app);
            if (!archivoClientes)
            {
                cout << "No se pudo abrir el archivo de clientes." << endl;
                return;
            }

            archivoClientes.write(reinterpret_cast<const char *>(&cliente), sizeof(Cliente));

            archivoClientes.close();
        }

        int idProducto;
        int cantidad;
        bool finalizarCompra = false;
        float total = 0;

        while (!finalizarCompra)
        {
            cout << "Ingrese el ID del producto: ";
            cin >> idProducto;

            if (idProducto == 0)
            {
                break;
            }

            Producto producto;
            bool productoEncontrado = false;

            fstream archivoProductos("productos.dat", ios::binary | ios::in | ios::out);
            if (!archivoProductos)
            {
                cout << "No se pudo abrir el archivo de productos." << endl;
                return;
            }

            while (archivoProductos.read(reinterpret_cast<char *>(&producto), sizeof(Producto)))
            {
                if (producto.id == idProducto)
                {
                    productoEncontrado = true;

                    cout << "Descripcion: " << producto.descripcion << endl;
                    cout << "Precio: " << producto.precio << endl;

                    cout << "Ingrese la cantidad: ";
                    cin >> cantidad;

                    if (cantidad <= producto.stock)
                    {
                        float subtotal = cantidad * producto.precio;
                        total += subtotal;

                        cout << "Producto: " << producto.descripcion << endl;
                        cout << "Precio unitario: " << producto.precio << endl;
                        cout << "Cantidad: " << cantidad << endl;
                        cout << "Subtotal: " << subtotal << endl;
                        cout << "--------------------------" << endl;

                        producto.stock -= cantidad;

                        int posicion = archivoProductos.tellg();
                        archivoProductos.seekp(posicion - static_cast<int>(sizeof(Producto)), ios::beg);
                        archivoProductos.write(reinterpret_cast<const char *>(&producto), sizeof(Producto));
                    }
                    else
                    {
                        cout << "Stock insuficiente." << endl;
                    }

                    break;
                }
            }

            archivoProductos.close();

            if (!productoEncontrado)
            {
                cout << "El producto no existe." << endl;
            }

            cout << "¿Desea finalizar la compra? (S/N): ";
            char opcion;
            cin >> opcion;

            if (opcion == 'S' || opcion == 's')
            {
                finalizarCompra = true;
            }

            cout << "--------------------------" << endl;
        }

        cout << "Total a pagar: " << total << endl;
        cout << "Compra procesada exitosamente." << endl;
    }

    void emitirListado()
    {
        ofstream archivoListado("productosReponer.txt");
        if (!archivoListado)
        {
            cout << "No se pudo crear el archivo de listado." << endl;
            return;
        }

        ifstream archivoProveedores("proveedores.dat", ios::binary);
        if (!archivoProveedores)
        {
            cout << "No se pudo abrir el archivo de proveedores." << endl;
            archivoListado.close();
            return;
        }

        Proveedor proveedor;
        archivoListado << "Listado de Proveedores:" << endl;

        while (archivoProveedores.read(reinterpret_cast<char *>(&proveedor), sizeof(Proveedor)))
        {
            archivoListado << "ID: " << proveedor.id << endl;
            archivoListado << "Nombre: " << proveedor.nombre << endl;
            archivoListado << "Telefono: " << proveedor.telefono << endl;
            archivoListado << "--------------------------" << endl;
        }

        archivoProveedores.close();

        ifstream archivoProductos("productos.dat", ios::binary);
        if (!archivoProductos)
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            archivoListado.close();
            return;
        }

        Producto producto;
        archivoListado << "Listado de Productos a Reponer:" << endl;

        while (archivoProductos.read(reinterpret_cast<char *>(&producto), sizeof(Producto)))
        {
            if (producto.stock <= producto.stock_min)
            {
                archivoListado << "ID: " << producto.id << endl;
                archivoListado << "ID Proveedor: " << producto.id_proveedor << endl;
                archivoListado << "Stock: " << producto.stock << endl;
                archivoListado << "Precio: " << producto.precio << endl;
                archivoListado << "Descripcion: " << producto.descripcion << endl;
                archivoListado << "Stock Minimo: " << producto.stock_min << endl;
                archivoListado << "--------------------------" << endl;
            }
        }

        archivoProductos.close();
        archivoListado.close();

        cout << "Listado de proveedores y productos generado correctamente." << endl;
    }
};

#endif