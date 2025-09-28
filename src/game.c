#include "game.h"
#include "arena.h"
#include "entity.h"
#include "eventbus.h"
#include "state.h"
#include "system.h"
#include "texture.h"
#include "tilemap.h"
#include "utils/utils.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <assert.h>
#include <tmx.h>

#define FPS 60
#define MILLISECS_PER_FRAME 1000 / FPS
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void* SDL_tex_loader(const char* path);

static void load_level(MemoryArena* game_mem);
static void process_input(void);
static void update(float* dt);
static void render(void);

static void on_collision(EventArgs args)
{
    util_info("collision %d", args.i);
}

GameState state = {0};
static u32 prev_frame_ms = 0;

bool game_init(MemoryArena* game_mem)
{
    // --------------------------------------------------------------------------------------------
    // Allocate managers
    // --------------------------------------------------------------------------------------------
    state.sysmgr = sysmgr_init(game_mem);
    state.entmgr = entitymgr_init(game_mem);

    state.texmgr = (TextureManager*)arena_alloc_aligned(game_mem, sizeof(TextureManager), 16);
    assert(state.texmgr && "Failed to allocate texture manager.");

    state.eventbus = (EventBus*)arena_alloc_aligned(game_mem, sizeof(EventBus), 16);
    assert(state.eventbus && "Failed to allocate event bus.");
    eventbus_init(state.eventbus);

    // --------------------------------------------------------------------------------------------
    // Register systems
    // --------------------------------------------------------------------------------------------

    sysmgr_register(state.sysmgr, SYS_SIG_MOVEMENT, movement_sys_update, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_CAMERA_MOVEMENT, camera_movement_sys_update, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_RENDER, render_sys_render, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_RENDER_COLLIDER, render_collider_sys_render, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_KEYBOARD_CONTROL, keyboard_control_sys_update, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_MOUSE_CONTROL, mouse_control_sys_update, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_ANIMATION, animation_sys_render, NULL);
    sysmgr_register(state.sysmgr, SYS_SIG_COLLISION, collision_sys_update, NULL);

    // --------------------------------------------------------------------------------------------
    // Bootstrap SDL
    // --------------------------------------------------------------------------------------------

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        util_error("Error init'ing SDL: %s", SDL_GetError());
        return false;
    }

    state.window = SDL_CreateWindow("DEV SDL3", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!state.window) {
        util_error("Error creating SDL window: %s", SDL_GetError());
        return false;
    }

    util_info("Available renderer drivers:");
    for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
        util_info("%d - %s", i + 1, SDL_GetRenderDriver(i));
    }

    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (!state.renderer) {
        util_error("Error creating SDL renderer: %s", SDL_GetError());
        return false;
    }

    // Init components that need the renderer
    texmgr_init(game_mem, state.renderer, state.texmgr);
    tilemap_init(state.renderer);

    util_info("Renderer: %s", SDL_GetRendererName(state.renderer));

    i32 w, h;
    if (SDL_GetWindowSize(state.window, &w, &h)) {
        // Move mouse to a location in the window
        SDL_WarpMouseInWindow(state.window, w / 2.0f, h / 2.0f);
    }

    // --------------------------------------------------------------------------------------------
    // Setup tile mapper
    // --------------------------------------------------------------------------------------------
    tmx_img_load_func = texmgr_load_texture;
    tmx_img_free_func = (void (*)(void*))SDL_DestroyTexture;

    state.level = tmx_load("res/level1.tmx");
    if (!state.level) {
        util_error("Failed to load level1");
        return false;
    }

    state.scale = 2.0f;
    // u32 render_w = state.level->width * state.level->tile_width * state.scale / 3.0f;
    // u32 render_h = state.level->height * state.level->tile_height * state.scale / 3.0f;
    SDL_SetRenderLogicalPresentation(
        state.renderer, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // SDL_SetWindowPosition(state.window, state.win_width, 300);
    // const char* platform = SDL_GetPlatform();
    // if (strncmp(platform, "Linux", strlen("Linux")) == 0) SDL_SetWindowPosition(state.window, 3200, 300);

    // --------------------------------------------------------------------------------------------
    // Setup game state
    // --------------------------------------------------------------------------------------------

    state.is_running = true;

    return true;
}

