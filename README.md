# Mixnet Library

## Estructura del Directorio

```plaintext
MixnetSO/
├── include/
│ └── mixnet.h
| └── json.hpp
├── lib/
│ └── libmixnet.so
├── example/
│ └── main.cpp
└── README.md


## Instrucciones de Uso

### Requisitos

- GCC (o cualquier compilador de C++ compatible)
- OpenSSL

### Compilar un Programa con la Biblioteca

1. **Copiar la Biblioteca y Archivos de Cabecera**

   Copia el directorio `MixnetSO` a tu proyecto o asegúrate de que la estructura de directorios esté accesible.

2. **Compilar y Enlazar**

   Asegúrate de que la biblioteca y los archivos de cabecera estén en las rutas correctas. Utiliza el siguiente comando para compilar tu programa:

   ```sh
   g++ -o main example/main.cpp -I./include -L./lib -lmixnet -lssl -lcrypto -std=c++17


2. **Ejecutar el programa**

   Asegúrate de que el sistema pueda encontrar la biblioteca compartida. Puedes hacerlo configurando la variable de entorno LD_LIBRARY_PATH:

   ```sh
   LD_LIBRARY_PATH=./lib ./main <numero_de_nodos> <ruta_votos> <ruta_cifrados> <ruta_descifrados>

