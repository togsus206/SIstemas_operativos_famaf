# Titulo 

MyBash

## Grupo 28

Integrantes:
* Agustin Capogrossi (agustin.capogrossi@mi.unc.edu.ar)
* Lisandro Martinez (matias.valle@mi.unc.edu.ar)
* Matias Valle (ligabriel01.lm@gmail.com)

# Compilación y ejecución

1) Dentro del directorio "Skeleton" abrir una terminal y usar el comando "make", el cual contiene todas las reglas para compilar el programa.

2) Para ejecutar "mybash", usar el comando "./mybash".

3)Ejemplos rápidos:
*ls -l | wc -l
*cd tests
*ls > out.txt

# Decisiones de diseño

* En el archivo execute.c decidimos modularizar el funcion execute_pipeline(), de manera tal que si el largo del pipeline es 2, ejecutamos la funcion externa execute_double_pipeline(), que se dedica a ejecutar este pipiline de 2 comando, y si es un unico comando seguimos sobre la ejecucion implementada en la misma funcion. A su vez, para trabajar sobre pipelines decidimos crear un nuevo scommand en command.c que devuelve la segunda y ultima pipeline, su prototipo fue agregado en command.h para evitar errores.
* Originalmente planeamos implementar el código para n pipes, pero finalmente debido a su complejidad preferimos limitar la cantidad a dos, agregamos los assert y guardas pertinentes para evitar este caso.
* Para revisar los elementos del pipe optamos por guardar cada elemento del scommand, salvo el primero, en un arreglo, para ello agregamos dos guardas, una que revisa si el scommand tiene argumentos, en cuyo caso los almacena en el arreglo y en caso contrario solo ejecuta el scommand; y otra guarda que revisa si la cantidad de argumentos es mayor a dos, cuando esto pasa tiene que realizar una busqueda un poco mas prolongada para agregar todos los argumentos de manera tal que se pueda ejecutar execvp. 


## Puntos estrella

3) Impresion en pantalla con el nombre del Host y ruta de direcciones: 
     
     Para la implementacion de este punto usamos la funcion "getlogin()" que retorna el nombre del usuario que se loguio. Luego declaramos dos variables user y hostname, y reservamos espacio en memoria con calloc para la variable hostname. Finalmente usamos la funcion "gethostname()" para poner en la variable hostname el nombre del usuario, para mas tarde poder imprimirlo en el prompt.
