#include "defs.h"

/* 
  Function: void initRoomList(struct RoomListType * roomList)
  Purpose:  Implement the void initRoomList(struct RoomListType * roomList) initializes a RoomListType
  Params:   roomList - RoomListType
  Output: Void
*/

void initRoomList(struct RoomListType * roomList){
	RoomNodeType * temp = NULL; 
	roomList -> head = temp;
	roomList -> tail = temp;
	roomList -> size = 0;
}

/* 
  Function: void initRoom(RoomType ** room, char * name)
  Purpose:  Implement the void initRoom(RoomType ** room, char * name) function that initializes a RoomType
  Params:   name - Character *
            room - RoomType **
  Output: Void
*/
void initRoom(RoomType ** room, char * name){

	RoomType * roomInit = (RoomType*)calloc(1,sizeof(RoomType));
		
	//Initialize the room's Name
	strcpy(roomInit -> name, name);
	
	//Initialize The Evidence List, Room List and Hunter Array
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	roomInit -> evidence = evidence;

	RoomListType * rooms = (RoomListType*)calloc(1, sizeof(RoomListType));
	initRoomList(rooms);
	roomInit -> attached = rooms;
	
	HunterArrayType * hunters = (HunterArrayType*)calloc(1, sizeof(HunterArrayType));
	initHunterArray(hunters);
	roomInit -> hunters = hunters;
	
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	roomInit -> mutex = mutex;
	
	
	*room = roomInit;
}

/* 
  Function: void addRoom(RoomListType * list, RoomType * room)
  Purpose: Implement the void addRoom(RoomListType * list, RoomType * room) function that adds the room r to the back of the array structure in parameter arr.
  Params:  list - RoomListType *
           room - RoomType *
  Output: Void
*/
void addRoom(RoomListType * list, RoomType * room){	
	//A temporary node to store the information of the new tail to be added
	RoomNodeType * newNode;
	
	//Assign memory to the new node
	newNode = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
	
	//The data of the new node is the room data
	newNode -> roomData = room;
	
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
  Function: void connectRooms(RoomType * room1, RoomType * room2)
  Purpose: Implement the void connectRooms(RoomType * room1, RoomType * room2) adds a room to the end of a RoomListType
  Params:  room1 - RoomType *
           room2 - RoomType *
  Output: Void
*/
void connectRooms(RoomType * room1, RoomType * room2){
	// Add Room 2 to Room 1's attached roomList
	addRoom(room1->attached, room2);
	
	// Add Room 1 to Room 2's attached roomList
	addRoom(room2->attached, room1);
}

/* 
  Function: void cleanupRoom(RoomType * room)
  Purpose: Implement the void cleanupRoom(RoomType * room) function that deallocates the dynamically allocated memory in the given room collection. This includes each evidence, and each evidence's ghosts list and 
  its nodes. Do not assume that if a room is being cleaned that the Ghosts are no longer needed.
  Params:  room - RoomType *
  Output: Void
*/
void cleanupRoom(RoomType * room){
	// Free Evidence and Room Linked List
	cleanupEvidenceListNodes(room -> evidence);
	cleanupRoomListNodes(room -> attached);
	// Free hunter array
	free(room -> hunters);
}

/* 
  Function: void cleanupRoomList(RoomListType * roomList)
  Purpose: Implement the void cleanupRoomList(RoomListType * roomList) function that deallocates the dynamically allocated memory in the given room collection. This includes each room, and each room’s ghosts list and its
  nodes. Do not assume that if a room is being cleaned that the Ghosts are no longer needed.
  Params:  roomList - RoomListType *
  Output: Void
*/
void cleanupRoomList(RoomListType * roomList){
	// Make two temporary nodes to iterate through the linked list
	RoomNodeType * currNode = roomList -> head;
	RoomNodeType * nextNode = roomList -> head -> next;
	
	// Clean up every attached room in the room list
	for(int i = 0; i < roomList -> size; i++){
		cleanupRoomListNodes(currNode -> roomData -> attached);
		currNode = currNode -> next;
	}
	
	currNode = roomList -> head;
	
	for(int i = 0; i < roomList -> size; i++){
		nextNode = currNode -> next;
		
		cleanupEvidenceListNodes(currNode -> roomData -> evidence);
		free(currNode -> roomData -> hunters);
		free(currNode -> roomData);
		free(currNode);
		
		currNode = nextNode;
	}	

	free(roomList);
}

/* 
  Function: void cleanupRoomListNodes(RoomListType * roomList)
  Purpose: Implement the void cleanupRoomListNodes(RoomListType * roomList) function that deallocates the dynamically allocated memory in the given room collection. This includes each room, and each room’s ghosts list and its
  nodes. Do not assume that if a room is being cleaned that the Ghosts are no longer needed.
  Params:  roomList - RoomListType *
  Output: Void
*/
void cleanupRoomListNodes(RoomListType * roomList) {
	
	// Make two temporary nodes to iterate through the linked list
	RoomNodeType * currNode = roomList -> head;
	RoomNodeType * nextNode;
	
	// Clean up every room in the room list
	while(currNode != NULL){
		nextNode = currNode -> next;
		free(currNode);
		currNode = nextNode;
	}
	free(roomList);
}

/* 
  Function: RoomType * randRoom(RoomListType * roomList, int spawnGhostCheck)
  Purpose: Implement the RoomType * randRoom(RoomListType * roomList, int spawnGhostCheck) function that deallocates the dynamically allocated memory in the given room collection. This includes each room, and each room’s ghosts list and its
  nodes. Do not assume that if a room is being cleaned that the Ghosts are no longer needed.
  Params:  roomList - RoomListType *
		   spawnGhostCheck - Integer
  Output: Void
*/
RoomType * randRoom(RoomListType * roomList, int spawnGhostCheck) {
	int selectedRoom;
    	if (spawnGhostCheck == C_FALSE) {
        	selectedRoom = randInt(0, roomList -> size - 1);
    	} else {
        	selectedRoom = randInt(1, roomList -> size - 1);
    	}
	
    	RoomNodeType * currRoom = roomList -> head;

    	for (int i = 0; i < roomList -> size; i++) {
        	if (i == selectedRoom) {
            		return currRoom -> roomData;
        	} else {
            		currRoom = currRoom -> next;
        	}
    	}
    	return roomList -> head -> roomData;
}