#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXNAMELEN 20 //the max size of a player name string
#define STARTCARDS 4 //the amount of cards that players recieve in the beginning of the game
#define MAXTYPELEN 8 //the max size of a type name string
#define NUMOFTYPES 14 // the number of card types in the game
#define NUMOFCOLORS 4 //the number of card colors in the game

#define PLUS 10 //the number given to + type card
#define STOP 11 //the number given to STOP type card
#define CHANGE_D 12 //the number given to <-> type card
#define TAKI 13 //the number given to TAKI type card
#define CHANGE_C 14 //the number given to COLOR type card

struct Card//struct for card data type
{
	//card type
	int type;//1=1,2=2,3=3,4=4,5=5,6=6,7=7,8=8,9=9, Plus=10, Stop=11, Change_D=12, TAKI=13, Change_C=14.

	//card color
	char color;//Y=Yellow,B=Blue,R=Red,G=Green
};
typedef struct Card CARD;

struct Player //struct for player data type
{
	char name[MAXNAMELEN];//name of a player
	CARD* cards; //array of player's cards
	int amount_of_cards; //the amount of cards that the player has(logic size)
	int maxcards; //the physic size of the cards array
};
typedef struct Player PLAYER;

struct cardStats//struct for card statistics data type
{
	char type[MAXTYPELEN];//card type
	int frequency;//the amount of times that this type of card was given to player in the game
};
typedef struct cardStats CARDSTATS;


CARD randomizecard(); //randomizing gamimg card and 
//output: randomized gamimg card

CARD randomizefirstcard(); //randomizing first card in the game
//output: randomized gaming card but without special cards types

int getNumOfPlayers();//getting the number of players from the client
//output: number of player that will play the game

//input: players array and its logic size
void givestartcards(PLAYER arr[], int size);
//randomizing and giving 4 cards to each player

//input: players array and its logic size
void fillPlayersarr(PLAYER arr[], int size);//getting the name of each player from the client

//input: gamimg card
void printcard(CARD card);//printing game card

//input: a specific player
void printplayercards(PLAYER player);//printing all cards of the player

//input:players array, its logic size and array of card statistics
void thegame(PLAYER players[], int amount_of_players, CARDSTATS cardstats[]);//main function that runs the game (a part of this function may be recursive but i decided that it wont)

//input:logic size of array of cards, the Upper card and the array itself 
int playerchoice(int size, CARD uppercard, CARD cards[]);//getting the choice of a player
//output: player's choice in the game. 0-to take a card from deck, otherwise wich card to put in the middle

//input:logic size of cards array, players choice, gaming card, upper card.
int playerchoicehelper(int size, int choice, CARD card, CARD uppercard);//this function is an inner function for function playerchoice, this function does the rapeated parts of the playerchoice function
//output: 1-if the choice is valid, 0 if its not

//input: specific player pointer, index of a card in his deck
void removefromdeck(PLAYER* player, int index);//removing the card from the players deck

//input: specific player pointer, array of cards  statistics.
void addcard(PLAYER* player, CARDSTATS cardstats[]);//adding a random card to the players deck and changing the cards statistics accordingly

//input: current direction of the game circle, the index of the current player, amount of players in the game
int changedirection(char d, int currentplayer, int amount_of_players);
//output: the index of the next player who is suposed to make a turn

//input: current direction of the game circle, the index of the current player, amount of players in the game
int changedirectionstop(char d, int currentplayer, int amount_of_players);//this function is called only when the last player placed STOP card in the middle
//output: the index of the next player who is suposed to make a turn 

//input: pointer of a specific COLOR type card
void changecolorcard(CARD* colorcard);//the player should choose wich color he wants the card to be
//changing the color of the card to a color that client decided to give for it.

//input:pointer of a player, upper card, string for the winner name, pointer of an integer that stands for "is this player a winner?"
CARD takicard(PLAYER* player, CARD uppercard, char winnername[], int* iswinner);
//output:new uppercard ,enabeling loop where the player who putted taki card can put other cards, if the player is the winner the function declares him to be so.

//input:logic size of cards array, taki card color
int playerchoicetaki(int size, char color, CARD cards[]);
//output: the players choice when the upper card is taki. 0 to end the turn,otherwise the card that he wants to put in the middle

//input:logic size of cards array, players choice, gaming card, upper card color.
int playerchoicehelpertaki(int size, int choice, CARD card, char color);//this function is an inner function for function playerchoicetaki, this function does the rapeated parts of the playerchoice function
//output: 1-if the choice is valid, 0 if its not

//input: cards statistics array
void startgamestats(CARDSTATS cardstats[]);//initialization of the array

