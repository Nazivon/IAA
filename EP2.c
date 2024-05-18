#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME 256

typedef struct {
	int dura;
	int nome;
} Att;

typedef struct {
char operacao;
int nome;
int dura;
int tempo;
} AuxDina;

typedef struct {
	Att* arr;
	int tamanho;
} Heap;

void merge(Att* A, int i, int m, int f);

void mergeSort(Att* A, int i, int f) {
  if (i < f) {
    int m = (i + f) / 2;
    mergeSort(A, i, m);
    mergeSort(A, m + 1, f);
    merge(A, i, m, f);
  }
}

void merge(Att* A, int i, int m, int f) {
  int n1 = m - i + 1;
  int n2 = f - m;

  Att *L = (Att *)malloc((n1 + 1) * sizeof(Att));
  Att *R = (Att *)malloc((n2 + 1) * sizeof(Att));

  for (int j = 0; j < n1; j++) {
    L[j] = A[i + j];
  }
  for (int j = 0; j < n2; j++) {
    R[j] = A[m + 1 + j];
  }

  L[n1].dura = 120;
  R[n2].dura = 120;

  int kL = 0, kR = 0;
  for (int k = i; k <= f; k++) {
    if (L[kL].dura <= R[kR].dura) {
      A[k] = L[kL];
      kL++;
    } else {
      A[k] = R[kR];
      kR++;
    }
  }

  free(L);
  free(R);
}

int Guloso (Att* vetor, int T, int nQa, int vetAux[]) {
if (nQa <= 0) return 0;
int horas = 0;
int selec = 0;
for (int i = 0; i < nQa; i++)
 if (T >= (horas + vetor[i].dura))
  {
vetAux[i] = vetor[i].nome;
horas += vetor[i].dura;
selec++;
  }
return selec;
}

void troca (Att* a, Att* b) {
    Att temp = *a;
    *a = *b;
    *b = temp;
}

void refazHeap(Heap* h, int j) {
int klein = j;
int esq = 2*j + 1;
int dir = 2*j + 2;
    if (esq < h->tamanho && h->arr[esq].dura < h->arr[klein].dura)
        klein = esq;
    if (dir < h->tamanho && h->arr[dir].dura < h->arr[klein].dura)
        klein = dir;
if (klein != j) {
        troca (&h->arr[j], &h->arr[klein]);
refazHeap (h, klein);
}
}

void constroiHeap(Heap* h) {
    for (int j = (h->tamanho) / 2 - 1; j >= 0; j--) {
      refazHeap(h, j);
}
}

int busca (Heap* h, int nomBus) {
  for (int i = 0; i < h->tamanho; i++) {
    if (h->arr[i].nome == nomBus) {
      return i;
    }
  }
  return -1;
}

void aumentaPrioridade (Heap*h, int duraC, int nomeC) {
int i = busca(h, nomeC);
 h->arr[i].dura = duraC;
 h->arr[i].nome = nomeC;
 while (i > 0 && h->arr[(i-1)/2].dura > h->arr[i].dura) {
  troca (&h->arr[i], &h->arr[(i-1)/2]);
  i = (i-1)/2;
}
}

void insere (Heap* h, int duraC, int nomeC) {
h->tamanho++;
h->arr[(h->tamanho)-1].nome = nomeC;
aumentaPrioridade (h, duraC, nomeC);
}

void leMuda (Heap*h, int horas, AuxDina* p, int* ant) {
int j;
for (j = *ant; p[j].tempo <= horas; j++) {
  if (p[j].operacao == 'c') {
	    aumentaPrioridade (h, p[j].dura, p[j].nome);
    } else if (p[j].operacao == 'i') {
         insere (h, p[j].dura, p[j].nome);
          }
} 
*ant = j;
}

Att extrai(Heap* h) {
    Att escolha = h->arr[0];
    Att ultimoEl = h->arr[h->tamanho-1];
    h->arr[0] = ultimoEl;
    h->tamanho = h->tamanho - 1;
    refazHeap(h, 0);
    return escolha;
}

int Guloso2 (Heap* h, AuxDina* p, int T, int vetAux[]) {
if (h->tamanho <= 0) return 0;
int horas = 0;
int selec = 0; 
int ant = 0;
for (int m = 0; T >= (horas+(h->arr[0].dura)); m++) {
  Att escolhido = extrai(h);
  vetAux[m] = escolhido.nome;
  horas += escolhido.dura;
  leMuda (h, horas, p, &ant);  
  selec++;
  }
return selec;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Este programa exige tres argumentos: a duracao do periodo de atualizacoes, o numero do cenario e o arquivo de entrada.\n");
        exit(1);
    }
    printf("%s ", argv[0]);
    printf("%s ", argv[1]);
    printf("%s", argv[2]);
    printf("%s\n", argv[3]);
    
int T = atoi(argv[1]);
int C = atoi(argv[2]);

char filename[MAX_FILENAME] = "";
strcpy(filename, argv[3]);
FILE* fp = fopen(filename, "r");
if (fp == NULL){
    fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
    return -1;
}

switch (C)
{
case 1: {
int nQa;
fscanf(fp, "%2d", &nQa);
Att* vetor = (Att*) malloc(nQa*sizeof(Att));
if (vetor == NULL) {
        printf("Alocacao de memoria falhou, preciso de espaco!\n");
        return 1;
    }
for (int i = 0; i < nQa; i++) {
fscanf(fp, "%d", &vetor[i].dura);
vetor[i].nome = i + 1;
}

fclose(fp);

mergeSort(vetor, 0, nQa-1);

int vetAux[20];
int nPa = Guloso (vetor, T, nQa, vetAux);

FILE *Saida = fopen("saida1.txt", "w");
if (Saida == NULL) {
    printf("Erro ao abrir o arquivo de saída.\n");
    return 1;
}

fprintf(Saida, "%d ", nPa);

for (int resp = 0; resp < nPa-1; resp++) {
 fprintf(Saida, "%d ", vetAux[resp]);
}
fprintf(Saida, "%d\n", vetAux[nPa-1]);

fclose(Saida);
break;
}

case 2: {
int nQb;
fscanf(fp, "%2d", &nQb);
Heap heap;
heap.arr = (Att*) malloc ((100+nQb)*sizeof(Att));
if (heap.arr == NULL) {
      	  fprintf(stderr, "Erro ao alocar memoria para heap.arr\n");
     	   return 1;
}
heap.tamanho = nQb;
for (int i = 0; i < nQb; i++) {
fscanf(fp, "%d", &heap.arr[i].dura);
heap.arr[i].nome = i + 1;
}

constroiHeap (&heap);

AuxDina* atualiza = (AuxDina*) malloc(100*sizeof(AuxDina));
int k = 0;
while (fscanf(fp, "%1s %d %d %d", &atualiza[k].operacao, &atualiza[k].tempo, &atualiza[k].nome, &atualiza[k].dura) == 4) {
k++;
}

int vetAux[20];
int nPb = Guloso2 (&heap, atualiza, T, vetAux);

fclose (fp);
free(atualiza);

FILE *Output = fopen("saida2.txt", "w");
if (Output == NULL) {
    printf("Erro ao abrir o arquivo de saida.\n");
    return 1;
}

fprintf(Output, "%d ", nPb);

for (int resp = 0; resp < nPb-1; resp++) {
 fprintf(Output, "%d ", vetAux[resp]);
}
fprintf(Output, "%d\n", vetAux[nPb-1]);

free(heap.arr);

break;
}

default:
printf("Cenario invalido!\n");
}
    return 0;
}
