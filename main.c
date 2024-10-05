#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_MONSTERS 5

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
} Room;

// Function declarations
void initInventory(Inventory *inventory);
void displayInventory(Inventory *inventory);
void collectItem(Inventory *inventory, const char *item);
void attackMonster(Monster *monster, int *userHealth);
void moveToDirection(Inventory *inventory, Room rooms[], int roomCount, int *userHealth, int *currentRoom);

// Main function
int main() {
    // Initialize inventory
    Inventory inventory;
    initInventory(&inventory);

    // Initialize rooms
    Room rooms[3] = {
        {"A dark room with a flickering torch.", 1, {"Goblin", 30, "Gold Coin"}},
        {"A bright room filled with treasures.", 0, {"", 0, ""}},
        {"A damp room with eerie sounds.", 1, {"Skeleton", 20, "Bone"}} 
    };

    int userHealth = 100; // Player health
    int currentRoom = 0;  // Starting room

    printf("***************************************\n");
    printf("*         Welcome to Dungeon Crawler!         *\n");
    printf("*  Prepare for an adventure filled with monsters and treasures! *\n");
    printf("***************************************\n");

    // Game loop
    while (1) {
        printf("\n--- Current Room ---\n");
        printf("%s\n", rooms[currentRoom].description);
        if (rooms[currentRoom].hasMonster) {
            printf("* A wild %s appears!\n", rooms[currentRoom].monster.name);
        } else {
            printf("* The room is empty.\n");
        }
        printf("--------------------\n");

        moveToDirection(&inventory, rooms, 3, &userHealth, &currentRoom);
    }

    return 0;
}

// Initialize the inventory
void initInventory(Inventory *inventory) {
    inventory->itemCount = 0;
}

// Display the inventory
void displayInventory(Inventory *inventory) {
    printf("\n* Your Inventory:\n");
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
    }
}

// Move to a direction
void moveToDirection(Inventory *inventory, Room rooms[], int roomCount, int *userHealth, int *currentRoom) {
    char action[30]; // Increased size to accommodate full input
    printf("\n* Choose an action (a: attack, c: collect item, i: check inventory, m: move <direction>, q: quit): ");
    scanf(" %[^\n]", action); // Allow multi-word input

    if (strcmp(action, "a") == 0 && rooms[*currentRoom].hasMonster) {
        attackMonster(&rooms[*currentRoom].monster, userHealth);
        if (rooms[*currentRoom].monster.health <= 0) {
            printf("* You have defeated the %s!\n", rooms[*currentRoom].monster.name);
            collectItem(inventory, rooms[*currentRoom].monster.loot); // Give loot after defeating the monster
            rooms[*currentRoom].hasMonster = 0; // Mark monster as defeated
        }
    } else if (strcmp(action, "c") == 0) {
        collectItem(inventory, "Health Potion"); // Example item
    } else if (strcmp(action, "i") == 0) {
        displayInventory(inventory);
    } else if (strncmp(action, "m ", 2) == 0) { // Check if the command starts with 'm '
        char direction[10];
        sscanf(action, "m %s", direction); // Extract direction

        if (strcmp(direction, "north") == 0) {
            if (*currentRoom > 0) {
                (*currentRoom)--; // Move to the previous room (north)
                printf("* You move north...\n");
            } else {
                printf("* You cannot move north. There's no room!\n");
            }
        } else if (strcmp(direction, "south") == 0) {
            if (*currentRoom < roomCount - 1) {
                (*currentRoom)++; // Move to the next room (south)
                printf("* You move south...\n");
            } else {
                printf("* You cannot move south. There's no room!\n");
            }
        } else if (strcmp(direction, "east") == 0) {
            printf("* You move east... But it's a wall! You cannot pass.\n");
        } else if (strcmp(direction, "west") == 0) {
            printf("* You move west... But it's a wall! You cannot pass.\n");
        } else {
            printf("* Invalid direction! Please use north, south, east, or west.\n");
        }
    } else if (strcmp(action, "q") == 0) {
        printf("* Thanks for playing!\n");
        exit(0);
    } else {
        printf("* Invalid action! Please choose again.\n");
    }
}
