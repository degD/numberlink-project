#include<stdio.h>
#define MAX 15

// Pre-defined
void drawBoard(int matris[][MAX], int N);
int readFromFile(int matris[][MAX], char *fileName);

// Game
void matrixAnalysis(int matrix[][MAX], int numberLocations[][MAX], int N);
int isLineValid(int matrix[][MAX], int i0, int j0, int i1, int j1, int N);
int boundaryCheck(int i0, int j0, int N);

// General matrix IO
void matrixPrint(int m[MAX][MAX], int h, int w);

void moveUndo(int matrix[][MAX], int numberLocations[][MAX], int listMoves[][MAX], int *movesIndex, int N);
void gameLoopManual(int matrix[][MAX], int numberLocations[][MAX], int *movesIndex, int listMoves[][MAX], int N);
void manualMove(int matrix[][MAX], int numberLocations[][MAX], int *movesIndex, int listMoves[][MAX], int N);
void waitForEnter();
void clearScreen();
int connectedCheck(int num, int numberLocations[][MAX]);
void saveMove(int i0, int j0, int i1, int j1, int num, int numberLocations[][MAX], int listMoves[][MAX], int *movesIndex, int N);
void drawLine(int matrix[][MAX], int num, int i0, int j0, int i1, int j1, int N);
int testLine(int matrix[][MAX], int i0, int j0, int i1, int j1, int numberLocations[][MAX], int N);
void lineDirection(int i0, int j0, int i1, int j1, int dirArr[2]);
void possibleHeadsPrint(int numberLocations[][MAX], int N);
int selectionHeadCheck(int matrix[][MAX], int i0, int j0, int numberLocations[][MAX], int N);
int isTargetValid(int matrix[][MAX], int i0, int j0, int i1, int j1, int N);
int boundaryCheck(int i0, int j0, int N);




