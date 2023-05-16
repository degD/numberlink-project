#include<stdio.h>
#define MAX 15


void drawBoard(int matris[][MAX], int N);
void readFromFile(int matris[][MAX], char *fileName);

int main()
{
	int numberLocations[MAX][5]={{0}}, matrixGhost[MAX][MAX]={{0}};
	int i, j, matrixRaw[MAX][MAX]={{0}}; 
    	int N = 5; 
	char fileName[200] = "./TestVerileri/5x5.txt";	

	readFromFile(matrixRaw, fileName);
	drawBoard(matrixRaw, N);
	
	matrixAnalysis(matrixRaw, numberLocations, N);
	
	
	return 0;
}

void readFromFile(int matris[][MAX], char *fileName){
	int i,j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("Dosya Acilamadi!");
		return;
    }
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matris[i][j]=temp; 
    }  
  	fclose(data); 
   // Buraya kadar olan bolumde dosyadan okuma yapiliyor. Kodu degistirmeyiniz.
}

void drawBoard(int matris[][MAX], int N){
    int i,j,k;
    for(i=0;i<N;i++){
        printf("\n");
        for (k=0;k<N;k++)
            printf("-------");
        printf("\n");
        for(j=0;j<N;j++)
            if (matris[i][j]!=0)
                printf("  %d   |",matris[i][j]);
            else
                 printf("      |",matris[i][j]);
    }
}

void matrixAnalysis(int matrixRaw[][MAX], int numberLocations[][4], int N)
{
	int i, j, num;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			num = matrixRaw[i][j];
			if (num != 0) {
				if (numberLocations[num-1][4] == 0) {
					numberLocations[num-1][0] = i;
					numberLocations[num-1][1] = j;
					numberLocations[num-1][4]++;
				} else {
					numberLocations[num-1][2] = i;
					numberLocations[num-1][3] = j;
				}
			}
		}
	}
}
					
