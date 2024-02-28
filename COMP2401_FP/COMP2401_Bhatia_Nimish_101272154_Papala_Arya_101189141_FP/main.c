#include "defs.h"

int main(int argc, char *argv[])
{
     	// Initialize a random seed for the random number generators
       	srand(time(NULL));
	
       	HouseType * house = (HouseType*)calloc(1, sizeof(HouseType));
       	initHouse(house);
       	populateRooms(house);

       	// Ghost
       	GhostType * ghost;
       	RoomType * ghostSpawnPoint = randRoom(house -> rooms, C_TRUE);

	char * ghostName;
	GhostClassType ghostClass;
	
	// Select a random ghost class (along with a name)
	int ghostSelector = randInt(1, 4);
	if (ghostSelector == 1) {
		ghostClass = POLTERGEIST;
		ghostName = "Poltergeist";
	} else if ( ghostSelector == 2) {
		ghostClass = BANSHEE;
		ghostName = "Banshee";
	} else if (ghostSelector == 3) {
		ghostClass = BULLIES;
		ghostName = "Bullies";
	} else if (ghostSelector == 4) {
		ghostClass = PHANTOM;
		ghostName = "Phantom";
	}
	
	// Initialize the ghost	
       	initGhost(ghostClass, ghostSpawnPoint, &ghost, house);

	char hunter1Name[MAX_STR] = {'\0'};;
	char hunter2Name[MAX_STR] = {'\0'};;
	char hunter3Name[MAX_STR] = {'\0'};;
	char hunter4Name[MAX_STR] = {'\0'};;
	
	// Get the hunter names from the user
	getHunterNames(hunter1Name, hunter2Name, hunter3Name, hunter4Name);
	
	HunterType * hunter1, * hunter2, * hunter3, * hunter4;					// Pointer to hunters
	RoomType * van = house -> rooms -> head -> roomData;					// Pointer to the van
	
	// Initialize the 4 hunters
	initHunter(EMF, hunter1Name, van, &hunter1, 0);
	initHunter(TEMPERATURE, hunter2Name, van, &hunter2, 1);
	initHunter(FINGERPRINTS, hunter3Name, van, &hunter3, 2);
	initHunter(SOUND, hunter4Name, van, &hunter4, 3);
	
	// Assign hunters to the van and to the house as well
	HunterArrayType * vanHunters = van -> hunters;						// Pointer to the hunters in the van
	vanHunters -> hunters[0] = hunter1; house -> hunters -> hunters[0] = hunter1;
	vanHunters -> hunters[1] = hunter2; house -> hunters -> hunters[1] = hunter2;
	vanHunters -> hunters[2] = hunter3; house -> hunters -> hunters[2] = hunter3;
	vanHunters -> hunters[3] = hunter4; house -> hunters -> hunters[3] = hunter4;
	
       	// Assign the ghost to the house
	house -> ghost = ghost;
	
	// Make 5 threads (4 For the hunters, 1 for the ghost)
	pthread_t ht1, ht2, ht3, ht4, gt1;

	printf("The mystery crew pull up to the haunted house. Time for some ghostbusting!\n\n");
	
	// Create threads for the hunters and ghost
	pthread_create(&ht1, NULL, hunterAction, hunter1);
	pthread_create(&ht2, NULL, hunterAction, hunter2);
	pthread_create(&ht3, NULL, hunterAction, hunter3);
	pthread_create(&ht4, NULL, hunterAction, hunter4);
	pthread_create(&gt1, NULL, ghostAction, ghost);
	
	// Join the threads
	pthread_join(ht1, NULL);
	pthread_join(ht2, NULL);
	pthread_join(ht3, NULL);
	pthread_join(ht4, NULL);
	pthread_join(gt1, NULL);
	
	// If all 4 hunters are too scared or bored:
	if ((hunter1 -> fear >= MAX_FEAR || hunter1 -> boredom <= 0) 
			&& (hunter2 -> fear >= MAX_FEAR || hunter2 -> boredom <= 0) 
				&& (hunter3 -> fear >= MAX_FEAR || hunter3 -> boredom <= 0) 
					&& (hunter4 -> fear >= MAX_FEAR || hunter4 -> boredom <= 0)) {
		//printf("All the hunters have gotten bored or been frightened off by the %s!\n", ghostName);
		if(LOGGING){
			l_hunterExit(hunter1->name, LOG_FEAR);
			l_hunterExit(hunter2->name, LOG_FEAR);
			l_hunterExit(hunter3->name, LOG_FEAR);
			l_hunterExit(hunter4->name, LOG_FEAR);
		}
	} else {
		printf("Your hunters have found out that the ghost was a %s type and won!\n", ghostName);
		if (hunter1 -> evidence -> size == 3) {
			typeCalculator(hunter1 -> evidence);
			if(LOGGING){
				l_hunterReview(hunter1 -> name, LOG_SUFFICIENT);				
			}
		} else if (hunter2 -> evidence -> size == 3) {
			typeCalculator(hunter2 -> evidence);
			if(LOGGING){
				l_hunterReview(hunter2 -> name, LOG_SUFFICIENT);
			}
		} else if (hunter3 -> evidence -> size == 3) {
			typeCalculator(hunter3 -> evidence);
			if(LOGGING){
				l_hunterReview(hunter3 -> name, LOG_SUFFICIENT);
			}
		} else if (hunter4 -> evidence -> size == 3) {
			typeCalculator(hunter4 -> evidence);
			if(LOGGING){
				l_hunterReview(hunter4 -> name, LOG_SUFFICIENT);
			}

		}	
	}
	
	// Clean up the house and free it from memory	
	cleanupHouse(house);
	free(house);
    	return 0;
}

