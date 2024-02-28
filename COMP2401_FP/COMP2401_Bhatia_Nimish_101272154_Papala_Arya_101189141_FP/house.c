#include "defs.h"

/*
  Function: void initHouse(HouseType *house)
  Purpose:  Initializes a HouseType structure by allocating memory for rooms, hunters, and evidence, 
  and initializing their respective fields.
  Params:   house - pointer to a HouseType structure
  Output:   Void
*/
void initHouse(HouseType * house){
	// Initialize The Rooms List and Hunter Array
	RoomListType * rooms = (RoomListType*)calloc(1, sizeof(RoomListType));
	initRoomList(rooms);
	house -> rooms = rooms;
	// Set the ghost pointer to NULL for now
	house -> ghost = NULL;
	
	// Allocate memory to the hunter array, initialize it after
	HunterArrayType * hunters = (HunterArrayType*)calloc(1,sizeof(HunterArrayType));
	initHunterArray(hunters);
	hunters -> size = 4;
	house -> hunters = hunters;
	house -> haunted = GHOSTLY;
	
	// Allocate memory to the evidence array, initialize it after
	EvidenceListType * evidence = (EvidenceListType*)calloc(1, sizeof(EvidenceListType));
	initEvidenceList(evidence);
	house -> evidence = evidence;
}

/*
  Function: void cleanupHouse(HouseType *house)
  Purpose:  Cleans up memory allocated for a HouseType structure, including hunters, rooms, evidence, and ghost data.
  Params:   house - pointer to a HouseType structure
  Output:   Void
*/
void cleanupHouse(HouseType * house){
	// Clean up the Hunters Array
	cleanupHunterArray(house -> hunters);
 	
	// Clean up room list
	cleanupRoomList(house -> rooms);	

	// Clean up the evidence List	
	cleanupEvidenceList(house -> evidence);	
	
	// Clean up ghost data
	cleanupGhostData(house -> ghost);
	
	// Free the hunters from memory
	free(house -> hunters);
}

/*
  Function: void populateRooms(HouseType *house)
  Purpose:  Populates the house with predefined rooms and connects them to create a layout.
  Params:   house - pointer to a HouseType structure
  Output:   Void
*/
void populateRooms(HouseType* house) {
    RoomType* van;
    initRoom(&van, "Van");
    RoomType* hallway;
    initRoom(&hallway, "Hallway");
    RoomType* master_bedroom;
    initRoom(&master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom;
    initRoom(&boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom;
    initRoom(&bathroom, "Bathroom");
    RoomType* basement;
    initRoom(&basement, "Basement Staircase");
    RoomType* basement_hallway;
    initRoom(&basement_hallway, "Basement Hallway");
    RoomType* right_storage_room;
    initRoom(&right_storage_room, "Right Storage Room");
    RoomType* left_storage_room;
    initRoom(&left_storage_room, "Left Storage Room");
    RoomType* kitchen;
    initRoom(&kitchen, "Kitchen");
    RoomType* living_room;
    initRoom(&living_room, "Living Room");
    RoomType* garage;
    initRoom(&garage, "Garage");
    RoomType* utility_room;
    initRoom(&utility_room, "Utility Room");


    addRoom(house->rooms, van);
    addRoom(house->rooms, hallway);
    addRoom(house->rooms, master_bedroom);
    addRoom(house->rooms, boys_bedroom);
    addRoom(house->rooms, bathroom);
    addRoom(house->rooms, basement);
    addRoom(house->rooms, basement_hallway);
    addRoom(house->rooms, right_storage_room);
    addRoom(house->rooms, left_storage_room);
    addRoom(house->rooms, kitchen);
    addRoom(house->rooms, living_room);
    addRoom(house->rooms, garage);
    addRoom(house->rooms, utility_room);

    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}