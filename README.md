# Mixnet Library

## Estructura del Directorio

```plaintext
MixnetSO/
├── example/
│ └── main.cpp
├── generadorVotos/
│ └── generador_votos.py
├── include/
│ └── json.hpp
│ └── mixnet.h
├── lib/
│ └── libmixnet.so
└── README.md
```

## Instrucciones de Uso

### Requisitos

- GCC (o cualquier compilador de C++ compatible)
- OpenSSL
- Python 3.10.*

### Compilar un Programa con la Biblioteca

0. **Generar votos ficticios**

    Ejecuta el script de python que se encuentra en el directorio "generadorVotos" de la siguiente manera:

    ```sh
    python3 generador_votos.py -n <número_de_votos> -o <ruta_votos>
    ```
    Por ejemplo:

    ```sh
    python3 generador_votos.py -n 1000 -o /home/usuario/votos
    ```
    Donde `n` representa el número de votos a generar y `o` (output) representa la ruta al directorio donde se desean guardar los votos.

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

