#include "game.h"
#include "arena.h"
#include "entity.h"
#include "state.h"
#include "system.h"
#include <assert.h>

static GameState state;
static EntityManager *ent_man;
static System system_render = {
    .component_mask = (1U << COMP_TRANSFORM) | (1U << COMP_SPRITE),
    .fn = render_sys_update,
    .ctx = NULL,
};
static System system_movement = {
    .component_mask = (1U << COMP_TRANSFORM),
    .fn = movement_sys_update,
    .ctx = NULL,
};

static void load_level(MemoryArena *game_mem);
static void tick(void);
static void process_input(void);
static void update(void);
static void render(void);

bool game_init(MemoryArena *game_mem)
{
    ent_man = entity_manager_new(game_mem);

    // state.tex_man = (TextureManager*)arena_alloc_aligned(game_mem, sizeof(TextureManager), 16);
    // assert(state.tex_man && "Failed to allocate texture manager.");
    //
    // state.sys_man = (SystemManager*)arena_alloc_aligned(game_mem, sizeof(SystemManager), 16);
    // assert(state.sys_man && "Failed to allocate system manager.");

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
    const char *platform = SDL_GetPlatform();
    if (strncmp(platform, "Linux", strlen("Linux")) == 0) SDL_SetWindowPosition(state.window, 3200, 300);

    state.is_running = true;

    return true;
}

bool game_run(MemoryArena *game_mem)
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

// ---------------------------------------------------------------------------------------

static void load_level(MemoryArena *game_mem)
{
    Entity player = entity_create(ent_man);
    transform_add(ent_man,
                  player,
                  (TransformComponent){
                      .pos =
                          {
                              .x = 10,
                              .y = 10,
                          },
                  });
    sprite_add(ent_man,
               player,
               (SpriteComponent){
                   .size =
                       {
                           .x = 20,
                           .y = 20,
                       },
               });
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
    const bool *keystate = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT: {
            state.is_running = false;
        } break;

        case SDL_EVENT_KEY_DOWN: {
            if (event.key.key == SDLK_Q || event.key.key == SDLK_ESCAPE) {
                state.is_running = false;
            }

            if (event.key.key == SDLK_F11) {
                SDL_SetWindowFullscreen(state.window, state.is_fullscreen);
                state.is_fullscreen = !state.is_fullscreen;
            }
        } break;

        case SDL_EVENT_KEY_UP: {
        } break;

        case SDL_EVENT_MOUSE_MOTION: {
            // SDL_Log("x:%f y:%f", event.motion.x, event.motion.y);
            // SDL_Log("xrel:%f yrel:%f", event.motion.xrel, event.motion.yrel);
        } break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_Log("Left mouse button clicked [%d]", event.button.clicks);
            }
        } break;

        default: {
        } break;
        }
    }
}

static void update(void)
{
    float dt = 1.0f;

    system_movement.ctx = &dt;

    SDL_Log("next: %d", ent_man->next_entity_id);

    for (Entity e = 0; e < ent_man->next_entity_id; ++e) {
        if (!ent_man->live_entities[e]) continue;

        if (SIGNATURE_MATCH(ent_man->signatures[e], system_movement.component_mask))
            system_movement.fn(ent_man, NULL, e, system_movement.ctx);
    }
}

static void render(void)
{
    SDL_SetRenderDrawColor(state.renderer, 0x00, 0xff, 0xaa, 0xff);
    SDL_RenderClear(state.renderer);

    system_render.ctx = NULL;

    for (Entity e = 0; e < ent_man->next_entity_id; ++e) {
        if (!ent_man->live_entities[e]) continue;

        if (SIGNATURE_MATCH(ent_man->signatures[e], system_render.component_mask))
            system_render.fn(ent_man, state.renderer, e, system_render.ctx);
    }

    SDL_RenderPresent(state.renderer);
}
