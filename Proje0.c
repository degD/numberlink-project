#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 15
#define PATHLEN 200

// Pre-defined
void drawBoard(int matris[][MAX], int N);
int readFromFile(int matris[][MAX], char *fileName);

// IO functions
void printMenuMain();
void waitEnter();
void clearSCR();

// Base game operators
int unitMove(int location[2], char direction);
int checkDirection(char direction);
int checkMatrixLocation(int matrix[][MAX], int N, int i, int j);
int checkLine(int matrix[][MAX], int location[2], char direction, int magnitude, int N);

void saveStep(int location[2], char direction, int magnitude, int steps[][4], int laststep);

// Game functions
void randomMatrix(int matrix[][MAX], int N); // i, j, dir, magnitude
void generateLineInteractive(int matrix[][MAX], int moves[][4], int step, int N);
void undoMove(int matrix[][MAX], int moves[][4], int laststep, int N);

// ./TestVerileri/5x5.txt Enter relative paths

int main()
{
    int movesbysteps[MAX*MAX][4]; // For saving steps/undo 0->i 1->j 2->direction 3->magnitude
    int opr, i, j, matrix[MAX][MAX]={{0}}; 
    int N = 5, step, ret_code;
    char fileName[PATHLEN] = "./TestVerileri/5x5.txt";

    srand(time(NULL));

  	// printf("Dosya Adini Giriniz");
  	// scanf("%s",fileName);
	// /* buradan itibaren men� vb kodlar� olmalidir. dosya okumak i�in asagidaki kod verilmistir */
	
	// readFromFile(matrix, fileName);
    // drawBoard(matrix,N);
	/*
	 * matrisi manuel doldurmak veya Oyunu oynamak icin yazilacak kodlarin
	 * fonksiyon cagrilari buradan  baslamalidir
	 */	

    do {
        clearSCR();
        printMenuMain();
        scanf("%d", &opr);

        switch (opr)
        {
            case 1:
                /* random matrix */
                randomMatrix(matrix, N);
                puts("Random matrix ready! Press Enter to start.");
                waitEnter();
                
                break;

            case 2:/* load matrix file */
                // printf("Enter grid size: ");
                // scanf("%d", &N);
                // printf("Enter file path: ");
                // getc(stdin);
                // scanf("%200s", fileName);
                
                if (readFromFile(matrix, fileName) == 0) {    
                    waitEnter();
                    break;
                }

                step = 0;
                do {
                    clearSCR();

                    drawBoard(matrix, N);
                    generateLineInteractive(matrix, movesbysteps, step++, N);

                } while (1); // fix
                break;

                    

            case 3:
                /* scores */
                break;

            case 4:
                /* info */
                break;

            case 0:
                /* exit */
                break;
            
            default:
                puts("Unknown operation. Try again. Press Enter to continue...");
                waitEnter();
                break;
        }
    } while (opr != 0);

    return 0;
}

int readFromFile(int matris[][MAX], char *fileName){
	int i, j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("Failed to open file!");
		return 0;
    }
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matris[i][j]=temp; 
    }  
  	fclose(data); 
    return 1;
   // Buraya kadar olan bolumde dosyadan okuma yapiliyor. Kodu degistirmeyiniz.
}

/* Matrisi ekrana yazdirma fonksiyonu
 * Bu fonksiyonu yapiyi bozmayacak sekilde degistirebilirsiniz.
 */
void drawBoard(int matris[][MAX], int N){
    int i,j,k;
    for(i=0;i<N;i++){
        printf("\n  ");
        for (k=0;k<N;k++)
            printf("-------");
        printf("\n%d  ", i);
        for(j=0;j<N;j++)
            if (matris[i][j]!=0)
                printf("  %d   |",matris[i][j]);
            else
                 printf("      |",matris[i][j]);
    }
    printf("\n  ");
    for(i=0;i<N;i++)
        printf("  %d    ", i);
    puts("");
}

void printMenuMain()
{
    puts("NUMBER-MATCH!");
    puts("-----------------------------");
    puts("1 - GENERATE RANDOM MATRIX");
    puts("2 - LOAD MATRIX FROM FILE");
    puts("-----------------------------");
    puts("3 - USER SCORES");
    puts("4 - INFO");
    puts("-----------------------------");
    puts("0 - EXIT");
    printf(": ");
}

