#include "aliens.h"

int loadFloorbmps(ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT]){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < TILE_TYPE_COUNT; j++){
            char fileName[64] = "bitmaps/tiles/terrainX/terrainX";
            //                   01234567890123456789012345678901234
            fileName[21] = j + 1 + '0';
            fileName[30] = j + 1 + '0';
            if(i == 0){
                strcat(fileName, "dark");
            }
            strcat(fileName, ".png");
            floorbmps[i][j] = al_load_bitmap(fileName);

            if(!floorbmps[i][j]){
                printf("Failed to load tile bitmap %d, %d\n", i + 1, j + 1);
                return -1;
            }
        }
    }


    return 0;
}

int destroyFloorbmps(ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT]){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < TILE_TYPE_COUNT; j++){
            al_destroy_bitmap(floorbmps[i][j]);
        }
    }
    return 0;
}

int loadMarinebmps(ALLEGRO_BITMAP *marinebmps[][4]){
    char fileName[40] = "bitmaps/marines/????.png";
    //                   012345678901234567890123
    for(int i = 0; i < 4; i++){
        switch(i){
            case ASSAULT:
                fileName[16] = 'a';
                fileName[17] = 's';
                fileName[18] = 's';
                break;
            case SNIPER:
                fileName[16] = 's';
                fileName[17] = 'n';
                fileName[18] = 'i';
                break;
            case SUPPORT:
                fileName[16] = 's';
                fileName[17] = 'u';
                fileName[18] = 'p';
                break;
            case HEAVY:
                fileName[16] = 'h';
                fileName[17] = 'e';
                fileName[18] = 'a';
                break;
        }

        for(int j = 0; j < 4; j++){
            switch(j){
                case FACING_E:
                    fileName[19] = 'E';
                    break;
                case FACING_N:
                    fileName[19] = 'N';
                    break;
                case FACING_W:
                    fileName[19] = 'W';
                    break;
                case FACING_S:
                    fileName[19] = 'S';
                    break;
            }

            marinebmps[i][j] = al_load_bitmap(fileName);
            if(!marinebmps[i][j]){
                printf("Failed to load marine bitmap %d %d\n", i + 1, j + 1);
                return -1;
            }else{
                al_convert_mask_to_alpha(marinebmps[i][j], PINK);
            }
        }
    }
    return 0;
}

int destroyMarinebmps(ALLEGRO_BITMAP *marinebmps[][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            al_destroy_bitmap(marinebmps[i][j]);
        }
    }
    return 0;
}

int loadAlienbmps(ALLEGRO_BITMAP *alienbmps[][4]){
    alienbmps[AYY_BEAN][FACING_E] = al_load_bitmap("bitmaps/ayys/beaE.bmp");
    alienbmps[AYY_BEAN][FACING_N] = al_load_bitmap("bitmaps/ayys/beaN.bmp");
    alienbmps[AYY_BEAN][FACING_W] = al_load_bitmap("bitmaps/ayys/beaW.bmp");
    alienbmps[AYY_BEAN][FACING_S] = al_load_bitmap("bitmaps/ayys/beaS.bmp");

    alienbmps[AYY_BLOB][FACING_E] = al_load_bitmap("bitmaps/ayys/bloE.bmp");
    alienbmps[AYY_BLOB][FACING_N] = al_load_bitmap("bitmaps/ayys/bloN.bmp");
    alienbmps[AYY_BLOB][FACING_W] = al_load_bitmap("bitmaps/ayys/bloW.bmp");
    alienbmps[AYY_BLOB][FACING_S] = al_load_bitmap("bitmaps/ayys/bloS.bmp");

    alienbmps[AYY_DEMON][FACING_E] = al_load_bitmap("bitmaps/ayys/demE.bmp");
    alienbmps[AYY_DEMON][FACING_N] = al_load_bitmap("bitmaps/ayys/demN.bmp");
    alienbmps[AYY_DEMON][FACING_W] = al_load_bitmap("bitmaps/ayys/demW.bmp");
    alienbmps[AYY_DEMON][FACING_S] = al_load_bitmap("bitmaps/ayys/demS.bmp");

    alienbmps[AYY_EYES][FACING_E] = al_load_bitmap("bitmaps/ayys/eyeE.bmp");
    alienbmps[AYY_EYES][FACING_N] = al_load_bitmap("bitmaps/ayys/eyeN.bmp");
    alienbmps[AYY_EYES][FACING_W] = al_load_bitmap("bitmaps/ayys/eyeW.bmp");
    alienbmps[AYY_EYES][FACING_S] = al_load_bitmap("bitmaps/ayys/eyeS.bmp");

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(!alienbmps[i][j]){
                printf("Failed the load alien bitmap %d %d", i, j);
            }else{
                al_convert_mask_to_alpha(alienbmps[i][j], PINK);
            }
        }
    }
    return 0;
}

int loadIconbmps(ALLEGRO_BITMAP *iconbmps[4]){
    iconbmps[A_SCOPE] = al_load_bitmap("bitmaps/icons/scope.png");
    iconbmps[A_CROSS] = al_load_bitmap("bitmaps/icons/cross.png");
    iconbmps[A_EYE] = al_load_bitmap("bitmaps/icons/eye.png");
    iconbmps[A_SHIELD] = al_load_bitmap("bitmaps/icons/shield.png");

    for(int i = 0; i < 4; i++){
        if(!iconbmps[i]){
            printf("Failed to load icon bitmap %d\n", i + 1);
            return -1;
        }else{
            al_convert_mask_to_alpha(iconbmps[i], PINK);
        }
    }
    return 0;
}

int destroyIconbmps(ALLEGRO_BITMAP *iconbmps[4]){
    for(int i = 0; i < 4; i++){
        al_destroy_bitmap(iconbmps[i]);
    }
    return 0;
}

int loadBulletbmps(ALLEGRO_BITMAP *bulletbmps[4]){
    bulletbmps[B_GREY] = al_load_bitmap("bitmaps/bullets/grey.png");
    bulletbmps[B_BLUE] = al_load_bitmap("bitmaps/bullets/blue.png");
    bulletbmps[B_RED] = al_load_bitmap("bitmaps/bullets/red.png");
    bulletbmps[B_GREEN] = al_load_bitmap("bitmaps/bullets/green.png");

    for(int i = 0; i < 4; i++){
        if(!bulletbmps[i]){
            printf("Failed to load bullet bitmap %d\n", i + 1);
            return -1;
        }else{
            al_convert_mask_to_alpha(bulletbmps[i], PINK);
        }
    }
    return 0;
}

int destroyBulletbmps(ALLEGRO_BITMAP *bulletbmps[4]){
    for(int i = 0; i < 4; i++){
        al_destroy_bitmap(bulletbmps[i]);
    }
    return 0;
}
