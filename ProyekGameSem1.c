#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define NUM_CARDS 52
#define HAND_SIZE 5
#define INITIAL_MONEY 1000
#define SCOREBOARD_SIZE 60

struct ScoreBoard{
	char name[20];
	float score;
};

void binarySort(struct ScoreBoard arr[], int n) {
    int i, j;
    struct ScoreBoard temp;
    int left, right, mid;

    for (i = 1; i < n; i++) {
        temp = arr[i];
        left = 0;
        right = i - 1;

         while (left <= right) {
            mid = (left + right) / 2;
            if (arr[mid].score > temp.score) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        for (j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }

        arr[left] = temp;
    }
}

int findRankByName(struct ScoreBoard arr[], int n, const char *searchName) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].name, searchName) == 0) {
            return i + 1; 
        }
    }
    return -1; 
}

void initializeDeck(int *deck) {
    for (int i = 0; i < NUM_CARDS; i++) {
        deck[i] = i;
    }
}

void displayRandomCardDuringGame(int *deck) {
    int randomCard = rand() % NUM_CARDS;
    printf("Initial card: ");
    printCard(deck[randomCard]);
}

void shuffleDeck(int *deck) {
    for (int i = NUM_CARDS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void printCard(int card) {
    char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    printf("%s of %s\n", ranks[card % 13], suits[card / 13]);
}

int getCardValue(int card) {
    int rank = card % 13; 
    if (rank >= 10) {
        return 10; 
    } else {
        return rank + 1; 
    }
}

int calculateTotalPoints(int *hand) {
    int totalPoints = 0;
    for (int i = 0; i < HAND_SIZE; i++) {
        totalPoints += getCardValue(hand[i]);
    }
    return totalPoints;
}

void dealCards(int *deck, int numCards, int *hand) {
    for (int i = 0; i < numCards; i++) {
        hand[i] = deck[i];
    }
}

int botAction(int botMoney, int playerBetAmount) {
    int botDecision = rand() % 4 + 1;
    int botBetAmount, botRaiseAmount; 

    switch (botDecision) {
        case 1:
            break;
        case 2:
            botBetAmount = rand() % (botMoney - playerBetAmount + 1);
            return botBetAmount;
        case 3:
            return -1; 
        case 4:
            botRaiseAmount = rand() % (botMoney - playerBetAmount + 1);
            return botRaiseAmount;
        default:
            break;
    }
    return 0;
}

int main() {
    int deck[NUM_CARDS];
    int playerHand[HAND_SIZE];
    int botHand[HAND_SIZE];
    int playerMoney = INITIAL_MONEY;
    int botMoney = INITIAL_MONEY;
    int betAmount = 0;
    bool gameOver = false;
    char playerName[50];
    char searchName[100];
    int haiya = 0;
    
    srand(time(NULL));
    
    struct ScoreBoard ScoreBoard[SCOREBOARD_SIZE]; 
    int i = 0; 

    FILE *fp;
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    
	printf("Enter your name: ");
	fgets(playerName, sizeof(playerName), stdin);
    playerName[strcspn(playerName, "\n")] = '\0';
    system("cls");
	
    int menuChoice;
    do {
        printf("\tMain Menu\n");
        printf("--------------------------\n");
        printf("1. Play\n");
        printf("2. Rules\n");
        printf("3. Score Board\n");
        printf("4. Enter Score\n");
        printf("5. Exit\n\n");
        
        printf("Welcome to the game %s!\n", playerName);
        printf("Enter your choice: ");
        scanf("%d", &menuChoice);
        getchar();

        system("cls");

        switch (menuChoice) {
            case 1:
                for (int i = 5; i > 0; i--) {
                    printf("Game is Starting in %d\n", i);
                    Sleep(1000);
                }

                printf("Game Starts!\n");
                Sleep(2000);
                system("cls");

              	playerMoney = INITIAL_MONEY;
                botMoney = INITIAL_MONEY;
               	gameOver = false;


                while (playerMoney > 0 && botMoney > 0 && !gameOver) {

                	initializeDeck(deck);
                	shuffleDeck(deck);
                	dealCards(deck, HAND_SIZE, playerHand);
               		dealCards(deck + HAND_SIZE, HAND_SIZE, botHand);
                	
        			printf("%s money: %d\n", playerName, playerMoney);
       				printf("Bot's money: %d\n\n", botMoney);

        			printf("Your hand:\n");
        			for (int i = 0; i < HAND_SIZE; i++) {
            			printCard(playerHand[i]);
        			}
        			
        			displayRandomCardDuringGame(deck);

        			printf("\nDo you want to (1) Call, (2) Bet, (3) Fold\n");
        			int choice;
        			scanf("%d", &choice);

        			switch (choice) {
            	case 1:
                		break;
            	case 2:
                	printf("Enter bet amount: ");
                	scanf("%d", &betAmount);
                	if (betAmount > playerMoney) {
                    	printf("You don't have enough money!\n");
                    	return 1;
                	}
                	playerMoney -= betAmount;
                	break;
            	case 3:
                	printf("You folded.\n");
                	haiya = 1;
                	break;
            default:
                printf("Invalid choice!\n");
                return;
        }
        
        // Bot action
        printf("\nBot's turn.\n");
        int botDecision = botAction(botMoney, betAmount);
        switch (botDecision) {
            case -1:
                printf("Bot folded.\n");
                haiya = 0;
                break;
            case 0:
                printf("Bot called.\n");
                break;
            default:
                printf("Bot bets: %d\n", botDecision);
                if (botDecision > botMoney) {
                    printf("Bot does not have enough money to bet. Bot calls.\n");
                    botDecision = botMoney;
                }
                
                botMoney -= botDecision;
                break;
        }
        
        if (!gameOver) {
            printf("\nBot's hand:\n");
            for (int i = 0; i < HAND_SIZE; i++) {
                printCard(botHand[i]);
                }
    
	              
    int playerPoints = calculateTotalPoints(playerHand);
    int botPoints = calculateTotalPoints(botHand);

    if (playerPoints > botPoints) {
    	printf("%s wins the round!\n", playerName);
    	if (haiya == 1) {
        	playerMoney += betAmount; 
    	} else if (haiya == 0) {
        	botMoney += betAmount; 
    	} else {
        	playerMoney += betAmount * 2;
    	}
    
	} else if (playerPoints < botPoints) {
   		printf("Bot wins the round!\n");
    	if (haiya == 1) {
        	playerMoney += betAmount; 
    	} else if (haiya == 0) {
        	botMoney += betAmount; 
    	} else {
        	botMoney += betAmount * 2; 
    	}
    	
	} else {
    	printf("It's a tie for this round!\n");
    	playerMoney += betAmount;
	}
        }
                    Sleep(3000);
                    system("cls");

                    if (playerMoney <= 0 || botMoney <= 0) {
                        gameOver = true;
                    }
                }
                	if (playerMoney <= 0) {
                    	printf("You have run out of money. Bot wins!\n");
                	} else if (botMoney <= 0) {
                    	printf("Bot has run out of money. You win!\n");
                	}
                	
                	printf("Returning to Main Menu...!\n");
                	Sleep(3000);
                    system("cls");

                break;
            
            case 2:
                printf("\n");
                printf("In this game, Player has 5 cards in hand\n");
                printf("Here are the various combinations, classified in increasing order by value:\n");
                printf("Ace, Jack, Queen, and King consist of value 10\n");
                printf("Meanwhile the number resembles the same value\n");
                printf("Add all the value together of the card and compare to the bot\n");

                printf("\n");
                printf("If a player has a better combination than the bot, they win\n");
                return 0;

            case 3:
            	fp = fopen("ScoreBoard.txt", "r");
            	printf("\tTOP Scoreboard\n");
    			printf("RANK %-20s %-10s\n", "NAME", "SCORE");
   	 			printf("----------------------------------\n");

    			 while (i < SCOREBOARD_SIZE && fscanf(fp, "%s %f\n", ScoreBoard[i].name, &ScoreBoard[i].score) != EOF) {
        		i++;
    			}
    			
    			binarySort(ScoreBoard, i);

    			for (int j = 0; j < i; j++) {
        			printf("%-4d %-20s %.2f\n", j + 1, ScoreBoard[j].name, ScoreBoard[j].score);
    			}	
				
        
    		printf("\nEnter a name to search: ");
    		scanf("%s", searchName);

    		int rank = findRankByName(ScoreBoard, 10, searchName);
    		if (rank != -1) {
        		printf("'%s' is ranked #%d\n", searchName, rank);
    		} else {
        		printf("Name '%s' not found in the scoreboard\n", searchName);
    		}

    		fclose(fp);
			return 0;
            
            
            case 4:
            fp = fopen("ScoreBoard.txt", "a"); 

    		if (fp == NULL) {
        		printf("Error opening file\n");
        		exit(1);
    		}
    			
    		struct ScoreBoard newEntry;

    		printf("Enter name: ");
    		scanf("%s", newEntry.name);

    		printf("Enter score: ");
    		scanf("%f", &newEntry.score);

    		fprintf(fp, "%s %.2f\n", newEntry.name, newEntry.score);
    		fclose(fp); 

    		return 0;
            break;
                
            case 5:
            	printf("Exiting the game...\n");
                break;
                
            default:
                printf("Invalid choice. Please select again.\n");
                break;
        }
    } while (menuChoice != 5);

    return 0;
}

