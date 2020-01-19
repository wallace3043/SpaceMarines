/*******************************************************************************
 *  Author: Wallace Long     Date: May 9, 2018       Course: ISC3U
 *
 *  Project Name: aliens
 *  Game Name: aliens 2099
 *  Game:
 *
 *  Notes:
 *  -space marine, marine, operative (ops) are all the same
 *  -scott is the example marine
 *  -this game is absolutely garbage
 *
 *  Known Issues:
 *  -sometimes crashes when it takes too many inputs
 *  -for some reason, the names read in don't work sometimes, probably cause they were
 *      copy pasted from the internet and some had non-ascii characters
 *  -
 *******************************************************************************/

#include "aliens.h"

int main(int argc, char *argv[]){
    srand(time(0));
    system("COLOR 0A"); //green w/ black
    initializeAllegro();

    //creating, loading allegro stuff and bitmaps
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_FONT *mainFont1 = al_load_ttf_font("fonts/main_space.ttf", 64, 0);
    ALLEGRO_FONT *mainFont2 = al_load_ttf_font("fonts/main_space.ttf", 36, 0);
    ALLEGRO_FONT *smallFont1 = al_load_ttf_font("fonts/small_futuristic.ttf", 16, 0);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_BITMAP *mainMenu = al_load_bitmap("bitmaps/misc/main_menu.bmp");
    checkAllegroSetup(display, mainFont1, mainFont2, smallFont1, event_queue);

    ALLEGRO_BITMAP *floorbmps[2][TILE_TYPE_COUNT] = {nullptr}; //dark then light
    ALLEGRO_BITMAP *marinebmps[4][4] = {nullptr};
    ALLEGRO_BITMAP *alienbmps[4][4] = {nullptr};
    ALLEGRO_BITMAP *iconbmps[4] = {nullptr};
    ALLEGRO_BITMAP *bulletbmps[4] = {nullptr};
    loadAlienbmps(alienbmps);
    loadFloorbmps(floorbmps);
    loadMarinebmps(marinebmps);
    loadIconbmps(iconbmps);
    loadBulletbmps(bulletbmps);

    int gameState = MAIN_MENU;
    bool fullQuit = false;
    int lvlsWon = 0;
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    while(!fullQuit){
        switch(gameState){
            case MAIN_MENU:
            {
                al_draw_bitmap(mainMenu, 0, 0, 0);
                al_draw_text(mainFont1, PANTONE_ORANGE, 512, 96, ALLEGRO_ALIGN_CENTER, "ALIENS : 2099"); //title
                al_draw_text(mainFont2, PANTONE_ORANGE, 512, 160, ALLEGRO_ALIGN_CENTER, "THE GAME"); //subtitle
                al_draw_filled_rectangle(386, 244, 642, 292, STORM_BLUE);//button
                al_draw_text(mainFont2, PANTONE_ORANGE, 512, 256, ALLEGRO_ALIGN_CENTER, "START GAME");
                al_draw_filled_rectangle(386, 340, 642, 388, STORM_BLUE);//button
                al_draw_text(mainFont2, PANTONE_ORANGE, 512, 352, ALLEGRO_ALIGN_CENTER, "QUIT");
                al_flip_display();
                bool doExit = false;
                while(!doExit){
                    ALLEGRO_EVENT ev;
                    al_wait_for_event(event_queue, &ev);
                    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                        if(isMouseIn(ev.mouse, 386, 244, 642, 292)){
                            doExit = true;
                            gameState = EXPOSITION;
                            al_clear_to_color(BLACK);
                            al_flip_display();
                        }else if(isMouseIn(ev.mouse, 386, 340, 642, 388)){
                            doExit = true;
                            gameState = QUIT;
                            al_clear_to_color(BLACK);
                            al_flip_display();
                        }
                    }else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                        switch(ev.keyboard.keycode){
                            case ALLEGRO_KEY_ENTER:
                            case ALLEGRO_KEY_SPACE:
                                doExit = true;
                                gameState = EXPOSITION;
                                al_clear_to_color(BLACK);
                                al_flip_display();
                                break;
                            case ALLEGRO_KEY_ESCAPE:
                                doExit = true;
                                gameState = QUIT;
                                al_clear_to_color(BLACK);
                                al_flip_display();
                                break;
                        }
                    }
                }
                break;
            }
            case EXPOSITION:
            {
                //printing rules, "story"
                al_clear_to_color(STEEL);
                int gap = 48;
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32, ALLEGRO_ALIGN_LEFT, "Earth is being invaded by aliens.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + gap, ALLEGRO_ALIGN_LEFT, "You command 6 elite marines.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 2 * gap, ALLEGRO_ALIGN_LEFT, "Use WASD or arrow keys to change where you're looking.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 3 * gap, ALLEGRO_ALIGN_LEFT, "Use tab/shift or click to change the marine selected. Use c to center the camera");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 4 * gap, ALLEGRO_ALIGN_LEFT, "Use 1-4 or click to change the selected action.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 5 * gap, ALLEGRO_ALIGN_LEFT, "Click to perform the selected action.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 6 * gap, ALLEGRO_ALIGN_LEFT, "Enter, space, or backspace to end your turn.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 7 * gap, ALLEGRO_ALIGN_LEFT, "Low and high cover represented by lines provide cover for both you and the aliens.");
                al_draw_text(smallFont1, PANTONE_ORANGE, 32, 32 + 8 * gap, ALLEGRO_ALIGN_LEFT, "Please be patient, don't click while the aliens' turn is still processing.");
                al_flip_display();
                al_rest(2);

                al_draw_text(smallFont1, PANTONE_ORANGE, 512, SCREEN_H - gap, ALLEGRO_ALIGN_CENTER, "Press any key to continue...");
                al_flip_display();

                ALLEGRO_EVENT ev1;
                do{
                    al_wait_for_event(event_queue, &ev1);
                }while(ev1.type != ALLEGRO_EVENT_KEY_DOWN && ev1.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);

                gameState = PLAYING_GAME;
                break;
            }
            case PLAYING_GAME:
            {
                //one campaign, creates the squad for the campaign
                Marine yourOps[SQUAD_SIZE];
                initSquad(yourOps);
                Map currentMap;
                lvlsWon = 0;
                int alienCount = 8;

                while(areMarinesAlive(yourOps) && lvlsWon < LVLS_TO_WIN){
                    //one level
                    int coverOdds[3] = {75, 15, 10};
                    int tileOdds[TILE_TYPE_COUNT] = {35, 20, 25, 15, 10};
                    char mapName[10] = "gen2.map";
                    generateMap(30 + lvlsWon * 6, 20 + lvlsWon * 4, coverOdds, tileOdds, mapName);
                    clearMap(currentMap);
                    readMap(currentMap, "maps/gen2.map");

                    //placing ops in the top left and refreshes their health and ap
                    for(int i = 0; i < SQUAD_SIZE; i++){
                        if(yourOps[i].isAlive){
                            yourOps[i].x = i / 2;
                            yourOps[i].y = i % 2;
                            yourOps[i].currHealth = yourOps[i].maxHealth;
                            yourOps[i].currAP = yourOps[i].maxAP;
                        }
                    }

                    Marine theirAyys[alienCount];
                    initAliens(theirAyys, alienCount, currentMap);
                    int camx = CAM_SIZE_RIGHT; //center coordinates of camera
                    int camy = CAM_SIZE_DOWN;
                    int selectedAction = A_NONE;
                    int selectedMarineIndex = 0;
                    int selectedAlienIndex = 0;
                    bool isFiring = false;
                    bool printingOpsStats = true;
                    bool printingStats = true;
                    int bullX, bullY;

                    char message[100] = "DEFEND ";
                    strcat(message, CITIES[lvlsWon]);

                    while(areAyysAlive(theirAyys, alienCount) && areMarinesAlive(yourOps)){
                        refreshMarines(yourOps, SQUAD_SIZE);
                        bool marineTurnDone = false;
                        while(!marineTurnDone){
                            //one marine turn

                            //printing all the graphics
                            al_clear_to_color(STORM_BLUE);
                            clearVisibility(currentMap);
                            for(int j = 0; j < SQUAD_SIZE; j++){
                                determineFOV(currentMap, yourOps[j]);
                            }
                            printMap(currentMap, camx, camy, floorbmps);
                            highlight((CAM_SIZE_RIGHT - camx + yourOps[selectedMarineIndex].x) * TILE_SIZE, (CAM_SIZE_DOWN - camy + yourOps[selectedMarineIndex].y) * TILE_SIZE, LIGHT_BLUE);
                            al_draw_filled_rectangle(SCREEN_W - 3 * TILE_SIZE, 0, SCREEN_W - 3 * TILE_SIZE + 12, SCREEN_H, STEEL);
                            printIcons(iconbmps);
                            printIconHighlight(selectedAction);
                            if(printingStats){
                                if(printingOpsStats){
                                    if(yourOps[selectedMarineIndex].isAlive)
                                        printSelectedOpStats(yourOps[selectedMarineIndex], currentMap, marinebmps, floorbmps, smallFont1);
                                }else{
                                    if(theirAyys[selectedAlienIndex].isAlive)
                                        printSelectedOpStats(theirAyys[selectedAlienIndex], currentMap, alienbmps, floorbmps, smallFont1);
                                }
                            }
                            printOps(yourOps, camx, camy, marinebmps);
                            printAyys(theirAyys, alienCount, camx, camy, alienbmps, currentMap);
                            if(isFiring){
                                isFiring = animateShot(yourOps[selectedMarineIndex], theirAyys[selectedAlienIndex], camx, camy, bullX, bullY, bulletbmps);
                            }
                            al_draw_text(smallFont1, PANTONE_ORANGE, SCREEN_W - 1.5 * TILE_SIZE, SCREEN_H - 1.5 * TILE_SIZE, ALLEGRO_ALIGN_CENTER, message);
                            al_flip_display();

                            /*marine turn*/
                            if(!isFiring){
                                ALLEGRO_EVENT ev1;
                                al_wait_for_event(event_queue, &ev1);

                                if(ev1.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                                    if(isMouseIn(ev1.mouse, TILE_SIZE * 13, 0, SCREEN_W, SCREEN_H)){
                                        //in left action bar
                                        if(isMouseIn(ev1.mouse, SCREEN_W - 2 * TILE_SIZE, 0, SCREEN_W - TILE_SIZE, TILE_SIZE)){
                                            selectedAction = A_SCOPE;
                                            strcpy(message, "SHOOT");
                                        }else if(isMouseIn(ev1.mouse, SCREEN_W - TILE_SIZE, 0, SCREEN_W, TILE_SIZE)){
                                            selectedAction = A_CROSS;
                                            strcpy(message, "MOVE");
                                        }else if(isMouseIn(ev1.mouse, SCREEN_W - 2 * TILE_SIZE, TILE_SIZE, SCREEN_W - TILE_SIZE, 2 * TILE_SIZE)){
                                            selectedAction = A_EYE;
                                            strcpy(message, "EXAMINE");
                                        }else if(isMouseIn(ev1.mouse, SCREEN_W - TILE_SIZE, TILE_SIZE, SCREEN_W, 2 * TILE_SIZE)){
                                            selectedAction = A_SHIELD;
                                            strcpy(message, "COVER");
                                        }else{
                                            selectedAction = A_NONE;
                                        }
                                    }else{
                                        //on map
                                        if(isMarineHereMouse(ev1.mouse, camx, camy, yourOps)){
                                            //change marine selected with mouse
                                            if(selectedAction != A_SHIELD){
                                                selectedMarineIndex = getMarineIHereMouse(ev1.mouse, camx, camy, yourOps);
                                                printingOpsStats = true;
                                                printingStats = true;
                                            }else{
                                                if(yourOps[selectedMarineIndex].currAP > 0){
                                                    yourOps[selectedMarineIndex].coverMultiplier = 2;
                                                    yourOps[selectedMarineIndex].currAP = 0;
                                                    strcpy(message, "Taking cover");
                                                }else{
                                                    strcpy(message, "No actions.");
                                                }
                                            }
                                        }else if(isAlienHereMouse(ev1.mouse, camx, camy, theirAyys, alienCount)){
                                                selectedAlienIndex = getAlienIHereMouse(ev1.mouse, camx, camy, theirAyys, alienCount);
                                                printingOpsStats = false;
                                                printingStats = true;
                                                switch(selectedAction){
                                                    case A_SCOPE:
                                                        if(yourOps[selectedMarineIndex].currAP > 0){
                                                            isFiring = shootMarine(yourOps[selectedMarineIndex], theirAyys[selectedAlienIndex], currentMap, message);
                                                            bullX = TILE_SIZE * (yourOps[selectedMarineIndex].x + CAM_SIZE_RIGHT - camx);
                                                            bullY = TILE_SIZE * (yourOps[selectedMarineIndex].y + CAM_SIZE_DOWN - camy);
                                                        }else{
                                                            strcpy(message, "No actions.");
                                                        }
                                                        break;
                                                    case A_CROSS:
                                                        strcpy(message, "You cannot move here.");
                                                        break;
                                                    case A_EYE:
                                                        printingOpsStats = false;
                                                        break;
                                                    case A_SHIELD:
                                                        strcpy(message, "Cover your own men.");
                                                        break;
                                                }
                                        }else{
                                            switch(selectedAction){
                                                case A_SCOPE:
                                                    strcpy(message, "Nothing to shoot here.");
                                                    break;
                                                case A_CROSS:
                                                    if(yourOps[selectedMarineIndex].currAP <= 0){
                                                        strcpy(message, "No actions.");
                                                    }else if( abs(yourOps[selectedMarineIndex].x - getXHere(ev1.mouse, camx)) + abs(yourOps[selectedMarineIndex].y - getYHere(ev1.mouse, camy)) > yourOps[selectedMarineIndex].moveDistance){
                                                        strcpy(message, "Too far.");
                                                    }else{
                                                        if(isTileOcc(getXHere(ev1.mouse, camx), getYHere(ev1.mouse, camy), yourOps, theirAyys, alienCount)){
                                                            strcpy(message, "Tile occupied.");
                                                        }else{
                                                            moveMarine(yourOps[selectedMarineIndex], getXHere(ev1.mouse, camx), getYHere(ev1.mouse, camy));
                                                            strcpy(message, "Moving.");
                                                        }
                                                    }
                                                    break;
                                                case A_EYE:
                                                    printingStats = false;
                                                    break;
                                                case A_SHIELD:
                                                    printingStats = true;
                                                    break;
                                                }
                                        }
                                    }
                                }else if(ev1.type == ALLEGRO_EVENT_KEY_CHAR){ //repeating
                                    switch(ev1.keyboard.keycode){
                                        //wasd moves area of vision
                                        case ALLEGRO_KEY_LEFT:
                                        case ALLEGRO_KEY_A:
                                            if(camx > CAM_SIZE_RIGHT){
                                                camx--;
                                            }
                                            break;
                                        case ALLEGRO_KEY_RIGHT:
                                        case ALLEGRO_KEY_D:
                                            if(camx < currentMap.width - CAM_SIZE_RIGHT - 1){
                                                camx++;
                                            }
                                            break;
                                        case ALLEGRO_KEY_UP:
                                        case ALLEGRO_KEY_W:
                                            if(camy > CAM_SIZE_DOWN){
                                                camy--;
                                            }
                                            break;
                                        case ALLEGRO_KEY_DOWN:
                                        case ALLEGRO_KEY_S:
                                            if(camy < currentMap.height - CAM_SIZE_DOWN - 1){
                                                camy++;
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }else if(ev1.type == ALLEGRO_EVENT_KEY_DOWN){ //one time press
                                    switch(ev1.keyboard.keycode){
                                        case ALLEGRO_KEY_ENTER:
                                        case ALLEGRO_KEY_BACKSPACE:
                                        case ALLEGRO_KEY_SPACE:
                                            //endturn
                                            marineTurnDone = true;
                                            break;
                                        case ALLEGRO_KEY_TAB:
                                            //tab, lshift change the selected marine
                                            do{
                                                selectedMarineIndex++;
                                                selectedMarineIndex %= SQUAD_SIZE;
                                            }while(!yourOps[selectedMarineIndex].isAlive);
                                            centerCamera(yourOps[selectedMarineIndex], currentMap, camx, camy);
                                            printingOpsStats = true;
                                            break;
                                        case ALLEGRO_KEY_LSHIFT:
                                            do{
                                                selectedMarineIndex += SQUAD_SIZE - 1;
                                                selectedMarineIndex %= SQUAD_SIZE;
                                            }while(!yourOps[selectedMarineIndex].isAlive);
                                            centerCamera(yourOps[selectedMarineIndex], currentMap, camx, camy);
                                            printingOpsStats = true;
                                            break;
                                        case ALLEGRO_KEY_C:
                                            //centers camera
                                            centerCamera(yourOps[selectedMarineIndex], currentMap, camx, camy);
                                            printingOpsStats = true;
                                            break;
                                        case ALLEGRO_KEY_1:
                                            //1,2,3,4 change the selected action
                                            selectedAction = A_SCOPE;
                                            strcpy(message, "SHOOT");
                                            break;
                                        case ALLEGRO_KEY_2:
                                            selectedAction = A_CROSS;
                                            strcpy(message, "MOVE");
                                            break;
                                        case ALLEGRO_KEY_3:
                                            selectedAction = A_EYE;
                                            strcpy(message, "EXAMINE");
                                            break;
                                        case ALLEGRO_KEY_4:
                                            selectedAction = A_SHIELD;
                                            strcpy(message, "COVER");
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }else{
                                //let the animations for shooting run
                                //printf("isFiring\n");
                            }
                        }/*marine turn*/

                        //ayy turn
                        //printf("ayyturn\n");
                        refreshMarines(theirAyys, alienCount);
                        ayyTurn(theirAyys, alienCount, currentMap, yourOps);
                        marineTurnDone = false;
                    }

                    if(!areAyysAlive(theirAyys, alienCount)){
                        //you win the level, else you lose the game
                        alienCount += 4;
                        lvlsWon++;
                    }

                }
                gameState = GAME_OVER;
                break;
            }
            case GAME_OVER:
            {
                //you win/lose. prints the background image and the message
                al_clear_to_color(STEEL);
                ALLEGRO_BITMAP *background = nullptr;
                char msg[100];

                if(lvlsWon >= LVLS_TO_WIN){
                    background = al_load_bitmap("bitmaps/misc/victory.bmp");
                    strcpy(msg, "Well done. Earth has been saved.");
                }else{
                    background = al_load_bitmap("bitmaps/misc/defeat.bmp");
                    strcpy(msg, "You are a complete disappointment.");
                }

                if(!background){
                        printf("failed to load victory background\n");
                }else{
                    al_draw_bitmap(background, 0, 0, 0);
                }
                al_draw_text(mainFont2, PANTONE_ORANGE, 512, 288, ALLEGRO_ALIGN_CENTER, msg);
                al_flip_display();
                al_destroy_bitmap(background);
                al_rest(3);

                //press any key to continue
                ALLEGRO_EVENT ev1;
                do{
                    al_wait_for_event(event_queue, &ev1);
                }while(ev1.type != ALLEGRO_EVENT_KEY_DOWN && ev1.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
                gameState = MAIN_MENU;
                break;
            }
            case QUIT:
            default:
                //closes allegro
                printf("Thanks for playing!\n");
                fullQuit = true;
                break;
        }
    }

    //destroying allegro stuff
    destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
    destroyFloorbmps(floorbmps);
    destroyMarinebmps(marinebmps);
    destroyMarinebmps(alienbmps);
    destroyIconbmps(iconbmps);

    return 0;
}

