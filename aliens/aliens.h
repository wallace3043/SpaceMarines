#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>

//gamestates
#define MAIN_MENU 0
#define PLAYING_GAME 1
#define GAME_OVER 2
#define QUIT 3
#define HIGH_SCORES 4
#define EXPOSITION 5
//for tile visibility
#define V_TRUE 1
#define V_FALSE 0
#define PI 3.1415926535897932384626433832795
#define FACING_N 1
#define FACING_E 0
#define FACING_S 3
#define FACING_W 2
//marines
#define ASSAULT 0
#define SNIPER 1
#define SUPPORT 2
#define HEAVY 3
//aliens
#define AYY_BEAN 0
#define AYY_BLOB 1
#define AYY_DEMON 2
#define AYY_EYES 3
//tiles
#define T_GRASS 0
#define T_SAND 1
#define T_WATER 2
#define T_ICE 3
#define T_METAL 4
//icons
#define A_SCOPE 0
#define A_CROSS 1
#define A_EYE 2
#define A_SHIELD 3
#define A_NONE 4
//bullets
#define B_GREY 0
#define B_BLUE 1
#define B_RED 2
#define B_GREEN 3

#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)
#define LIGHT_BLUE al_map_rgb(93, 188, 210)
#define STORM_BLUE al_map_rgb(44, 64, 99)
#define ORANGE al_map_rgb(220, 126, 26)
#define PANTONE_ORANGE al_map_rgb(255, 88, 0)
#define STEEL al_map_rgb(67, 70, 75)
#define PINK al_map_rgb(255, 0, 243)
#define T0LOW al_map_rgb(16, 109, 9)
#define T0HIGH al_map_rgb(53, 141, 25)
#define T1LOW al_map_rgb(94, 62, 11)
#define T1HIGH al_map_rgb(117, 88, 28)
#define T2LOW al_map_rgb(49, 4, 119)
#define T2HIGH al_map_rgb(61, 47, 141)
#define T3LOW al_map_rgb(99, 89, 198)
#define T3HIGH al_map_rgb(177, 172, 226)
#define T4LOW al_map_rgb(19, 91, 11)
#define T4HIGH al_map_rgb(73, 101, 45)

const int SCREEN_W = 1024;
const int SCREEN_H = 576;
const int NONE = 0; //cover
const int LOW = 2;
const int HIGH = 4;
const int LOW_W = 8; //cover drawing
const int HIGH_W = 6;
const int MAP_W_L = 255; //map dimensions, width, max size
const int MAP_H_L = 255;
const int V_TILES_W = 13; //visible tiles by the camera at any one time
const int V_TILES_H = 9;
const int CAM_SIZE_RIGHT = (V_TILES_W - 1) / 2; //actually 13x9
const int CAM_SIZE_DOWN = (V_TILES_H - 1) / 2;
const int TILE_TYPE_COUNT = 5;
const int TILE_SIZE = 64; //64x64
const int LVLS_TO_WIN = 4; //if you win/lose if you beat enough levels
const int NAME_L = 40;
const char CITIES[LVLS_TO_WIN][NAME_L] = {"OTTAWA", "PARIS", "BEIJING", "NEW YORK"};
//the space marines
const int SQUAD_SIZE = 6;
const int MARINE_DAMAGE[4] = {6, 5, 4, 6};
const int MARINE_AP[4] = {3, 1, 2, 2};
const int MARINE_HEALTH[4] = {6, 8, 8, 10};
const int MARINE_MOVE_DISTANCE[4] = {10, 6, 8, 6};
const char MARINE_ROLE[4][NAME_L] = {"Assault", "Sniper", "Support", "Heavy"};
const int MARINE_DRAW_DISTANCE = 6;
//the aliens
const int SHOOT_THRES = 50; //% chance to hit for the alien to shoot
const int POD_SIZE = 4;
const int AYY_DAMAGE[4] = {4, 6, 3, 5};
const int AYY_AP[4] = {2, 3, 1, 2};
const int AYY_HEALTH[4] = {8, 12, 8, 6};
const int AYY_MOVE_DISTANCE[4] = {6, 5, 4, 8};
const char AYY_ROLE[4][NAME_L] = {"Bean", "Smacky", "Slacker", "Sniper"};



struct Tile{
    int cover[4]; ///east, north, west, south
    int type; //corresponds to bitmap
    int visible; //0 is false 1 is true
};

//also struct for aliens
struct Marine{
    int x, y, currHealth, maxHealth;
    int accuracy; //0-100
    char name[NAME_L];
    bool isAlive, isMarine;
    //AP stands for action points
    int role, facingDirection, drawDistance, currAP, maxAP;
    int bullType, moveDistance, coverMultiplier;
};

struct Map{
    int width, height;
    Tile tiles[MAP_W_L][MAP_H_L]; //some will be empty
};

