#include "game.h"
#include "arena.h"
#include "entity.h"
#include "state.h"
#include "system.h"
#include "texture.h"
#include <SDL3/SDL_log.h>
#include <assert.h>

static void load_level(MemoryArena* game_mem);
static void tick(void);
static void process_input(void);
static void update(void);
static void render(void);

GameState state = {0};

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

    // --------------------------------------------------------------------------------------------
    // Register systems
    // --------------------------------------------------------------------------------------------

    sysmgr_register(state.sysmgr, (1U << COMP_TRANSFORM) | (1U << COMP_RIGID_BODY), movement_sys_update, NULL);
    sysmgr_register(state.sysmgr, (1U << COMP_TRANSFORM) | (1U << COMP_SPRITE), render_sys_render, NULL);
    sysmgr_register(state.sysmgr, (1U << COMP_TRANSFORM) | (1U << COMP_BOX_COLLIDER), render_collider_sys_render, NULL);
    sysmgr_register(state.sysmgr, (1U << COMP_KEYBOARD_CONTROL), keyboard_control_sys_update, NULL);

    // --------------------------------------------------------------------------------------------
    // Bootstrap SDL
    // --------------------------------------------------------------------------------------------

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error init'ing SDL: %s", SDL_GetError());
        return false;
    }

    state.window = SDL_CreateWindow("DEV SDL3", 320, 240, SDL_WINDOW_RESIZABLE);
    if (!state.window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating SDL window: %s", SDL_GetError());
        return false;
    }

    SDL_Log("Available renderer drivers:");
    for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
        SDL_Log("%d - %s", i + 1, SDL_GetRenderDriver(i));
    }

    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (!state.renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error creating SDL renderer: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderLogicalPresentation(state.renderer, 320, 240, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_Log("Renderer: %s", SDL_GetRendererName(state.renderer));

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

bool game_run(MemoryArena* game_mem)
{
    load_level(game_mem);

    while (state.is_running) {
        tick();

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

static void load_level(MemoryArena* game_mem)
{
    Entity player = entity_create(state.entmgr);
    transform_add(state.entmgr, player, (Vector2){64, 64});

    texmgr_add_texture(state.renderer, state.texmgr, "playersheet", "res/walk.png");
    sprite_add(state.entmgr, player, "playersheet", (Vector2){64.0, 64.0f}, (SDL_FRect){0, 0, 64.0, 64.0f}, false);

    rigid_body_add(state.entmgr, player, (Vector2){0});
    box_collider_add(state.entmgr, player, (Vector2){64, 64}, (Vector2){0});

    g_player = player;
}

static void tick(void)
{
    process_input();
    update();
    render();
}

static void process_input(void)
{
    SDL_Event event;
    const bool* keystate = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            state.is_running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            keyboard_control_sys_update(&state, g_player, &event);
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            mouse_control_sys_update(&state, g_player, &event);
        }
    }
}

static void update(void)
{
    float dt = 1.0f;

    // TODO: update to map
    for (uint32_t i = 0; i < state.sysmgr->count; ++i) {
        // Update systems with delta time
        if (state.sysmgr->systems[i].fn == movement_sys_update) state.sysmgr->systems[i].ctx = &dt;
    }

    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) continue;

        sysmgr_update_entity(&state, e);
    }
}

static void render(void)
{
    SDL_SetRenderDrawColor(state.renderer, 0x00, 0xff, 0xaa, 0xff);
    SDL_RenderClear(state.renderer);

    for (Entity e = 0; e < state.entmgr->next_entity_id; ++e) {
        if (!state.entmgr->live_entities[e]) continue;

        sysmgr_update_entity(&state, e);
    }

    SDL_RenderPresent(state.renderer);
}
