# Informe TPE Arquitectura de las Computadoras

## Syscalls



## Excepciones

Para las excepciones, para poder imprimir los registros decidimos desde assembler pasarle un puntero a donde se pushearon todos los registros apenas empezo la excepción, y un puntero a los valores de RIP y RSP que fueron guardados en el stack cuando se realizo la interrupción, luego se imprime el tipo de error y todos los registros en color rojo.

## Interrupciones

### Scheduler

En este caso, dado que el Scheduler se maneja por interrupciones, y tiene que terminar la interrupción antes de poder correr la siguiente tarea, decidimos que cuando se realizan interrupciones tambien se pase la dirección de memoria donde pusheamos todos los registros, entonces la memoria queda:

| Stack  | Posicion |
|--------|----------|
| R15    | RSP      |
| RAX    | RSP+14*8 |
| RIP    | RSP+15*8 |
| CS     | RSP+16*8 |
| RFLAGS | RSP+17*8 |
| RSP    | RSP+18*8 |

De esta forma, al pasar a la proxima tarea solo tenemos que acceder al puntero a R15 (que es el ultimo pusheado) para tratar a todos los registros como un array. Ponemos todos los valores de registros, RIP, RSP y RFLAGS de la tarea actual en una estructura auxiliar, y luego modificamos esas direcciones de memoria por los valores de los registros de la nueva tarea, asi cuando termina la interrupción se vuelven a cargar y vuelve al lugar donde se quedo en la nueva tarea a la que paso.
