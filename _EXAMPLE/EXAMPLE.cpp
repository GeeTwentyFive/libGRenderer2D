#include <GWindower.hpp>
#include <glad/glad.h>
#include "../include/GRenderer2D.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb/stb_image.h>
#include <linalg/linalg.h>
using namespace linalg::aliases;
#include <GFramePacer/GFramePacer.hpp>

#include <stdexcept>
#include <string>
#include <math.h>
#include <iostream>


const double TARGET_FRAMETIME_MS = 1.0;  // Should be a power-of-2 (like 1.0, 2.0, 4.0, 8.0) (to align as fraction/multiple of USB input report rate)


#define ERROR(msg) throw std::runtime_error(std::string("[ERROR] ") + __FILE__ + "@" + std::to_string(__LINE__) + " (" + __func__ + "): " + (msg))


int main() { try {
        GWindower gw{0, 0, 4, 6};
        
        if (!gladLoadGL()) ERROR("Failed to load OpenGL");

        GRenderer2D gr2d;

        int texture_width, texture_height, _texture_comp;
        unsigned char* texture = stbi_load("Square.png", &texture_width, &texture_height, &_texture_comp, 4);
        uint64_t sprite_id = gr2d.CreateSprite((uint32_t*)texture, texture_width, texture_height);
        stbi_image_free(texture);

        GRenderer2D::SpriteInstance* sprite_instance = gr2d.AddSprite(sprite_id);
        sprite_instance->size[0] = (float)GWindower::GetScreenHeight()/8; sprite_instance->size[1] = (float)GWindower::GetScreenHeight()/8;
        sprite_instance->position[0] = (float)GWindower::GetScreenWidth()/2 - sprite_instance->size[0]/2; sprite_instance->position[1] = (float)GWindower::GetScreenHeight()/2 - sprite_instance->size[1]/2;

        static const uint32_t BG_TEXTURE[] = {0xFFFFFFFF};
        GRenderer2D::SpriteInstance* bg = gr2d.AddSprite(gr2d.CreateSprite(BG_TEXTURE, 1, 1));
        bg->size[0] = (float)GWindower::GetScreenHeight()/2; bg->size[1] = (float)GWindower::GetScreenHeight()/2;
        bg->position[0] = (float)GWindower::GetScreenWidth()/2 - sprite_instance->size[0]/2; bg->position[1] = (float)GWindower::GetScreenHeight()/2 - sprite_instance->size[1]/2;
        bg->z_depth = -0.99f;

        glClearColor(0.0, 0.0, 0.0, 1.0);
        GFramePacer gfp; gfp.target_frametime_ms = TARGET_FRAMETIME_MS;
        while (gw.Update()) { if(gw.key_states[GWindower::KEY_ESCAPE]) break;
                double delta_time = gfp.Wait();

                glViewport(0, 0, gw.window_width, gw.window_height);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                gr2d.camera_pos[0] += ((float)gw.mouse_x)/GWindower::GetScreenWidth();
                gr2d.camera_pos[1] -= ((float)gw.mouse_y)/GWindower::GetScreenHeight();

                gr2d.camera_zoom += gw.mouse_scroll_delta/((double)GWindower::GetScreenHeight()/45);

                if (gw.key_states[GWindower::KEY_D]) sprite_instance->position[0] += delta_time * GWindower::GetScreenHeight();
                if (gw.key_states[GWindower::KEY_A]) sprite_instance->position[0] -= delta_time * GWindower::GetScreenHeight();
                if (gw.key_states[GWindower::KEY_W]) sprite_instance->position[1] += delta_time * GWindower::GetScreenHeight();
                if (gw.key_states[GWindower::KEY_S]) sprite_instance->position[1] -= delta_time * GWindower::GetScreenHeight();

                gr2d.DrawFrame();
        }

        return 0;
} catch (const std::exception& e) { std::cout << e.what() << std::endl; return 1; } }