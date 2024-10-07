#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 10
#define MAX_MONSTERS 5
#define MAX_ROOMS 100

typedef struct {
    char name[30];
    int health;
    char loot[30];
} Monster;

typedef struct {
    char items[MAX_ITEMS][30];
    int itemCount;
} Inventory;

typedef struct {
    char description[100];
    int hasMonster;
    Monster monster;
    int hasItem;
    char item[30];
} Room;

// Function declarations
void initInventory(Inventory *inventory);
void displayInventory(Inventory *inventory);
void collectItem(Inventory *inventory, const char *item);
void attackMonster(Monster *monster, int *userHealth);
void moveToDirection(Inventory *inventory, Room *currentRoom, int *userHealth, Room visitedRooms[], int *visitedRoomCount);
void generateRandomRoom(Room *room, Room visitedRooms[], int visitedRoomCount);
int isRoomRepeated(Room *newRoom, Room visitedRooms[], int visitedRoomCount);
void copyRoom(Room *dest, Room *src);

// Main function
int main() {
    srand(time(0)); // Initialize random seed

    // Initialize inventory
    Inventory inventory;
    initInventory(&inventory);

    // Track visited rooms to avoid repetition
    Room visitedRooms[MAX_ROOMS];
    int visitedRoomCount = 0;

    // Initial room
    Room currentRoom;
    generateRandomRoom(&currentRoom, visitedRooms, visitedRoomCount); // Generate first room
    visitedRooms[visitedRoomCount++] = currentRoom; // Add first room to visited list

    int userHealth = 100; // Player health

    printf("***************************************\n");
    printf("*         Welcome to Dungeon Crawler!         *\n");
    printf("*  Prepare for an adventure filled with monsters and treasures! *\n");
    printf("***************************************\n");

    // Game loop
    while (1) {
        printf("\n--- Current Room ---\n");
        printf("%s\n", currentRoom.description);
        if (currentRoom.hasMonster) {
            printf("* A wild %s appears!\n", currentRoom.monster.name);
        } else if (currentRoom.hasItem) {
            printf("* You see a %s in the room.\n", currentRoom.item);
        } else {
            printf("* The room is empty.\n");
        }
        printf("--------------------\n");

        moveToDirection(&inventory, &currentRoom, &userHealth, visitedRooms, &visitedRoomCount);
    }

    return 0;
}

// Initialize the inventory
void initInventory(Inventory *inventory) {
    inventory->itemCount = 0;
}

// Display the inventory
void displayInventory(Inventory *inventory) {
    printf("\n* Your Inventory\n");
    for (int i = 0; i < inventory->itemCount; i++) {
        printf(" - %s\n", inventory->items[i]);
    }
    if (inventory->itemCount == 0) {
        printf(" * Inventory is empty.\n");
    }
}

// Collect an item
void collectItem(Inventory *inventory, const char *item) {
    if (inventory->itemCount < MAX_ITEMS) {
        strcpy(inventory->items[inventory->itemCount++], item);
        printf("* You collected a %s!\n", item);
    } else {
        printf("* Inventory full! Cannot collect %s.\n", item);
    }
}

// Attack a monster
void attackMonster(Monster *monster, int *userHealth) {
    printf("* You attack the %s!\n", monster->name);
    monster->health -= 20; // Example damage dealt

    if (monster->health > 0) {
        printf("* The %s has %d health left.\n", monster->name, monster->health);
        *userHealth -= 10; // Example damage taken
        printf("* You take damage! Your health is now %d.\n", *userHealth);
    } else {
        printf("* You have defeated the %s!\n", monster->name);
    }
}

