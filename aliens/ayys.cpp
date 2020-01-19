#include "aliens.h"

int initAliens(Marine ayys[], int alienCount, Map &m){
    //initializing the aliens

    int podCount = alienCount / POD_SIZE;
    for(int i = 0; i < podCount; i++){
        bool uniquexy = false;
        int prevxy[podCount][2];
        while(!uniquexy){
            uniquexy = true;
            prevxy[i][0] = 2 * randInt(8 / 2, (m.width - 1) / 2);
            prevxy[i][1] = 2 * randInt(8 / 2, (m.height - 1) / 2);
            for(int j = 0; j < i; j++){
                if(prevxy[i][0] == prevxy[j][0] && prevxy[i][1] == prevxy[j][1]){
                    uniquexy = false;
                }
            }
        }
        ayys[POD_SIZE * i].x = prevxy[i][0];
        ayys[POD_SIZE * i].y = prevxy[i][1];

        //printf("%d %d \n", ayys[POD_SIZE * i].x, ayys[POD_SIZE * i].y);
        ayys[POD_SIZE * i + 1].x = ayys[POD_SIZE * i].x + 1;
        ayys[POD_SIZE * i + 1].y = ayys[POD_SIZE * i].y;
        ayys[POD_SIZE * i + 2].x = ayys[POD_SIZE * i].x;
        ayys[POD_SIZE * i + 2].y = ayys[POD_SIZE * i].y + 1;
        ayys[POD_SIZE * i + 3].x = ayys[POD_SIZE * i].x + 1;
        ayys[POD_SIZE * i + 3].y = ayys[POD_SIZE * i].y + 1;

        for(int j = 0; j < POD_SIZE; j++){
            int k = POD_SIZE * i + j;
            ayys[k].role = randInt(0, 3);
            char holder[10];
            sprintf(holder, "%d", k + 1);
            strcpy(ayys[k].name, "alien ");
            strcat(ayys[k].name, holder);
            ayys[k].maxHealth = AYY_HEALTH[ayys[k].role];
            ayys[k].currHealth = ayys[k].maxHealth;
            ayys[k].accuracy = randInt(50, 80);
            ayys[k].isAlive = true;
            ayys[k].isMarine = false;
            ayys[k].facingDirection = i % 4;
            ayys[k].maxAP = AYY_AP[ayys[k].role];
            ayys[k].currAP = ayys[k].maxAP;
            ayys[k].bullType = B_GREEN;
            ayys[k].moveDistance = AYY_MOVE_DISTANCE[ayys[k].role];
            ayys[k].coverMultiplier = 1;
        }
    }

    return 0;
}

bool areAyysAlive(Marine ayys[], int alienCount){
    for(int i = 0; i < alienCount; i++){
        if(ayys[i].isAlive){
            return true;
        }
    }
    return false;
}

bool isAlienHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ayys[], int alienCount){
    for(int i = 0; i < alienCount; i++){
        int rx = ayys[i].x + CAM_SIZE_RIGHT - camx;
        int ry = ayys[i].y + CAM_SIZE_DOWN - camy;
        if(isMouseIn(mouse, rx * TILE_SIZE, ry * TILE_SIZE, (rx + 1) * TILE_SIZE, (ry + 1) * TILE_SIZE) && ayys[i].isAlive){
            return true;
        }
    }
    return false;
}

int getAlienIHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ayys[], int alienCount){
    for(int i = 0; i < alienCount; i++){
        int rx = ayys[i].x + CAM_SIZE_RIGHT - camx;
        int ry = ayys[i].y + CAM_SIZE_DOWN - camy;
        if(isMouseIn(mouse, rx * TILE_SIZE, ry * TILE_SIZE, (rx + 1) * TILE_SIZE, (ry + 1) * TILE_SIZE) && ayys[i].isAlive){
            return i;
        }
    }
    return 0;
}


void ayyTurn(Marine ayys[], int alienCount, Map &m, Marine ops[]){
    char useless[100]; //for the message param
    for(int i = 0; i < alienCount; i++){
        //printf("ayy %d\n", i);
        if(ayys[i].isAlive){
            //making the alien take an action
            while(ayys[i].currAP >= 1){
                int mI = 0; //index of the marine to shoot
                int bestChance = 0; //highest hitchance
                for(int j = 0; j < SQUAD_SIZE; j++){
                    if(determineHitChance(ayys[i], ops[j], m) >= bestChance && ops[j].isAlive){
                        mI = j;
                        bestChance = determineHitChance(ayys[i], ops[j], m);
                    }
                }

                if(bestChance >= SHOOT_THRES){
                    //printf("shooting %s\n", ops[mI].name);
                    shootMarine(ayys[i], ops[mI], m, useless);
                }else{
                    //printf("moving %d %d\n", ayys[i].x, ayys[i].y);

                    int dx = 0;
                    int dy = 0;
                    do{
                        do{
                            dx = randInt(-ayys[i].moveDistance, ayys[i].moveDistance);
                            dy = randInt(-dx, dx);
                        }while(!(ayys[i].x + dx >= 0 && ayys[i].x + dx < m.width && ayys[i].y + dy >= 0 && ayys[i].y + dy < m.height));
                    }while(isTileOcc(ayys[i].x + dx, ayys[i].y + dy, ops, ayys, alienCount));

                    //printf("ayy %d %d %d\n", i, dx + ayys[i].x, dy + ayys[i].y);
                    moveMarine(ayys[i], ayys[i].x + dx, ayys[i].y + dy);
                }
            }
        }

        al_rest(0.2);
    }
}
