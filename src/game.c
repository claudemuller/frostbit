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
static void process_input(MemoryArena* frame_mem);
static void update(MemoryArena* frame_mem, const float dt);
static void render(MemoryArena* frame_mem);

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

    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_MOVEMENT, sys_update_movement, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_CAMERA_MOVEMENT, sys_update_camera_movement, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_KEYBOARD_CONTROL, sys_update_keyboard_control, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_MOUSE_CONTROL, sys_update_mouse_control, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_COLLISION, sys_update_collision, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_DEBUG, sys_update_debug, NULL);
    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_PHYSICS, sys_update_physics, NULL);

    sysmgr_register_render_sys(state.sysmgr, SYS_SIG_RENDER, sys_render_entities, NULL);
    sysmgr_register_render_sys(state.sysmgr, SYS_SIG_TILEMAP_RENDER, sys_render_tilemap, NULL);
    sysmgr_register_render_sys(state.sysmgr, SYS_SIG_TILEMAP_RENDER, sys_render_tilemap_collider, NULL);
    sysmgr_register_render_sys(state.sysmgr, SYS_SIG_RENDER_COLLIDER, sys_render_render_collider, NULL);
    sysmgr_register_render_sys(state.sysmgr, SYS_SIG_ANIMATION, sys_render_animation, NULL);

    sysmgr_register_update_sys(state.sysmgr, SYS_SIG_APPLY, sys_update_apply, NULL);

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
        state.renderer, WINDOW_WIDTH / state.scale, WINDOW_HEIGHT / state.scale, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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
        arena_init(&frame_mem, sizeof(SystemCtx) * 20);
        {
            u32 time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - prev_frame_ms);
            if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) {
                SDL_Delay(time_to_wait);
            }

            f64 dt = (SDL_GetTicks() - prev_frame_ms) / 1000.0;
            prev_frame_ms = SDL_GetTicks();

            process_input(&frame_mem);
            state.eventbus->process_events(state.eventbus);
            update(&frame_mem, dt);
            render(&frame_mem);
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

static void on_collision(Event ev)
{
    util_info("collision with %d", ev.entity_id);
    // state.entmgr->live_entities[ev.entity_id] = false;
}

static void load_level(MemoryArena* game_mem)
{
    tilemap_load_level(game_mem, &state);

    state.eventbus->on_event(state.eventbus, EVT_DEAD, &on_collision);
}

static void process_input(MemoryArena* frame_mem)
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

            if (ev.key.key == SDLK_F1) {
                state.debug = !state.debug;
            }
        }

        // Process player key events
        if (ev.type == SDL_EVENT_KEY_DOWN || ev.type == SDL_EVENT_KEY_UP) {
            if (SIGNATURE_MATCH(state.entmgr->signatures[state.player], SYS_SIG_KEYBOARD_CONTROL)) {
                SystemCtx* ctx = (SystemCtx*)arena_alloc_aligned(frame_mem, sizeof(SystemCtx), 16);
                if (!ctx) {
                    exit(1);
                }
                ctx->tag = CTX_EVENT;
                ctx->event = (EventCtx){.ev = ev};
                sys_update_keyboard_control(&state, state.player, ctx);
            }
        }

        if (ev.type == SDL_EVENT_MOUSE_MOTION || ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (SIGNATURE_MATCH(state.entmgr->signatures[state.player], SYS_SIG_MOUSE_CONTROL)) {
                SystemCtx* ctx = (SystemCtx*)arena_alloc_aligned(frame_mem, sizeof(SystemCtx), 16);
                if (!ctx) {
                    exit(1);
                }
                ctx->tag = CTX_EVENT;
                ctx->event = (EventCtx){.ev = ev};
                sys_update_mouse_control(&state, state.player, ctx);
            }
        }
    }
}

static void update(MemoryArena* frame_mem, const float dt)
{
    // TODO: update to map
    for (size_t i = 0; i < state.sysmgr->n_update_systems; ++i) {
        SystemCtx* ctx = (SystemCtx*)arena_alloc_aligned(frame_mem, sizeof(SystemCtx), 16);
        if (!ctx) {
            exit(1);
        }
        ctx->tag = CTX_MOVEMENT;
        ctx->movement = (MovementCtx){.dt = dt};
        if (state.sysmgr->update_systems[i].fn == sys_update_movement) state.sysmgr->update_systems[i].ctx = ctx;
    }

    // TODO: split system render and update types so that we don't call them twice?
    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) {
            continue;
        }

        sysmgr_update_entity(&state, e);
    }
}

static void render(MemoryArena* frame_mem)
{
    SDL_SetRenderDrawColor(state.renderer, 0x00, 0xff, 0xaa, 0xff);
    SDL_RenderClear(state.renderer);

    // Draw tilemap
    // SystemCtx* ctx = (SystemCtx*)arena_alloc_aligned(frame_mem, sizeof(SystemCtx), 16);
    // if (!ctx) {
    //     exit(1);
    // }
    // ctx->tag = CTX_MOVEMENT;
    // ctx->movement = (MovementCtx){.dt = dt};

    sys_render_tilemap(&state, state.player, NULL);

    if (state.debug) {
        sys_render_tilemap_collider(&state, state.player, NULL);
    }

    // TODO: split system render and update types so that we don't call them twice?
    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) {
            continue;
        }

        sysmgr_render_entity(&state, e);
    }

    SDL_RenderPresent(state.renderer);
}