// Move to a direction
void moveToDirection(Inventory *inventory, Room *currentRoom, int *userHealth, Room visitedRooms[], int *visitedRoomCount) {
    char action[30]; // Increased size to accommodate full input
    printf("\n* Choose an action (a: attack, c: collect item, i: check inventory, m: move <direction>, q: quit): ");
    scanf(" %[^\n]", action); // Allow multi-word input

    if (strcmp(action, "a") == 0 && currentRoom->hasMonster) {
        attackMonster(&currentRoom->monster, userHealth);
        if (currentRoom->monster.health <= 0) {
            collectItem(inventory, currentRoom->monster.loot); // Give loot after defeating the monster
            currentRoom->hasMonster = 0; // Mark monster as defeated
        }
    } else if (strcmp(action, "c") == 0 && currentRoom->hasItem) {
        collectItem(inventory, currentRoom->item);
        currentRoom->hasItem = 0; // Remove item from the room after collection
    } else if (strcmp(action, "i") == 0) {
        displayInventory(inventory);
    } else if (strncmp(action, "m ", 2) == 0) { // Check if the command starts with 'm '
        char direction[10];
        sscanf(action, "m %s", direction); // Extract direction

        // Random chance generation for the new room
        generateRandomRoom(currentRoom, visitedRooms, *visitedRoomCount);
        visitedRooms[(*visitedRoomCount)++] = *currentRoom; // Add to visited list
    } else if (strcmp(action, "q") == 0) {
        printf("* Thanks for playing!\n");
        exit(0);
    } else {
        printf("* Invalid action! Please choose again.\n");
    }
}

// Generate a random room with 40% monster, 40% item, 10% empty, 10% no room
void generateRandomRoom(Room *room, Room visitedRooms[], int visitedRoomCount) {
    char *roomDescriptions[] = {
        "A room with ancient stone carvings.",
        "A dimly lit room with a strange glow.",
        "A room with walls covered in moss.",
        "A quiet room with a faint breeze.",
        "A room with flickering lights."
    };
    
    char *monsterNames[] = {"Goblin", "Orc", "Dragon", "Skeleton", "Zombie"};
    char *itemNames[] = {"Health Potion", "Magic Sword", "Shield", "Gold Coin", "Treasure Chest"};

    int randomChoice = rand() % 100; // Get a random number between 0-99
    int descriptionIndex = rand() % 5; // Random room description

    strcpy(room->description, roomDescriptions[descriptionIndex]); // Set a random description

    if (randomChoice < 40) {
        // Monster encounter
        room->hasMonster = 1;
        strcpy(room->monster.name, monsterNames[rand() % 5]);
        room->monster.health = (rand() % 30) + 20; // Random health between 20 and 50
        strcpy(room->monster.loot, itemNames[rand() % 5]); // Monster drops random loot
        room->hasItem = 0; // No items in the room
    } else if (randomChoice < 80) {
        // Item in room
        room->hasMonster = 0;
        room->hasItem = 1;
        strcpy(room->item, itemNames[rand() % 5]);
    } else if (randomChoice < 90) {
        // Empty room
        room->hasMonster = 0;
        room->hasItem = 0;
    } else {
        // No room (10% chance)
        printf("* You move but find there's no room in that direction!\n");
        strcpy(room->description, "A blocked path.");
        room->hasMonster = 0;
        room->hasItem = 0;
    }

    // Ensure no room repetition
    while (isRoomRepeated(room, visitedRooms, visitedRoomCount)) {
        generateRandomRoom(room, visitedRooms, visitedRoomCount); // Generate again if repeated
    }
}

// Check if the room description has been used before
int isRoomRepeated(Room *newRoom, Room visitedRooms[], int visitedRoomCount) {
    for (int i = 0; i < visitedRoomCount; i++) {
        if (strcmp(newRoom->description, visitedRooms[i].description) == 0) {
            return 1; // Room has been visited
        }
    }
    return 0; // Room is new
}

void copyRoom(Room *dest, Room *src) {
    strcpy(dest->description, src->description);
    dest->hasMonster = src->hasMonster;
    dest->monster = src->monster;
    dest->hasItem = src->hasItem;
    strcpy(dest->item, src->item);
}
