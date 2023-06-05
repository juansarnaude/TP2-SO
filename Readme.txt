# TP1 Sistemas Operativos
## Integrantes
* Juan Segundo Arnaude - Legajo: 62184
* Bautista Ignacio Canevaro - Legajo: 62179
* Matias Wodtke - Legajo: 62098

## Requerimientos
En primera instancia, se debe instalar el entorno de docker, corriendo el siguiente comando en la terminal.
```bash
docker pull agodio/itba-so:1.0
```  
Luego, se requiere iniciar un contenedor de docker con esa imagen con el siguiente comando.
```bash
docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so:1.0
```
Finalmente, accedemos al directorio root del docker para usar el programa.
```bash
cd root
```
## Instrucciones de compilación
Para comenzar con la compilación, este proyecto cuenta con un Makefile que facilita este proceso. Este proyecto cuenta con dos implementaciones de memory manager, "fixed size allocation"
y "buddy", a continuación se detallan los pasos a seguir para compilar usando el primero de ellos.
```bash
cd Toolchain
make clean
make all
cd ..
make clean
make all
```

Si se quisiera usar el buddy, se deben ingresar los siguientes comandos en la terminal.
```bash
cd Toolchain
make clean
make all
cd ..
make clean
make buddy
```

Cualquiera sea el memory manager elegido, luego salimos del docker.
```bash
exit
```

## Instrucciones de ejecución
### Correr el sistema operativo McWhigginOS.
Finalmente, ejecutamos la siguiente línea para correr el programa.
```bash
./run.sh
```

## Comandos disponibles
### cat
Al ejecutarlo, replica lo introducido por el usuario mediante teclado.
```bash
cat
```

### ps
Al ejecutarlo, muestra información sobre todos los procesos que se encuentran vivos.
```bash
ps
```

### loop
Al ejecutarlo, comienza un ciclo infinito que imprime cada cierta cantidad de tiempo, un saludo al usuario.
```bash
loop
```

### filter
Al ejecutarlo, filtra las vocales de la entrada ingresada.
```bash
filter <input-to-be-filtered>
```

### block
Al ejecutarlo, bloquea o desbloquea (dependiendo del estado actual del proceso) el proceso con el pid ingresado.
```bash
block <pid>
```

### kill
Al ejecutarlo, termina el proceso con el pid ingresado.
```bash
kill <pid>
```

### nice
Al ejecutarlo, cambia la prioridad (número de prioridad entre 1 y 9 inclusives) del proceso con el pid ingresado.
```bash
nice <pid> <new-priority>
```

### wc
Al ejecutarlo, cuenta la cantidad de saltos de línea que hay en el input ingresado.
```bash
wc <input-to-be-analysed>
```

### help
Al ejecutarlo, imprime todos los comandos disponibles junto a una breve descripción de su funcionamiento.
```bash
help
```

### time
Al ejecutarlo, imprime la hora y la fecha actual (GMT-3)
```bash
time
```

### fibonacci
Al ejecutarlo, imprime los números de la serie de Fibonacci.
```bash
fibonacci
```

### phylo
Al ejecutarlo, simula el famoso problema de los filósofos, usando la tecla "a" se agrega un filósofo y usando la tecla "r" se elimina un filósofo.
```bash
phylo
```

### test_mm
Al ejecutarlo, testea el memory manager con el parámetro <max-memory> ingresado.
```bash
test_mm <max-memory>
```

### test_priority 
Al ejecutarlo, testea el sistema de prioridades de McWhigginOS.
```bash
test_priority
```

### test_processes  
Al ejecutarlo, testea el ciclo de vida de un proceso en McWhigginOS.
```bash
test_processes <max-processes>
```

### test_sync  
Al ejecutarlo con un 0 en <sem-flag>, realiza el test sin sincronización. En cambio, al ejecutarlo con un 1 en <sem-flag>, se realiza el test con sincronización.
```bash
test_sync <max-processes> <sem-flag>
```

### <COMMAND1> | <COMMAND2>
Al ejecutarlo, la salida de <COMMAND1> pasa a ser la entrada de <COMMAND2>, devolviendo a la salida el resultado de <COMMAND2> con dicho input.
```bash
<COMMAND1> | <COMMAND2>
```

### &<COMMAND1>
Al ejecutarlo, se corre <COMMAND1> en background
```bash
&<COMMAND1>
```