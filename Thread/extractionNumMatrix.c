/**
 *  Da data una matrice nxn( con n pari) di interi generati casualmente, allocata dinamicamente, 
 *  con n argomento da riga di comando, creare n thread che prelevano casualmente un elemento 
 *  della riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano concorrentemente 
 *  in un vettore di (n + 1)/2 elementi.
 *  Un thread n + 1 - esimo attende il riempimento del vettore per stampare il contenuto dello
 *  stesso e per stampare il numero di elementi inseriti nel vettore da ciascun thread. 
 *  Usare mutex e variabili di condizione
*/

