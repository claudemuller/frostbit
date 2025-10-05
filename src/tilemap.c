#include "tilemap.h"
#include "arena.h"
#include "texture.h"
#include "utils/utils.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tmx.h>

static void parse_all_layers(MemoryArena* game_mem, GameState* state, tmx_layer* layer);
static void parse_tile_layer(GameState* state, tmx_layer* layer);
static void parse_object_layer(GameState* state, const char* layer_type, tmx_object_group* objgr);
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
            case L_LAYER: {
                if (!state->terrain_tiles) {
                    size_t n_tiles = state->level->width * state->level->width;
                    state->terrain_tiles = arena_alloc_aligned(game_mem, sizeof(Tile) * n_tiles, 16);
                    if (!state->terrain_tiles) {
                        util_fatal("Ekk! couldn't thing the thing");
                    }
                    state->n_terrain_tiles = n_tiles;
                }

                if (!state->terrain_collisions) {
                    state->terrain_collisions =
                        arena_alloc_aligned(game_mem, sizeof(Tile) * MAX_TERRAIN_COLLISIONS, 16);
                    if (!state->terrain_collisions) {
                        util_fatal("Ekk! couldn't thing the thing");
                    }
                }

                parse_tile_layer(state, layer);
            } break;

            case L_GROUP: {
                parse_all_layers(game_mem, state, layer->content.group_head);
            } break;

            case L_OBJGR: {
                parse_object_layer(state, layer->class_type, layer->content.objgr);
            } break;

            case L_IMAGE: {
                // parse_objects(state, map, layer, layer->content.objgr);
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

static void parse_tile_layer(GameState* state, tmx_layer* layer)
{
    tmx_map* map = state->level;
    // f32 op = layer->opacity;

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

                size_t idx = (i * map->width) + j;

                state->terrain_tiles[idx] = (Tile){
                    .pos =
                        {
                            .x = j * ts->tile_width,
                            .y = i * ts->tile_height,
                        },
                    .size = {.w = ts->tile_width, .h = ts->tile_height},
                    .src = {.x = map->tiles[gid]->ul_x, .y = map->tiles[gid]->ul_y},
                    .texture = tex,
                };

                if (tile->collision) {
                    switch (tile->collision->obj_type) {
                    case OT_SQUARE: {
                        // state->terrain_collisions[idx] = (SDL_FRect){
                        //     .x = tile->collision->x,
                        //     .y = tile->collision->y,
                        //     .w = tile->collision->width,
                        //     .h = tile->collision->height,
                        // };
                    } break;

                    case OT_POLYGON: {
                        util_info("implement: OT_POLYLINE tile collision parsing");
                    } break;

                    case OT_POLYLINE: {
                        util_info("implement: OT_POLYLINE tile collision parsing");
                    } break;

                    case OT_ELLIPSE: {
                        util_info("implement: OT_ELLIPSE tile collision parsing");
                    } break;

                    default: {
                        util_info("Object type unsupported");
                    } break;
                    }
                }
            }
        }
    }
}

static void parse_object_layer(GameState* state, const char* layer_type, tmx_object_group* objgr)
{
    tmx_object* obj = objgr->head;
    tmx_map* map = state->level;

    while (obj) {
        if (obj->visible) {
            if (layer_type && strncmp(layer_type, "collision", strlen("collision")) == 0) {
                switch (obj->obj_type) {
                case OT_SQUARE: {
                    state->terrain_collisions[state->n_terrain_collisions++] = (SDL_FRect){
                        .x = obj->x,
                        .y = obj->y,
                        .w = obj->width,
                        .h = obj->height,
                    };
                } break;

                default:
                    util_info("Collision object type unsupported");
                    break;
                }

                obj = obj->next;
                continue;
            }

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
                    camera_follow_add(state->entmgr, ent);
                    keyboard_control_add(state->entmgr, ent);
                    state->player = ent;
                }
                // }

            } break;

            case OT_SQUARE: {
                util_info("implement: OT_SQUARE parsing");
            } break;

            case OT_POLYGON: {
                util_info("implement: OT_POLYGON parsing");
            } break;

            case OT_POLYLINE: {
                util_info("implement: OT_POLYLINE parsing");
            } break;

            case OT_ELLIPSE: {
                util_info("implement: OT_ELLIPSE parsing");
            } break;

            default: {
                util_info("Object type unsupported");
            } break;
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

    Vector2 ent_pos = {.x = obj->x, .y = obj->y - obj->height};
    transform_add(entmgr, ent, ent_pos);

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

    if (obj->name && strncmp(obj->name, "player", strlen("player")) == 0) {
        rigid_body_add(entmgr, ent, (Vector2){0}, ent_pos);
    }

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

    tileinfo_add(entmgr, ent);

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
