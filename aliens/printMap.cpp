#include "aliens.h"

/*****************************************************************************************
*prints graphics to allegro display
*printMap only prints the tiles
*
*printOps only prints your ops
*
*printAyys only prints the ayys
******************************************************************************************/

int printMap(Map &m, int camx, int camy, ALLEGRO_BITMAP *floorbmps[2][TILE_TYPE_COUNT]){
    for(int i = -CAM_SIZE_RIGHT; i < CAM_SIZE_RIGHT + 1; i++){
        for(int j = -CAM_SIZE_DOWN; j < CAM_SIZE_DOWN + 1; j++){
            al_draw_bitmap(floorbmps[m.tiles[camx + i][camy + j].visible][m.tiles[camx + i][camy + j].type],
                           TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j), 0);
            //printf("printing bitmap %d %d\n", camy + i, camx + j);
            //determines the colour of the cover
            ALLEGRO_COLOR lowCol, highCol;
            switch(m.tiles[camx + i][camy + j].type){
                case 0:
                default:
                    lowCol = T0LOW;
                    highCol = T0HIGH;
                    break;
                case 1:
                    lowCol = T1LOW;
                    highCol = T1HIGH;
                    break;
                case 2:
                    lowCol = T2LOW;
                    highCol = T2HIGH;
                    break;
                case 3:
                    lowCol = T3LOW;
                    highCol = T3HIGH;
                    break;
                case 4:
                    lowCol = T4LOW;
                    highCol = T4HIGH;
                    break;
            }

            //printing the cover for E,N,W,S
            if(m.tiles[camx + i][camy + j].cover[FACING_E] != NONE){
                //printf("drawing east cover %d %d\n", camx + i, camy + j);
                al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i + 1) - LOW_W, TILE_SIZE * (CAM_SIZE_DOWN + j),
                                         TILE_SIZE * (CAM_SIZE_RIGHT + i + 1), TILE_SIZE * (CAM_SIZE_DOWN + j + 1), lowCol);
                if(m.tiles[camx + i][camy + j].cover[FACING_E] == HIGH){
                    al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i + 1) - LOW_W, TILE_SIZE * (CAM_SIZE_DOWN + j),
                                             TILE_SIZE * (CAM_SIZE_RIGHT + i + 1) - HIGH_W, TILE_SIZE * (CAM_SIZE_DOWN + j+ 1), highCol);
                }
            }
            if(m.tiles[camx + i][camy + j].cover[FACING_N] != NONE){
                //printf("drawing north cover %d %d\n", camx + i, camy + j);
                al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j),
                                         TILE_SIZE * (CAM_SIZE_RIGHT + i + 1), TILE_SIZE * (CAM_SIZE_DOWN + j) + LOW_W, lowCol);
                if(m.tiles[camx + i][camy + j].cover[FACING_N] == HIGH){
                    al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j) + HIGH_W,
                                             TILE_SIZE * (CAM_SIZE_RIGHT + i + 1), TILE_SIZE * (CAM_SIZE_DOWN + j) + LOW_W, highCol);
                }
            }
            if(m.tiles[camx + i][camy + j].cover[FACING_W] != NONE){
                //printf("drawing west cover %d %d\n", camx + i, camy + j);
                al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j),
                                         TILE_SIZE * (CAM_SIZE_RIGHT + i) + LOW_W, TILE_SIZE * (CAM_SIZE_DOWN + j + 1), lowCol);
                if(m.tiles[camx + i][camy + j].cover[FACING_W] == HIGH){
                    al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i) + HIGH_W, TILE_SIZE * (CAM_SIZE_DOWN + j),
                                             TILE_SIZE * (CAM_SIZE_RIGHT + i) + LOW_W, TILE_SIZE * (CAM_SIZE_DOWN + j+ 1), highCol);
                }
            }
            if(m.tiles[camx + i][camy + j].cover[FACING_S] != NONE){
                //printf("drawing south cover %d %d\n", camx + i, camy + j);
                al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j + 1) - LOW_W,
                                         TILE_SIZE * (CAM_SIZE_RIGHT + i + 1), TILE_SIZE * (CAM_SIZE_DOWN + j + 1), lowCol);
                if(m.tiles[camx + i][camy + j].cover[FACING_S] == HIGH){
                    al_draw_filled_rectangle(TILE_SIZE * (CAM_SIZE_RIGHT + i), TILE_SIZE * (CAM_SIZE_DOWN + j + 1) - LOW_W,
                                             TILE_SIZE * (CAM_SIZE_RIGHT + i + 1), TILE_SIZE * (CAM_SIZE_DOWN + j+ 1) - HIGH_W, highCol);
                }
            }
        }
    }

    return 0;
}