/*
  Function: void getHunterNames(char *hunter1Name, char *hunter2Name, char *hunter3Name, char *hunter4Name)
  Purpose:  Get names for four hunters from the user.
  Params:   hunter1Name - char pointer to store the name of hunter 1
            hunter2Name - char pointer to store the name of hunter 2
            hunter3Name - char pointer to store the name of hunter 3
            hunter4Name - char pointer to store the name of hunter 4
  Output:   Void
*/
void getHunterNames(char * hunter1Name, char * hunter2Name, char * hunter3Name, char * hunter4Name) {
	printf("Enter hunter 1's name: ");
	scanf("%s",hunter1Name);

	printf("Enter hunter 2's name: ");
	scanf("%s",hunter2Name);
		
	printf("Enter hunter 3's name: ");
	scanf("%s",hunter3Name);

	printf("Enter hunter 4's name: ");
	scanf("%s",hunter4Name);	
}

/*
  Function: void typeCalculator(EvidenceListType *evidenceList)
  Purpose:  Calculate and print the type of ghost based on the evidence collected by hunters.
  Params:   evidenceList - pointer to the EvidenceListType containing evidence collected by hunters
  Output:   Void
*/
void typeCalculator(EvidenceListType * evidenceList) {

	EvidenceNodeType * currNode = evidenceList -> head;
		
	int poltergeist = 0, banshee = 0, bullies = 0, phantom = 0;
	char * ghostType;

	// Keep looping till you find a matching index
	while (currNode != NULL){
		if (currNode -> evidenceData -> type == EMF) {
			poltergeist++; banshee++; bullies++;
		}
		if (currNode -> evidenceData -> type == TEMPERATURE) {
			poltergeist++; banshee++; phantom++;
		}
		if (currNode -> evidenceData -> type == FINGERPRINTS) {
			poltergeist++; bullies++; phantom++;
		}
		if (currNode -> evidenceData -> type == SOUND) {
			banshee++; bullies++; phantom++;
		}
		currNode = currNode -> next;
	}
	if (poltergeist >= 3) ghostType = "Poltergeist";
	if (banshee >= 3) ghostType = "Banshee";
	if (bullies >= 3) ghostType = "Bullies";
	if (phantom >= 3) ghostType = "Phantom";
		
	printf("With careful teamwork and great detective skills, the hunters discovered that the ghost was really a %s!\n", ghostType);
	
}

/*
  Function:  hunterAction
  Purpose:   processes a hunter's action during his turn via threading
       in:   void pointer to the thread
   return:   a hunter pointer with updated data regarding his location, evidence array, etc
*/
void * hunterAction (void * hunter) {

	HunterType * gameHunter = (HunterType*)hunter;

    	while (C_TRUE) {
        	int actionChoice;

        	if (gameHunter -> room -> ghost != NULL) {
        		//printf("%s feels a chill run down their spine.\n", gameHunter -> name);
            	gameHunter -> fear += FEAR_RATE;
        	}
        	if (gameHunter -> room -> hunters -> size > 1) {
            		actionChoice = randInt(1,3);
        	} else {
            		actionChoice = randInt(1,2);
        	}

        	if (actionChoice == 1) {
        		moveHunter(gameHunter);

        	} else if (actionChoice == 2) {
			if (sem_wait(&gameHunter -> room -> mutex) == 0) {
				sem_wait(&gameHunter -> evidence -> mutex);
				collectEvidence(gameHunter);
				sem_post(&gameHunter -> evidence -> mutex);
				sem_post(&gameHunter -> room -> mutex);
			} else {
				printf("%s's readings get drowned out by static.\n", gameHunter -> name);
			}
        	} else if (actionChoice == 3) {
        		shareEvidence(gameHunter);
        	}


		if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
			int endGame = endGameConditions(gameHunter);
			if (endGame == C_TRUE) {
				break;
			}
        	}

        	usleep(USLEEP_TIME);
    	}
    	return 0;
}