int main()
{
	int numberLocations[MAX][MAX] = {{0}}, listMoves[MAX][MAX] = {{0}};
	int movesIndex = 0;
	int matrix[MAX][MAX] = {{0}}; 
    int N = 5; 
	char c, fileName[200] = "./TestVerileri/5x5.txt";	

	readFromFile(matrix, fileName);
	drawBoard(matrix, N);

	// Game	
	matrixAnalysis(matrix, numberLocations, N);
	gameLoopManual(matrix, numberLocations, &movesIndex, listMoves, N);
//	
//	printf("Move(m), Undo(u), Exit(e): ");
//	scanf(" %c", &c);
//	switch (c)
//	{
//		case 'M': case 'm':
//			
//			break;
//
//		case 'U': case 'u':
//			break;
//
//		case 'E': case 'e':
//			break;
//
//		default:
//			break;
//
//	}

	
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

/* Forms up the numberLocations helper matrix.
 * First four elements are coordinates of the numbers. The last 
 * element defines the state of the that number. "1" stands for not connected, 
 * "0" is for connected
 */
void matrixAnalysis(int matrix[][MAX], int numberLocations[][MAX], int N)
{
	int i, j, num;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			num = matrix[i][j];
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
					
void matrixPrint(int m[MAX][MAX], int h, int w) 
{
	int i, j;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}

}

// Is given coord inside matrix boundaries?
int boundaryCheck(int i0, int j0, int N)
{
	if (i0 < 0 || i0 >= N) return 0;
	if (j0 < 0 || j0 >= N) return 0;
	return 1;
}

// Is the head and the selection on the same line (direction) and also inside the matrix?
int isTargetValid(int matrix[][MAX], int i0, int j0, int i1, int j1, int N)
{
	if ( boundaryCheck(i0, j0, N) && boundaryCheck(i1, j1, N) )
	{
		if (i0 == i1 && j0 == j1) return 0;
		if (i0 == i1 || j0 == j1) {
			if (matrix[i1][j1] == matrix[i0][j0] || matrix[i1][j1] == 0) return 1;
		}
	}
	return 0;
}

// Is head selection valid?
int selectionHeadCheck(int matrix[][MAX], int i0, int j0, int numberLocations[][MAX], int N)
{
	int n;
	if (boundaryCheck(i0, j0, N) == 0) return 0;
	if (matrix[i0][j0] == 0) return 0;
	else {
		n = matrix[i0][j0];
		if (numberLocations[n-1][4] == 1) return 1; // Is an un-connected number
		else return 0;
	}
}

// Informs about connected and disconnected numbers.
void possibleHeadsPrint(int numberLocations[][MAX], int N)
{
	int i;
	for (i = 1; i <= N; i++) {
		if (numberLocations[i-1][4] == 1) {
			printf("%3d is not connected...\n", i);
		} else {
			printf("%3d is connected...\n", i);
		}
	}
}

// Find direction of movement for valid targets. 
void lineDirection(int i0, int j0, int i1, int j1, int dirArr[2])
{
	if (i0 == i1 && j0 < j1) {
		dirArr[0] = 0;
		dirArr[1] = 1;
	} else if (i0 == i1 && j0 > j1) {
		dirArr[0] = 0;
		dirArr[1] = -1;
	} else if (i0 < i1 && j0 == j1) {
		dirArr[0] = 1;
		dirArr[1] = 0;
	} else if (i0 > i1 && j0 == j1) {
		dirArr[0] = -1;
		dirArr[1] = 0;
	}
}

// Are there any obstacles on line direction? 1 -> CLEAR, 0 -> BLOCKED
int testLine(int matrix[][MAX], int i0, int j0, int i1, int j1, int numberLocations[][MAX], int N)
{
	int i, j, num, dir[2];
	int isObstacle = 0; 

	if(isTargetValid(matrix, i0, j0, i1, j1, N) == 0) return 0;
	
	num = matrix[i0][j0];	
	lineDirection(i0, j0, i1, j1, dir);
	i = i0;
	j = j0;
	do {
		i += dir[0];
		j += dir[1];
		if (matrix[i][j] != 0) isObstacle++;
	} 
	while (!(i == i1 && j == j1));
	
	// "End" number may also got counted as an obstacle. 
	if (isObstacle == 0)
		return 1;
	else if (isObstacle == 1 && matrix[i][j] == num)
		return 1;
	else 
		return 0;
}

// Draws a line between two cells.
void drawLine(int matrix[][MAX], int num, int i0, int j0, int i1, int j1, int N)
{
	int i, j, dir[2];

	lineDirection(i0, j0, i1, j1, dir);
	i = i0;
	j = j0;
	do {
		i += dir[0];
		j += dir[1];
		matrix[i][j] = num;
	} 
	while (!(i == i1 && j == j1));
}

// Saves movement information. 
void saveMove(int i0, int j0, int i1, int j1, int num, int numberLocations[][MAX], int listMoves[][MAX], int *movesIndex, int N)
{
	listMoves[*movesIndex][0] = i0;
	listMoves[*movesIndex][1] = j0;
	listMoves[*movesIndex][2] = i1;
	listMoves[*movesIndex][3] = j1;
	listMoves[*movesIndex][4] = num;
	*movesIndex++;

	numberLocations[num-1][0] = i1;
	numberLocations[num-1][1] = j1;
}

// Are cells with the given number are connected?
int connectedCheck(int num, int numberLocations[][MAX])
{
	if (numberLocations[num-1][0] != numberLocations[num-1][2]) return 0;
	if (numberLocations[num-1][1] != numberLocations[num-1][3]) return 0;
	return 1;
}

void clearScreen() 
{
	printf("\e[;H\e[J");
}

void waitForEnter()
{
	getc(stdin);
	getc(stdin);
}

void manualMove(int matrix[][MAX], int numberLocations[][MAX], int *movesIndex, int listMoves[][MAX], int N)
{
	int num, i0, j0, i1, j1;
	int isValid, se;

	clearScreen();
	printf("Numbers...\n");
	possibleHeadsPrint(numberLocations, N);
	printf("Press Enter to continue...");
	waitForEnter();
	clearScreen();

	// TODO: MAY THERE BE A REASON FOR EXITING ALONG THE MOVE??

	// Selecting a head
	do {
		clearScreen();
		drawBoard(matrix, N);
		printf("Head...\n");
		printf("Enter row index: ");
		scanf("%d", &i0);
		printf("Enter column index: ");
		scanf("%d", &j0);

		// Checking the head selection
		isValid = selectionHeadCheck(matrix, i0, j0, numberLocations, N);
		if (isValid == 0) {
			printf("Selection is not valid. Press Enter and try again...\n");
			waitForEnter();
		}
	} while(isValid == 0);

	num = matrix[i0][j0];
	do {
		// Selecting a target 
		clearScreen();
		drawBoard(matrix, N);
		do {
			printf("Target from [%3d,%3d]...\n", i0, j0);
			printf("Enter row index: ");
			scanf("%d", &i1);
			printf("Enter column index: ");
			scanf("%d", &j1);

			isValid = testLine(matrix, i0, j0, i1, j1, numberLocations, N);
			if (isValid == 0) {
				printf("Target selection is not valid. Press Enter and try again...\n");
				waitForEnter();
			}
		} while(isValid == 0);
		clearScreen();

		// Draw the line and save
		drawLine(matrix, num, i0, j0, i1, j1, N); 
		saveMove(i0, j0, i1, j1, num, numberLocations, listMoves, movesIndex, N);
		i0 = i1;
		j0 = j1;

		// Check for number connections
		if (connectedCheck(num, numberLocations)) {
			printf("Numbers connected! Press Enter to continue...\n");
			numberLocations[num-1][4] = 0;
			se = 1;
			waitForEnter();
		} 
		else {
			drawBoard(matrix, N);
			do {
				printf("Press \"0\" to select another target from the current position.\nPress \"1\" to exit and undo.\n");
				printf("Selection: ");
				scanf("%d", &se);
			} while (se != 0 && se != 1);
			if (se == 1) {
				moveUndo(matrix, numberLocations, listMoves, movesIndex, N);
			}
		}
	} while (se != 1);
}

void gameLoopManual(int matrix[][MAX], int numberLocations[][MAX], int *movesIndex, int listMoves[][MAX], int N)
{
	int se;

	do {
		// Operation selection
		clearScreen();
		drawBoard(matrix, N);
		do {
			printf("0 -> Move\n1 -> Undo\n2 -> Exit\nSelection: ");
			scanf("%d", &se);
		} while (!(se == 0 || se == 1 || se == 2)); 

		switch (se)
		{
			case 0:
				manualMove(matrix, numberLocations, movesIndex, listMoves, N);
				break;

			case 1:
				if (*movesIndex == 0) {
					printf("No moves have made. Unable to undo. Press Enter to continue...");
					waitForEnter();
				} else {
					moveUndo(matrix, numberLocations, listMoves, movesIndex, N);
					printf("Undo complete...");
					waitForEnter();
				}
				break;

			case 2: 
				printf("Press Enter to return to the menu...");
				waitForEnter();
				break;
		}
	} while (se != 2);
}	

void moveUndo(int matrix[][MAX], int numberLocations[][MAX], int listMoves[][MAX], int *movesIndex, int N)
{
	int i0, j0, i1, j1, num;

	i0 = listMoves[*movesIndex][0];
	j0 = listMoves[*movesIndex][1];
	i1 = listMoves[*movesIndex][2];
	j1 = listMoves[*movesIndex][3];
	num = listMoves[*movesIndex][4];
	*movesIndex--;

	numberLocations[num-1][0] = i0;
	numberLocations[num-1][1] = j0;
	numberLocations[num-1][4] = 1;

	drawLine(matrix, 0, i1, j1, i0, j0, N);
	matrix[i0][j0] = num;
	matrix[i1][j1] = 0;
}
	
	
