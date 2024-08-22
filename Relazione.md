# Simulazione Stormi

#### Mattia Corrà
Github Repository: https://github.com/mathblue/stormi

### Logica e Design 
Il progetto è suddiviso in tre Translation Units, la strategia implementativa è quella della programmazione ad oggetti e qui la TU che contiene la classe principale è la "boid.cpp" con il suo header "boid.hpp". L'oggetto Boid rappresenta il singolo uccello, caratterizzato dagli attributi posizione e velocità (ci siamo limitati a moti nel piano) più un set di puntatori Boid chiamati "neighbors". Quest'ultimo attributo dovrebbe contenere l'insieme degli uccelli che sono entro il raggio d'influenza reciproca, la scelta di usare un set ci consente di avere una complessità logaritmica nelle query grazie all'ordinamento interno (di default rispetto all'indirizzo di memoria che per noi è perfetto).  La scelta di avere questo attributo, oltre ad essere una comodità strutturale doveva aiutare per implementare delle funzioni di aggiornamento più intelligenti di quella in realtà usata, speravo di poter sfruttare la struttura di grafo non orientato conseguente per aggiornare i vicini in maniera sub quadratica ma non è riusulato fattibile come credevo.
Usando raw pointers non si incorre nell'eventualità di memory leaks come nel caso di oggetti allocati nella Heap dinamicamente dallo sviluppatore, in questo caso la gestione dell'allocazione e successiva distruzione nella stack è lasciata al compilatore. L'unica eventuale problematica sarebbe che questi puntatori diventassero dangling nel caso in cui si punti ad una variabile out of scope; nel nostro caso ho inizializzato il vettore di Boids una volta per tutte all'inizio del main e tutta l'esecuzione rimane dentro lo scope del vettore.
L'oggetto Boid ha poi due metodi updateVelocity(), updatePosition(), che tramite le regole di volo aggiornano la velocità e in seguito integrano il moto con il semplice metodo d'Eulero.
La Translation Unit che contiene le regole di volo usate nell'aggiornamento dei Boid è composta da "rules.cpp" e il suo header che condivide con la precedente "boid.hpp". Qui sono definite le funzioni separation(), alignment() e cohesion() che prendono in input il Boid da aggiornare e il suo set di vicini. In questa TU ho poi implementato la funzione mod() che permette di simulare l'influenza di volo su spazi toroidali (possibilità configurabile come vedremo tra un attimo). Sarebbe stato probabilmente più pulito creare una nuova variabile doubleMod che gestisse al suo interno l'aritmetica modulare ma per non scardinare l'intero codice ho preferito questa funzione.
La TU contenente le free functions utilizzate dal main è composta da "helper.cpp" e il suo header "helper.hpp", qui vengono definite le semplici averageDistanceAndStdDev(...) e averageVelocityAndStdDev(...), la generateBoids(...) che mi genera a partire da delle distribuzioni uniformi (attingendo a delle risorse random dell'hardware per i seed delle funzioni pseudorandomiche) il vettore di Boids iniziale, e infine contiene  lazyUpdateNeighbors(...) che è la funzione che riveste il ruolo più importante dal punto di vista dell'efficienza (in questo caso inefficienza...) della logica del progetto, essa aggiorna le liste (set) di vicinanza dei vari Boid. Purtroppo non sono arrivato ad implementare qualcosa che eviti una complessità quadratica, l'unico modo non probabilistico sembrerebbe quello di suddividere lo spazio in maniera intelligente con una struttura dati tipo QuadTree (https://en.wikipedia.org/wiki/Quadtree) oppure una semplice griglia con una scelta giudiziosa della dimensione delle suddivisioni.
Per quanto riguarda l'efficienza sarebbe stato possibile implementare questa e l'integrazione di Eulero sfruttando i vari core del processore procedendo con una suddivisione in parallelo del processo che qui sarebbe stata abbastanza facile non essendoci interazioni tra gli eventuali thread di un singolo aggiornamento, per partizionare gli agenti e gestirli in thread paralleli dall'inizio alla fine sarebbe invece necessario uno studio più complesso.
Abbiamo poi la TU dedicata alla visualizzazione grafica "Boidrenderer.cpp" e "Boidrenderer.hpp".
Infine ho scelto di avere una classe Singleton come configurazione globale per poter condividere a tutte le TU i parametri necessari senza dover includere un header modificabile in ognuno ovviando in questo modo alla conseguente ricompilazione di ogni file a seguito di modifiche nei parametri. Ho voluto evitare input da tastiera per avere un file di configurazione più compatto ed esplicativo, è possibile infatti inserire i propri valori nel file config.txt che verrano poi caricati nel main e condivisi all'intero progetto.
La logica evolutiva del main è abbastanza immediata: si carica la configurazione, si inizializza l'istanza del renderizzatore (per la visualizzazione grafica), si inizializza il vettore di Boid, si fa evolvere a step in un for loop il moto dei Boid aggiornando le posizioni, velocità e vicini, poi si renderizza il frame ed in fine ogni secondo vengono aggiornate e visualizzate a video le informazioni sulle medie e deviazioni standard della fase del sistema.

### Compilazione ed esecuzione
Per il build del projetto ho optato per CMake che permette di gestire facilmente le librerie esterne (nel nostro caso quella grafica) e soprattutto di creare una directory di build separata dal sorgente e di compilare solo i file effettivamente modificati (che è anche, tra le varie cose, uno dei motivi per avere un progetto modulare).

```
CMake SetUp
Debug:
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
Release:
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

Compilation
cmake --build build

execution
./build/boids
testing
./build/boids.t

```

### Input/Output
L'input è dato tramite il file di configurazione config.txt, è controllata l'effetiva sensatezza dei dati inseriti e la possibile mancanza di parametri che devono avere i nomi indicati. Essi sono il numero di agenti, il passo temporale, il tempo di volo simulato, la velocità massima, la topologia dello spazio, il fattore di repulsione nel caso di spazio limitato, XSpace e YSpace sono le dimensioni dello spazio, seguono i parametri delle regole di volo.

L'output è la visualizzazione della simulazione tramite un rendering grafico dell'evoluzione degli agenti con delle informazioni statistiche visualizzate in un angolo.

### Interpretazione Risultati
Un grosso riscontro del corretto funzionamento del progetto è stato dato sicuramente dall'osservazione del moto degli uccelli simulati. Usando dei valori abbastanza abbozzati per i parametri non è stato facile riconoscere la fenomenologia di uno stormo, in seguito ho cercato di capire quali regole di volo stavano dando troppo contributo e quali meno cercando così di raggiungere un equilibro passando dagli agglomerati vaganti iniziali a dei moti di sciami di mosche a un effettivo comportamento che si potrebbe definire a "stormo" con i boids inseriti in una griglia abbastanza equidistanziata e stabile che assorbe i gruppi più piccoli.
Non ho avuto tempo di implementare un angolo di vista che limitasse l'influenza dei vicini che avrebbe probabilmente portato a dei moti più uniformi verso una direzione e non a zig-zag.
Osservando la simulazione si può inoltre confermare l'effettiva topologia scelta.

### Test/Debug
Durante lo sviluppo mi sono aiutato con varie stampe a terminale per delle verifiche capillari nel caso si presentassero bachi inconsueti.
Negli altri casi ho utilizzato qualche assert per evitare banali errori e sviste ma il corpo più ampio di test è stato fatto tramite lo strumento Doctest che ci permette di fare vari controlli sulle varie funzionalità prese una per una separatamente con uno scope dedicato ad ognuna dove poter inizializzare e configurare le variabili richieste per i singoli metodi e oggetti.





