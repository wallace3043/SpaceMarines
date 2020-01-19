
#include "aliens.h"

/**
map format      fileName.map
5 5             width height
a0b0c2d2e       tileCOVERtileCOVER...
4-0-0-4-4       COVER COVER COVER...
f2g0h0i0j       Terrain types in the map file range from a,b,c,d,e
2-2-2-2-2
k0l0m0n0o
0-0-0-4-0
p2q0r4s4t
0-0-2-4-2
u0v0w0x0y
etc.
**/

void clearMap(Map &m){
    for(int i = 0; i < MAP_W_L; i++){
        for(int j = 0; j < MAP_H_L; j++){
            m.tiles[i][j].cover[FACING_E] = NONE;
            m.tiles[i][j].cover[FACING_N] = NONE;
            m.tiles[i][j].cover[FACING_W] = NONE;
            m.tiles[i][j].cover[FACING_S] = NONE;
            m.tiles[i][j].type = 0;
        }
    }
    clearVisibility(m);
}

void clearVisibility(Map &m){
    for(int i = 0; i < MAP_H_L; i++){
        for(int j = 0; j < MAP_W_L; j++){
            m.tiles[i][j].visible = V_FALSE;
        }
    }
}

int readMap(Map &m, char fileName[]){
    printf("reading map %s\n", fileName);

    FILE *ftpr;
    ftpr = fopen(fileName, "r");
    if(ftpr == NULL){
        return 1;
    }

    fscanf(ftpr, "%d", &m.width);
    fscanf(ftpr, "%d", &m.height);
    //printf("%d %d\n", m.width, m.height);

    //reads in the text file line by line, and reads from those lines for info
    char holder;
    fscanf(ftpr, "%1c", &holder); //newline
    char holdStr[m.width * 2];

    for(int i = 0; i < m.height; i++){
        fscanf(ftpr, "%s", holdStr);
        //printf("%s\n", holdStr);
        for(int j = 0; j < m.width; j++){
            m.tiles[j][i].type = holdStr[2 * j] - 'a';
            //printf("%c%c%d\t", m.tiles[j][i].type + 'a', holdStr[2*j],j);
            //determine cover of this tile East and next tile West
            if(j != m.width - 1){
                m.tiles[j][i].cover[FACING_E] = holdStr[2 * j + 1] - '0';
                m.tiles[j + 1][i].cover[FACING_W] = holdStr[2 * j + 1] - '0';
                //printf("%c%c%c%c%d\t", m.tiles[j][i].cover[FACING_E] + '0',holdStr[2 * i + 1], m.tiles[j + 1][i].cover[FACING_W],holdStr[2 * i + 1] + '0',j);
            }
        }
        //printf("\n");

        //read in south and north side cover
        if(i != m.height - 1){
            fscanf(ftpr, "%s", holdStr);
            for(int j = 0; j < m.width; j++){
                m.tiles[j][i].cover[FACING_S] = holdStr[2 * j] - '0';
                m.tiles[j][i + 1].cover[FACING_N] = holdStr[2 * j] - '0';
                //printf("%c%c", m.tiles[j][i].cover[FACING_N] + '0', m.tiles[j][i + 1].cover[FACING_S] + '0');

                if(j != m.width - 1){
                    //printf("-");
                }
            }
        }
        //printf("\n");
    }

    printf("map %s read\n", fileName);
    return 0;
}