/*
  Function:  ghostAction
  Purpose:   processes a ghost's action during it's turn via threading
       in:   void pointer to the thread
   return:   a ghost pointer with updated data regarding it's location, dropping evidence in a room, etc
*/
void * ghostAction (void * ghost) {
	GhostType * gameGhost = (GhostType*)ghost;
	
    	while (C_TRUE) {
        	int actionChoice;
	
        	if (gameGhost -> room -> hunters -> size == 0) {
        		gameGhost -> boredom -= BOREDOM_RATE;
        	} else {
            		gameGhost -> boredom = BOREDOM_MAX;
        	}


        	if (gameGhost -> room -> hunters -> size >= 1) {
            		actionChoice = randInt(1,2);
        	} else {
	            	actionChoice = randInt(1,3);
        	}

        	if (actionChoice == 1) {
			if(sem_wait(&gameGhost -> room -> mutex) == 0) {
				addGhostEvidence(gameGhost);
				sem_post(&gameGhost -> room -> mutex);
			} else {
				printf("An ominous presence looms over the %s.\n", gameGhost -> room -> name);
			}
        	} else if (actionChoice == 2) {
            		printf("The ghost twiddles its thumbs.\n");
        	} else if (actionChoice == 3) {
        		if (sem_trywait(&gameGhost -> room -> mutex) == 0) {
        			
				// Check the rooms that are attached to the room the ghost is currently in:
				RoomListType * roomList = gameGhost -> room -> attached;
				RoomType * newRoom = randRoom(roomList, C_FALSE);
	
				if (sem_trywait(&newRoom -> mutex) == 0) {
		
					gameGhost -> room -> ghost = NULL;
					sem_post(&gameGhost -> room -> mutex);
					// Change the ghost's room pointer to a random room from that room's connected rooms
					gameGhost -> room = newRoom;
					gameGhost -> room -> ghost = gameGhost;
					if(LOGGING){
						l_ghostMove(newRoom -> name);
					}else{
						printf("The ghost dissaparates from the %s and appears in the %s.\n", gameGhost -> room -> name, newRoom -> name);					
					}
		
				} else {
					printf("The ghost peeks through the wall of the %s andinto the %s.\n", gameGhost -> room -> name, newRoom -> name);
				}
        			
        			sem_post(&gameGhost -> room -> mutex);
        		} else {
				printf("The ghost contemplates leaving the %s, but decides that it's comfy where it is.\n", gameGhost -> room -> name);
        		}
        	} else {
        		printf("Error\n");
        	}


        	if (gameGhost -> boredom <= 0) {
        		printf("\n\nThe ghost has gotten bored and exited the house!\n\n\n");
            		break;            
        	}

        	usleep(USLEEP_TIME);
    	}
    	return 0;
}

/*
  Function:  moveHunter
  Purpose:   Moves a hunter from one location to the next
       in:   pointer to a HunterType
   return:   an updated pointer to a HunterType with a new location
*/
void moveHunter(HunterType * gameHunter) {
	if (sem_trywait(&gameHunter -> room -> mutex) == 0) {
		// Check the rooms that are attached to the room the hunter is currently in:
		RoomListType * roomList = gameHunter -> room -> attached;
		RoomType * newRoom = randRoom(roomList, C_FALSE);
	
		if (sem_trywait(&newRoom -> mutex) == 0) {	
			// Remove hunter from linked list
			removeHunterFromRoom(gameHunter, gameHunter -> room);
	
			sem_post(&gameHunter -> room -> mutex);
				

			// Change the hunters's room pointer to a random room from that room's connected rooms
			gameHunter -> room = newRoom;

			// Add hunter to the linked list
			addHunterToRoom(gameHunter, gameHunter -> room);
			if(LOGGING){
				l_hunterMove(gameHunter -> name, newRoom -> name);		
			}else{
				printf("%s leaves the %s and moves into the %s.\n", gameHunter -> name, gameHunter -> room -> name, newRoom -> name);
			}
		}
		sem_post(&gameHunter -> room -> mutex);
	} else {
		printf("%s Do Nothing !\n", gameHunter -> name);
	}
}