//input: cards statistics array
void printgamestatistics(CARDSTATS cardstats[]);//printing the game satistics

//input: cards statistics array and its size
void statisticsmergesort(CARDSTATS cardstats[], int size);//sorting the array by frequency with merge sort

//input: first statistics array and itss size, secound statistics array and its size and third statistics array
void mergestats(CARDSTATS a1[], int n1, CARDSTATS a2[], int n2, CARDSTATS res[]);//merging the first and the secount into the third.

//input: destination statistics array, resource statistics array and its size
void copystats(CARDSTATS dest[], CARDSTATS res[], int size);//copying the resource array values into destinatioon array.
void main()
{
	srand(time(NULL));
	int amount_of_players;//number of players
	PLAYER* playersarr = NULL;//players array
	CARDSTATS cardstats[NUMOFTYPES];//statistics array
	startgamestats(cardstats);//initializing statistics array

	printf("\n************  Welcome to TAKI game !!! ***********");

	amount_of_players = getNumOfPlayers();//getting the amount of players
	playersarr = (PLAYER*)malloc(amount_of_players * sizeof(PLAYER));//inidynamic allocation for the  players array
	if (playersarr == NULL)//checking the allocation
	{
		printf("ERROR! Memory allocation failed\n");
		return;
	}
	fillPlayersarr(playersarr, amount_of_players);//getting the players names
	givestartcards(playersarr, amount_of_players);//giving players 4 starting cards

	//calling for the main game function, the game is operated in this function
	thegame(playersarr, amount_of_players, cardstats);

	statisticsmergesort(cardstats, NUMOFTYPES);//sorting the statistics array
	printgamestatistics(cardstats);//printing the game statistics

	//freeing the dynamic arrays
	for (int i = 0; i < amount_of_players; i++)
	{
		free(playersarr[i].cards);
	}
	free(playersarr);
}
void statisticsmergesort(CARDSTATS cardstats[], int size)
{
	CARDSTATS* tmparr = NULL;
	if (size <= 1)
		return;

	statisticsmergesort(cardstats, size / 2);
	statisticsmergesort(cardstats + size / 2, size - size / 2);

	tmparr = (CARDSTATS*)malloc(size * sizeof(CARDSTATS));
	if (tmparr)
	{
		mergestats(cardstats, size / 2, cardstats + size / 2, size - size / 2, tmparr);
		copystats(cardstats, tmparr, size);
		free(tmparr);
	}
	else
	{
		printf("\nMemory allocation failure!");
		exit(1);
	}
}
void mergestats(CARDSTATS a1[], int n1, CARDSTATS a2[], int n2, CARDSTATS res[])
{
	int ind1 = 0;
	int ind2 = 0;
	int resind = 0;
	while ((ind1 < n1) && (ind2 < n2))
	{
		if (a1[ind1].frequency <= a2[ind2].frequency)
		{
			res[resind] = a1[ind1];
			ind1++;
		}
		else
		{
			res[resind] = a2[ind2];
			ind2++;
		}
		resind++;
	}
	while (ind1 < n1)
	{
		res[resind] = a1[ind1];
		ind1++;
		resind++;
	}
	while (ind2 < n2)
	{
		res[resind] = a2[ind2];
		ind2++;
		resind++;
	}

}
void copystats(CARDSTATS dest[], CARDSTATS res[], int size)
{
	for (int i = 0; i < size; i++)
	{
		dest[i] = res[i];
	}
}
void startgamestats(CARDSTATS cardstats[])
{
	strcpy(cardstats[0].type, "   1   ");
	strcpy(cardstats[1].type, "   2   ");
	strcpy(cardstats[2].type, "   3   ");
	strcpy(cardstats[3].type, "   4   ");
	strcpy(cardstats[4].type, "   5   ");
	strcpy(cardstats[5].type, "   6   ");
	strcpy(cardstats[6].type, "   7   ");
	strcpy(cardstats[7].type, "   8   ");
	strcpy(cardstats[8].type, "   9   ");
	strcpy(cardstats[9].type, "   +   ");
	strcpy(cardstats[10].type, " STOP  ");
	strcpy(cardstats[11].type, "  <->  ");
	strcpy(cardstats[12].type, " TAKI  ");
	strcpy(cardstats[13].type, " COLOR ");
	for (int i = 0; i < NUMOFTYPES; i++)
	{
		cardstats[i].frequency = 0;
	}

}
void printgamestatistics(CARDSTATS cardstats[])
{
	printf("\n\n************ Game Statistics ************");
	printf("\nCard # | Frequency");
	printf("\n__________________");
	for (int i = NUMOFTYPES - 1; i >= 0; i--)
	{
		printf("\n%s|    %d", cardstats[i].type, cardstats[i].frequency);
	}
}
int getNumOfPlayers()
{
	int amount_of_players;
	printf("\nPlease enter the number of players:");
	scanf("%d", &amount_of_players);
	//cheking if the choice is valid
	while (amount_of_players < 1)
	{
		printf("\nInvalid choice! Try again.:\n");
		printf("\nPlease enter the number of players:");
		scanf("%d", &amount_of_players);
	}
	return amount_of_players;
}
void fillPlayersarr(PLAYER arr[], int size)
{
	int count = 1;
	for (int i = 0; i < size; i++)
	{
		char name[MAXNAMELEN];
		printf("\nPlease enter the first name of player #%d:", count);
		scanf("%s", &name);
		strcpy(arr[i].name, name);
		count++;
	}
}
CARD randomizecard()
{
	CARD card;
	int type;
	int color;
	type = 1 + rand() % (int)NUMOFTYPES;//randomizig threw all card types
	card.type = type;
	if (type != (int)CHANGE_C)
	{
		color = rand() % (int)NUMOFCOLORS;
		switch (color)
		{
		case 0:
			card.color = 'R';//red
			break;
		case 1:
			card.color = 'B';//blue
			break;
		case 2:
			card.color = 'G';//green
			break;
		case 3:
			card.color = 'Y';//yellow
			break;
		default:
			break;
		}
	}
	else
	{
		card.color = 'C';
	}
	return card;
}

