//For compiling: gcc -o main main.c
//For executing: ./main
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INVENTORY 10

typedef struct {
    char name[50];
    int health;
} Enemy;

typedef struct {
    char name[50];
} Item;

typedef struct {
    Item items[MAX_INVENTORY];
    int itemCount;
} Inventory;

int turn = 1;
int damageDealt = 10;
int goblinDamageDealt = 5;

void printCommands() {
    printf("+------------------------------+\n");
    printf("|  Commands Menu:              |\n");
    printf("|  N - Move North              |\n");
    printf("|  S - Move South              |\n");
    printf("|  E - Move East               |\n");
    printf("|  W - Move West               |\n");
    printf("|  I - View Inventory          |\n");
    printf("|  H - Heal                    |\n");
    printf("|  C - Collect Item            |\n");
    printf("|  A - Attack                  |\n");
    printf("|  Q - Quit Game               |\n");
    printf("+------------------------------+\n");
}

void printRoom(const char *room, Inventory *inventory, Enemy *enemy) {
    printf("===============================================================================\n");
    printf("                      Current Room: %s\n", room);
    printf("===============================================================================\n");

    if (enemy) {
        printf("The room is dimly lit, and there's a small chest in the corner.\n");
        printf("+------------------------------+\n");
        printf("|  Items in this room:         |\n");
        printf("|  - None                      |\n");
        printf("|  Enemies in this room:       |\n");
        printf("|  - %s (Health: %d)           |\n", enemy->name, enemy->health);
        printf("+------------------------------+\n");
    } else {
        printf("You find nothing of interest in this room.\n");
    }

    printCommands();
}

void printCombatStatus(Enemy *enemy, int playerHealth) {
    printf("===============================================================================\n");
    printf("                      Combat - Turn %d\n", turn);
    printf("===============================================================================\n");

    printf("You attack the %s with your rusty sword!\n", enemy->name);
    printf("* You dealt %d damage to the %s.\n", damageDealt, enemy->name);

    printf("+------------------------------+\n");
    printf("|  Enemy: %s (Health: %d)      |\n", enemy->name, enemy->health);
    printf("+------------------------------+\n");

    printf("The %s retaliates!\n", enemy->name);
    printf("* The %s dealt %d damage to you.\n", enemy->name, goblinDamageDealt);

    printf("+------------------------------+\n");
    printf("|  Your Health: %d/100         |\n", playerHealth);
    printf("+------------------------------+\n");

    printCommands();
}

void printInventory(Inventory *inventory) {
    printf("===============================================================================\n");
    printf("                      Your Inventory\n");
    printf("===============================================================================\n");

    if (inventory->itemCount == 0) {
        printf("You have no items in your inventory.\n");
    } else {
        for (int i = 0; i < inventory->itemCount; i++) {
            printf("- %s\n", inventory->items[i].name);
        }
    }

    printf("+------------------------------+\n");
    printf("|  Commands Menu:              |\n");
    printf("|  N - Move North              |\n");
    printf("|  S - Move South              |\n");
    printf("|  E - Move East               |\n");
    printf("|  W - Move West               |\n");
    printf("|  I - View Inventory          |\n");
    printf("|  Q - Quit Game               |\n");
    printf("+------------------------------+\n");
}

void processCommand(char command, Inventory *inventory, Enemy *enemy, int *playerHealth, int *gameRunning) {
    static int enemyHealth = 50;

    switch (command) {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            printf("You move in the specified direction.\n");
            break;
        case 'I':
            printInventory(inventory);
            break;
        case 'H':
            printf("You use a Health Potion. Healed for 20 points.\n");
            *playerHealth += 20;
            if (*playerHealth > 100) *playerHealth = 100;
            break;
        case 'C':
            if (inventory->itemCount < MAX_INVENTORY) {
                printf("You collect the item.\n");
                // Example item collection
                strcpy(inventory->items[inventory->itemCount].name, "Health Potion");
                inventory->itemCount++;
            } else {
                printf("Inventory is full. Cannot collect more items.\n");
            }
            break;
        case 'A':
            printCombatStatus(enemy, *playerHealth);
            turn++;
            enemyHealth -= damageDealt;
            if (enemyHealth <= 0) {
                printf("The %s falls to the ground, defeated.\n", enemy->name);
                printf("+------------------------------+\n");
                printf("|  Enemy: %s (Defeated)        |\n", enemy->name);
                printf("+------------------------------+\n");
                printf("+------------------------------+\n");
                printf("|  Rewards:                    |\n");
                printf("|  - 50 XP                     |\n");
                printf("|  - %s's Dagger               |\n", enemy->name);
                printf("+------------------------------+\n");
                printf("+------------------------------+\n");
                printf("|  Commands Menu:              |\n");
                printf("|  N - Move North              |\n");
                printf("|  S - Move South              |\n");
                printf("|  E - Move East               |\n");
                printf("|  W - Move West               |\n");
                printf("|  I - View Inventory          |\n");
                printf("|  Q - Quit Game               |\n");
                printf("+------------------------------+\n");
                *gameRunning = 0;
            }
            break;
        case 'Q':
            printf("Game Over.\n");
            *gameRunning = 0;
            break;
        default:
            printf("Invalid command. Please try again.\n");
    }
}

int main() {
    Inventory inventory = { .itemCount = 0 };
    Enemy goblin = { .name = "Goblin", .health = 50 };
    int playerHealth = 100;
    int gameRunning = 1;
    char command;

    printf("===============================================================================\n");
    printf("                     WELCOME TO THE DUNGEON CRAWLER GAME\n");
    printf("===============================================================================\n");

    printRoom("Goblin's Lair", &inventory, &goblin);

    while (gameRunning) {
        printf("Enter command: ");
        scanf(" %c", &command);
        command = toupper(command);
        processCommand(command, &inventory, &goblin, &playerHealth, &gameRunning);
    }

    return 0;
}
