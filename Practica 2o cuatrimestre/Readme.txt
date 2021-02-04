Dada una pared de longitud l y un número de cuadros con dimensiones altoXancho y un valor, el algoritmo encuentra, utilizando cotas de distintos tipos, la combinación de cuadros que cubrirán la pared por completo (sin pasarse ni dejar un hueco) que aporten mayor valor al conjunto.

Los datos se introducirán a tavés de un archivos "datos.txt" con formato:

Una primera línea con dos enteros, el primero (N) indicará el número de cuadros que estudiaremos a continuación; el segundo indicará el tamaño de la pared que debemos rellenar.
A continuación, escribiremos N líneas con la información de cada uno de los cuadros siguiente el siguiente orden:
1.	Entero que indica una de las dimensiones del cuadro (el ancho si se coloca en vertical).
2.	Entero que indica la otra dimensión del cuadro (el ancho si se coloca en horizontal).
3.	Un número real que indica el prestigio que aportará ese cuadro a nuestra pared.

Los resultados se mostrarán en consola y se guardarán en un archivo llamado "resultados.txt" de la siguiente forma:

Se guardarán tres bloques de soluciones: en el primero se mostrarán los resultados cuando solo se usa factible como cota, en los otros dos se mostrarán los resultados cuando se utilice una cota optimista. El primero de ellos utilizará una cota más ingenua, mientras que el segundo usará una cota mucho mejor.
Cada bloque seguirá el siguiente formato, por líneas:
1.	Se indica la poda utilizada.
2.	Se indica la solución con los datos otorgados. Para cada cuadro (y en el orden en que se introdujo su información), aparecerá un 1 cuando el cuadro se coloque y 0 cuando no. A continuación, y separado por un guión, pueden aparecer las letras V (si el cuadro se coloca en vertical), H (si se coloca en horizontal) o N (si no se coloca).
3.	El prestigio conseguido con esa solución, que será el mayor de los posibles con los datos proporcionados.
4.	El número de nodos explorados.
5.	El tiempo que ha tardado el algoritmo en ejecutarse y el número de cuadros de que disponíamos.
6.	El tiempo que ha tardado el algoritmo en función de los nodos explorados.
