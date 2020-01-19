#include "aliens.h"
///for functions shared by both marines and ayys

int getDirection(int x1, int y1, int x2, int y2){
    int direction;
    if(x1 != x2){
        float angle = atanf((y1 - y2)/(x1 - x2));
        if(angle <= PI / 4 && angle >= - PI / 4){
            //facing east or west
            if(x1 < x2){
                direction = FACING_E;
            }else{
                direction = FACING_W;
            }
        }else{
            //facing north or south
            if(y1 < y2){
                direction = FACING_S;
            }else{
                direction = FACING_N;
            }
        }
    }else if(y1 != y2){
        if(y1 < y2){
            direction = FACING_S;
        }else{
            direction = FACING_N;
        }
    }else{
        direction = FACING_S;
    }
    return direction;
}

int determineHitChance(Marine &shooter, Marine &target, Map &m){
    int hitChance = shooter.accuracy;
    int targetCover = m.tiles[target.x][target.y].cover[getDirection(shooter.x, shooter.y, target.x, target.y)];
    shooter.facingDirection = getDirection(shooter.x, shooter.y, target.x, target.y);
    hitChance -= 10 * targetCover * target.coverMultiplier;

    //range based hitChance changing
    float distance = (float) sqrt( pow(shooter.x - target.x, 2) + pow(shooter.y - target.y, 2) );
    //assault range is 5 tiles, snipers is 6 tiles out, heavies is between 3 and 7, support is 7
    if(shooter.isMarine){
        //determines hitchance based off role for marines
        switch(shooter.role){
            case ASSAULT:
                if(distance >= 5){
                    hitChance -= 5 * (distance - 4);
                }
                break;
            case SNIPER:
                if(distance <= 6){
                    hitChance -= 5 * (6 - distance);
                }
                break;
            case HEAVY:
                if(distance <= 3){
                    hitChance -= 7 * (3 - distance);
                }else if(distance >= 7){
                    hitChance -= 7 * (distance - 7);
                }
                break;
            case SUPPORT:
            default:
                if(distance >= 7){
                    hitChance -= 7 * (distance - 7);
                }
                break;
        }
    }else{
        //for aliens
        switch(shooter.role){
            case AYY_BEAN:
                if(distance >= 7){
                    hitChance -= 10 + 7 * (distance - 7);
                }
                break;
            case AYY_BLOB:
                if(distance >= 2){
                    hitChance = -100;
                }
                break;
            case AYY_DEMON:
                if(distance >= 5){
                    hitChance -= 20 + 7 * (distance - 5);
                }
                break;
            case AYY_EYES:
            default:
                if(distance <= 5){
                    hitChance -= 10 + 7 * (5 - distance);
                }
                break;
        }


    }
    return hitChance;
}

void refreshMarines(Marine ops[], int dudeCount){
    for(int i = 0; i < dudeCount; i++){
        if(ops[i].isAlive){
            ops[i].currAP = ops[i].maxAP;
            ops[i].coverMultiplier = 1;
        }
    }
}

bool animateShot(Marine &shooter, Marine &target, int camx, int camy, int &bullX, int &bullY, ALLEGRO_BITMAP *bulletbmps[4]){
    int direction = getDirection(bullX, bullY, TILE_SIZE * (target.x + CAM_SIZE_RIGHT - camx), TILE_SIZE * (target.y + CAM_SIZE_DOWN - camy));
    //jagged bullet drawing towards the target
    int change = 4;
    switch(direction){
        case FACING_E:
            bullX += change;
            break;
        case FACING_N:
            bullY -= change;
            break;
        case FACING_W:
            bullX -= change;
            break;
        case FACING_S:
            bullY += change;
            break;
    }
    al_draw_bitmap(bulletbmps[shooter.role], bullX, bullY, 0);
    al_rest(0.001);
    al_flip_display();
    //if the bullet touches the target
    if(abs(bullX - TILE_SIZE * (target.x + CAM_SIZE_RIGHT - camx)) + abs(bullY - TILE_SIZE * (target.y + CAM_SIZE_DOWN - camy)) <= 10){
        return false;
    }

    return true;
}

bool shootMarine(Marine &shooter, Marine &target, Map &m, char buffer[]){
    int hitChance = determineHitChance(shooter, target, m);
    shooter.currAP = 0;
    //the shootings
    if(rand()%100 <= hitChance){
        //hits, damages for damage +/- 1
        int damage = randInt(-1, 1);
        if(shooter.isMarine){
            damage += MARINE_DAMAGE[shooter.role];
        }else{
            damage += AYY_DAMAGE[shooter.role];
        }
        target.currHealth -= damage;
        strcpy(buffer, "HIT -");
        char holder[10];
        sprintf(holder, "%d", damage);
        strcat(buffer, holder);
        if(target.currHealth <= 0){
            target.isAlive = false;
            strcat(buffer, " AND KILLED");
        }
        return true;
    }else{
        strcpy(buffer, "MISS");
        return false;
    }

}

void moveMarine(Marine &mover, int destX, int destY){
    mover.facingDirection = getDirection(mover.x, mover.y, destX, destY);
    mover.x = destX;
    mover.y = destY;
    mover.currAP--;
}

bool isTileOcc(int x, int y, Marine ops[], Marine ayys[], int alienCount){
    for(int i = 0; i < SQUAD_SIZE; i++){
        if(ops[i].x == x && ops[i].y == y && ops[i].isAlive){
            return true;
        }
    }

    for(int i = 0; i < alienCount; i++){
        if(ayys[i].x == x && ayys[i].y == y && ayys[i].isAlive){
            return true;
        }
    }
    return false;
}
