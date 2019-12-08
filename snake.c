#include "snakeAPI.h"
#include "clientAPI.h"
#include "ret_type.h"
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int move; //manual
	



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

	char* messageFinal = "VICTRE";
	char* messageFinal2 = "de";

	
	//	ME
	
	//		MY COORDS
	int myPOSx;
	int myPOSy;
	int myLastPosx;
	int myLastPosy;

	//	HIM
	//		HIS COORDS
	int hisPOSx;
	int hisPOSy;
	int hisLastPosx;
	int hisLastPosy;

	int* directionsInterdites = (int*) (malloc(4 * sizeof(int)));
	directionsInterdites[0] = 0;
	directionsInterdites[1] = 0;
	directionsInterdites[2] = 0;
	directionsInterdites[3] = 0;
	
	int* myBody = (int*) (malloc(2* 200 * sizeof(int)));
	int* hisBody = (int*) (malloc(2* 200 * sizeof(int)));

	// int : ma taille
	int mySize = 1;
	int hisSize = 1;

	//int : conteur de tours
	int ctr = 0;

	// boolean : vaut 0 si le jeu doit s'arreter
	int gameON = 1;
	connectToServer("polydev.cia-polytech-sorbonne.fr", 8080, "ovni");

	// seed = 123
	waitForSnakeGame("SUPER_PLAYER difficulty=0 timeout=1000 seed=420 start=0", gameName, &sizeX, &sizeY, &nbWalls);
	int obstacles[sizeX*sizeY];
	

	printf("%d\n", sizeX);
//	myTurn != myTurn;	//IF START = 1
	
	// init pos joueur 0
	myPOSx = 2;
	myLastPosx = 2;
	myPOSy = sizeY / 2;
	myLastPosy = sizeY / 2;
	
	myBody[0] = myPOSx;	
	myBody[1] = myPOSy;

	// init pos joueur 1
	hisPOSx = sizeX - 3;
	hisLastPosx = sizeX - 3;
	hisPOSy = sizeY / 2;
	hisLastPosy = sizeY / 2;

	hisBody[0] = hisPOSx;	
	hisBody[1] = hisPOSy;

	for (int i = 2; i < 400; i++){myBody[i] = -1; hisBody[i] = -1;}

	
	// creation tableau murs
	walls = (int*) (realloc(walls, 4 * nbWalls * sizeof(int)));

	// = 1 si c'est mon tour, = 0 sinon
	myTurn = !getSnakeArena(walls); 

	myMove = 0;
	while(gameON){
		//printArena();

		if (myTurn)
		{
			printArena(); // so faz print na minha vez
			
			for (int i = 0; i < sizeX*sizeY; ++i)
			{
				obstacles[i]=0;
			}
			
			for (int i = 0; i < 4; ++i)
			{
				directionsInterdites[i] = 0;
			}
			
			//wall awarness
			for (int i = 0; i < 4*nbWalls; i+=2){

				if (myPOSx == walls[i] && walls[i] == walls[i-2] && myPOSy >= walls[i+1] && myPOSy <= walls[i+3])		//meme coord barriere && je suis a GAUCHE de la barriere && barriere verticale && je suis devant la barriere
				{
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


			// SELF-AWARENESS
			for (int i = 0; i < mySize*2; i+=2)
			{
				switch((myBody[i] - myPOSx) * (myBody[i+1] == myPOSy)){
					case -1:
						directionsInterdites[3] = 1;
						break;
					case 1:
						directionsInterdites[1] = 1;
						break;
					default:
						break;
				}
				switch((myBody[i+1] - myPOSy) * (myBody[i] == myPOSx)){
					case -1:
						directionsInterdites[0] = 1;
						break;
					case 1:
						directionsInterdites[2] = 1;
						break;
					default:
						break;
				}

				obstacles[myBody[i] + sizeX * myBody[i+1]] = 1;
			}

			// ENEMY-AWARENESS
			for (int i = 0; i < hisSize*2; i+=2)
			{
				switch((hisBody[i] - myPOSx) * (hisBody[i+1] == myPOSy)){
					case -1:
						directionsInterdites[3] = 1;
						break;
					case 1:
						directionsInterdites[1] = 1;
						break;
					default:
						break;
				}
				switch((hisBody[i+1] - myPOSy) * (hisBody[i] == myPOSx)){
					case -1:
						directionsInterdites[0] = 1;
						break;
					case 1:
						directionsInterdites[2] = 1;
						break;
					default:
						break;
				}

				obstacles[hisBody[i] + sizeX * hisBody[i+1]] = 1;
			}

			// DODGE WALLS
			if (myPOSy == 0){directionsInterdites[0] = 1;}
			if (myPOSx == 0){directionsInterdites[3] = 1;}
			if (myPOSy == sizeY - 1){directionsInterdites[2] = 1;}
			if (myPOSx == sizeX - 1){directionsInterdites[1] = 1;}

			// DETECT 1 SQUARE TRAPS
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy - 1)] || myPOSx == 0) && (obstacles[myPOSx + sizeX*(myPOSy - 2)] || myPOSy == 1) && (obstacles[myPOSx + 1 + sizeX*(myPOSy - 1)] || myPOSx == sizeX - 1))
			{
				directionsInterdites[0] = 1;	//NORTH
			}
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy + 1)] || myPOSx == 0) && (obstacles[myPOSx + sizeX*(myPOSy + 2)] || myPOSy == sizeY - 2) && (obstacles[myPOSx + 1 + sizeX*(myPOSy + 1)] || myPOSx == sizeX - 1))
			{
				directionsInterdites[2] = 1;	//SOUTH
			}
			if ((obstacles[myPOSx - 1 + sizeX*(myPOSy + 1)] || myPOSy == sizeY - 1) && (obstacles[myPOSx - 2 + sizeX*myPOSy] || myPOSx == 1) && (obstacles[myPOSx - 1 + sizeX*(myPOSy - 1)] || myPOSy == 0))
			{
				directionsInterdites[1] = 1;	//EAST
			}
			//if ((obstacles[myPOSx + 1 + sizeX*(myPOSy + 1)] || myPOSy == sizeY - 1) && (obstacles[myPOSx + 2 + sizeX*myPOSy] || myPOSx == sizeX - 2) && (obstacles[myPOSx + 1 + sizeX*(myPOSy - 1)] || myPOSy == 0))
			//{
			//	directionsInterdites[3] = 1;	//WEST
			//}


			while (directionsInterdites[myMove])
			{
				myMove += 3;
				if (myMove > 3)
				{
					myMove %= 4;
				}
				
				if (directionsInterdites[0] == directionsInterdites[1] == directionsInterdites[2] == directionsInterdites[3] == 1)
				{
					break;
				}
			}