bool game_run(MemoryArena* game_mem)
{
    load_level(game_mem);

    state.camera.x = 0;
    state.camera.y = 0;
    state.camera.w = WINDOW_WIDTH;
    state.camera.h = WINDOW_HEIGHT;

    MemoryArena frame_mem;

    while (state.is_running) {
        arena_init(&frame_mem, sizeof(SystemCtx));
        {
            u32 time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - prev_frame_ms);
            if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) {
                SDL_Delay(time_to_wait);
            }

            float* dt = arena_alloc_aligned(&frame_mem, sizeof(float), 16);
            *dt = (SDL_GetTicks() - prev_frame_ms) / 1000.0;
            prev_frame_ms = SDL_GetTicks();

            process_input();
            state.eventbus->process_events(state.eventbus);
            update(dt);
            render();
        }
        arena_free(&frame_mem);
    }

    return true;
}

void game_destroy(MemoryArena* game_mem)
{
    texmgr_destroy();
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    arena_free(game_mem);
    SDL_Quit();
}

// ------------------------------------------------------------------------------------------------

static void load_level(MemoryArena* game_mem)
{
    tilemap_load_level(game_mem, &state);

    state.eventbus->on_event(state.eventbus, EVT_DEAD, &on_collision);
}

static void process_input(void)
{
    SDL_Event ev;
    // const bool* keystate = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) {
            state.is_running = false;
        }

        // Process system wide key events
        if (ev.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_Q || ev.key.key == SDLK_ESCAPE) {
                state.is_running = false;
            }

            if (ev.key.key == SDLK_F11) {
                SDL_SetWindowFullscreen(state.window, state.is_fullscreen);
                state.is_fullscreen = !state.is_fullscreen;
            }
        }

        // Process player key events
        if (ev.type == SDL_EVENT_KEY_DOWN || ev.type == SDL_EVENT_KEY_UP) {
            if (SIGNATURE_MATCH(state.entmgr->signatures[state.player], SYS_SIG_KEYBOARD_CONTROL))
                keyboard_control_sys_update(&state, state.player, &ev);
        }

        if (ev.type == SDL_EVENT_MOUSE_MOTION || ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (SIGNATURE_MATCH(state.entmgr->signatures[state.player], SYS_SIG_MOUSE_CONTROL))
                mouse_control_sys_update(&state, state.player, &ev);
        }
    }
}

static void update(float* dt)
{
    // TODO: update to map
    for (size_t i = 0; i < state.sysmgr->count; ++i) {
        if (state.sysmgr->systems[i].fn == movement_sys_update) state.sysmgr->systems[i].ctx = dt;
        if (state.sysmgr->systems[i].fn == camera_movement_sys_update) state.sysmgr->systems[i].ctx = &state.camera;
    }

    // TODO: split system render and update types so that we don't call them twice?
    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) continue;

        sysmgr_update_entity(&state, e);
    }
}

static void render(void)
{
    SDL_SetRenderDrawColor(state.renderer, 0x00, 0xff, 0xaa, 0xff);
    SDL_RenderClear(state.renderer);

    for (size_t i = 0; i < state.n_terrain_tiles; ++i) {
        Tile tile = state.terrain_tiles[i];
        SDL_RenderTexture(state.renderer,
                          tile.texture,
                          &(SDL_FRect){
                              .x = tile.src.x,
                              .y = tile.src.y,
                              .w = tile.size.w,
                              .h = tile.size.h,
                          },
                          &(SDL_FRect){
                              .x = tile.pos.x,
                              .y = tile.pos.y,
                              .w = tile.size.w,
                              .h = tile.size.h,
                          });
    }

    // TODO: split system render and update types so that we don't call them twice?
    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) continue;

        sysmgr_update_entity(&state, e);
    }

    SDL_RenderPresent(state.renderer);
}
