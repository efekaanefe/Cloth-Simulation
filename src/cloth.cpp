#pragma once

#include "cloth.h"
#include "constants.h"
#include "raylib.h"


Cloth::Cloth() {
    float start_x = 0.2 * SCREEN_WIDTH;
    float end_x = 0.8 * SCREEN_WIDTH;
    float start_y = 0.2 * SCREEN_HEIGHT;
    float end_y = 0.8 * SCREEN_HEIGHT;
    float del_x = (end_x - start_x) / NUM_VERTECES_WIDTH;
    float del_y = (end_y - start_y) / NUM_VERTECES_HEIGHT;

    for (int i = 0; i < NUM_VERTECES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTECES_HEIGHT; j++) {

            int body_index = i + NUM_VERTECES_HEIGHT * j;
            bodies[body_index].position = {start_x + i * del_x,
                                           start_y + j * del_y};
            bodies[body_index].mass = BODY_MASS;
            bodies[body_index].is_fixed = true;
        }
    }
}

void Cloth::Draw() {
    for (int i = 0; i < NUM_BODIES ; i++) {
        bodies[i].Draw();
    }
}

void Cloth::Update(float dt) {

}