void waitEnter()
{
    getc(stdin);
    getc(stdin);
}

void clearSCR()
{
    int i;
    for (i = 0; i < 50; i++) puts("");
}

void undoMove(int matrix[][MAX], int moves[][4], int laststep, int N)
{
    int i;
    int cellRowIndex, cellColIndex;
    int moveMagnitude, moveDirection;

    cellRowIndex = moves[laststep][0];
    cellColIndex = moves[laststep][1];
    moveDirection = moves[laststep][2];
    moveMagnitude = moves[laststep][3];

    // while ()

}

void generateLineInteractive(int matrix[][MAX], int moves[][4], int laststep, int N)
{
    int i;
    int cellNum;
    int cellLocation[2], startLocation[2];
    int moveProgress = 0, moveMagnitude;
    char moveDirection;
    
    // Prompts
    printf("Location of the cell (ROW): ");
    scanf("%d", &cellLocation[0]);
    printf("Location of the cell (COLUMN): ");
    scanf("%d", &cellLocation[1]);
    printf("Movement direction (U/D/R/L): ");
    scanf(" %c", &moveDirection);
    printf("Movement magnitude: ");
    scanf("%d", &moveMagnitude);

    startLocation[0] = cellLocation[0];
    startLocation[1] = cellLocation[1];
    cellNum = matrix[cellLocation[0]][cellLocation[1]];

    if (checkDirection(moveDirection) && checkLine(matrix, cellLocation, moveDirection, moveMagnitude, N)) 
    {
        saveStep(cellLocation, moveDirection, moveMagnitude, moves, laststep);
        for (i = 0; i <= moveMagnitude; i++) {
            matrix[cellLocation[0]][cellLocation[1]] = cellNum;
            unitMove(cellLocation, moveDirection);
        }
    } 
    else
        printf("Incorrect entry... Press Enter to try again...");
        waitEnter();
}

// 0: Invalid, 1: Valid
int checkLine(int matrix[][MAX], int location[2], char direction, int magnitude, int N)
{
    int retCode, progress = 0;
    int initLocation[2];

    initLocation[0] = location[0];
    initLocation[1] = location[1];

    do {
        unitMove(location, direction);
        progress++;
    }
    while (progress < magnitude && checkMatrixLocation(matrix, N, location[0], location[1]) == 1);

    if (checkMatrixLocation(matrix, N, location[0], location[1]) == 1) 
        retCode = 1;
    else 
        retCode = 0;

    location[0] = initLocation[0];
    location[1] = initLocation[1];
    return retCode;
}

void saveStep(int location[2], char direction, int magnitude, int steps[][4], int laststep)
{
    laststep++;
    steps[laststep][0] = location[0];
    steps[laststep][1] = location[1];
    steps[laststep][2] = direction;
    steps[laststep][3] = magnitude;
}

// 0: unknown direction, 1: success
int checkDirection(char direction)
{
    switch (direction) {
        case 'r': case 'R': break;
        case 'l': case 'L': break;
        case 'u': case 'U': break;
        case 'd': case 'D': break;
        default: return 0;
    }
    return 1;
}

// 0: unknown direction, 1: success
int unitMove(int location[2], char direction)
{
    switch (direction) {
        case 'r': case 'R': 
            location[1]++; 
            break;
        case 'l': case 'L': 
            location[1]--; 
            break;
        case 'u': case 'U': 
            location[0]--; 
            break;
        case 'd': case 'D': 
            location[0]++; 
            break;
        default:
            return 0;
    }
    return 1;
}

/*
Not on board -> -1
On board, occupied -> -1
Free -> 1
*/
int checkMatrixLocation(int matrix[][MAX], int N, int i, int j)
{
    if (i < 0 && i >= N) return 0;
    if (j < 0 && j >= N) return 0;
    if (matrix[i][j] == 0) return 1;
    else return -1;
}


void randomMatrix(int matrix[][MAX], int N)
{
    int row, col, i = 0;
    while (i < N*2) {
        row = rand() % N;
        col = rand() % N;
        if (matrix[row][col] == 0) matrix[row][col] = (i++ % 2)+1;
    }
    // Lists every number between 1-N twice and places them on different locations
}
