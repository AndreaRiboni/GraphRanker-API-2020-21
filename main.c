#include <stdio.h>
#include <stdlib.h>
#define INFTY 0xffffffff

typedef unsigned int uint;
//Dimensione del grafo e numero di grafi in classifica
uint SIZE, K;
//Dimensione del buffer per leggere il singolo carattere
const int BUFFER_SIZE = 11;
//Array per calcolare la potenza di 10
const int results[10] = {
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

typedef struct {
    uint sum;
    uint index;
} Graph;

typedef struct {
    uint heap_size;
    uint* scores;
    uint* indexes;
} Heap;

void swap(uint *n1, uint *n2){
    uint temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

uint getParent(uint i){
    return i/2;
}
uint getLeft(uint i){
    return 2*i+1;
}
uint getRight(uint i){
    return 2*i+2;
}

void maxHeapify(Heap* heap, uint n){
    uint l = getLeft(n), r = getRight(n), posmax;
    if(l < heap->heap_size && heap->scores[l] > heap->scores[n]){
        posmax = l;
    } else posmax = n;
    if(r < heap->heap_size && heap->scores[r] > heap->scores[posmax]) {
        posmax = r;
    }
    if(posmax != n){
        swap(&heap->scores[n], &heap->scores[posmax]);
        swap(&heap->indexes[n], &heap->indexes[posmax]);
        maxHeapify(heap, posmax);
    }
}

void deleteMax(Heap* heap, uint* min_score, uint* min_index){
    *min_score = heap->scores[0];
    *min_index = heap->indexes[0];
    heap->scores[0] = heap->scores[heap->heap_size - 1];
    heap->indexes[0] = heap->indexes[heap->heap_size - 1];
    heap->heap_size--;
    maxHeapify(heap, 0);
}

void addToHeap(Heap* heap, uint node_score, uint node_index){
    /*heap->heap_size++;
    uint i = heap->heap_size;
    heap->scores[i] = node_score;
    heap->indexes[i] = node_index;
    while(i > 0 && heap->scores[getParent(i)] < heap->scores[i]){
        swap(&heap->scores[getParent(i)], &heap->scores[i]);
        swap(&heap->indexes[getParent(i)], &heap->indexes[i]);
        i = getParent(i);
    }*/
    heap->scores[heap->heap_size] = node_score;
    heap->indexes[heap->heap_size] = node_index;
    uint i = heap->heap_size;
    heap->heap_size++;
    while(i != 0 && heap->scores[getParent(i)] < heap->scores[i]){
        swap(&heap->scores[getParent(i)], &heap->scores[i]);
        swap(&heap->indexes[getParent(i)], &heap->indexes[i]);
        i = getParent(i);
    }
}

int isEmpty(Heap* heap){
    return heap->heap_size == 0;
}

uint fastDijkstra(uint matrix[SIZE][SIZE]){
    uint dist[SIZE];
    int visited[SIZE];
    //inizializzo le distanze
    for(uint i = 1; i < SIZE; i++){
        dist[i] = INFTY;
        visited[i] = 0;
    }
    dist[0] = 0;
    visited[0] = 0;
    for(uint vertex = 0; vertex < SIZE - 1; vertex++){
        uint MinDist = INFTY, MinIndex = 0;
        //Determino il vertice a distanza minima
        for(uint v = 0; v < SIZE; v++){
            if(visited[v] == 0 && dist[v] <= MinDist){
                MinDist = dist[v];
                MinIndex = v;
            }
        }
        //Lo visito
        visited[MinIndex] = 1;
        //Aggiorno eventualmente la distanza
        for(uint i = 0; i < SIZE; i++){
            if(visited[i] == 1 || matrix[MinIndex][i] == 0){
                continue;
            }
            if(dist[MinIndex] != INFTY && dist[MinIndex] + matrix[MinIndex][i] < dist[i]){
                dist[i] = dist[MinIndex] + matrix[MinIndex][i];
            }
        }
    }
    uint sum = 0;
    for(uint i = 0; i < SIZE; i++){
        //printf("Distanza 0-%u: %u\n", i, dist[i]);
        if(dist[i] != INFTY){
            sum += dist[i];
        }
    }
    //printf("Somma dei cammini minimi: %u\n", sum);
    return sum;
}

void printRanking(Heap* ranking){
    //uint index, score;
    if(ranking->heap_size == 0) {
        printf("\n");
        return;
    }
    for(uint i = 0; i < ranking->heap_size - 1; i++){
        if(ranking->indexes[i] != -1)
            printf("%u ", ranking->indexes[i]);
    }
    if(ranking->indexes[ranking->heap_size - 1] != -1)
        printf("%u", ranking->indexes[ranking->heap_size - 1]);
    printf("\n");
}

void updateRanking(Heap* ranking, uint index, uint score){
    uint trash_index, trash_score;
    //printf("UPDATE --- INDEX: %u\tSCORE: %u\n", index, score);
    if(ranking->heap_size < K){
        addToHeap(ranking, score, index);
        //printf("\tAdded easily --- heap_size: %u\tK: %u\n", ranking->heap_size, K);
    } else if(score < ranking->scores[0]){
        deleteMax(ranking, &trash_score, &trash_index);
        //printf("\tAdded normally --- score: %u\told max: %u\n", score, trash_score);
        addToHeap(ranking, score, index);
        //printf("\t\tNew max is %u\n\t\tBye bye index %u\n", ranking->scores[0], trash_index);
    } else {
        //printf("\tNot added --- score: %u\tmax: %u\n", score, ranking->scores[0]);
    }
}

uint getUintFromBuffer(const char* buffer, int *sz){
    uint sum = 0;
    int exp = 0;
    for(int i = *sz - 1; i >= 0; i--){
        //printf("Carattere: %c\tPosizione: %d\tNumero: %d\tSomma: %u\n", buffer[i], i, (int)(buffer[i]-'0') * tenExp(exp), sum);
        sum += (int)(buffer[i]-'0') * results[exp];
        exp++;
    }
    *sz = 0;
    return sum;
}

void printHeap(Heap* heap){
    for(int i = 0; i < heap->heap_size; i++){
        printf("Index: %u\tScore: %u\n", heap->indexes[i], heap->scores[i]);
    }
    //printf("Done\n\n");
}

int main() {

    //Apro il file
    //FILE *test = fopen("../open_tests/input_6", "r");
    FILE *test = stdin;
    if(test == NULL){
        printf("File inesistente");
        return 1;
    }
    int garbage = fscanf(test, "%u %u", &SIZE, &K);
    if(garbage){}
    //Creo le variabili necessarie
    uint matrix[SIZE][SIZE]; //matrice rappresentante il grafo su cui sto iterando
    uint index = 0; //indice del grafo su cui sto iterando
    uint num; //Peso dell'arco su cui sto iterando
    char buffer[BUFFER_SIZE]; //buffer per convertire il numero da stringa a uint
    int offset = 0; //cifra decimale del numero su cui sto iterando
    char c = getc_unlocked(test); //carattere letto + consumo il \n
    //Leggo tutto il file fino alla fine carattere per carattere
    uint row = 0, col = 0; //coordinate della matrice
    Heap* ranking = (Heap*)malloc(sizeof(Heap));
    ranking->heap_size = 0;
    ranking->scores = (uint*)malloc(sizeof(uint) * K);
    ranking->indexes = (uint*)malloc(sizeof(uint) * K);
    //Graph ranking[K]; //top-K
    //Graph* graph = (Graph*)malloc(sizeof(Graph));
    //inizializzo il ranking
    //for(uint i = 0; i < K; i++){
    //    ranking[i].sum = INFTY;
    //    ranking[i].index = -1;
    //}

    //Leggo il file
    while((c=getc_unlocked(test))!=EOF){
        if(c == 'A'){ //Skippo il resto di AggiungiGrafo
            while((c=getc_unlocked(test))!='\n'){}
            row = col = 0;
        } else if(c == 'T'){ //Skippo il resto di top-k
            while((c=getc_unlocked(test))!='\n'){}
            printRanking(ranking);
        } else { //Potrebbe essere un numero
            if(c >= '0' && c <= '9'){ //Caso in cui è un numero
                buffer[offset++] = c; //Aggiungo il carattere al buffer
            } else { //Numero terminato
                num = getUintFromBuffer(buffer, &offset);
                //aggiungo il numero al grafo
                if(col >= SIZE){
                    col = 0;
                    row++;
                }
                matrix[row][col] = num;
                if(row == col && col == SIZE - 1) {
                    //fastDijkstra(matrix);
                    updateRanking(ranking, index, fastDijkstra(matrix));
                    index++;
                } else {
                    col++;
                }
            }
        }
    }

    //Chiudo il file e dealloco
    free(ranking);
    fclose(test);
}