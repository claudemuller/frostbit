#include "tilemap.h"
#include "arena.h"
#include "texture.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>

// static void set_color(i32 color);
// static void draw_polyline(f64** points, f64 x, f64 y, i32 pointsc);
// static void draw_polygon(f64** points, f64 x, f64 y, i32 pointsc);
// static void draw_polyline(f64** points, f64 x, f64 y, i32 pointsc);
// static void draw_polygon(f64** points, f64 x, f64 y, i32 pointsc);
// static void draw_objects(GameState* state, tmx_map* map, tmx_layer* layer, tmx_object_group* objgr);
// static void draw_tile(void* image, u32 sx, u32 sy, u32 sw, u32 sh, u32 dx, u32 dy);
// static void draw_layer(tmx_map* map, tmx_layer* layer);
// static void draw_image_layer(tmx_image* image);
// static void draw_all_layers(GameState* state, tmx_map* map, tmx_layer* layers);

static void parse_all_layers(MemoryArena* game_mem, GameState* state, tmx_layer* layer);
static void parse_objects(MemoryArena* game_mem, GameState* state, tmx_object_group* objgr);
static Entity
parse_entity(MemoryArena* game_mem, EntityManager* entmgr, TextureManager* texmgr, tmx_object* obj, tmx_tileset* ts);

static SDL_Renderer* renderer;

void tilemap_init(SDL_Renderer* r)
{
    renderer = r;
}

void tilemap_load_level(MemoryArena* game_mem, GameState* state)
{
    parse_all_layers(game_mem, state, state->level->ly_head);
}

// void tilemap_render_map(GameState* state, tmx_map* map)
// {
//     if (!renderer) {
//         util_error("Renderer is NULL :(");
//         return;
//     }
//
//     // set_color(map->backgroundcolor);
//     SDL_RenderClear(renderer);
//     // draw_all_layers(state, map, map->ly_head);
// }

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

