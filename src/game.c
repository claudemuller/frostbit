#include "game.h"
#include "arena.h"
#include "entity.h"
#include "event_bus.h"
#include "state.h"
#include "system.h"
#include "texture.h"
#include "utils/utils.h"
#include <SDL3/SDL_log.h>
#include <assert.h>

#define FPS 60
#define MILLISECS_PER_FRAME 1000 / FPS

static void load_level(void);
static void process_input(void);
static void update(float dt);
static void render(void);

static void on_collision(EventArgs args)
{
    util_info("collision");
}

GameState state = {0};
static int prev_frame_ms = 0;

bool game_init(MemoryArena* game_mem)
{
    // --------------------------------------------------------------------------------------------
    // Allocate managers
    // --------------------------------------------------------------------------------------------

    state.sysmgr = (SystemManager*)arena_alloc_aligned(game_mem, sizeof(SystemManager), 16);
    assert(state.sysmgr && "Failed to allocate system manager.");

    state.entmgr = (EntityManager*)arena_alloc_aligned(game_mem, sizeof(EntityManager), 16);
    assert(state.entmgr && "Failed to allocate entity manager.");

    state.texmgr = (TextureManager*)arena_alloc_aligned(game_mem, sizeof(TextureManager), 16);
    assert(state.texmgr && "Failed to allocate texture manager.");

    state.eventbus = (EventBus*)arena_alloc_aligned(game_mem, sizeof(EventBus), 16);
    assert(state.eventbus && "Failed to allocate event bus.");
    event_bus_init(state.eventbus);

    // --------------------------------------------------------------------------------------------
    // Register systems
    // --------------------------------------------------------------------------------------------

    sysmgr_register(state.sysmgr, SYS_SIG_MOVEMENT, movement_sys_update, NULL);
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

    state.window = SDL_CreateWindow("DEV SDL3", 320, 240, SDL_WINDOW_RESIZABLE);
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
    SDL_SetRenderLogicalPresentation(state.renderer, 320, 240, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    util_info("Renderer: %s", SDL_GetRendererName(state.renderer));

    int w, h;
    if (SDL_GetWindowSize(state.window, &w, &h)) {
        // Move mouse to a location in the window
        SDL_WarpMouseInWindow(state.window, w / 2.0f, h / 2.0f);
    }

    SDL_SetWindowSize(state.window, 320 * 2, 240 * 2);
    const char* platform = SDL_GetPlatform();
    if (strncmp(platform, "Linux", strlen("Linux")) == 0) SDL_SetWindowPosition(state.window, 3200, 300);

    // --------------------------------------------------------------------------------------------
    // Setup game state
    // --------------------------------------------------------------------------------------------

    state.is_running = true;

    return true;
}

bool game_run(void)
{
    load_level();

    while (state.is_running) {
        int time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - prev_frame_ms);
        if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) {
            SDL_Delay(time_to_wait);
        }

        double dt = (SDL_GetTicks() - prev_frame_ms) / 1000.0;
        prev_frame_ms = SDL_GetTicks();

        process_input();
        state.eventbus->process_events(state.eventbus);
        update(dt);
        render();

        // TODO: fix
        SDL_Delay(16);
    }

    return true;
}

void game_destroy(void)
{
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

// ------------------------------------------------------------------------------------------------

static Entity g_player;

static void load_level(void)
{
    Entity player = entity_create(state.entmgr);
    transform_add(state.entmgr, player, (Vector2){64, 64});

    texmgr_add_texture(state.renderer, state.texmgr, "playersheet", "res/walk.png");
    sprite_add(state.entmgr, player, "playersheet", (Vector2){64.0, 64.0f}, (SDL_FRect){0, 0, 64.0, 64.0f}, false);

    rigid_body_add(state.entmgr, player, (Vector2){0});
    box_collider_add(state.entmgr, player, (Vector2){64, 64}, (Vector2){0});
    animation_add(state.entmgr, player, 9, 15, true);
    keyboard_control_add(state.entmgr, player);

    Entity floor = entity_create(state.entmgr);
    transform_add(state.entmgr, floor, (Vector2){10, 200});
    box_collider_add(state.entmgr, floor, (Vector2){100, 10}, (Vector2){0});

    state.eventbus->on_event(state.eventbus, EVT_DEAD, &on_collision);

    g_player = player;
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
            if (SIGNATURE_MATCH(state.entmgr->signatures[g_player], SYS_SIG_KEYBOARD_CONTROL))
                keyboard_control_sys_update(&state, g_player, &ev);
        }

        if (ev.type == SDL_EVENT_MOUSE_MOTION || ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (SIGNATURE_MATCH(state.entmgr->signatures[g_player], SYS_SIG_MOUSE_CONTROL))
                mouse_control_sys_update(&state, g_player, &ev);
        }
    }
}

static void update(float dt)
{
    // TODO: update to map
    for (uint32_t i = 0; i < state.sysmgr->count; ++i) {
        if (state.sysmgr->systems[i].fn == movement_sys_update) state.sysmgr->systems[i].ctx = &dt;
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

    // TODO: split system render and update types so that we don't call them twice?
    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) continue;

        sysmgr_update_entity(&state, e);
    }

    SDL_RenderPresent(state.renderer);
}
