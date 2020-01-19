#include "aliens.h"

int randInt(int low, int high){
    return (rand() % (high - low + 1) + low);
}

bool isMouseIn(ALLEGRO_MOUSE_EVENT &mouse, int x1, int y1, int x2, int y2){
    int tmp;
    if(x1 > x2){
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if(y1 > y2){
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    if(mouse.x >= x1 && mouse.x <= x2 && mouse.y >= y1 && mouse.y <= y2){
        return true;
    }
    return false;
}

//these functions exist because i have royally messed up in FOV
//angle divided by PI/2
int cosf2(int angle){
    int out = 1;
    switch(angle){
        case 0:
        default:
            out = 1;
            break;
        case 1:
            out = 0;
            break;
        case 2:
            out = -1;
            break;
        case 3:
            out = 0;
            break;
    }

    return out;
}

int sinf2(int angle){
    int out = 0;
    switch(angle){
        case 0:
        default:
            out = 0;
            break;
        case 1:
            out = 1;
            break;
        case 2:
            out = 0;
            break;
        case 3:
            out = -1;
            break;
    }

    return out;

}

void generateMap(int width, int height, int cover[3], int tiles[TILE_TYPE_COUNT], char fileName[]){
    //none, low, and high are the percentages of of frequency of cover types out of 100, similar for tiles
    char name[100] = "maps/";
    strcat(name, fileName);
    printf("generating map %s\n", fileName);
    FILE *ftpr;
    ftpr = fopen(name, "w");
    fprintf(ftpr, "%d %d\n", width, height);

    int rcover[3] = {0};
    rcover[0] = cover[0];
    for(int i = 1; i < 3; i++){
        rcover[i] = cover[i] + rcover[i - 1];
    }

    int rtiles[TILE_TYPE_COUNT] = {0};
    rtiles[0] = tiles[0];
    for(int i = 1; i < TILE_TYPE_COUNT; i++){
        rtiles[i] = tiles[i] + rtiles[i - 1];
    }

    char buffer[width * 2];
    int odds;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            odds = randInt(0, 100);
            for(int k = 0; k < TILE_TYPE_COUNT; k++){
                if(odds <= rtiles[k]){
                    buffer[2 * j] = k + 'a';
                    break;
                }
            }

            if(j != width - 1){
                odds = randInt(0, 100);
                for(int k = 0; k < 3; k++){
                    if(odds <= rcover[k]){
                        buffer[2 * j + 1] = (2 * k) + '0';
                        break;
                    }
                }
            }
        }

        fprintf(ftpr, buffer);
        //printf("%s", buffer);

        if(i != height - 1){
            for(int j = 0; j < width; j++){
                odds = randInt(0, 100);
                for(int k = 0; k < 3; k++){
                    if(odds <= rcover[k]){
                        buffer[2 * j] = (2 * k) + '0';
                        if(j != width - 1){
                            buffer[2 * j + 1] = '-';
                        }
                        break;
                    }
                }
            }
            fprintf(ftpr, "\n");
            fprintf(ftpr, buffer);
            fprintf(ftpr, "\n");
            //printf("\n%s\n", buffer);
        }
    }
    fclose(ftpr);
}