static void parse_objects(MemoryArena* game_mem, GameState* state, tmx_object_group* objgr)
{
    tmx_object* obj = objgr->head;
    tmx_map* map = state->level;

    while (obj) {
        if (obj->visible) {
            switch (obj->obj_type) {
            case OT_TILE: {
                tmx_tileset* ts = map->tiles[obj->content.gid]->tileset;
                if (!ts) {
                    util_error("no tileset");
                    obj = obj->next;
                    continue;
                }

                // tmx_image* im = map->tiles[obj->content.gid]->image;
                // f32 op;

                // u32 x = map->tiles[obj->content.gid]->ul_x;
                // u32 y = map->tiles[obj->content.gid]->ul_y;
                // u32 w = ts->tile_width;
                // u32 h = ts->tile_height;

                // void* image;
                // if (im) {
                //     image = im->resource_image;
                // } else {
                //     image = ts->image->resource_image;
                // }
                //
                // u32 flags = obj->id & ~TMX_FLIP_BITS_REMOVAL;

                if (obj->type && strncmp(obj->type, "entity", strlen("entity")) == 0) {
                    if (strncmp(obj->name, "player", strlen("player")) == 0) {
                        state->player = parse_entity(game_mem, state->entmgr, state->texmgr, obj, ts);
                        if (state->player < 0) {
                            break;
                        }
                        obj = obj->next;
                        continue;
                    }

                    Entity ent = parse_entity(game_mem, state->entmgr, state->texmgr, obj, ts);
                    if (ent < 0) {
                        break;
                    }
                }
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

static Entity
parse_entity(MemoryArena* game_mem, EntityManager* entmgr, TextureManager* texmgr, tmx_object* obj, tmx_tileset* ts)
{
    Entity ent = entity_create(entmgr);

    transform_add(entmgr, ent, (Vector2){.x = obj->x, .y = obj->y - obj->height});

    u8 slen = 5 + strlen(ts->image->source);
    char texid[255];
    u8 rlen = snprintf(texid, slen, "res/%s", ts->image->source);
    if (rlen < 0) {
        util_error("Failed concatenating strings");
        return -1;
    }

    SDL_Texture* tex = texmgr_get_texture(texmgr, texid);

    sprite_add(entmgr,
               ent,
               tex,
               (Vector2){.h = obj->height, .w = obj->width},
               (SDL_FRect){0, 0, obj->width, obj->height},
               false);

    rigid_body_add(entmgr, ent, (Vector2){0});
    box_collider_add(entmgr, ent, (Vector2){.x = obj->width, .y = obj->height}, (Vector2){0});
    animation_add(entmgr, ent, 3, 15, true);
    keyboard_control_add(entmgr, ent);

    return ent;
}

// ---------

// static void set_color(i32 color)
// {
//     tmx_col_bytes col = tmx_col_to_bytes(color);
//     SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
// }
//
// static void draw_polyline(f64** points, f64 x, f64 y, int pointsc)
// {
//     for (size_t i = 1; i < pointsc; i++) {
//         SDL_RenderLine(renderer, x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1]);
//     }
// }
//
// static void draw_polygon(f64** points, f64 x, f64 y, int pointsc)
// {
//     draw_polyline(points, x, y, pointsc);
//     if (pointsc > 2) {
//         SDL_RenderLine(
//             renderer, x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1]);
//     }
// }
//
// static void draw_objects(GameState* state, tmx_map* map, tmx_layer* layer, tmx_object_group* objgr)
// {
//     SDL_FRect rect;
//     set_color(objgr->color);
//     tmx_object* head = objgr->head;
//
//     while (head) {
//         if (head->visible) {
//             switch (head->obj_type) {
//             case OT_TILE: {
//                 if (strncmp(head->name, "player", strlen("player")) == 0) {
//                     break;
//                 }
//                 if (strncmp(head->name, "rock", strlen("rock")) == 0) {
//                     printf("rockc");
//                 }
//
//                 tmx_tileset* ts = map->tiles[head->content.gid]->tileset;
//                 tmx_image* im = map->tiles[head->content.gid]->image;
//                 f32 op;
//
//                 u32 x = map->tiles[head->content.gid]->ul_x;
//                 u32 y = map->tiles[head->content.gid]->ul_y;
//                 u32 w = ts->tile_width;
//                 u32 h = ts->tile_height;
//                 tmx_tile* t = map->tiles[head->content.gid];
//
//                 void* image;
//                 if (im) {
//                     image = im->resource_image;
//                 } else {
//                     image = ts->image->resource_image;
//                 }
//
//                 u32 flags = head->id & ~TMX_FLIP_BITS_REMOVAL;
//
//                 draw_tile(image, x, y, w, h, head->x, head->y - head->height, op, flags);
//             } break;
//
//             case OT_SQUARE: {
//                 rect.x = head->x;
//                 rect.y = head->y;
//                 rect.w = head->width;
//                 rect.h = head->height;
//                 SDL_RenderRect(renderer, &rect);
//             } break;
//
//             case OT_POLYGON: {
//                 draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
//             } break;
//
//             case OT_POLYLINE: {
//                 draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
//             } break;
//
//             case OT_ELLIPSE: {
//                 /* FIXME: no function in SDL2 */
//             } break;
//
//             default:
//                 break;
//             }
//         }
//
//         head = head->next;
//     }
// }
//
// static void draw_tile(void* image, u32 sx, u32 sy, u32 sw, u32 sh, u32 dx, u32 dy)
// {
//     SDL_FRect src_rect = {
//         .x = sx,
//         .y = sy,
//         .w = sw,
//         .h = sh,
//     };
//     SDL_FRect dest_rect = {
//         .x = dx,
//         .y = dy,
//         .w = sw,
//         .h = sh,
//     };
//     SDL_RenderTexture(renderer, (SDL_Texture*)image, &src_rect, &dest_rect);
// }

// static void draw_layer(tmx_map* map, tmx_layer* layer)
// {
//     u32 gid, x, y, w, h, flags;
//     f32 op;
//     tmx_tileset* ts;
//     tmx_image* im;
//     void* image;
//     op = layer->opacity;
//
//     for (size_t i = 0; i < map->height; i++) {
//         for (size_t j = 0; j < map->width; j++) {
//             gid = (layer->content.gids[(i * map->width) + j]) & TMX_FLIP_BITS_REMOVAL;
//
//             if (map->tiles[gid] != NULL) {
//                 ts = map->tiles[gid]->tileset;
//                 im = map->tiles[gid]->image;
//                 x = map->tiles[gid]->ul_x;
//                 y = map->tiles[gid]->ul_y;
//                 w = ts->tile_width;
//                 h = ts->tile_height;
//
//                 if (im) {
//                     image = im->resource_image;
//                 } else {
//                     image = ts->image->resource_image;
//                 }
//
//                 flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
//                 draw_tile(image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
//             }
//         }
//     }
// }

// static void draw_image_layer(tmx_image* image)
// {
//     SDL_FRect dim = {0};
//     SDL_Texture* texture = (SDL_Texture*)image->resource_image;
//
//     SDL_GetTextureSize(texture, &dim.w, &dim.h);
//     SDL_RenderTexture(renderer, texture, NULL, &dim);
// }
//
// static void draw_all_layers(GameState* state, tmx_map* map, tmx_layer* layers)
// {
//     while (layers) {
//         if (layers->visible) {
//             if (layers->type == L_GROUP) {
//                 draw_all_layers(state, map, layers->content.group_head);
//             } else if (layers->type == L_OBJGR) {
//                 draw_objects(state, map, layers, layers->content.objgr);
//             } else if (layers->type == L_IMAGE) {
//                 draw_image_layer(layers->content.image);
//             } else if (layers->type == L_LAYER) {
//                 // draw_layer(map, layers);
//             }
//         }
//         layers = layers->next;
//     }
// }
