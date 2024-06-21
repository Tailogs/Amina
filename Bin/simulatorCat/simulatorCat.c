#include "simulatorCat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../color/color.h"

void birth(Cat* cat) {
    printf("Your kitten is born, what will you name it?\n> ");
    fgets(cat->name, 50, stdin);
    cat->name[strcspn(cat->name, "\n")] = 0; // Remove the newline character
}

void tick(Cat* cat) {
    for (int i = 0; i < 10; i++) {
        cat->health -= rand() % 2;
        cat->satisfied -= rand() % 2;
        cat->rest -= rand() % 2;
        cat->hunger -= rand() % 2;
        cat->purity -= rand() % 2;
        cat->wastes -= rand() % 2;
    }
}

void addHealth(Cat* cat) {
    cat->health += rand() % 30;
    if (cat->health > 100) cat->health = 100;
}

void sleep(Cat* cat) {
    printf("%s is sleeping...\n", cat->name);
    cat->rest += rand() % 30;
    if (cat->rest > 100) cat->rest = 100;
    addHealth(cat);
}

void eat(Cat* cat) {
    printf("%s is eating...\n", cat->name);
    cat->hunger += rand() % 30;
    if (cat->hunger > 100) cat->hunger = 100;
    addHealth(cat);
}

void wash(Cat* cat) {
    printf("%s is washing...\n", cat->name);
    cat->purity += rand() % 30;
    if (cat->purity > 100) cat->purity = 100;
    addHealth(cat);
}

void play(Cat* cat) {
    printf("%s is playing...\n", cat->name);
    cat->satisfied += rand() % 30;
    if (cat->satisfied > 100) cat->satisfied = 100;
    addHealth(cat);
}

void toilet(Cat* cat) {
    printf("%s is going to the toilet...\n", cat->name);
    cat->wastes += rand() % 30;
    if (cat->wastes > 100) cat->wastes = 100;
    addHealth(cat);
}

void pet(Cat* cat) {
    printf("%s is happy with the petting...\n", cat->name);
    cat->satisfied += rand() % 30;
    if (cat->satisfied > 100) cat->satisfied = 100;
    addHealth(cat);
}

void voice() {
    const char* catVoice[] = { "Purr", "Meow", "Mrrr", "Meow", "Rrrrr", "Rawr" };
    int randomIndex = rand() % 6;
    printf("%s =>-<=\n", catVoice[randomIndex]);
}

int death(Cat* cat) {
    if (cat->rest <= 0) {
        printf("The kitten is too tired to live and has passed away\n");
        return 0;
    } else if (cat->hunger <= 0) {
        printf("The kitten has passed away from hunger\n");
        return 0;
    } else if (cat->purity <= 0) {
        printf("The kitten became very sick and passed away\n");
        return 0;
    } else if (cat->satisfied <= 0) {
        printf("The kitten got too sad and ran away\n");
        return 0;
    } else if (cat->wastes <= 0) {
        printf("The kitten had nowhere to go and ran outside\n");
        return 0;
    } else if (cat->health <= 0) {
        printf("The kitten passed away due to poor health\n");
        printf("Kitten's lifespan: %d\n", cat->daysLife);
        return 0;
    }
    cat->daysLife++;
    return 1;
}

void live(Cat* cat) {
    int livePet = 1;
    while (livePet) {
        setColor("bright_magenta");
        printf("+-----------------+------------------+\n");
        printf("| PET: %s |\n", cat->name);
        setColor("cyan");
        printf("+-----------------+------------------+\n");
        printf("|        <<< Health: %d >>>          |\n", cat->health);
        printf("+-----------------+------------------+\n");
        setColor("blue");
        printf("1. Pet            | Satisfaction: %d\n", cat->satisfied);
        printf("2. Play           | Satisfaction: %d\n", cat->satisfied);
        printf("3. Sleep          | Rest:         %d\n", cat->rest);
        printf("4. Feed           | Hunger:       %d\n", cat->hunger);
        printf("5. Wash           | Cleanliness:  %d\n", cat->purity);
        printf("6. Change litter  | Toilet:       %d\n", cat->wastes);
        printf("+-----------------+------------------+\n");
        printf("|          999. Exit game            |\n");
        printf("+-----------------+------------------+\n");
        resetColor();
        
        printf("> ");
        int c = 0;
        if (scanf("%d", &c) != 1) {
            system("cls");
            printf("Input error: Incorrect value entered.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        if (rand() % 2 == 1)
            voice();

        system("cls");

        setColor("yellow");
        switch (c) {
            case 1:
                pet(cat);
                tick(cat);
                break;
            case 2:
                play(cat);
                tick(cat);
                break;
            case 3:
                sleep(cat);
                tick(cat);
                break;
            case 4:
                eat(cat);
                tick(cat);
                break;
            case 5:
                wash(cat);
                tick(cat);
                break;
            case 6:
                toilet(cat);
                tick(cat);
                break;
            case 999:
                return;
            default:
                printError1("[Selected item does not exist]");
                break;
        }
        livePet = death(cat);
        resetColor();
    }
}
