#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROOMS 10
#define MAX_INVENTORY 10

// Structure definitions
typedef struct {
    char name[20];
    int health;
    int damage;
    char loot[20];
} Monster;

typedef struct {
    char description[100];
    Monster monster;
    int hasMonster;
} Room;

typedef struct {
    char items[MAX_INVENTORY][20];
    int itemCount;
} Inventory;

// Function prototypes
void initializeRooms(Room rooms[], int *roomCount);
void initializeMonsters(Monster monsters[]);
void displayRoom(const Room *room);
void attackMonster(Monster *monster, int *userHealth);
void collectItem(Inventory *inventory, const char *item);
void displayInventory(const Inventory *inventory);
void moveToDirection(Inventory *inventory, Room rooms[], int roomCount, int *userHealth);
void displayWelcomeMessage();
void giveReward(Inventory *inventory); // Function to give reward after defeating a monster

int main() {
    srand(time(0));
    Room rooms[MAX_ROOMS];
    int roomCount;
    Inventory inventory = {{}, 0};
    int userHealth = 100; // User's initial health

    displayWelcomeMessage(); // Display welcome message
    initializeRooms(rooms, &roomCount);

    // Game loop
    while (userHealth > 0) {
        displayRoom(&rooms[0]); // Display the current room
        
        // Perform actions based on user input
        moveToDirection(&inventory, rooms, roomCount, &userHealth);
    }

    printf("You have been defeated. Game Over!\n");
    return 0;
}

// Function implementations
void displayWelcomeMessage() {
    printf("********************************************\n");
    printf("*                                          *\n");
    printf("*         Welcome to Dungeon Crawler!     *\n");
    printf("*                                          *\n");
    printf("********************************************\n");
    printf("Prepare for an adventure filled with monsters and treasures!\n\n");
}

void initializeRooms(Room rooms[], int *roomCount) {
    const char *descriptions[] = {
        "A dark room with a flickering torch.",
        "A room filled with sparkling treasures.",
        "An eerie room with shadows lurking.",
        "A room with an ancient statue.",
        "A room echoing with strange noises.",
        "A dusty room with cobwebs.",
        "A bright room with flowers.",
        "A mysterious room with a glowing portal.",
        "A cold room filled with ice.",
        "A room with walls covered in runes."
    };

    Monster monsters[] = {
        {"Goblin", 30, 10, "Goblin Tooth"},
        {"Troll", 50, 15, "Troll Club"},
        {"Dragon", 100, 25, "Dragon Scale"}
    };

    *roomCount = rand() % (MAX_ROOMS - 5) + 5; // Random number of rooms between 5 and MAX_ROOMS
    for (int i = 0; i < *roomCount; i++) {
        rooms[i].hasMonster = rand() % 2; // Randomly decide if the room has a monster
        strcpy(rooms[i].description, descriptions[i % 10]);
        if (rooms[i].hasMonster) {
            int monsterIndex = rand() % 3; // Random monster
            rooms[i].monster = monsters[monsterIndex];
        }
    }
}

// Shortened attack function
void attackMonster(Monster *monster, int *userHealth) {
    int damageDealt = rand() % 20 + 1; // Random damage between 1 and 20
    monster->health -= damageDealt;
    printf("You attack the %s! You deal %d damage! Current Monster Health: %d\n", monster->name, damageDealt, monster->health > 0 ? monster->health : 0);

    // Monster attacks back
    if (monster->health > 0) {
        *userHealth -= monster->damage; // Reduce user's health
        printf("The %s attacks you! You take %d damage! Your Health: %d\n", monster->name, monster->damage, *userHealth);
    }
}

// Function to collect items and add them to inventory
void collectItem(Inventory *inventory, const char *item) {
    if (inventory->itemCount < MAX_INVENTORY) {
        strcpy(inventory->items[inventory->itemCount++], item);
        printf("You found a %s and added it to your inventory!\n", item);
    } else {
        printf("Your inventory is full! Can't collect %s.\n", item);
    }
}

// Function to give a reward to the player after defeating a monster
void giveReward(Inventory *inventory) {
    const char *rewards[] = {
        "Legendary Sword",
        "Mystic Amulet of Power",
        "Cloak of Invisibility",
        "Elixir of Immortality",
        "Golden Dragon Egg",
        "Phoenix Feather",
        "Talisman of Fortune",
        "Enchanted Ring",
        "Divine Shield",
        "Crown of the Undead King"
    };
    const char *reward = rewards[rand() % 10]; // Random reward from the list
    collectItem(inventory, reward); // Add reward to inventory
    printf("Congratulations! You received a %s as a reward!\n", reward);
}

// Function to display the player's inventory
void displayInventory(const Inventory *inventory) {
    printf("\nYour Inventory:\n");
    if (inventory->itemCount == 0) {
        printf(" - Your inventory is empty!\n");
    } else {
        for (int i = 0; i < inventory->itemCount; i++) {
            printf(" - %s\n", inventory->items[i]);
        }
    }
}

// Function to move in a direction and encounter random events
void moveToDirection(Inventory *inventory, Room rooms[], int roomCount, int *userHealth) {
    // Display user health before choosing options
    printf("Your Health: %d\n", *userHealth);
    
    printf("\nChoose a direction to move:\n");
    printf("1. Move East\n");
    printf("2. Move West\n");
    printf("3. Move North\n");
    printf("4. Move South\n");
    printf("5. Check Inventory\n");
    printf("6. Exit\n");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 5) {
        displayInventory(inventory);
        return;
    } else if (choice == 6) {
        printf("Exiting the game. Goodbye!\n");
        exit(0);
    }
    
    if (choice < 1 || choice > 4) {
        printf("Invalid direction! You remain in the same room.\n");
        return;
    }

    printf("You move to the %s.\n", choice == 1 ? "East" : choice == 2 ? "West" : choice == 3 ? "North" : "South");

    // Randomly decide to attack a monster or collect an item
    if (rooms[0].hasMonster) {
        printf("Oh no! You encounter a %s!\n", rooms[0].monster.name);
        while (rooms[0].monster.health > 0 && *userHealth > 0) {
            // Ask the user for action
            printf("Press 'a' to attack or 'c' to collect an item:\n");
            char input;
            scanf(" %c", &input);
            
            if (input == 'a') {
                attackMonster(&rooms[0].monster, userHealth);
            } else if (input == 'c') {
                const char *items[] = {
                    "Magic Potion",
                    "Ancient Artifact",
                    "Golden Crown",
                    "Mysterious Amulet",
                    "Enchanted Sword",
                    "Healing Herb",
                    "Mystic Scroll",
                    "Crystal Orb",
                    "Dragon Egg",
                    "Philosopher's Stone"
                };
                collectItem(inventory, items[rand() % 10]); // Random item from the list
            }
        }
        if (*userHealth > 0) {
            printf("You defeated the %s!\n", rooms[0].monster.name);
            giveReward(inventory); // Give reward after defeating the monster
        }
    } else {
        const char *items[] = {
            "Magic Potion",
            "Ancient Artifact",
            "Golden Crown",
            "Mysterious Amulet",
            "Enchanted Sword",
            "Healing Herb",
            "Mystic Scroll",
            "Crystal Orb",
            "Dragon Egg",
            "Philosopher's Stone"
        };
        collectItem(inventory, items[rand() % 10]); // Random item from the list
    }
}

// Function to display the current room
void displayRoom(const Room *room) {
    printf("\nRoom Description: %s\n", room->description);
    if (room->hasMonster) {
        printf("A wild %s appears!\n", room->monster.name);
    }
}
