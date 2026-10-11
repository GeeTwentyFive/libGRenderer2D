#pragma once

#include <memory>
#include <stddef.h>
#include <stdint.h>

class GRenderer2D { public: ~GRenderer2D(); explicit GRenderer2D(); private: struct _impl; std::unique_ptr<_impl> _; public:
        float camera_pos[2] = {0, 0};  // in Normalized Device Coordinates (-1 - 1)
        float camera_zoom = 1.0;

        uint64_t CreateSprite( // returns sprite ID
                const uint32_t* texture_RGBA, const uint16_t texture_width, const uint16_t texture_height
        );
        struct SpriteInstance { void* user_data = nullptr; uint64_t sprite_id; GRenderer2D* _renderer_instance = nullptr;
                float size[2] = {1.0f, 1.0f};  // in pixels
                float z_depth = -0.5f;
                float position[2] = {0, 0};  // in pixels
                uint32_t color_RGBA = 0xFFFFFFFF;  // Multiplied with sampled texture color
                void Remove() noexcept;
        };
        SpriteInstance* AddSprite(const uint64_t sprite_id) noexcept;

        int DrawFrame(int framebuffer_width, int framebuffer_height) noexcept;  // returns non-0 on error
};