# Listado de las Funciones más Utiles

| Función      | Librería     | Descripción                                                            | Ejemplo de uso                         |
| ------------ | ------------ | ---------------------------------------------------------------------- | -------------------------------------- |
| `strlen()`   | `<string.h>` | Retorna la longitud de la cadena (sin contar el `\0`)                  | `int len = strlen(buffer);`            |
| `strcpy()`   | `<string.h>` | Copia una cadena a otra (incluye el `\0`)                              | `strcpy(dest, src);`                   |
| `strncpy()`  | `<string.h>` | Copia hasta `n` caracteres, útil para evitar desbordes                 | `strncpy(dest, src, 10);`              |
| `strcat()`   | `<string.h>` | Concatena (añade) una cadena al final de otra                          | `strcat(buffer, "OK");`                |
| `strncat()`  | `<string.h>` | Concatena hasta `n` caracteres                                         | `strncat(buffer, input, 5);`           |
| `strcmp()`   | `<string.h>` | Compara dos cadenas (retorna 0 si son iguales)                         | `strcmp(buffer, "ON") == 0`            |
| `strncmp()`  | `<string.h>` | Compara hasta `n` caracteres de dos cadenas                            | `strncmp(buffer, "CMD", 3) == 0`       |
| `memset()`   | `<string.h>` | Rellena memoria con un valor (útil para limpiar el buffer)             | `memset(buffer, 0, sizeof(buffer));`   |
| `memcpy()`   | `<string.h>` | Copia datos binarios (sin depender de `\0`)                            | `memcpy(dest, src, length);`           |
| `strchr()`   | `<string.h>` | Busca un carácter en una cadena y retorna un puntero                   | `char *p = strchr(buffer, '\n');`      |
| `strtok()`   | `<string.h>` | Divide una cadena en tokens (por separadores como `,` o `\n`)          | `char *tok = strtok(buffer, ",");`     |
| `sprintf()`  | `<stdio.h>`  | Formatea texto en una cadena (como `printf` pero a un `char[]`)        | `sprintf(buffer, "Temp: %.2f", temp);` |
| `snprintf()` | `<stdio.h>`  | Igual que `sprintf` pero limita el número de caracteres para seguridad | `snprintf(buffer, 32, "ID: %d", id);`  |
| `atoi()`     | `<stdlib.h>` | Convierte una cadena a `int`                                           | `int valor = atoi(buffer);`            |
| `atof()`     | `<stdlib.h>` | Convierte una cadena a `float`                                         | `float temp = atof(buffer);`           |