void printOps(Marine ops[], int camx, int camy, ALLEGRO_BITMAP *marinebmps[4][4]){
    //prints your ops to the screen
    for(int i = 0; i < SQUAD_SIZE; i++){
        if(ops[i].x >= camx - CAM_SIZE_RIGHT && ops[i].x <= camx + CAM_SIZE_RIGHT &&
           ops[i].y >= camy - CAM_SIZE_DOWN && ops[i].y <= camy + CAM_SIZE_DOWN &&
           ops[i].isAlive){
            al_draw_bitmap(marinebmps[ops[i].role][ops[i].facingDirection], TILE_SIZE * (CAM_SIZE_RIGHT + ops[i].x - camx), TILE_SIZE * (CAM_SIZE_DOWN + ops[i].y - camy), 0);
        }
    }
}

void printAyys(Marine ayys[], int alienCount, int camx, int camy, ALLEGRO_BITMAP *alienbmps[][4], Map &m){
    //prints the aliens if they are visible
    for(int i = 0; i < alienCount; i++){
        if(ayys[i].x >= camx - CAM_SIZE_RIGHT && ayys[i].x <= camx + CAM_SIZE_RIGHT &&
           ayys[i].y >= camy - CAM_SIZE_DOWN && ayys[i].y <= camy + CAM_SIZE_DOWN &&
           ayys[i].isAlive && m.tiles[ayys[i].x][ayys[i].y].visible == V_TRUE){
            al_draw_bitmap(alienbmps[ayys[i].role][ayys[i].facingDirection], TILE_SIZE * (CAM_SIZE_RIGHT + ayys[i].x - camx), TILE_SIZE * (CAM_SIZE_DOWN + ayys[i].y - camy), 0);
        }
    }
}

void printSelectedOpStats(Marine scott, Map &m, ALLEGRO_BITMAP *marinebmps[][4], ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT], ALLEGRO_FONT *font){
    //prints the alien or the op that is selected's stats to the right side
    al_draw_bitmap(floorbmps[m.tiles[scott.x][scott.y].visible][m.tiles[scott.x][scott.y].type], SCREEN_W - 2 * TILE_SIZE, 3 * TILE_SIZE, 0);
    al_draw_bitmap(marinebmps[scott.role][scott.facingDirection], SCREEN_W - 2 * TILE_SIZE, 3 * TILE_SIZE, 0);

    char buffer[100];
    char holder[100];
    strcpy(buffer, scott.name);
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, 4 * TILE_SIZE, ALLEGRO_ALIGN_CENTER, buffer);

    strcpy(buffer, "HP: ");
    sprintf(holder, "%d", scott.currHealth);
    strcat(buffer, holder);
    strcat(buffer, "/");
    sprintf(holder, "%d", scott.maxHealth);
    strcat(buffer, holder);
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 2.75 * TILE_SIZE, 4.5 * TILE_SIZE, ALLEGRO_ALIGN_LEFT, buffer);

    strcpy(buffer, "AP: ");
    sprintf(holder, "%d", scott.currAP);
    strcat(buffer, holder);
    strcat(buffer, "/");
    sprintf(holder, "%d", scott.maxAP);
    strcat(buffer, holder);
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, 4.5 * TILE_SIZE, ALLEGRO_ALIGN_LEFT, buffer);

    strcpy(buffer, "Acc: ");
    sprintf(holder, "%d", scott.accuracy);
    strcat(buffer, holder);
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 2.75 * TILE_SIZE, 5 * TILE_SIZE, ALLEGRO_ALIGN_LEFT, buffer);

    strcpy(buffer, "Role: ");
    if(scott.isMarine){
        strcat(buffer, MARINE_ROLE[scott.role]);
    }else{
        strcat(buffer, AYY_ROLE[scott.role]);
    }
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, 5 * TILE_SIZE, ALLEGRO_ALIGN_LEFT, buffer);

    strcpy(buffer, "Movement range: ");
    sprintf(holder, "%d", scott.moveDistance);
    strcat(buffer, holder);
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, 5.5 * TILE_SIZE, ALLEGRO_ALIGN_CENTER, buffer);

    strcpy(buffer, "Position: [");
    sprintf(holder, "%d", scott.x);
    strcat(buffer, holder);
    sprintf(holder, "%d", scott.y);
    strcat(buffer, ",");
    strcat(buffer, holder);
    strcat(buffer, "]");
    al_draw_text(font, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, 6 * TILE_SIZE, ALLEGRO_ALIGN_CENTER, buffer);
}

