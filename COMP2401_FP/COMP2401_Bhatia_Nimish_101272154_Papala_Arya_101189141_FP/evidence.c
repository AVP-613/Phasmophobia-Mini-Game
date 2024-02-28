#include "defs.h"

/* 
  Function: void initEvidenceList(EvidenceListType * evidenceList)
  Purpose:  Implement the void initEvidenceList(EvidenceListType * evidenceList) function that initializes both fields of the given list parameter to default values.
  Params:   evidenceList - EvidenceListType * 
  Output: Void
*/
void initEvidenceList(EvidenceListType * evidenceList) {
	evidenceList->head = NULL;
	evidenceList->tail = NULL;
	evidenceList->size = 0;
	
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	evidenceList -> mutex = mutex;
}

/* 
  Function: void initEvidence(EvidenceClassType evClass, EvidenceType ** evidence)
  Purpose:  Implement the void initEvidence(EvidenceClassType evClass, EvidenceType ** evidence) function that dynamically allocates memory for a EvidenceType structure, initializes its fields to the 
  given parameters, and “returns” this new structure using the evidence parameter.
  Params:   evidence - EvidenceType **
            evClass - EvidenceClassType 
  Output: Void
*/
void initEvidence(EvidenceClassType evClass, EvidenceType ** evidence) {
	free(*evidence);
	// Allocate memory to the evidence 
	EvidenceType * tempEv = (EvidenceType*) calloc(1, sizeof(EvidenceType));
	
	if (evClass == EMF) {
		tempEv->value = randFloat(4.7, 5);
		if (tempEv->value >= 4.9) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	} else if (evClass == TEMPERATURE) {
		tempEv->value = randFloat(-10,1);
		if (tempEv->value <= 0) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	} else if (evClass == FINGERPRINTS) {
		tempEv->value = 1;
		tempEv->ghostliness = GHOSTLY;
	} else if (evClass == SOUND) {
		tempEv->value = randInt(65,75);
		if (tempEv->value >= 70) {
			tempEv->ghostliness = GHOSTLY;
		} else {tempEv->ghostliness = NORMAL;}
	}
	
	tempEv -> type = evClass;
	*evidence = tempEv;
}

/* 
  Function: EvidenceType * getRandEvidence(EvidenceListType * evidenceList, EvidenceClassType evidenceClass)
  Purpose:  Implement the EvidenceType * getRandEvidence(EvidenceListType * evidenceList, EvidenceClassType evidenceClass) function that gets an evidence at a random index
  Params:   evidenceList - EvidenceListType
            evClass - EvidenceClassType 
  Output: NULL
*/
EvidenceType * getRandEvidence(EvidenceListType * evidenceList, EvidenceClassType evidenceClass) {
	// Set the current evidence node to the head of the list
	EvidenceNodeType * currEvidenceNode = evidenceList -> head;
	
	// Loop through each element of the hunter's evidence linked list
	while (currEvidenceNode != NULL && evidenceList -> size > 0){
		// Retrieve the evidence type of the current node
		if (currEvidenceNode -> evidenceData != NULL) {
			EvidenceClassType currEvidenceType = currEvidenceNode -> evidenceData -> type;
			
			// Compare against the evidence type you are trying to add
			if (evidenceClass == currEvidenceType) {
				EvidenceType * returnEvidence = currEvidenceNode -> evidenceData;
				return returnEvidence;
			}
		}
		// Set the current node to the next node
		currEvidenceNode = currEvidenceNode -> next;
	}
	return NULL;
}

/* 
  Function: EvidenceType * getEvidenceAtIndex(EvidenceListType * evidenceList, int index)
  Purpose:  Implement the EvidenceType * getEvidenceAtIndex(EvidenceListType * evidenceList, int index) returns the evidence at the index specified
  Params:   evidenceList - EvidenceListType *
            index - int 
  Output: NULL
*/
EvidenceType * getEvidenceAtIndex(EvidenceListType * evidenceList, int index){
	int evidenceListSize = evidenceList -> size;					// Size of the evidence list
	int i = 0;									// Current index in the list
	
	// If the index is valid:
	if (index <= evidenceListSize && evidenceListSize > 0) {
		// Make two temporary nodes to iterate through the linked list
		struct EvidenceNodeType * currNode = evidenceList -> head;
		
		// Keep looping till you find a matching index
		while (currNode != NULL){
			i++;
			if (i == index) break;
			currNode = currNode -> next;
		}
		
		if (i != index) printf("SOMETHING WENT WRONG WHILE SEARCHING FOR THE EVIDENCE!\n");
		else return currNode -> evidenceData;
	}
	else printf("Invalid Index!\n");
	return NULL;
}

