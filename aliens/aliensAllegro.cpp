#include "aliens.h"

void initializeAllegro(){
    al_init();

    al_install_audio();
    al_install_keyboard();
    al_install_mouse();

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_native_dialog_addon();
    al_init_image_addon();
    al_init_primitives_addon();
}

int destroyAll(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *mainFont1, ALLEGRO_FONT *mainFont2, ALLEGRO_FONT *smallFont1, ALLEGRO_EVENT_QUEUE *event_queue){
    al_destroy_display(display);
    al_destroy_font(mainFont1);
    al_destroy_font(mainFont2);
    al_destroy_font(smallFont1);
    al_destroy_event_queue(event_queue);
    return 0;
}

int checkAllegroSetup(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *mainFont1, ALLEGRO_FONT *mainFont2, ALLEGRO_FONT *smallFont1, ALLEGRO_EVENT_QUEUE *event_queue){
    //0: works, -1: stop
    if(!display){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!mainFont1){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize mainfont1",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!mainFont2){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize mainfont2",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!smallFont1){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize smallFont1",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!event_queue){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize event queue",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!al_is_mouse_installed()){
        al_show_native_message_box(display, "Error", "Error", "Failed to install mouse",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!al_is_keyboard_installed()){
        al_show_native_message_box(display, "Error", "Error", "Failed to install keyboard",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    if(!al_is_audio_installed()){
        al_show_native_message_box(display, "Error", "Error", "Failed to install audio",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        destroyAll(display, mainFont1, mainFont2, smallFont1, event_queue);
        return -1;
    }

    return 0;
}