/*
//manual
			scanf("%d", &move);
			switch(move){
				case 2: myMove = SOUTH; break;	
				case 4:	myMove = WEST; break;
				case 6:	myMove = EAST; break;
				case 8:	myMove = NORTH; break;
			}
*/
			switch(myMove){
				case NORTH: myLastPosy = myPOSy; myPOSy--; break;
				case WEST:	myLastPosx = myPOSx; myPOSx--; break;
				case EAST:	myLastPosx = myPOSx; myPOSx++; break;
				case SOUTH: myLastPosy = myPOSy; myPOSy++; break;
			}

			


			if (!(ctr % 20)){
				mySize++;
			}else{
				for (int i = 0; i < 2*(mySize - 1); i+=2){
					myBody[i] = myBody[i+2];
					myBody[i+1] = myBody[i+3];
				}
			}
			myBody[2*(mySize-1)] = myPOSx;
			myBody[2*(mySize-1) + 1] = myPOSy;

			//si le jeu termine a cause de mon move, j'ai perdu
			messageFinal = "DÉFAITE";
			messageFinal2;
			gameON = (ret == sendMove(myMove));
		}else{
			gameON = (ret == getMove(&hisMove));
			
			switch(hisMove){
				case NORTH: hisLastPosy = hisPOSy; hisPOSy--; break;
				case WEST:	hisLastPosx = hisPOSx; hisPOSx--; break;
				case EAST:	hisLastPosx = hisPOSx; hisPOSx++; break;
				case SOUTH: hisLastPosy = hisPOSy; hisPOSy++; break;
			}

			if (ctr % 20 == 1){
				hisSize++;
			}else{
				for (int i = 0; i < 2*(hisSize - 1); i+=2){
					hisBody[i] = hisBody[i+2];
					hisBody[i+1] = hisBody[i+3];
				}
			}
			hisBody[2*(hisSize-1)] = hisPOSx;
			hisBody[2*(hisSize-1) + 1] = hisPOSy;			

			//si le jeu termine a cause de son move, j'ai gagne
			messageFinal = "VICTOIRE";
		}
		myTurn = !myTurn;
		ctr ++;

	}
	printf("%s\n", messageFinal);

	//why he lost
	switch(hisMove){
		case 0: printf("NORTH\n"); 	break;
		case 1: printf("EAST\n");	break;
		case 2: printf("SOUTH\n");	break;
		case 3: printf("WEST\n");	break;
	}
	
	closeConnection();
	free(walls);
	return 0;
}