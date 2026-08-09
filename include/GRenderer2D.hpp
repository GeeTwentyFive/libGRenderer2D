#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <stddef.h>
#include <stdint.h>

class GRenderer2D { public: ~GRenderer2D(); struct SpriteInstance; private: struct _impl; std::unique_ptr<_impl> _; std::unordered_map<uint64_t, std::vector<std::unique_ptr<GRenderer2D::SpriteInstance>>> sprite_instances; public:
        explicit GRenderer2D();

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
                void Remove() { for (auto it = this->_renderer_instance->sprite_instances[sprite_id].begin(); it != this->_renderer_instance->sprite_instances[sprite_id].end(); it++) { if (it->get() == this) {this->_renderer_instance->sprite_instances[sprite_id].erase(it); return;} }; }
        };
        SpriteInstance* AddSprite(const uint64_t sprite_id) noexcept { auto sprite_instance = std::make_unique<SpriteInstance>(); sprite_instance->_renderer_instance = this; sprite_instance->sprite_id = sprite_id; SpriteInstance* ptr = sprite_instance.get(); sprite_instances[sprite_id].push_back(std::move(sprite_instance)); return ptr; }

        int DrawFrame() noexcept;  // returns non-0 on error
};