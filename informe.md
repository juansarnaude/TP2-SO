
- [Userland](#userland)
- [Interrupciones](#interrupciones)
  - [Handler](#handler)
  - [Timer Tick](#timer-tick)
  - [Keyboard](#keyboard)
- [Syscalls](#syscalls)
  - [Handler](#handler-1)
  - [0 - Read](#0---read)
  - [1 - Write](#1---write)
  - [2 - Get registers](#2---get-registers)
  - [3 - Exec](#3---exec)
  - [4 - Exit](#4---exit)
  - [5 - Time](#5---time)
- [Excepciones](#excepciones)
- [Scheduler](#scheduler)
  - [Estructura](#estructura)
  - [Cargado de Tareas](#cargado-de-tareas)

## Userland

Decidimos dejar la dirección de Userland en la dirección en la que vino, es decir, `0x400000`, pero `RSP` lo seteamos a `0x800000`, asi dejamos 2MB para codigo de Userland, y luego las funciones del Scheduler empiezan en `0x600000`, y asi cada una tiene 1MB y Userland puede crecer en memoria todo lo que quiera.

## Interrupciones

Apenas comienza el main del kernel, se setea una IDT de 256 entradas en la posición 0, hasta la posición 4096, con las siguientes entradas:

Primero se desactivan los interrupts, y despues en la posición `0x00`, que es la llamada a la excepción por dividir con 0, se pone su handler, y lo mismo con `0x06`, que es la excepción por invalid opcode. En `0x20` y `0x21`, las interrupciones de timer tick y de teclado, se setean sus handlers. Y por ultimo decidimos utilizar el numero `0x80` para las syscalls, imitando a Linux, por lo que allí pusimos el puntero al handler de las syscalls.

Despues de setear la IDT, cambiamos las mascaras del Master PIC para que atienda las interrupciones por timer tick y keyboard, y despues habilitamos las interrupciones en el procesador.

### Handler

El handler es el mismo para todas las interrupciones: Pushea todos los registros generales (`RAX` hasta `R15`), y pone en `RDI` el valor de la interrupción, en `RSI` un backup a los registros, y luego llama a un dispatcher que se encarga de llamar a sus respectivos handlers.

Cuando el dispatcher termina su trabajo, el handler le indica al PIC el end of interrupt, restaura los registros y vuelve al lugar donde fue llamado.

### Timer Tick

El handler de timer tick recive solo el puntero a los backups de los registros, y simplemente realiza dos tareas: Aumenta en uno el contador de ticks, y llama a `nextTask` y le pasa el pointer para que este pueda modificarlos en caso de ser necesario.

### Keyboard

Cuando se realiza una interrupción de keyboard, se llama a la función `keyboard_handler`, que se encarga de fijarse si se apreto la tecla, si es una letra con un caracter ASCII la agrega al buffer, y en caso contrario no hace nada.

***

## Syscalls

Decidimos realizar syscalls de solo cuatro argumentos por simplicidad, dado que las que implementamos no necesitaban nada más.

### Handler

Al realizar una syscall, el `syscallHandler` pushea al stack todos los registros generales, y ya al momento de llamar a una interrupt con `int` quedan puestos los registros `RIP`, `CS`, `RFLAGS` y `RSP` en ese orden. Luego, movemos `RAX`, el registro donde esta la syscall a desarrollar, a `RCX` y seteamos `R8` por el lugar en espacio donde quedo pusheado el ultimo registro.

Esto lo hacemos especialmente para la syscall `exec` que utiliza un scheduler para "paralelizar" dos procesos, y la syscall `exit`, que es usada por el scheduler para saber cuando terminan los procesos que esta corriendo, y tambien directamente pasa a la proxima tarea, por lo que necesita modificar el contexto de la tarea actual.

Por simplicidad a la hora de modificar los registros en memoria, tambien pusheamos `RAX`, pese a ser un registro que no podemos restaurar pues al salir de una syscall toma el valor de retorno de esta.

En caso de que el numero de syscall no sea valido, simplemente retorna.

### 0 - Read

La syscall `read` decidimos hacerla igual a la de Linux, por lo que recibe tres parametros, un file descriptor, un pointer a un buffer donde va a guardar las cosas, y un limite de cuanto debe leer.

En nuestro caso, lo unico que puede ser leido es el buffer de teclado, mediante el file descriptor 0, el uso de cualquier otro hace que la syscall inmediatamente termine.

### 1 - Write

Recibe los mismos argumentos que la de Linux, es decir, un file descriptor, un pointer al buffer, y la cantidad a leer. En este caso, solo se aceptan dos file descriptors, que lo unico que cambian es el color:

1. **STDOUT**: Imprime a la pantalla en color blanco con fondo negro.
2. **STDERR**: Imprime a la pantalla en color rojo con fondo negro.

Al momento de hacer el driver de video, que permite que la consola se divida en dos, debatimos darles file descriptors separados a cada ventana, pero como hicimos un scheduler en el kernel, decidimos que tenia mas sentido que el kernel tomara la decisión de cuando se debe dividir la ventana, y donde tiene que escribir cada programa, pues de esta forma no pueden escribir a la ventana de otro.

### 2 - Get registers

Esta es la que decidimos hacer para obtener un screenshot de los registros en determinado momento. Decidimos que resiva como argumento un puntero a una posición donde deben entrar los 18 registros de 64 bits que se copian, asi el usuario no tiene acceso al kernel space en memoria, y que retorne 1 si se copiaron los registros y 0 si no habia registros para copiar.

Para hacer posible que se tome una imagen en cualquier momento, decidimos utilizar la combinacion de teclas Ctrl + C. Si no se apretaron en algun momento antes de llamar a get registers, el return va a ser 0.

### 3 - Exec

La idea de esta syscall es poder simular la paralelización de dos funciones al mismo tiempo, de forma que el usuario no note el hecho de que realmente no estan corriendo al mismo tiempo.

De esta forma, `exec` recibe tres parametros, dos que son seteados por el llamador de la interrupción y otro que agrega el syscall handler que es el pointer a los registros en memoria.

Las funciones que recibe deben ser del tipo `int (*)(void)`, pues decidimos que la complejidad de implementar argumenos era innecesaria teniendo en cuenta que se puede utilizar una variable global para simularlo.

### 4 - Exit

Esta syscall realmente solo sirve para el Scheduler, pues es usada para que cuando termine uno de los programas en el scheduler, este pueda saber que termino y dejar de alocarle tiempo. Recive el valor de retorno de la función, lo imprime en pantalla, y setea un flag para que el scheduler sepa que termino.

Si se la llama cuando el scheduler no esta funcionando, no hace nada, y como cuando esta funcionando la unica forma en que puede ser llamada es que una de las funciones halla terminado, no se hacen más checkeos.

### 5 - Time

No recibe argumentos, simplemente imprime a pantalla el tiempor en formato DD/MM/AAAA, y luego en renglones separados la hora, los minutos y los segundos.

***

## Excepciones

Para las excepciones, para poder imprimir los registros decidimos desde assembler pasarle un puntero a donde se pushearon todos los registros apenas empezo la excepción, y un puntero a los valores de RIP y RSP que fueron guardados en el stack cuando se realizo la interrupción, luego se imprime el tipo de error y todos los registros en color rojo.

Luego de imprimirse los registros, entra en un loop llamado `haltcpu` que setea los interrupts y luego la instrucción `hlt`. De esta forma, nos aseguramos que cuando una excepción ocurre durante el scheduler, la otra función pueda seguir funcionando.

***

## Scheduler

Luego de mirar al mapa de memoria de Pure64, decidimos que las variables "paralelizadas" empiecen su stack en la posición `0x700000`, y a cada una le reservamos 1MB de memoria en el stack.

Tambien tenemos un valor inicial default para el registro `RFLAGS` de cada tarea, `0x202`, ya que necesitamos que las interrupciones esten activadas, e Intel requiere que el bit uno este activado.

`Current` indica el indice de la tarea corriendo en el momento, y cuando no esta corriendo el Scheduler vale -1 para indicar que este recien comienza y asi se guarda el contexto del origen en vez de en una de las funciones.

`Amount` indica la cantidad de tareas corriendo, asi el Scheduler sabe cuando debe volver al contexto original. Si vale -1, entonces recien se terminaron las dos tareas mediante el uso de Ctrl+Esc.

### Estructura

En el caso del scheduler decidimos que solo se use para simular la paralelización de dos funciones, mediante el uso de una estructura que utilizamos para guardar el contexto de la función que lo llamo en `origin` y el de las funciones corriendo en el vector `tasks`.

Tiene no solo el espacio para los registros, si no que tambien guarda unas variables utilizadas para el control de las tasks:

- `buRIP`: Es un backup del registro `RIP` usado cuando se pausa una de las funciones.
- `window`: Lleva control de en que ventana debe escribir cada tarea.
- `active`: Si esta seteada en 1, entonces la tarea esta activa, si no, esta pausada.
- `finished`: Cuando una tarea termina mediante la syscall exit, se setea en 1 para indicarle al Scheduler que la función termino.

### Cargado de Tareas

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
