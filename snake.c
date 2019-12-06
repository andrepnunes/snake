#include "snakeAPI.h"
#include "clientAPI.h"
#include "ret_type.h"
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	//variables wait for snake game
	char* gameName;
	int sizeX;
	int sizeY;
	int nbWalls;

	// boolean : vaut 1 si c'est mon tour, vaut 0 sinon
	int myTurn;
	
	t_return_code ret = NORMAL_MOVE;
	t_move myMove;
	t_move myLastMove;//nonutilise
	t_move hisMove;
	
	// tableau murs
	int* walls = (int*) (malloc(sizeof(int)));

	char* messageFinal = "VICTOIRE";
	
	//mes coordonées : joueur 0
	int myPOSx;
	int myPOSy;
	int myLastPosx;
	int myLastPosy;

	int* directionsInterdites = (int*) (malloc(4 * sizeof(int)));
	directionsInterdites[0] = 0;
	directionsInterdites[1] = 0;
	directionsInterdites[2] = 0;
	directionsInterdites[3] = 0;
	
	int* myBody = (int*) (malloc(2* 100 * sizeof(int)));
	// int : ma taille
	int mySize = 1;

	//int : conteur de tours
	int ctr = 0;

	// boolean : vaut 0 si le jeu doit s'arreter
	int gameON = 1;
	//int move; //manual
	connectToServer("polydev.cia-polytech-sorbonne.fr", 8080, "ovni");

	// seed = 123
	waitForSnakeGame("RANDOM_PLAYER difficulty=2 timeout=1000 seed=1 start=0", gameName, &sizeX, &sizeY, &nbWalls);
	
	myPOSx = 2;
	myLastPosx = 2;
	myPOSy = sizeY/2;
	myLastPosx = sizeY/2;
/*
	for (int i = 0; i < 200; i+=2)
	{
		myBody[i] = myPOSx;	
		myBody[i+1] = myPOSy;	
	}
	*/
	printf("(%d, %d)\n", myPOSx, myPOSy);
	walls = (int*) (realloc(walls, 4 * nbWalls * sizeof(int)));

	myTurn = !getSnakeArena(walls); // = 1 si c'est mon tour, = 0 sinon
	printf("%d\n", nbWalls);

	myMove = 0;
	while(gameON){
		printArena();
		if (myTurn)
		{
			ctr ++;

			for (int i = 0; i < 4; ++i)
			{
				directionsInterdites[i] = 0;
			}
/*			
			for (int i = 0; i < 4*nbWalls; i+=4)
			{
				printf("(%d, %d) -> (%d, %d)\n", walls[i], walls[i+1], walls[i+2], walls[i+3]);
			}
			*/

			for (int i = 0; i < 4*nbWalls; i+=2)
			{
				//printf("%d %d %d %d =%d %d =%d \n" , myPOSx == walls[i], i%4, walls[i] == walls[i+2], myPOSy , walls[i+1], myPOSy , walls[i+3]);

				//printf("%d %d\n", myPOSx, myPOSy);
				if (myPOSx == walls[i] && walls[i] == walls[i-2] && myPOSy >= walls[i+1] && myPOSy <= walls[i+3])		//meme coord barriere && je suis a GAUCHE de la barriere && barriere verticale && je suis devant la barriere
				{
					printf("jfv\n");
					directionsInterdites[1] = 1;
				}else if (myPOSx == walls[i] && walls[i] == walls[i+2] && myPOSy >= walls[i+1] && myPOSy <= walls[i+3])		//meme coord barriere && je suis a DROITE de la barriere && barriere verticale && je suis devant la barriere
				{
					directionsInterdites[3] = 1;
				}
				if (myPOSy == walls[i+1] && walls[i] == walls[i-2] && myPOSx >= walls[i-1] && myPOSx <= walls[i+1])		//meme coord barriere && je suis a HAUT de la barriere && barriere verticale && je suis devant la barriere
				{
					directionsInterdites[2] = 1;
				}else if (myPOSy == walls[i+1] && walls[i] == walls[i+2] && myPOSx >= walls[i-1] && myPOSx <= walls[i+1])		//meme coord barriere && je suis a BAS de la barriere && barriere verticale && je suis devant la barriere
				{
					directionsInterdites[0] = 1;
				}

			}

			for (int i = 0; i < mySize*2; i+=2)
			{
				switch((myBody[i] - myPOSx) * (myBody[i+1] == myPOSy)){
					case -1:
						directionsInterdites[1] = 1;
					case 1:
						directionsInterdites[3] = 1;
				}
				switch((myBody[i+1] - myPOSy) * (myBody[i] == myPOSx)){
					case -1:
						directionsInterdites[0] = 1;
					case 1:
						directionsInterdites[2] = 1;
				}
			}
			if (myPOSy == 0){directionsInterdites[0] = 1;}
			if (myPOSx == 0){directionsInterdites[3] = 1;}
			if (myPOSy == sizeY - 1){directionsInterdites[2] = 1;}
			if (myPOSx == sizeX - 1){directionsInterdites[1] = 1;}

			for (int i = 0; i < 4; ++i)
			{
				printf("--%d", directionsInterdites[i]);
			}
			printf("%d\n", directionsInterdites[myMove]);
			while (directionsInterdites[myMove])
			{
				if (++myMove > 3)
				{
					myMove = 0;
				}
			}
			printf("%d\n", 4==4);
			switch(myMove){
				case NORTH: myLastPosy = myPOSy; myPOSy--; break;
				case WEST:	myLastPosx = myPOSx; myPOSx--; break;
				case EAST:	myLastPosx = myPOSx; myPOSx++; break;
				case SOUTH: myLastPosy = myPOSy; myPOSy++; break;
			}


			printf("(%d, %d)\n", myPOSx, myPOSy);
/*
manual
			scanf("%d", &move);
			switch(move){
				case 2: myMove = SOUTH; break;	
				case 4:	myMove = WEST; break;
				case 6:	myMove = EAST; break;
				case 8:	myMove = NORTH; break;
			}
*/

			gameON = (ret == sendMove(myMove));
			
			if (!ctr%400 && ctr){
				mySize++;
				myBody[2*mySize] = myLastPosx;
				myBody[2*mySize + 1] = myLastPosy;
			}else{
				for (int i = 0; i < 2*(mySize-1); i+=2)
				{
					myBody[i] = myBody[i+2];
					myBody[i+1] = myBody[i+3];
				}
				myBody[mySize-2] = myLastPosx;
				myBody[mySize-1] = myLastPosy;
			}
			
			
			messageFinal = "DÉFAITE";
		}else{
			// si gameON devient 0 (perdu) pendant le tour adv, je sors du while avec victoire
			gameON = (ret == getMove(&hisMove));
			messageFinal = "VICTOIRE";
		}
		myTurn = !myTurn;
	}
	printf("%s\n", messageFinal);
	closeConnection();
	free(walls);
	return 0;
}