/*
  Function:  shareEvidence
  Purpose:   Share the input hunter's evidence with another random hunter in the same room
       in:   a pointer to a HunterType
   return:   a randomly chosen second HunterType with evidence inside of it's evidence list
*/
void shareEvidence(HunterType * gameHunter) {
	if (sem_wait(&gameHunter -> room -> mutex) == 0) {
		sem_wait(&gameHunter -> evidence -> mutex);
		// Note: Only transfer evidence that is not in standard values
		HunterArrayType * huntersInRoom = gameHunter -> room -> hunters;		// An array of hunters currently in the room
		int numOfHuntersInRoom = huntersInRoom -> size;					// The number of hunters currently in the room
		int numOfEvidence = gameHunter -> evidence -> size;				// The number of evidence in the hunter's evidence list
	
		// If there are at least two hunters in the room:
		if (numOfHuntersInRoom >= 2 && gameHunter -> evidence > 0){
			// Choose a second random hunter by index
			int otherHunterIndex = randInt(1,numOfHuntersInRoom);			// A randomly chosen index in the hunter's array
    			int count = 0;

			HunterType * currHunter;
		
			// In a for loop, transfer all evidence to the other hunter's evidence list
			for (int i = 0; i < numOfHuntersInRoom; i++){
				currHunter = huntersInRoom -> hunters[i];
				//If they are not the same hunter
				if (currHunter == NULL) {
					count++;
				} else if (currHunter -> id != gameHunter -> id){
					count++;
					if (count == otherHunterIndex && otherHunterIndex <= numOfHuntersInRoom) {
						HunterType * otherHunter = huntersInRoom -> hunters[i];
						sem_wait(&otherHunter -> evidence -> mutex);
						EvidenceNodeType * evidenceNode = gameHunter -> evidence -> head;
						for (int i = 0; i < numOfEvidence; i++){
							transferEvidenceData(otherHunter, evidenceNode -> evidenceData);
							evidenceNode = evidenceNode -> next;
						}
						printf("%s tells %s about some exciting evidence that they found.\n", gameHunter -> name, otherHunter -> name);
						sem_post(&otherHunter -> evidence -> mutex);
						break;
					} 
				}
			}
		} else {
			printf("%s Do nothing.\n", gameHunter -> name);
		}
		sem_post(&gameHunter -> evidence -> mutex);
		sem_post(&gameHunter -> room -> mutex);
	} else {
		printf("%s is overcome by social anxiety and doesn't want to talk to anyone.\n", gameHunter -> name);
	}
}

/*
  Function:  endGameConditions
  Purpose:   Checks if the conditions for the game to end are complete
       in:   a pointer to a HunterType
   return:   string print regarding the status of the game's end
*/
int endGameConditions(HunterType * gameHunter) {
	int endCondition = C_FALSE;
	if (gameHunter -> evidence -> size >= 3) {
		//printf("%s has found out the ghost's type and won!\n", gameHunter -> name);
		EvidenceNodeType * currNode = gameHunter -> evidence -> head;
	
		// Keep looping till you find a matching index
		while (currNode != NULL){
			if (currNode -> evidenceData -> type == EMF) {
				printf("EMF\n");
			}
			if (currNode -> evidenceData -> type == TEMPERATURE) {
				printf("TEMPERATURE\n");
			}
			if (currNode -> evidenceData -> type == FINGERPRINTS) {
				printf("FINGERPRINTS\n");
			}
			if (currNode -> evidenceData -> type == SOUND) {
				printf("SOUND\n");
			}
			currNode = currNode -> next;
		}
		if(LOGGING){
			l_hunterReview(gameHunter -> name, LOG_SUFFICIENT);
		}
		endCondition = C_TRUE;
	}
  
	if (gameHunter -> boredom <= 0) {
		//printf("\n\n%s has gotten bored and exited the house!\n\n\n", gameHunter -> name);
		l_hunterExit(gameHunter -> name, LOG_BORED);
		endCondition = C_TRUE;
	}          
	
	if (gameHunter -> fear >= MAX_FEAR) {
		//printf("\n\n%s has gotten too scared and run from the house!\n\n\n", gameHunter -> name);
		l_hunterExit(gameHunter -> name, LOG_FEAR);
		endCondition = C_TRUE;
	}  
	if (endCondition == C_TRUE){
		gameHunter -> room -> hunters -> hunters[gameHunter -> id] = NULL;
		gameHunter -> room -> hunters -> size--;
		sem_post(&gameHunter -> room -> mutex);  
		return C_TRUE;
	} else {
		sem_post(&gameHunter -> room -> mutex);
		return C_FALSE;
	}
}

