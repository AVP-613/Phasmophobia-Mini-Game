#include "defs.h"

/*
  Function: void initHunter(EvidenceClassType ec, char *name, RoomType *room, HunterType **hunterPtr, int id)
  Purpose:  Initializes a HunterType structure by allocating memory, initializing its fields, and assigns it to the given pointer.
  Params:   ec - EvidenceClassType
            name - char*
            room - RoomType*
            hunterPtr - pointer to a pointer to HunterType
            id - int
  Output:   Void
*/
void initHunter(EvidenceClassType ec, char * name, RoomType * room, HunterType ** hunterPtr, int id){
	// Assign some memory to the hunter
	HunterType * hunterInit = (HunterType*)calloc(1,sizeof(HunterType));
	
	// Initialize the hunter's name, the current room he is in and the type of evidence they can collect
	strcpy(hunterInit -> name, name);
	hunterInit -> room = room;
	room -> hunters -> size++;
	
	hunterInit -> evidenceType = ec;
	
	// Initialize the hunter's evidence linked list
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	hunterInit -> evidence = evidence;
	
	// Set the hunter's fear and boredom level to 0
	hunterInit -> fear = 0;
	hunterInit -> boredom = BOREDOM_MAX;
	hunterInit -> id = id;

	*hunterPtr = hunterInit;
}

/*
  Function: void initHunterArray(HunterArrayType *hunterArray)
  Purpose:  Initializes a HunterArrayType structure by setting all hunter pointers to NULL.
  Params:   hunterArray - pointer to a HunterArrayType structure
  Output:   Void
*/
void initHunterArray(HunterArrayType * hunterArray) {
	for (int i = 0; i < MAX_HUNTERS; i++){
		hunterArray -> hunters[i] = NULL;
	}
	
	hunterArray -> size = 0;
}

/*
  Function: void removeHunterFromRoom(HunterType *hunter, RoomType *room)
  Purpose:  Removes a hunter from a room by setting the corresponding array element to NULL and reducing the room's hunter count.
  Params:   hunter - pointer to a HunterType structure
            room - pointer to a RoomType structure
  Output:   Void
*/
void removeHunterFromRoom(HunterType * hunter, RoomType * room){
	// Find the hunter with the same ID in the hunters array
	room -> hunters -> hunters[hunter -> id] = NULL;
	room -> hunters -> size--;
}

/*
  Function: void addHunterToRoom(HunterType *hunter, RoomType *room)
  Purpose:  Adds a hunter to a room by setting the corresponding array element and increasing the room's hunter count.
  Params:   hunter - pointer to a HunterType structure
            room - pointer to a RoomType structure
  Output:   Void
*/
void addHunterToRoom(HunterType * hunter, RoomType * room){
	// Add hunter 
	room -> hunters -> hunters[hunter -> id] = hunter;
	// Increase the number of hunters in the room
	room -> hunters -> size++;
}

/*
  Function: void collectEvidence(HunterType *hunter)
  Purpose:  Simulates a hunter collecting evidence in a room based on predefined conditions.
  Params:   hunter - pointer to a HunterType structure
  Output:   Void
*/
void collectEvidence(HunterType * hunter){
	RoomType * currRoom = hunter -> room;				// Current room hunter is in
	EvidenceListType * evidenceList = currRoom -> evidence;		// Said room's evidence List
	int evidenceSize = evidenceList -> size;			// Size of the evidence list
	EvidenceType * tempEvidence;					// Temporarily store the evidence in this variable
	
	// Does the room have any evidence in it's evidence linked list?
	if(evidenceSize > 0){
		// If yes, collect a random piece of evidence (choose an int between 0 and size of the linked list)
		tempEvidence = getRandEvidence(evidenceList, hunter -> evidenceType);

		if (tempEvidence != NULL) {
			// Check if the evidence is ghostly or not:
			if (tempEvidence -> ghostliness == GHOSTLY){
				if (containsEvidenceType(tempEvidence -> type, hunter) == C_FALSE) {
					addEvidence(hunter -> evidence, tempEvidence);
					if(LOGGING){
						l_hunterCollect(hunter ->name, hunter -> evidenceType, hunter -> room -> name);
					}
				}else{
					hunter -> boredom = BOREDOM_MAX;
				}
			} else {
                hunter -> boredom -= BOREDOM_RATE;
				//printf("%s scans the %s and finds standard readings.		(The hunter finds standard readings)\n", hunter -> name, hunter -> room -> name);
			}
			removeEvidence(hunter -> room -> evidence, tempEvidence);
		} else {
			//printf("%s hits his reader, something seems off about it.		(The hunter is getting interference)\n", hunter -> name);
			printf("Hunter  %s Doing nothing\n", hunter->name);
		}
	} else {
		//printf("The %s seems perfectly normal to %s. Too normal...		(There is no evidence in the room)\n", hunter -> room -> name, hunter -> name);
		printf("Hunter  %s Doing nothing\n", hunter->name);
	}
}

/*
  Function: void transferEvidenceData(HunterType *hunter, EvidenceType *evidence)
  Purpose:  Transfers evidence data from a room's evidence list to a hunter's evidence list if the hunter doesn't already have it.
  Params:   hunter - pointer to a HunterType structure
            evidence - pointer to an EvidenceType structure
  Output:   Void
*/
void transferEvidenceData(HunterType * hunter, EvidenceType * evidence){
	// Check the type of evidence we are about to send
	EvidenceClassType evidenceClass = evidence -> type;
	
	// See if the hunter in question already has that type of evidence
	if (containsEvidenceType(evidenceClass, hunter) == C_FALSE ) {
		addEvidence(hunter -> evidence, evidence);
		
	}
	if(LOGGING){
		l_hunterCollect(hunter->name, evidence->type, hunter->room->name);
	}
}

/*
  Function: int containsEvidenceType(EvidenceClassType evidenceClass, HunterType *hunter)
  Purpose:  Checks if a hunter already has a certain type of evidence in their evidence list.
  Params:   evidenceClass - EvidenceClassType
            hunter - pointer to a HunterType structure
  Output:   int (C_TRUE or C_FALSE)
*/
int containsEvidenceType (EvidenceClassType evidenceClass, HunterType * hunter){
	EvidenceListType * evidenceList = hunter -> evidence;		// Temporary evidence list
	
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	while (currEvidenceNode != NULL){
		// Retrieve the evidence type of the current node
		if (currEvidenceNode -> evidenceData != NULL) {
		
			EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
			
			// Compare against the evidence type you are trying to add
			if (evidenceClass == currEvidenceType) return C_TRUE;
		}

		
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	return C_FALSE;
}

/*
  Function: void cleanupHunterArray(HunterArrayType *hunterArray)
  Purpose:  Cleans up memory allocated for a HunterArrayType structure, including each individual hunter's evidence list and the hunters themselves.
  Params:   hunterArray - pointer to a HunterArrayType structure
  Output:   Void
*/
void cleanupHunterArray(HunterArrayType * hunterArray){
	for (int i = 0; i < MAX_HUNTERS; i++){
		cleanupEvidenceListNodes(hunterArray -> hunters[i] -> evidence);
		
		free(hunterArray -> hunters[i]);
	}
}