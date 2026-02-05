#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct{
    char spellName[20];
    char spellClass[20];
    int minDamage;
    int maxDamage;
    int minManaCost;
    int maxManaCost;
}spell;

typedef struct{
    char name[20];
    char magicClass[20];
    spell spellType[3];
    int hp;
    int mana;
    bool isAlive;
    int stats[3];
}wizard;

int *random_spells(int *arr);
void getSpells(FILE *fp, spell *spells);
int myRandomF(int min, int max);
void duel(wizard* attacker, wizard* defender);
int calculateScore(int stats[], int n);

int main(){
    int i;
    int *randomSpells1;
    int *randomSpells2;
    FILE *fp;
    spell *spells=malloc(sizeof(spell)*6);
    wizard w1={"Merlin","fire",{},100,30,true};
    wizard w2={"Frostina","ice",{},100,40,true};
    wizard *w1_ptr=&w1;
    wizard *w2_ptr=&w2;

    srand(time(NULL));

    fp=fopen("spellbook.txt","r");
    for(i=0;i<6;i++){
        getSpells(fp,&spells[i]);
    }
 
    randomSpells1=random_spells(randomSpells1);
    randomSpells2=random_spells(randomSpells2);

    /*Saves random spells to wizard struct*/
    w1.spellType[0]=spells[randomSpells1[0]];
    w1.spellType[1]=spells[randomSpells1[1]];
    w1.spellType[2]=spells[randomSpells1[2]];

    w2.spellType[0]=spells[randomSpells2[0]];
    w2.spellType[1]=spells[randomSpells2[1]];
    w2.spellType[2]=spells[randomSpells2[2]];

    printf("\nWizard Duel Begins: %s vs %s!\n",w1.name,w2.name);
    printf("-------------------------------------------\n\n");
    duel(w1_ptr,w2_ptr);

    free(spells);
    fclose(fp);
    return 0;
}
int *random_spells(int *arr){
    srand(time(0));
    arr=malloc(sizeof(int)*3);
    arr[0]=(rand()%6);
    arr[1]=(rand()%6);
    arr[2]=(rand()%6);
    return arr;
}

void getSpells(FILE *fp, spell *spells){

    fscanf(fp,"%[^,],%[^,],%d,%d,%d,%d\n",
        spells->spellName,
        spells->spellClass,
        &spells->minDamage,
        &spells->maxDamage,
        &spells->minManaCost,
        &spells->maxManaCost);
}

int myRandomF(int min, int max){
    return rand()%(max-min+1)+min; 
}

void duel(wizard* attacker, wizard* defender){
    int attackSpell;
    int manaCost;
    int damage;

    attackSpell=rand()%3;

    printf("%s casts %s on %s!\n",attacker->name,
        attacker->spellType[attackSpell].spellName,
        defender->name);
        
    manaCost=myRandomF(
        attacker->spellType[attackSpell].minManaCost,
        attacker->spellType[attackSpell].maxManaCost);
        
    if(attacker->mana<manaCost){
        printf("%s is low on mana and meditates...\n",attacker->name);

        manaCost=rand()%11+10;/*Recovers mana*/

        printf("%s recovers %d mana. Current mana:%d\n",
        attacker->name,manaCost,attacker->mana+manaCost);

        attacker->mana+=manaCost;
        attacker->stats[2]++;
    }

    else{
        attacker->mana-=manaCost;
        damage=myRandomF(
            attacker->spellType[attackSpell].minDamage,
            attacker->spellType[attackSpell].maxDamage);
        
        if(strcmp(attacker->magicClass,attacker->spellType[attackSpell].spellClass)==0){
            damage+=5;
        }
        defender->hp-=damage;
    
        printf("Damage: %d | %s's HP: %d | %s's Mana: %d\n\n",
        damage,defender->name,defender->hp,attacker->name,attacker->mana);

        attacker->stats[0]+=damage;
        attacker->stats[1]+=manaCost;
    }
    if(defender->hp<=0){
        defender->isAlive=false;
        printf("Winner: %s the %s Wizard!\n",attacker->name,attacker->magicClass);
        printf("\nBattle Summary:\n");
        printf("%s--Damage: %d|Mana Spent: %d|Recoveries: %d|Score:%d\n",
            defender->name,defender->stats[0],
            defender->stats[1],defender->stats[2],
            calculateScore(defender->stats,3));
        printf("%s--Damage: %d|Mana Spent: %d|Recoveries: %d|Score:%d\n",
            attacker->name,attacker->stats[0],
            attacker->stats[1],attacker->stats[2],
            calculateScore(attacker->stats,3));
        return ;
    }
    return duel(defender,attacker);
}

int calculateScore(int stats[], int n){
    static int score=0;

    if(n==0){
        return score;
    }
    if(n==1){
        score+=stats[n-1]*2;
        return score, calculateScore(stats,n-1);
    }
    if(n==2){
        score+=stats[n-1];
        return score, calculateScore(stats,n-1);
    }
    if(n==3){
        score-=stats[n-1]*3;
        return score, calculateScore(stats,n-1);
    }
    else{
        printf("ERROR!\n");
        return 0;
    }
}