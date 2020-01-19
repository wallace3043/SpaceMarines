#include "aliens.h"

/*********************************************************
*get aiming angle:
*                   s      leftx upy
*                  2#
*         t###################t       rightx downy
*            10    2#    10
*                   s
*2 angles
**********************************************************/

int initSquad(Marine ops[]){
    //generates unique last names for the marines
    FILE *ftpr;
    ftpr = fopen("lastNames.txt", "r");
    if(!ftpr){
        printf("cant find lastNames.txt");
        return -1;
    }
    char names[100][20];
    char holder = 0;
    int j = 0;
    int k = 0;
    while(holder != '?'){
        fscanf(ftpr, "%c", &holder);
        if(holder == '\n'){
            j++;
            k = 0;
        }else if(holder != '?'){
            names[j][k] = holder;
            k++;
        }
    }
    fclose(ftpr);

    //guarantees uniqueness of names
    int nameI[6] = {-1};
    for(int i = 0; i < 6; i++){
        do{
            nameI[i] = randInt(0, j);
        }while(nameI[i] == nameI[(i + 1) % 6] || nameI[i] == nameI[(i + 2) % 6] || nameI[i] == nameI[(i + 3) % 6] || nameI[i] == nameI[(i + 4) % 6] || nameI[i] == nameI[(i + 5) % 6]);
    }

    //initializing things
    for(int i = 0; i < SQUAD_SIZE; i++){
        ops[i].facingDirection = randInt(0, 3);
        ops[i].role = randInt(0, 3); //i % 4;
        ops[i].maxHealth = MARINE_HEALTH[ops[i].role];
        ops[i].accuracy = randInt(55, 85);
        ops[i].isAlive = true;
        ops[i].isMarine = true;
        ops[i].drawDistance = MARINE_DRAW_DISTANCE;
        ops[i].maxAP = MARINE_AP[ops[i].role];
        ops[i].bullType = randInt(0, 2);
        ops[i].moveDistance = MARINE_MOVE_DISTANCE[ops[i].role];
        ops[i].coverMultiplier = 1;
        strcpy(ops[i].name, names[nameI[i]]);
    }
    return 0;
}

bool areMarinesAlive(Marine ops[]){
    for(int i = 0; i < SQUAD_SIZE; i++){
        if(ops[i].isAlive){
            return true;
        }
    }
    return false;
}

bool isMarineHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ops[]){
    for(int i = 0; i < SQUAD_SIZE; i++){
        int rx = ops[i].x + CAM_SIZE_RIGHT - camx;
        int ry = ops[i].y + CAM_SIZE_DOWN - camy;
        if(isMouseIn(mouse, rx * TILE_SIZE, ry * TILE_SIZE, (rx + 1) * TILE_SIZE, (ry + 1) * TILE_SIZE) && ops[i].isAlive){
            return true;
        }
    }
    return false;
}

int getMarineIHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ops[]){
    for(int i = 0; i < SQUAD_SIZE; i++){
        int rx = ops[i].x + CAM_SIZE_RIGHT - camx;
        int ry = ops[i].y + CAM_SIZE_DOWN - camy;
        if(isMouseIn(mouse, rx * TILE_SIZE, ry * TILE_SIZE, (rx + 1) * TILE_SIZE, (ry + 1) * TILE_SIZE) && ops[i].isAlive){
            return i;
        }
    }
    return 0;
}
