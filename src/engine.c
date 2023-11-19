#include "array.h"
#include "engine.h"
#include "entity.h"
#include "event_bus.h"
#include "state.h"
#include "state_manager.h"
#include "systems/keyboard_control_system.h"
#include "systems/render_collider_system.h"
#include "vector.h"
#include <stdio.h>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

static int prev_frame_ms = 0;

bool engine_init(engine_t *engine, struct engine_options *options)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: find out what this category thing is
        SDL_LogCritical(1, "Unable to init SDL: %s", SDL_GetError());
        return false;
    }

    if (options) {
        engine->graphics.width = options->width;
        engine->graphics.height = options->height;
        engine->graphics.window_title = options->title;
    }

    if (!gfx_init(&engine->graphics)) {
        SDL_LogCritical(1, "Unable to init graphics: %s", SDL_GetError());
        return false;
    }

    if (!state_manager_init()) {
        SDL_LogCritical(1, "Unable to init state manager");
        return false;
    }

    asset_store_init();

    if (!event_bus_init()) {
        SDL_LogCritical(1, "Unable to init event bus");
        return false;
    }

    engine->running = true;

    return true;
}

void engine_setup(engine_t *engine)
{
    state_t *level1 = state_new();
    if (!level1) {
        SDL_LogError(1, "Error initialising level 1\n");
        exit(1);
    }

    init_render_collider_system();
    init_keyboard_control_system();

    asset_store_add_texture(engine->graphics.renderer, "tilemap", "./assets/tilemaps/jungle.png");
    asset_store_add_texture(engine->graphics.renderer, "tank", "./assets/tank.png");
    asset_store_add_texture(engine->graphics.renderer, "chopper", "./assets/chopper.png");

    load_tilemap_data("./assets/tilemaps/jungle.map", level1);

    entity_t player = {
        .id = "player",
    };
    add_component_transform(&player, 10, 10, (vec2_t) { 2, 2 }, 0);
    add_component_boxcollider(
        &player,
        32,
        32,
        (vec2_t) { 1, 1 },
        (SDL_Color) { 255, 0, 0, 255 }
    );
    add_component_rigidbody(&player, (vec2_t) { 0 });
    add_component_sprite(
        &player,
        "chopper",
        32,
        32,
        0,
        0,
        1,
        false,
        SDL_FLIP_NONE
    );
    add_component_keyboard_control(&player);
    array_push(level1->entities, player);

    entity_t entity2 = {
        .id = "enemy1",
    };
    add_component_transform(&entity2, 100, 100, (vec2_t) { 2, 2 }, 0);
    add_component_boxcollider(
        &entity2,
        30,
        30,
        (vec2_t) { 1, 1 },
        (SDL_Color) { 255, 0, 0, 255 }
    );
    add_component_rigidbody(&entity2, (vec2_t) { -10, -10 });
    add_component_sprite(
        &entity2,
        "tank",
        32,
        32,
        0,
        0,
        1,
        false,
        SDL_FLIP_NONE
    );
    array_push(level1->entities, entity2);

    state_manager_push(level1);
}

void engine_run(engine_t *engine)
{
    engine_setup(engine);

    while (engine->running) {
        engine_process_input(engine);
        engine_update(engine);
        engine_render(engine);
    }
}

void engine_process_input(engine_t *engine)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            engine->running = false;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE: {
                engine->running = false;
            } break;

            case SDLK_w:
            case SDLK_a:
            case SDLK_s:
            case SDLK_d: {
                args_t args = {
                    .event = e,
                };
                event_bus_emit(EVT_PLAYER_MOVE, args);
            } break;
            }
        }
    }
}

void engine_update(engine_t *engine)
{
    int time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - prev_frame_ms);
    if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) {
        SDL_Delay(time_to_wait);
    }

    double dt = (SDL_GetTicks() - prev_frame_ms) / 1000.0;
    prev_frame_ms = SDL_GetTicks();

    state_manager_update(dt);
    event_bus_process_events();
}

void engine_render(engine_t *engine)
{
    SDL_SetRenderDrawColor(engine->graphics.renderer, 21, 21, 21, 255);
    SDL_RenderClear(engine->graphics.renderer);

    state_manager_render(engine->graphics.renderer);

    SDL_RenderPresent(engine->graphics.renderer);
}

int engine_clean(engine_t *engine)
{
    state_manager_free();
    asset_store_free();
    gfx_free(&engine->graphics);
    SDL_Quit();

    return 0;
}

bool load_tilemap_data(const char *filename, state_t *state)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        SDL_LogError(1, "Error opening tilemap\n");
        return false;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int x = 0, y = 0;
    int tile_width = 32;

    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            char tile_id[50] = { 0 };
            sprintf(tile_id, "tile%d_%d", x, y);

            entity_t tile = {
                .id = tile_id,
            };
            add_component_transform(
                &tile,
                x * tile_width,
                y * tile_width,
                (vec2_t) { 2, 2 },
                0
            );

            int tile_num = atoi(token);
            int tile_map_x = tile_num % 10;
            int tile_map_y = tile_num / 10;
            add_component_sprite(
                &tile,
                "tilemap",
                tile_width,
                tile_width,
                tile_map_x * tile_width,
                tile_map_y * tile_width,
                1,
                false,
                SDL_FLIP_NONE
            );
            array_push(state->entities, tile);

            token = strtok(NULL, ",");
            x++;
        }
        x = 0;
        y++;
    }

    fclose(fp);
    free(line);

    return true;
}
