//PS D:\DSA Mini Project\dungeon-crawler> [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
//PS D:\DSA Mini Project\dungeon-crawler> ^C
//PS D:\DSA Mini Project\dungeon-crawler> gcc -o dungeon_crawler main.c                           
//PS D:\DSA Mini Project\dungeon-crawler> ./dungeon_crawler


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// ANSI escape codes for coloring
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

// Constants for room probabilities
#define EMPTY_ROOM_PROBABILITY 5
#define ITEM_ROOM_PROBABILITY 40
#define MONSTER_ROOM_PROBABILITY 40

// Constants for the game
#define MAX_INVENTORY 10
#define MAX_ITEMS 5

// Global variables
const char *itemDescriptions[MAX_ITEMS] = {
    "Golden Sword 🗡️",
    "Healing Potion 🧪",
    "Magic Amulet ✨",
    "Shield 🛡️",
    "Ancient Scroll 📜"
};

const char *inventory[MAX_INVENTORY];  // Change to const char* to avoid warnings
int inventoryCount = 0;

// Function prototypes
void displayWelcomeMessage();
void displayRoom();
void displayMenu();
void displayInventory(const char *inventory[], int inventoryCount);  // Change to const
void movePlayer(int *playerHealth);
void battle(int *playerHealth);
void heal(int *playerHealth);

// Main function
int main() {
    srand(time(0));  // Seed for random number generation
    int playerHealth = 100;
    
    displayWelcomeMessage();
    displayRoom();
    displayMenu();

    while (1) {
        printf("> ");
        char choice;
        scanf(" %c", &choice);

        switch (choice) {
            case 'm':
                movePlayer(&playerHealth);
                break;
            case 'i':
                displayInventory(inventory, inventoryCount);
                break;
            case 'q':
                printf(RED "Quitting game.\n" RESET);
                exit(0);
            default:
                printf("Invalid option! Try again.\n");
                break;
        }

        // Check if player health is above 0
        if (playerHealth <= 0) {
            printf(RED "You have died. Game Over!\n" RESET);
            break;
        }
    }

    return 0;
}

// Function to display welcome message
void displayWelcomeMessage() {
    printf(YELLOW "------------------------------------------------------------\n");
    printf("|  *** Welcome to the Dungeon Adventure RPG Game! ***      |\n");
    printf("------------------------------------------------------------" RESET "\n");
}

// Function to display the current room and randomly determine its contents
void displayRoom() {
    int roomType = rand() % 100;  // Random number between 0 and 99

    printf(YELLOW "*******************************************************\n");
    if (roomType < EMPTY_ROOM_PROBABILITY) {
        printf("*    This room is empty.                              *\n");
    } else if (roomType < EMPTY_ROOM_PROBABILITY + ITEM_ROOM_PROBABILITY) {
        printf("*    You found an item!\n");
        if (inventoryCount < MAX_INVENTORY) {
            int itemIndex = rand() % MAX_ITEMS;
            inventory[inventoryCount++] = itemDescriptions[itemIndex];  // No warning now
            printf("*    Collected: %s\n", itemDescriptions[itemIndex]);
        } else {
            printf("*    Your inventory is full!\n");
        }
    } else {
        printf("*    A fierce monster appears!                         *\n");
    }
    printf("*******************************************************" RESET "\n");
}

// Function to show main menu with colored options
void displayMenu() {
    printf("------------------------------------------------------------\n");
    printf("| " BLUE "m: [🚪 Move] " RESET "| " BLUE "i: [📦 Inventory] " RESET "| " BLUE "q: [❌ Quit] " RESET " |\n");
    printf("------------------------------------------------------------\n");
}

// Function to display the player's inventory
void displayInventory(const char *inventory[], int inventoryCount) {  // Change to const
    printf(YELLOW "Your Inventory:\n");
    for (int i = 0; i < inventoryCount; i++) {
        printf("[%d] %s\n", i + 1, inventory[i]);
    }
    printf(RESET "\n");
}

// Function to handle player movement and possible encounters
void movePlayer(int *playerHealth) {  // Fix: Added playerHealth parameter
    printf(GREEN "You moved to a new room.\n" RESET);
    displayRoom();
    battle(playerHealth);
}

// Function to handle battle logic
void battle(int *playerHealth) {
    int monsterHealth = rand() % 50 + 50;  // Monster health between 50 and 99
    int attackCount = 0;
    int healed = 0;  // To track if the player has healed during the battle

    while (monsterHealth > 0) {
        printf(RED "Monster's Health: %d | Your Health: %d\n" RESET, monsterHealth, *playerHealth);
        printf(BLUE "Choose action: a - Attack, h - Heal\n" RESET);
        char action;
        scanf(" %c", &action);

        if (action == 'h' && healed == 0) {
            heal(playerHealth);
            healed = 1;  // Mark that healing has been used
        } else if (action == 'a') {
            int damage = rand() % 20 + 10;  // Random damage between 10 and 29
            monsterHealth -= damage;
            printf(GREEN "You attack the monster for %d damage!\n" RESET, damage);
            attackCount++;
            if (monsterHealth <= 0) {
                printf(GREEN "You defeated the monster!\n" RESET);
                // Reward for defeating the monster
                int itemIndex = rand() % MAX_ITEMS;
                if (inventoryCount < MAX_INVENTORY) {
                    inventory[inventoryCount++] = itemDescriptions[itemIndex];  // No warning now
                    printf("You found a %s as a reward!\n", itemDescriptions[itemIndex]);
                } else {
                    printf("Your inventory is full! No reward collected.\n");
                }
                break;
            }
        } else {
            printf("Invalid action! Try again.\n");
        }

        // Monster's turn to attack
        if (monsterHealth > 0) {
            int monsterDamage = rand() % 20 + 5;  // Random damage between 5 and 24
            *playerHealth -= monsterDamage;
            printf(RED "The monster attacks you for %d damage!\n" RESET, monsterDamage);
            if (*playerHealth <= 0) {
                printf(RED "You have died. Game Over!\n" RESET);
                exit(0);
            }
        }

        // Reset healing for the next round
        if (healed == 1) {
            healed = 0;  // Reset after the attack phase
        }
    }
}

// Function to heal the player
void heal(int *playerHealth) {
    int healingAmount = rand() % 15 + 5;  // Random healing between 5 and 19
    *playerHealth += healingAmount;
    printf(GREEN "You healed for %d health!\n" RESET, healingAmount);
}
