#ifndef USUARIO_H
#define USUARIO_H

class Usuario
{
protected:
    char clave[50];

public:
    Usuario(const char *clave)
    {
        strcpy(this->clave, clave);
    };
    virtual void login() = 0;
    virtual void desloguear() = 0;
};

#endif