/* 
  Function: void removeEvidence(EvidenceListType * evidenceList, EvidenceType * evidence)
  Purpose:  Implement the void removeEvidence(EvidenceListType * evidenceList, EvidenceType * evidence) removes evidence from a list if it matches the input evidence
  Params:   evidenceList - EvidenceListType *
            evidence - EvidenceType *
  Output: Void
*/
void removeEvidence(EvidenceListType * evidenceList, EvidenceType * evidence){
	EvidenceNodeType * currNode = evidenceList -> head;
	EvidenceNodeType * temp;

	// checks if head is null
	if (currNode == NULL) {
	} else { // else loops through and checks if the evidence matches
		while (currNode -> next != NULL) {
			if (currNode -> next -> evidenceData -> ghostliness == evidence -> ghostliness && currNode -> next -> evidenceData -> type == evidence -> type && currNode -> next -> evidenceData -> value == evidence -> value) {	
				evidenceList -> size--;
				if (currNode -> next == evidenceList -> tail) {
					cleanupEvidenceNode(currNode -> next);
					currNode -> next = NULL;
					evidenceList -> tail = currNode;
					break;
				}
				temp = currNode -> next -> next;
				cleanupEvidenceNode(currNode -> next);
				currNode -> next = temp;
				break;
			}
			currNode = currNode -> next;
		}
	}
	
}

/* 
  Function: void addEvidence(EvidenceListType * list, EvidenceType * evidence)
  Purpose:  Implemented the void addEvidence(EvidenceListType * list, EvidenceType * evidence) function that adds the evidence directly at the back (i.e. the end) of the given list, designed to take advantage of 
  the fields of the structure to simplify and speed up the code.
  Params:   list - EvidenceListType *
            ghost - EvidenceType *
  Returns: Void 
*/
void addEvidence(EvidenceListType * list, EvidenceType * evidence){
	//A temporary node to store the information of the new tail to be added
	EvidenceNodeType * newNode = (EvidenceNodeType*)calloc(1, sizeof(EvidenceNodeType));
	
	//The data of the new node is the evidence data
	newNode -> evidenceData = evidence;
	
	//The new tail does not have any nodes after it, so make sure to mark it as so
	newNode -> next = NULL;
	
	// If list is empty:
	if (list -> head == NULL) {
		list -> head = newNode;
		list -> tail = newNode;
    	}
    	
    	// If list is not empty
    	else {
		list -> tail -> next = newNode;
		list -> tail = newNode;
		
   	}
   	list -> size++;
}

/* 
  Function: void cleanupEvidenceData(EvidenceNodeType * evNode)
  Purpose:  Implement the void cleanupEvidenceData(EvidenceNodeType * evNode) function that traverses the given list and deallocates its data only.
  Params:   evNode - EvidenceNodeType *
  Output: Void
*/
void cleanupEvidenceData(EvidenceNodeType * evNode) {
	free(evNode -> evidenceData);
}	
	
/* 
  Function: void cleanupEvidenceNode(EvidenceNodeType * evNode)
  Purpose:  Implement the void cleanupEvidenceNode(EvidenceNodeType * evNode) function that traverses the given list for nodes and deallocates its data only.
  Params:   evNode - EvidenceNodeType *
  Output: Void
*/
void cleanupEvidenceNode(EvidenceNodeType * evNode) {
    free(evNode);
} 	
	
/* 
  Function: void cleanupEvidenceList(EvidenceListType * evList)
  Purpose:  Implement the void cleanupEvidenceList(EvidenceListType * evList) function that traverses the given list and deallocates its nodes only.
  Params:   evList - EvidenceListType *
  Output: Void
*/
void cleanupEvidenceList(EvidenceListType * evList) {
	int evListSize = evList -> size;
	
	// Make two temporary nodes to iterate through the linked list
	EvidenceNodeType * currEv = evList -> head;
	EvidenceNodeType * nextEv;
	
	// Clean up every evidence in the evidence list
	while (currEv != NULL){
		nextEv = currEv -> next;
		cleanupEvidenceData(currEv);
        	cleanupEvidenceNode(currEv);
		currEv = nextEv;
	}
	free(evList);
}	

/*
  Function:  cleanupEvidenceListNodes
  Purpose:   cleans up all the nodes in the evidence list
       in:   a pointer to the EvidenceListType
   return:   an evidence list with no nodes in it
*/
void cleanupEvidenceListNodes(EvidenceListType * evList) {
	// Make two temporary nodes to iterate through the linked list
	EvidenceNodeType * currEv = evList -> head;
	EvidenceNodeType * nextEv;
	
	// Clean up every evidence in the evidence list
	for (int i = 0; i < evList -> size; i++){
		nextEv = currEv -> next;
        	cleanupEvidenceNode(currEv);
		currEv = nextEv;
	}
	free(evList);
}	