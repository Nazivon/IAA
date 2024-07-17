#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME 256

typedef struct color {
  int preto;
  int branco;
} COR;

int calculaLogDoTam(int tam) {
  int i = 0;
  while (tam != 1) {
    tam = tam / 2;
    i++;
  }
  return i;
}

int N;

void processarQuadrantes(int quad[], int tamanho, COR* armazenaCor, int elem) {
  for (int i = 0; i < tamanho; i++) {
        if (quad[i] == 1) {
            armazenaCor[elem].branco++;
        } else if (quad[i] != -1) {
            armazenaCor[elem].preto++;
        }
    }
}

int Quadrantes(int** matriz, int comecoA, int comecoB, int tamanho, COR* armazenaCor) {
  int elem = 0;
  if (tamanho == 1) {
      return matriz[comecoA][comecoB];
    }

  int metade = tamanho / 2;
  int quad1 = Quadrantes(matriz, comecoA, comecoB, metade, armazenaCor);
  int quad2 = Quadrantes(matriz, comecoA, comecoB + metade, metade, armazenaCor);
  int quad3 = Quadrantes(matriz, comecoA + metade, comecoB, metade, armazenaCor);
  int quad4 = Quadrantes(matriz, comecoA + metade, comecoB + metade, metade, armazenaCor);

  elem = calculaLogDoTam(metade);
  
  if (quad1 == quad2 && quad1 == quad3 && quad1 == quad4 && quad1 == 0) {
    return 0;
  }
  if (quad1 + quad2 + quad3 + quad4 == 4) {
    return 1;
  }

  int quad[] = {quad1, quad2, quad3, quad4};
  processarQuadrantes(quad, 4, armazenaCor, elem);
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Este programa exige dois argumentos: o número N de linhas da matriz e o arquivo de entrada.\n");
    exit(1);
  }
  
  printf("%s ", argv[0]);
  printf("%s ", argv[1]);
  printf("%s\n", argv[2]);

  int N = atoi(argv[1]);
    
  char filename[MAX_FILENAME] = "";
  strcpy(filename, argv[2]);
  FILE* fp = fopen(filename, "r");
  if (fp == NULL){
    fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
    return -1;
  }
  COR* armazenaCor = (COR*) malloc(11*sizeof(COR));
  
  for (int elem = 0; elem < 11; elem++) {
    armazenaCor[elem].preto = 0;
    armazenaCor[elem].branco = 0;
  }

  int **matriz = malloc(N * sizeof(int*));
  for (int i = 0; i < N; i++) {
    matriz[i] = malloc(N * sizeof(int));
  }
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      fscanf(fp, "%1d", &matriz[i][j]);
    }
  }
    
  fclose(fp);
    
  Quadrantes(matriz, 0, 0, N, armazenaCor);
    
  FILE *Saida = fopen("saida.txt", "w");
  if (Saida == NULL) {
    printf("Erro ao abrir o arquivo de saída.\n");
    return 1;
  }
  
  for (int posicao = calculaLogDoTam(N); posicao >= 0; posicao--) {
    fprintf(Saida, "%dx%d %d %d\n", N, N, armazenaCor[posicao].branco, armazenaCor[posicao].preto);
    N /= 2;
  }
  
  fclose(Saida);
  free(armazenaCor);
  return 0;
}