void printIcons(ALLEGRO_BITMAP *iconbmps[4]){
    al_draw_bitmap(iconbmps[A_SCOPE], SCREEN_W - 2 * TILE_SIZE, 0, 0);
    al_draw_bitmap(iconbmps[A_CROSS], SCREEN_W - TILE_SIZE, 0 ,0);
    al_draw_bitmap(iconbmps[A_EYE], SCREEN_W - 2* TILE_SIZE, TILE_SIZE, 0);
    al_draw_bitmap(iconbmps[A_SHIELD], SCREEN_W - TILE_SIZE, TILE_SIZE, 0);
}

void highlight(int x, int y, ALLEGRO_COLOR colour){
    al_draw_rectangle(x, y, x + TILE_SIZE, y + TILE_SIZE, colour, 3);
}

void printIconHighlight(int selectedAction){
    switch(selectedAction){
        case A_NONE:
        default:
            break;
        case A_SCOPE:
            highlight(SCREEN_W - 2 * TILE_SIZE, 0, LIGHT_BLUE);
            break;
        case A_CROSS:
            highlight(SCREEN_W - TILE_SIZE, 0, LIGHT_BLUE);
            break;
        case A_EYE:
            highlight(SCREEN_W - 2 * TILE_SIZE, TILE_SIZE, LIGHT_BLUE);
            break;
        case A_SHIELD:
            highlight(SCREEN_W - TILE_SIZE, TILE_SIZE, LIGHT_BLUE);
            break;
    }
}

void centerCamera(Marine scott, Map &m, int &camx, int &camy){
    if(!(scott.x >= CAM_SIZE_RIGHT && scott.x <= m.width - CAM_SIZE_RIGHT - 1 && scott.y >= CAM_SIZE_DOWN && scott.y <= m.height - CAM_SIZE_DOWN - 1)){
        //invalid
        if(scott.x < CAM_SIZE_RIGHT){
            camx = CAM_SIZE_RIGHT;
        }
        if(scott.x > m.width - CAM_SIZE_RIGHT - 1){
            camx = m.width - CAM_SIZE_RIGHT - 1;
        }
        if(scott.y < CAM_SIZE_DOWN){
            camy = CAM_SIZE_DOWN;
        }
        if(scott.y > m.height - CAM_SIZE_DOWN - 1){
            camy = m.height - CAM_SIZE_DOWN - 1;
        }
    }else{
        camx = scott.x;
        camy = scott.y;
    }
}

int getXHere(ALLEGRO_MOUSE_EVENT mouse, int camx){
    return ( (int)(mouse.x / TILE_SIZE) - CAM_SIZE_RIGHT + camx );
}

int getYHere(ALLEGRO_MOUSE_EVENT mouse, int camy){
    return ( (int)(mouse.y / TILE_SIZE) - CAM_SIZE_DOWN + camy );
}
