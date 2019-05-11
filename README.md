## Teoría Avanzada de la Computación (TAC): Ensayo Final
### Estudio de la complejidad de AES-256 en modo CBC
Este es un fork de [Tiny AES C](https://github.com/kokke/tiny-AES-c) que carece de utilidad práctica, se ha usado para analizar la complejidad del algoritmo de cifrado/descifrado en AES con clave de 256 bits por su fácil lectura comparado con otras implementaciones libres que existen por Github.

### Modificaciones respecto al repositorio oficial
Como se menciona arriba, las modificaciones realizadas han sido únicamente con el objeto de realizar el estudio y han sido las siguientes:
* Se ha guardado el fichero *test.c* como *test.c.old*.
* Se ha creado un nuevo fichero *test.c* partiendo del original pero más simple y aplicado a nuestro caso particular. También con la funcionalidad de especificarle por parámetro el tamaño del mensaje y la generación aleatoria de la clave, el vector de inicialización (IV) y el mensaje en cuestión.
* Se ha creado un fichero *analisis.c* en el que se ha copiado el código de las funciones de AES que nos interesa analizar: KeyExpansion, AddRoundKey, SybBytes, ShiftRows y MixColumns.
* Se ha creado un fichero de bash que ejecuta el programa porque nos daba error si hacía todas las iteraciones el propio fichero *test.c*.

#### Problemas
* El registro de los tiempos para la realización del gráfico ha sido a mano porque al guardarlo directamente desde *test.c* se guardaban mal los valores (en negativo).
* Parece que el primer bloque después de cifrarlo no se descifra bien, pero el resto del mensaje sí. No hemos conseguido saber cuál es el problema pero para analizar tiempos y complejidad esto es irrelevante.