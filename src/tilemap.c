#include "tilemap.h"
#include "arena.h"
#include "texture.h"
#include "utils/utils.h"
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <string.h>

static void parse_all_layers(MemoryArena* game_mem, GameState* state, tmx_layer* layer);
static void parse_layer(MemoryArena* game_mem, GameState* state, tmx_layer* layer);
static void parse_objects(MemoryArena* game_mem, GameState* state, tmx_object_group* objgr);
static SDL_Texture* get_tileset_texture(TextureManager* texmgr, tmx_tile* tile);
static Entity parse_entity(EntityManager* entmgr, TextureManager* texmgr, tmx_object* obj, tmx_tile* tile);

static SDL_Renderer* renderer;

void tilemap_init(SDL_Renderer* r)
{
    renderer = r;
}

void tilemap_load_level(MemoryArena* game_mem, GameState* state)
{
    parse_all_layers(game_mem, state, state->level->ly_head);
}

// Uint32 timer_func(Uint32 interval, void* param)
// {
// SDL_Event event;
// SDL_UserEvent userevent;
//
// userevent.type = SDL_USEREVENT;
// userevent.code = 0;
// userevent.data1 = NULL;
// userevent.data2 = NULL;
//
// event.type = SDL_USEREVENT;
// event.user = userevent;
//
// SDL_PushEvent(&event);
//     return (interval);
// }

// ------------------------------------------------------------------------------------------------

static void parse_all_layers(MemoryArena* game_mem, GameState* state, tmx_layer* layer)
{
    while (layer) {
        if (layer->visible) {
            switch (layer->type) {
            case L_GROUP: {
                parse_all_layers(game_mem, state, layer->content.group_head);
            } break;

            case L_OBJGR: {
                parse_objects(game_mem, state, layer->content.objgr);
            } break;

            case L_IMAGE: {
                // parse_objects(state, map, layer, layer->content.objgr);
            } break;

            case L_LAYER: {
                parse_layer(game_mem, state, layer);
            } break;

            case L_NONE: {
            } break;

            default: {
                util_info("Unsupported layer");
            } break;
            }
        }

        layer = layer->next;
    }
}

static void parse_layer(MemoryArena* game_mem, GameState* state, tmx_layer* layer)
{
    tmx_map* map = state->level;
    // f32 op = layer->opacity;

    state->terrain_tiles = arena_alloc_aligned(game_mem, sizeof(Tile) * map->width * map->height, 16);
    if (!state->terrain_tiles) {
        util_fatal("Ekk! couldn't thing the thing");
    }
    state->n_terrain_tiles = map->width * map->height;

    for (size_t i = 0; i < map->height; i++) {
        for (size_t j = 0; j < map->width; j++) {
            u32 gid = (layer->content.gids[(i * map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
            tmx_tile* tile = map->tiles[gid];

            if (tile != NULL) {
                tmx_tileset* ts = tile->tileset;
                u32 flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;

                SDL_Texture* tex = get_tileset_texture(state->texmgr, tile);
                if (!tex) {
                    return; // -1;
                }

                state->terrain_tiles[(i * map->width) + j] = (Tile){
                    .pos =
                        {
                            .x = j * ts->tile_width,
                            .y = i * ts->tile_height,
                        },
                    .size = {.w = ts->tile_width, .h = ts->tile_height},
                    .src = {.x = map->tiles[gid]->ul_x, .y = map->tiles[gid]->ul_y},
                    .texture = tex,
                };
            }
        }
    }
}

static void parse_objects(MemoryArena* game_mem, GameState* state, tmx_object_group* objgr)
{
    tmx_object* obj = objgr->head;
    tmx_map* map = state->level;

    while (obj) {
        if (obj->visible) {
            switch (obj->obj_type) {
            case OT_TILE: {
                tmx_tile* tile = map->tiles[obj->content.gid];

                // if (obj->type && strncmp(obj->type, "entity", strlen("entity")) == 0) {
                Entity ent = parse_entity(state->entmgr, state->texmgr, obj, tile);
                if (ent == 0) {
                    util_err("Parsing entity failed");
                    break;
                }

                if (obj->name && strncmp(obj->name, "player", strlen("player")) == 0) {
                    state->player = ent;
                }
                // }

            } break;

                // case OT_SQUARE: {
                //     rect.x = head->x;
                //     rect.y = head->y;
                //     rect.w = head->width;
                //     rect.h = head->height;
                //     SDL_RenderRect(renderer, &rect);
                // } break;
                //
                // case OT_POLYGON: {
                //     draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
                // } break;
                //
                // case OT_POLYLINE: {
                //     draw_polyline(head->content.shape->points, head->x, head->y,
                //     head->content.shape->points_len);
                // } break;
                //
                // case OT_ELLIPSE: {
                //     /* FIXME: no function in SDL2 */
                // } break;

            default:
                break;
            }
        }

        obj = obj->next;
    }
}

static SDL_Texture* get_tileset_texture(TextureManager* texmgr, tmx_tile* tile)
{
    tmx_tileset* tileset = tile->tileset;
    if (!tileset) {
        util_error("no tileset");
        return NULL;
    }
    const char* tilesetimg = tileset->image->source;

    u8 slen = 5 + strlen(tilesetimg);
    char texid[255];
    u8 rlen = snprintf(texid, slen, "res/%s", tilesetimg);
    if (rlen < 0) {
        util_error("Failed concatenating strings");
        return NULL;
    }

    SDL_Texture* tex = texmgr_get_texture(texmgr, texid);
    if (!tex) {
        util_error("Texture not found: %s", texid);
    }

    return tex;
}

static Entity parse_entity(EntityManager* entmgr, TextureManager* texmgr, tmx_object* obj, tmx_tile* tile)
{
    Entity ent = entity_create(entmgr);

    transform_add(entmgr, ent, (Vector2){.x = obj->x, .y = obj->y - obj->height});

    SDL_Texture* tex = get_tileset_texture(texmgr, tile);
    if (!tex) {
        return -1;
    }

    // TODO: flip
    u32 flags = obj->id & ~TMX_FLIP_BITS_REMOVAL;

    tmx_tileset* tileset = tile->tileset;

    sprite_add(entmgr,
               ent,
               tex,
               (Vector2){.h = obj->height, .w = obj->width},
               (SDL_FRect){.x = tile->ul_x, .y = tile->ul_y, .w = tileset->tile_width, .h = tileset->tile_height},
               false);

    rigid_body_add(entmgr, ent, (Vector2){0});

    if (tile->collision) {
        box_collider_add(entmgr,
                         ent,
                         (Vector2){.w = tile->collision->width, .h = tile->collision->height},
                         (Vector2){
                             .x = tile->collision->x,
                             .y = tile->collision->y,
                         });
    }

    if (tile->animation) {
        animation_add(entmgr, ent, tile->animation->tile_id, tile->animation_len, tile->animation[0].duration, false);
    }

    keyboard_control_add(entmgr, ent);

    return ent;
}

// static void draw_image_layer(tmx_image* image)
// {
//     SDL_FRect dim = {0};
//     SDL_Texture* texture = (SDL_Texture*)image->resource_image;
//
//     SDL_GetTextureSize(texture, &dim.w, &dim.h);
//     SDL_RenderTexture(renderer, texture, NULL, &dim);
// }
