#include "aliens.h"

/*********************************
*nvm any of this jsut do a simple triangle
*
*you cannot see past high cover, but you can see past low cover at no cost //or not????
*draw distance of 10??????
*recursive determining of FOV??? nah
*or determine by drawing lines, determining the # of covers in that direction
*or split into two triangles, left and right, with the left with the big vertical line?
*FOR NOW, COVER DOES NOT IMPEDE VISION
*FACING_S = 3   cos=0 sin=-1
*0       OOOOOOOMOOOOOOO
*1       OOOOOOVVVOOOOOO
*2       OOOOOVVVVLOOOOO
*3       OOOOVHVVVVVOOOO
*4       OOOVNNVVLVVVOOO
*5       OOVNNNVVVVVVVOO
*6       OVNNNNVVVVVVVVO
*7       NNNNNNNNNNNNNNN
*        765432101234567
*        ------- +++++++
*
*FACING_N = 1   cos=0 sin=1
*-7      NNNNNNNNNNNNNNN
*-6      OVNNNNVVVVVVVVO
*-5      OOVNNNVVVVVVVOO
*-4      OOOVNNVVLVVVOOO
*-3      OOOOVHVVVVVOOOO
*-2      OOOOOVVVVLOOOOO
*-1      OOOOOOVVVOOOOOO
*0       OOOOOOOMOOOOOOO
*        765432101234567
*        ------- +++++++
*FACING_E = 0   cos=1 sin=0
*01234567
*OOOOOOON -7
*OOOOOOVN -6
*OOOOOVVN -5
*OOOOVVVN -4
*OOOVVVVN -3
*OOLVVVVN -2
*OVVVLVVN -1
*MVVVVVVN 0 mXPos, mYPos (middle x, y coordinates)
*OVVVVVVN 1 rXPos, rYPos (real x, y coordinates of thing checked)
*OOVHNNNN 2
*OOOVNNNN 3
*OOOOVNNN 4
*OOOOOVNN 5
*OOOOOOVN 6
*OOOOOOON 7
*********************************/

int determineFOV(Map &m, Marine scott){
    if(scott.isAlive){
        int rad = 2;
        for(int i = -rad; i <= rad; i++){
            for(int j = -rad; j <= rad; j++){
                if(0 <= scott.x + i && scott.x + i <= m.width - 1 && 0 <= scott.y + j && scott.y + j <= m .height - 1){
                    m.tiles[scott.x + i][scott.y + j].visible = V_TRUE;
                }
            }
        }

        //conic vision from scott
        for(int i = 0; i < scott.drawDistance + 1; i++){
            //middle x/y
            int mXPos = scott.x + (i * cosf2(scott.facingDirection));
            int mYPos = scott.y - (i * sinf2(scott.facingDirection));
            for(int j = -i; j <= i; j++){//-i to i
                //shift left/right or up/down
                int rXPos = mXPos + (j * sinf2(scott.facingDirection));
                int rYPos = mYPos + (j * cosf2(scott.facingDirection));
                if(!(rXPos < 0 || rXPos >= m.width || rYPos < 0 || rYPos >= m.height)){
                    m.tiles[rXPos][rYPos].visible = V_TRUE;
                }
            }
        }
    }
    //printf("FOV determined\n");
    return 0;
}

