#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/* Test modificado para las pruebas de la asignatura TAC (tesis sobre AES-256 CBC).
 * Simplificado a partir del original de Tiny AES: https://github.com/kokke/tiny-AES-c
 */

#include "aes.h"


int main(int argc, char const *argv[]) {

    // Declaración de variables
    uint8_t i;
    uint8_t block_size = 16;
    uint8_t key[32], iv[16];
    uint32_t length;
    
    clock_t begin, end;
    double c_time_spent, d_time_spent;

    struct AES_ctx ctx;

    // Inicializamos valores
    FILE *f = fopen("result.csv", "a");
    srand(time(NULL));  // Semilla para aleatorios

    if (argc < 2) {
        printf("Falta el parámetro\n");
        exit(1);
    }

    int m_size = strtol(argv[1], NULL, 10) * block_size;
    uint8_t message[m_size];    // Creamos array del tamaño deseado


    printf("\nAES CBC Test\n");
    printf("============\n\n");

    
    if (f == NULL) {
        printf("No se ha podido crear el fichero");
        exit(1);
    }
    
    fprintf(f, "bytes,ctime,dtime\n");

    // Generamos clave e iv aleatoriamente
    for (i = 0; i < 32; i++) {
        key[i] = rand();
        if ( i < 16)    // El tamaño de iv es de la mitad
            iv[i] = rand();
    }

    AES_init_ctx_iv(&ctx, key, iv);
    
    printf("CBC encrypt:\n\n");

    printf("key: ");
    for (i = 0; i < sizeof(key) / sizeof(uint8_t); i++)
        printf("%.2x", key[i]);
    printf("\n");

    length = sizeof(message) / sizeof(uint8_t);
    // Fill message
    for (i = 0; i < length; i++) {
        message[i] = rand();
    }

    /*
    printf("M: ");
    for (i = 0; i < sizeof(message) / sizeof(uint8_t); i++)
        printf("%.2x", message[i]);
    printf("\n");
    */
    
    begin = clock();
    AES_CBC_encrypt_buffer(&ctx, message, length);
    end = clock();
    c_time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("[ENCRYPT] - %d bytes, spent %lf of CPU time\n", m_size, c_time_spent);

    /*
    printf("C: ");
    for (j = 0; j < sizeof(message) / sizeof(uint8_t); j++)
        printf("%.2x", message[j]);
    printf("\n");
    */

    begin = clock();
    AES_CBC_decrypt_buffer(&ctx, message, length);
    end = clock();
    d_time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("[DECRYPT] - %d bytes, spent %lf of CPU time\n", m_size, d_time_spent);

    fprintf(f, "%d,%lf,%lf\n", m_size, c_time_spent, d_time_spent);
    /*
    printf("D: ");
    for (j = 0; j < sizeof(message) / sizeof(uint8_t); j++)
        printf("%.2x", message[j]);
    printf("\n");
    */
    fclose(f);
    return 0;
}