///prototyping all the functions w/ a description
void initializeAllegro(); //initializes allegro
int checkAllegroSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *mainFont1, ALLEGRO_FONT *mainFont2,
                      ALLEGRO_FONT *smallFont1, ALLEGRO_EVENT_QUEUE *event_queue); //sees if all was created properly
int destroyAll(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *mainFont1, ALLEGRO_FONT *mainFont2,
               ALLEGRO_FONT *smallFont1, ALLEGRO_EVENT_QUEUE *event_queue); //destroys all the allegro stuff
int randInt(int low, int high); //returns a random int b/w low and high, inclusive
bool isMouseIn(ALLEGRO_MOUSE_EVENT &mouse, int x1, int y1, int x2, int y2); //t/f if the mouse is b/w coordinates
int cosf2(int angle); //cosine but returns only 0,1,-1
int sinf2(int angle); //sine but returns only 0,1,-1
void generateMap(int width, int height, int cover[3], int tiles[TILE_TYPE_COUNT], char fileName[]); //writes a map to a text file

void clearMap(Map &m); //sets all data to 0
void clearVisibility(Map &m); //sets all visibility to false
int readMap(Map &m, char fileName[]); //reads map from map file, writes data to map m
int determineFOV(Map &m, Marine scott); //determines what is visible to a marine, writes visibility to map m

int printMap(Map &m, int camx, int camy, ALLEGRO_BITMAP *floorbmps[2][TILE_TYPE_COUNT]); //draws the map to display
void printOps(Marine ops[], int camx, int camy, ALLEGRO_BITMAP *marinebmps[4][4]); //draws your ops to display
void printAyys(Marine ayys[], int alienCount, int camx, int camy, ALLEGRO_BITMAP *alienbmps[][4], Map &m); //draws aliens to display
void printSelectedOpStats(Marine scott, Map &m, ALLEGRO_BITMAP *marinebmps[][4], ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT],
                          ALLEGRO_FONT *font); //prints op's/alien's stats to the bottom right
void printIcons(ALLEGRO_BITMAP *iconbmps[4]); //prints the action icons to the top right
void highlight(int x, int y, ALLEGRO_COLOR colour); //highlights what's selected w/ a box of a colour
void printIconHighlight(int selectedAction); //highlights the selected action
void centerCamera(Marine scott, Map &m, int &camx, int &camy); //centers the camera to a character
int getXHere(ALLEGRO_MOUSE_EVENT mouse, int camy); //gets the x of the tile on the map where the mouse is
int getYHere(ALLEGRO_MOUSE_EVENT mouse, int camy); //same but for y

int loadFloorbmps(ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT]); //loads bmps, same for the below
int destroyFloorbmps(ALLEGRO_BITMAP *floorbmps[][TILE_TYPE_COUNT]); //destroys them, same for the below
int loadMarinebmps(ALLEGRO_BITMAP *marinebmps[][4]);
int destroyMarinebmps(ALLEGRO_BITMAP *marinebmps[][4]);
int loadAlienbmps(ALLEGRO_BITMAP *alienbmps[][4]);
int loadIconbmps(ALLEGRO_BITMAP *iconbmps[4]);
int destroyIconbmps(ALLEGRO_BITMAP *iconbmps[4]);
int loadBulletbmps(ALLEGRO_BITMAP *bulletbmps[4]);
int destroyBulletbmps(ALLEGRO_BITMAP *bulletbmps[4]);

int determineHitChance(Marine &shooter, Marine &target, Map &m); //returns a chance to hit based off cover, distance
bool animateShot(Marine &shooter, Marine &target, int camx, int camy, int &bullX, int &bullY,
                 ALLEGRO_BITMAP *bulletbmps[4]); //draws the bullet to the display
int getDirection(int x1, int y1, int x2, int y2); //gets the direction you would be facing from one point to another
void refreshMarines(Marine ops[], int dudeCount); //refreshes ap, cover at the end of a turn
bool shootMarine(Marine &shooter, Marine &target, Map &m, char buffer[]); //fires a shot at the target, may or may not hit, does some amount of damage
void moveMarine(Marine &mover, int destX, int destY); //moves a character to some destination
bool isTileOcc(int x, int y, Marine ops[], Marine ayys[], int alienCount); //true if there is an alien or an op on the tile

int initSquad(Marine ops[]); //name
bool areMarinesAlive(Marine ops[]); //true if at least one is alive
bool isMarineHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ops[]); //true if there is an alien there
int getMarineIHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ops[]); //gets the index

int initAliens(Marine ayys[], int alienCount, Map &m); //name
bool areAyysAlive(Marine ayys[], int alienCount); //true if at least one is alive
bool isAlienHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ayys[], int alienCount); //same as above
int getAlienIHereMouse(ALLEGRO_MOUSE_EVENT &mouse, int camx, int camy, Marine ayys[], int alienCount); //""
void ayyTurn(Marine ayys[], int alienCount, Map &m, Marine ops[]); //processes to aliens' turn, moving and shooting
