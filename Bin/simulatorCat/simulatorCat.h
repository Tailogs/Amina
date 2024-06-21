#ifndef CAT_H
#define CAT_H

typedef struct {
    int health;
    int satisfied;
    int rest;
    int hunger;
    int purity;
    int wastes;
    int daysLife;
    char name[50];
} Cat;

void birth(Cat* cat);
void tick(Cat* cat);
void addHealth(Cat* cat);
void sleep(Cat* cat);
void eat(Cat* cat);
void wash(Cat* cat);
void play(Cat* cat);
void toilet(Cat* cat);
void pet(Cat* cat);
void voice();
int death(Cat* cat);
void clearConsole();
void live(Cat* cat);

#endif // CAT_H
