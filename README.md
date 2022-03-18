# GraphRanker-API-2020-21
## Progetto di API, PoliMi A.A. 2020/21

Il progetto si pone l'obiettivo di realizzare efficientemente, sia per complessità temporale che per complessità spaziale un classificatore di grafi: si deve gestire una classifica di grafi diretti e pesati che tenga conto dei k migliori grafi.

Viene reso possibile interfacciarsi con il programma attraverso lo STDIN: sono possibili i comandi AggiungiGrafo per aggiungere un grafo alla classifica e TopK per mostrare i k migliori grafi. Il numero di grafi e k vengono specificati sempre attraverso lo STDIN.

La metrica della classifica è definita dalla somma di tutti i cammini minimi tra il nodo 0 e tutti gli altri nodi del grafo raggiungibili da 0
