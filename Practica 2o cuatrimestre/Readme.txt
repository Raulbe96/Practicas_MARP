Dada una pared de longitud l y un n�mero de cuadros con dimensiones altoXancho y un valor, el algoritmo encuentra, utilizando cotas de distintos tipos, la combinaci�n de cuadros que cubrir�n la pared por completo (sin pasarse ni dejar un hueco) que aporten mayor valor al conjunto.

Los datos se introducir�n a tav�s de un archivos "datos.txt" con formato:

Una primera l�nea con dos enteros, el primero (N) indicar� el n�mero de cuadros que estudiaremos a continuaci�n; el segundo indicar� el tama�o de la pared que debemos rellenar.
A continuaci�n, escribiremos N l�neas con la informaci�n de cada uno de los cuadros siguiente el siguiente orden:
1.	Entero que indica una de las dimensiones del cuadro (el ancho si se coloca en vertical).
2.	Entero que indica la otra dimensi�n del cuadro (el ancho si se coloca en horizontal).
3.	Un n�mero real que indica el prestigio que aportar� ese cuadro a nuestra pared.

Los resultados se mostrar�n en consola y se guardar�n en un archivo llamado "resultados.txt" de la siguiente forma:

Se guardar�n tres bloques de soluciones: en el primero se mostrar�n los resultados cuando solo se usa factible como cota, en los otros dos se mostrar�n los resultados cuando se utilice una cota optimista. El primero de ellos utilizar� una cota m�s ingenua, mientras que el segundo usar� una cota mucho mejor.
Cada bloque seguir� el siguiente formato, por l�neas:
1.	Se indica la poda utilizada.
2.	Se indica la soluci�n con los datos otorgados. Para cada cuadro (y en el orden en que se introdujo su informaci�n), aparecer� un 1 cuando el cuadro se coloque y 0 cuando no. A continuaci�n, y separado por un gui�n, pueden aparecer las letras V (si el cuadro se coloca en vertical), H (si se coloca en horizontal) o N (si no se coloca).
3.	El prestigio conseguido con esa soluci�n, que ser� el mayor de los posibles con los datos proporcionados.
4.	El n�mero de nodos explorados.
5.	El tiempo que ha tardado el algoritmo en ejecutarse y el n�mero de cuadros de que dispon�amos.
6.	El tiempo que ha tardado el algoritmo en funci�n de los nodos explorados.
