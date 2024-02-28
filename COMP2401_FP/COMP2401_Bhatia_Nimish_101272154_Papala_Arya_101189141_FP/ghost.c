#include "defs.h"

/* 
  Function: void initGhost(int id, GhostEnumType gt, RoomType *r, float like, GhostType **ghost)
  Purpose:  Implement the void initGhost(int id, GhostEnumType gt, RoomType *r, float like, GhostType **ghost) function that dynamically allocates memory for a GhostType structure, initializes its fields to the 
  given parameters, and “returns” this new structure using the ghost parameter.
  Params:   gc - GhostClassType 
            room - RoomType * 
            ghost - GhostType ** 
            house - HouseType *
  Output: Void
*/
void initGhost(GhostClassType gc, RoomType * room, GhostType ** ghost, HouseType * house){
	// Assign the ghost some memory
	GhostType * ghostInit = (GhostType*)calloc(1, sizeof(GhostType));
	
	// Initialize it's ghost class, the current room it's in, and the boredom
	ghostInit -> ghostType = gc;
	ghostInit -> room = room;
	ghostInit -> boredom = BOREDOM_MAX;
	ghostInit -> house = house;
	
	*ghost = ghostInit; // sets ghost double pointer
}

/*
  Function: void moveGhost(GhostType *ghost)
  Purpose:   Move the ghost to a random, connected room. Update the ghost's and the new room's pointers accordingly.
  The function checks the rooms attached to the current room the ghost is in, selects a random room,
  and moves the ghost to that room if it's available.
  Params:    ghost - GhostType pointer representing the ghost to be moved
  Output:    Void
*/
void moveGhost(GhostType * ghost){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = ghost -> room -> attached;
	RoomType * newRoom = randRoom(roomList, C_FALSE);
	
	if (sem_trywait(&newRoom -> mutex) == 0) {
		
		ghost -> room -> ghost = NULL;
		sem_post(&ghost -> room -> mutex);
		// Change the ghost's room pointer to a random room from that room's connected rooms
		ghost -> room = newRoom;
		ghost -> room -> ghost = ghost;
	
	}	

}

/* 
  Function: void addGhostEvidence(GhostType * ghost)
  Purpose:  Implemented the void addGhostEvidence(GhostType * ghost) function that adds the ghost evidence directly at the back (i.e. the end) of the given list, designed to take advantage of the fields of the 
  list structure to simplify and speed up the code.
  Params:   ghost - GhostType * 
  Returns: Void 
*/
void addGhostEvidence(GhostType * ghost){
	GhostClassType ghostType = ghost -> ghostType;		// The Type of Ghost
	EvidenceListType * roomEvidenceList = ghost -> room -> evidence;		// The Room's Evidence List
	EvidenceType * tempEvidence = (EvidenceType*)calloc(1, sizeof(EvidenceType));				// A Temporary Evidence Variable
	int ghostTypeNum = randInt(1,3);			// A random number b/w one to three, used to determine which type of evidence to drop
	
	//Print out Ghost Type:
	switch(ghostType){
		case POLTERGEIST :
			// Choose b/w EMF, Temperature and Fingerprints
			if(ghostTypeNum == 1) { 
				initEvidence(EMF, &tempEvidence);
				printf("A menacing aura emanates from the %s.		(The ghost creates EMF evidence)\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("The air of the %s chills suddenly.		(The ghost creates TEMPERATURE evidence)\n", ghost -> room -> name);
			}
			else {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("Fingerprints start to appear on the walls of the %s.		(The ghost creates FINGERPRINT evidence)\n", ghost -> room -> name);
			}
			break;
			
		case PHANTOM:
			// Choose b/w Temperature, Fingerprints and Sound
			if(ghostTypeNum == 1) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("Frost crackles on the walls in the %s.		(The ghost creates TEMPERATURE evidence)\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("In the %s, a mug slowly starts to levitate.		(The ghost creates FINGERPRINT evidence)\n", ghost -> room -> name);
			}
			else { 
				initEvidence(SOUND, &tempEvidence);
				printf("A scream can be heard coming from the %s.		(The ghost creates SOUND evidence)\n", ghost -> room -> name);
			}
			break;
			
		case BULLIES:
			// Choose b/w EMF, Fingerprints and Sound
			if(ghostTypeNum == 1) { 
				initEvidence(EMF, &tempEvidence);
				printf("The air seems different in the %s.		(The ghost creates EMF evidence)\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(FINGERPRINTS, &tempEvidence);
				printf("Several things are tossed arund in the %s.		(The ghost creates FINGERPRINT evidence)\n", ghost -> room -> name);
			}
			else {
				initEvidence(SOUND, &tempEvidence);
				printf("A baby crying is heard coming from the %s.		(The ghost creates SOUND evidence)\n", ghost -> room -> name);
			}
			break;
			
		case BANSHEE:
			// Choose b/w EMF, Temperature and Sound
			if(ghostTypeNum == 1) {
				initEvidence(EMF, &tempEvidence);
				printf("Lights in the %s shift and fray.		(The ghost creates EMF evidence)\n", ghost -> room -> name);
			}
			else if (ghostTypeNum == 2) {
				initEvidence(TEMPERATURE, &tempEvidence);
				printf("Ice crackles underfoot in the %s.		(The ghost creates TEMPERATURE evidence)\n", ghost -> room -> name);
			}
			else {
				initEvidence(SOUND, &tempEvidence);
				printf("Something is scratching the walls of the %s.		(The ghost creates SOUND evidence)\n", ghost -> room -> name);
			}
			break;
			
		default:
			printf("UNKNOWN GHOST TYPE!");
			break;	
	}
	
	// Add the evidence data to the room's evidence linked list
	addEvidence(roomEvidenceList, tempEvidence);
	addEvidence(ghost -> house -> evidence, tempEvidence);
	if(LOGGING){
		l_ghostEvidence(tempEvidence->type, ghost->room->name);
	}
}

/* 
  Function: void spawnGhost(GhostType * ghost, HouseType * house)
  Purpose:  Implemented the void spawnGhost(GhostType * ghost, HouseType * house) adds the ghost to a random room that is not the van
  Params:   ghost - GhostType * 
  Returns: Void 
*/
void spawnGhost(GhostType * ghost, HouseType * house){
	// Check the rooms that are attached to the room the ghost is currently in:
	RoomListType * roomList = house -> rooms;

	// Change the ghost's room pointer to a random room from that room's connected rooms
	ghost -> room = randRoom(roomList, C_TRUE);
}

/*
  Function: void cleanupGhostData(GhostType *ghost)
  Purpose:   Free the memory allocated for the GhostType structure.
  Params:    ghost - GhostType pointer representing the ghost whose memory is to be freed
  Output:    Void
*/
void cleanupGhostData(GhostType * ghost) {
	free(ghost);
}