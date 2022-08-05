# COMPILADORES PRACTICA 3

Práctica probada en MacOs Monterrey 12.1. No se garantiza el funcionamiento en sistemas Windows, se esperan códigos .go con retornos de carro tipo UNIX '\n' y no tipo Windows '\r'.

Varios módulos siguen el patrón singleton. Es responsabilidad de quien los use llamar a las funciones que exportan en orden adecuado (inicializar, acciones, cerrar).

## Como Ejecutar el Interprete

1. La primera vez debes compilar el interprete : 
    * Puedes hacerlo ejecutando el comando make en la carpeta del proyecto
    * Esto generará el ejecutable "interprete"

2. Para utilizar el interprete debes ejecutar "./interprete"
    * Esto abrirá el interprete que leerá de la entrada estándar
    * Alternativamente  el interprete puede recibir varios ficheros comoe entrada que se procesarán en el orden en que los recibe: "./interprete file1.jp file2.jp"
   

## Relatorio de Ficheros
Como ya habrás hay varios archivos en la carpeta del proyecto. Puedes ver aquí que hay en cada uno sin tener que abrir todos los archivos.

1. **abb.c/.h** 
    * Librería que proporciona un árbol binario de búsqueda a la tabla de símbolos
    * De la materia Algoritmos y Estructuras de Datos con algunas funciones extra:

1. **ejemplo.jp** 
    * Fichero de ejemplo para ilustrar la carga de ficheros en el programa
    * El propio fichero llama a ejemplo2.jp

1. **ejemplo2.jp** 
    * Fichero de ejemplo para ilustrar la carga de ficheros en el programa

5. **errores.c/h**
    * Módulo simple para notificiación de errores

5. **estandar.c/h**
    * Módulo que exporta las funciones y comandos básicos del intérprete

5. **gramatica.tab.c/h**
    * Analizador sintáctico generado con bison a partir de gramatica.y.
    * Usamos la flag "-d" de bison para generar el header file (Comando "bison -d gramatica.y").

6. **gramatica.y**
    * "Código" bison para generar el analizador sintáctico
    * Si lo modificas deberás ejecutar el comando ""bison -d gramatica.y") y make para actualizar el interprete.

6. **lexico.l**
    * "Código" flex para generar el analizador léxico
    * Si lo modificas deberás ejecutar el comando "flex lexico.l" y make para actualizar el interprete.

7. **lex.yy.c/h**
    * Código del analizar léxico generado con flex 

8. **main.c**
    * Módulo principal
    * Inicializa y cierra los módulos y pone en marcha el análisis.

9. **plugin_financiero.c**
    * Ejemplo de plugin externo que se puede cargar dinamicamente en el interprete
    * Comiplar con la linea: "gcc -shared -o plugin_financiero.so -fPIC plugin_financiero.c"

9. **plugin_medida.c**
    * Ejemplo de plugin externo que se puede cargar dinamicamente en el interprete
    * Comiplar con la linea: "gcc -shared -o plugin_medida.so -fPIC plugin_medida.c"

9. **makefile**
    * Soporta los comandos make, clean y cleanall

10. **README.md**
    * Documentación

11. **tabla_simbolos.c/.h**
    * Tabla de símbolos (patrón singleton)
    * Los demás módulos interactúan con esta librería que les "oculta" los detalles de implementación de la tabla (en este caso abb.c/.h)

1. **workspace.jp** 
    * Fichero generado por el interprete cuando el usuario sale salvando el workspace
    * Para restaurar el workspace ejecuta "./interprete workspace.jp" o simplemente escribe "load :workspace.jp" dentro del intérprete

## Funcionalidad
- Para terminar el programa escriba "quit"
- Para ver las variables del espacio de trabajo escriba "workspace"
- Para limpiar el espacio de trabajo escriba "reset"
- Para cargar un fichero de instrucciones escriba "load :filename", el fichero debe terminar en \n
- Para importar un modulo externo escriba "import_mod :path_modulo :nombre_modulo"
        - Usted puede dar al modulo externo el nombre que quiera
        - Para ejecutar funciones del modulo escriba "nombre_modulo.funname(expresion)"
- Para importar una funcion de un modulo externo escriba "import_mod :path_modulo :nombre_funcion"
        - Podra ejecutar la funcion importada con la sintaxis habitual: "funname(expresion)"
- Usted puede realizar operaciones aritmeticas habituales
- Dispone de un amplio surtido de funciones matematicas.
        - Para invocar una funcion escriba "funname(expresion)".
- Las constantes PI y E estan disponibles en la calculadora
- Escriba ";" al final de una cuenta para que sea silenciosa
- Puede dejar mensajes o comentarios escribiendo "#comentario"
- Para recibir ayuda escriba "help"

## Trabajo posterior
- Pulir la notificación de errores para que estos sean más explicativos
- Mejorar la gestión de múltiples argumentos en comandos y funciones
- Soportar funciones de más de un argumento
- Definir en todos los módulos externos una función help() propia e implementar en el intérprete un comando "help_mod :nombre_modulo"
- Definir en todos los módulos externos una función get_nargs(void* funcion) que devuelva el número de argumentos de cada función que exporta el módulo.