CARD randomizefirstcard()
{
	CARD card;
	int type;
	int color;
	type = 1 + rand() % 9;//randomizig threw only digit card types
	card.type = type;
	color = rand() % (int)NUMOFCOLORS;
	switch (color)
	{
	case 0:
		card.color = 'R';//red
		break;
	case 1:
		card.color = 'B';//blue
		break;
	case 2:
		card.color = 'G';//green
		break;
	case 3:
		card.color = 'Y';//yellow
		break;
	default:
		break;
	}
	return card;
}
void givestartcards(PLAYER arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		//initializing the cards array for each player
		arr[i].cards = NULL;
		arr[i].cards = (PLAYER*)malloc(STARTCARDS * sizeof(PLAYER));
		arr[i].amount_of_cards = STARTCARDS;
		arr[i].maxcards = STARTCARDS;
		if (arr[i].cards == NULL)//checking the allocation
		{
			printf("ERROR! Memory allocation failed\n");
			return;
		}
		for (int j = 0; j < STARTCARDS; j++)//giving each player 4 random cards
		{
			arr[i].cards[j] = randomizecard();
		}
	}
}
void printcard(CARD card)
{
	printf("\n*********");
	printf("\n*       *");
	switch (card.type)
	{
	case 10:
		printf("\n*   +   *");
		break;
	case 11:
		printf("\n*  STOP *");
		break;
	case 12:
		printf("\n*  <->  *");
		break;
	case 13:
		printf("\n*  TAKI *");
		break;
	case 14:
		printf("\n* COLOR *");
		break;
	default:
		printf("\n*   %d   *", card.type);
		break;
	}
	if (card.color != 'C')//c is an indicator to know if the card wasnt used by player in the game
	{
		printf("\n*   %c   *", card.color);
	}
	else
	{
		printf("\n*       *", card.color);
	}
	printf("\n*       *");
	printf("\n*********\n");
}
void printplayercards(PLAYER player)
{
	for (int i = 0; i < player.amount_of_cards; i++)
	{
		printf("\nCard #%d", i + 1);
		printcard(player.cards[i]);
	}
}
int playerchoicehelper(int size, int choice, CARD card, CARD uppercard)
{
	int goodchoice = 0;
	if ((choice >= 0) && (choice <= size))//cheking if the choice is in the range
	{
		if (choice == 0)
		{
			goodchoice = 1;
		}
		else
		{
			if ((card.type == (int)CHANGE_C) || ((card.type == uppercard.type) || (card.color == uppercard.color)))//changing if the move is legal
			{
				goodchoice = 1;
			}
		}
	}
	return goodchoice;
}
int playerchoice(int size, CARD uppercard, CARD cards[])
{
	int choice;
	int goodchoice = 0;
	printf("\nPlease enter 0 if you want to take a card from the deck\nor 1 - %d if you want to put one of your cards in the middle : ", size);
	scanf("%d", &choice);
	goodchoice = playerchoicehelper(size, choice, cards[(choice - 1)], uppercard);
	while (goodchoice != 1)//cheking if the choice is valid
	{
		if ((choice < 0) || (choice > size))
		{
			printf("\nInvalid choice! Try again.");
		}
		else if ((cards[(choice - 1)].type != (int)CHANGE_C) && ((cards[(choice - 1)].type != uppercard.type) && (cards[(choice - 1)].color != uppercard.color)))
		{
			printf("\nInvalid card! Try again.");
		}
		printf("\nPlease enter 0 if you want to take a card from the deck\nor 1 - %d if you want to put one of your cards in the middle : ", size);
		scanf("%d", &choice);
		goodchoice = playerchoicehelper(size, choice, cards[(choice - 1)], uppercard);
	}
	return choice;
}
void removefromdeck(PLAYER* player, int index)
{
	index--;
	for (int i = index; i < (*player).amount_of_cards; i++)
	{
		(*player).cards[i] = (*player).cards[i + 1];
	}
	(*player).amount_of_cards--;

}
void addcard(PLAYER* player, CARDSTATS cardstats[])
{
	if ((*player).maxcards != (*player).amount_of_cards)
	{
		(*player).cards[(*player).amount_of_cards] = randomizecard();
		(*player).amount_of_cards++;
	}
	else
	{
		(*player).maxcards++;
		(*player).cards = realloc((*player).cards, sizeof(CARD) * (*player).maxcards);//re allocation to add a card
		if ((*player).cards == NULL)//check
		{
			printf("ERROR! Memory allocation failed\n");
			return;
		}
		(*player).cards[(*player).amount_of_cards] = randomizecard(cardstats);
		(*player).amount_of_cards++;
	}
	cardstats[((int)(*player).cards[(*player).amount_of_cards - 1].type - 1)].frequency++;//adding to the statistics
}
int changedirection(char d, int currentplayer, int amount_of_players)
{
	if (d == 'R')//R stands for virtual direction of the game turns to the right,L for left
	{
		if (currentplayer == (amount_of_players - 1))
			currentplayer = 0;
		else
			currentplayer++;
	}
	else
	{
		if (currentplayer == 0)
			currentplayer = (amount_of_players - 1);
		else
			currentplayer--;
	}
	return currentplayer;
}
int changedirectionstop(char d, int cp, int amount_of_players)
{
	//changing direction but passing threw one turn
	int currentplayer = cp;
	if (d == 'R')
	{
		if (currentplayer == (amount_of_players - 1))
			currentplayer = 1;
		else if (currentplayer == (amount_of_players - 2))
			currentplayer = 0;
		else
			currentplayer++;
	}
	else
	{
		if (currentplayer == 0)
			currentplayer = (amount_of_players - 2);
		else if (currentplayer == 1)
			currentplayer = amount_of_players - 1;
		else
			currentplayer--;
	}
	return currentplayer;
}
void changecolorcard(CARD* colorcard)
{
	int num;
	printf("\nPlease enter your color choice:");
	printf("\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green");
	scanf("%d", &num);
	while ((num < 1) || (num > 4))//checking if the choice is valid
	{
		printf("\nInvalid color! Try again:");
		printf("\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green");
		scanf("%d", &num);
	}
	switch (num)//giving the chocen color to the card
	{
	case 1:
		(*colorcard).color = 'Y';
		break;
	case 2:
		(*colorcard).color = 'R';
		break;
	case 3:
		(*colorcard).color = 'B';
		break;
	case 4:
		(*colorcard).color = 'G';
		break;
	default:
		break;
	}
}
int playerchoicetaki(int size, char color, CARD cards[])
{
	int choice;
	int goodchoice = 0;
	printf("\nPlease enter 0 if you want to finish your turn\nor 1 - %d if you want to put one of your cards in the middle : ", size);
	scanf("%d", &choice);
	goodchoice = playerchoicehelpertaki(size, choice, cards[(choice - 1)], color);//getting valid choice
	while (goodchoice != 1)//cheking if the choice is legal
	{
		if ((choice < 0) || (choice > size))
		{
			printf("\nInvalid choice! Try again.");
		}
		else if ((cards[(choice - 1)].color != color))
		{
			printf("\nInvalid card! Try again.");
		}
		printf("\nPlease enter 0 if you want to finish your turn\nor 1 - %d if you want to put one of your cards in the middle : ", size);
		scanf("%d", &choice);
		goodchoice = playerchoicehelpertaki(size, choice, cards[(choice - 1)], color);//getting valid choice
	}
	return choice;
}
int playerchoicehelpertaki(int size, int choice, CARD card, char color)
{
	int goodchoice = 0;
	if ((choice >= 0) && (choice <= size))//cheking if the choice is in the range
	{
		if (choice == 0)
		{
			goodchoice = 1;
		}
		else
		{
			if (card.color == color)//cheking if the choice is legal
			{
				goodchoice = 1;
			}
		}
	}
	return goodchoice;
}
CARD takicard(PLAYER* player, CARD uppercard, char winnername[], int* iswinner)
{
	CARD newuppercard = uppercard;
	int choice;
	printf("\n%s's turn:\n", (*player).name);
	printplayercards(*(player));
	choice = playerchoicetaki((*player).amount_of_cards, uppercard.color, (*player).cards);//getting the choice
	while (choice != 0)
	{
		newuppercard = (*player).cards[(choice - 1)];//getting the new upper card from the players deck
		removefromdeck(player, choice);
		printf("\nUpper card:");
		printcard(newuppercard);
		if ((*player).amount_of_cards == 0)//cheking if the player won
		{
			strcpy(winnername, (*player).name);//setting his name as a winner name
			(*iswinner) = 1;//letting the thegame function know that there is a winner and it can break the game loop
			break;

		}
		printf("\n%s's turn:\n", (*player).name);
		printplayercards(*(player));
		choice = playerchoicetaki((*player).amount_of_cards, uppercard.color, (*player).cards);
	}
	return newuppercard;
}
void thegame(PLAYER playersarr[], int amount_of_players, CARDSTATS cardstats[])
{
	char direction = 'R';//initializing the game standard direction of turns
	int loop = -1;//setting the loop condition
	int currentplayer = 0;//initializing the index of the current player who has the turn
	int stopactive = 0;//initializng the check integer for stop card
	int plusactive = 0;//initializng the check integer for + card
	int takiactive = 0;//initializng the check integer for TAKI card
	int iswinner = 0;////initializng the check integer for is there a winner
	char* winnername[MAXNAMELEN];//declaration of winner variable string
	CARD uppercard = randomizefirstcard();//generating the first card for the game

	printf("\nUpper card:");
	printcard(uppercard);

	while (loop == -1)//entering the game loop. iteretion for each turn
	{
		if (uppercard.type == TAKI)//if the upper  card is taki(can happen only mid game) the turn is different
		{
			uppercard = takicard(&playersarr[currentplayer], uppercard, winnername, &iswinner);
		}
		else
		{

			int choice;//declaration for players choice in his turn
			printf("\n%s's turn:\n", playersarr[currentplayer].name);
			printplayercards(playersarr[currentplayer]);//printing players cards
			choice = playerchoice(playersarr[currentplayer].amount_of_cards, uppercard, playersarr[currentplayer].cards);//getting players choice
			if (choice == 0)//if choice is 0 giving him new card from the deck
			{
				addcard(&playersarr[currentplayer], cardstats);
			}
			else
			{
				uppercard = playersarr[currentplayer].cards[(choice - 1)];//setting the uppercard to be the card the player chose to put in the middle
				removefromdeck(&playersarr[currentplayer], choice);//removing this card from players deck
				if (playersarr[currentplayer].amount_of_cards == 0)//cheking if the player won
				{
					strcpy(winnername, playersarr[currentplayer].name);
					break;

				}
				switch (uppercard.type)
				{
				case PLUS:
					plusactive = 1;//if the card the player chose is plus the setting is active
					break;
				case STOP:
					stopactive = 1;//if the card the player chose is stop the setting is active
					break;
				case CHANGE_D://if the player chose to change the direction of the game
					if (direction == 'R')//we change the direction according to active direction
						direction = 'L';
					else
						direction = 'R';
					break;
				case TAKI:
					printf("\nUpper card:");
					printcard(uppercard);
					uppercard = takicard(&playersarr[currentplayer], uppercard, winnername, &iswinner);//if the player chose taki card we will enter to the taki turn loop
					break;
				case CHANGE_C:
					changecolorcard(&uppercard);//chaging the color of the COLOR card that the player chose to put in the middle
					break;
				default:
					break;
				}

			}

		}
		if (iswinner == 1)//cheking if there a winner (it may accure after the taki loop)
		{
			break;
		}
		if (plusactive != 1)//if the plus setting active the next turn will last on the same player
		{
			if (amount_of_players != 1)
			{
				if (stopactive == 1)
				{
					currentplayer = changedirectionstop(direction, currentplayer, amount_of_players);
					stopactive = 0;
				}
				else
				{
					currentplayer = changedirection(direction, currentplayer, amount_of_players);
				}
			}
		}
		else
		{
			plusactive = 0; //going back to standard setting
		}
		printf("\nUpper card:");
		printcard(uppercard);

	}
	printf("\n\nThe winner is... %s! Congratulations!", winnername);


	//P.s it was fun thing to make but very hard to QA.
}