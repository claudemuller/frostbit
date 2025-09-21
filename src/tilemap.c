#include "tilemap.h"
#include "utils/utils.h"

static void set_color(i32 color);
static void draw_polyline(f64** points, f64 x, f64 y, i32 pointsc);
static void draw_polygon(f64** points, f64 x, f64 y, i32 pointsc);
static void draw_polyline(f64** points, f64 x, f64 y, i32 pointsc);
static void draw_polygon(f64** points, f64 x, f64 y, i32 pointsc);
static void draw_objects(tmx_object_group* objgr);
static void draw_tile(void* image, u32 sx, u32 sy, u32 sw, u32 sh, u32 dx, u32 dy, float opacity, u32 flags);
static void draw_layer(tmx_map* map, tmx_layer* layer);
static void draw_image_layer(tmx_image* image);
static void draw_all_layers(tmx_map* map, tmx_layer* layers);

static SDL_Renderer* renderer;

void tilemap_init(SDL_Renderer* r)
{
    renderer = r;
}

void tilemap_render_map(tmx_map* map)
{
    if (!renderer) {
        util_error("Renderer is NULL :(");
        return;
    }

    set_color(map->backgroundcolor);
    SDL_RenderClear(renderer);
    draw_all_layers(map, map->ly_head);
}

Uint32 timer_func(Uint32 interval, void* param)
{
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
    return (interval);
}

// ------------------------------------------------------------------------------------------------

static void set_color(i32 color)
{
    tmx_col_bytes col = tmx_col_to_bytes(color);
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
}

static void draw_polyline(f64** points, f64 x, f64 y, int pointsc)
{
    for (size_t i = 1; i < pointsc; i++) {
        SDL_RenderLine(renderer, x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1]);
    }
}

static void draw_polygon(f64** points, f64 x, f64 y, int pointsc)
{
    draw_polyline(points, x, y, pointsc);
    if (pointsc > 2) {
        SDL_RenderLine(
            renderer, x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1]);
    }
}

static void draw_objects(tmx_object_group* objgr)
{
    SDL_FRect rect;
    set_color(objgr->color);
    tmx_object* head = objgr->head;

    while (head) {
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
                rect.x = head->x;
                rect.y = head->y;
                rect.w = head->width;
                rect.h = head->height;
                SDL_RenderRect(renderer, &rect);
            } else if (head->obj_type == OT_POLYGON) {
                draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            } else if (head->obj_type == OT_POLYLINE) {
                draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
            } else if (head->obj_type == OT_ELLIPSE) {
                /* FIXME: no function in SDL2 */
            }
        }

        head = head->next;
    }
}

static void draw_tile(void* image, u32 sx, u32 sy, u32 sw, u32 sh, u32 dx, u32 dy, f32 opacity, u32 flags)
{
    SDL_FRect src_rect = {
        .x = sx,
        .y = sy,
        .w = sw,
        .h = sh,
    };
    SDL_FRect dest_rect = {
        .x = dx,
        .y = dy,
        .w = sw,
        .h = sh,
    };
    SDL_RenderTexture(renderer, (SDL_Texture*)image, &src_rect, &dest_rect);
}

static void draw_layer(tmx_map* map, tmx_layer* layer)
{
    u32 gid, x, y, w, h, flags;
    f32 op;
    tmx_tileset* ts;
    tmx_image* im;
    void* image;
    op = layer->opacity;

    for (size_t i = 0; i < map->height; i++) {
        for (size_t j = 0; j < map->width; j++) {
            gid = (layer->content.gids[(i * map->width) + j]) & TMX_FLIP_BITS_REMOVAL;

            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                x = map->tiles[gid]->ul_x;
                y = map->tiles[gid]->ul_y;
                w = ts->tile_width;
                h = ts->tile_height;

                if (im) {
                    image = im->resource_image;
                } else {
                    image = ts->image->resource_image;
                }

                flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                draw_tile(image, x, y, w, h, j * ts->tile_width, i * ts->tile_height, op, flags);
            }
        }
    }
}

static void draw_image_layer(tmx_image* image)
{
    SDL_FRect dim = {0};
    SDL_Texture* texture = (SDL_Texture*)image->resource_image;

    SDL_GetTextureSize(texture, &dim.w, &dim.h);
    SDL_RenderTexture(renderer, texture, NULL, &dim);
}

static void draw_all_layers(tmx_map* map, tmx_layer* layers)
{
    while (layers) {
        if (layers->visible) {
            if (layers->type == L_GROUP) {
                draw_all_layers(map, layers->content.group_head);
            } else if (layers->type == L_OBJGR) {
                draw_objects(layers->content.objgr);
            } else if (layers->type == L_IMAGE) {
                draw_image_layer(layers->content.image);
            } else if (layers->type == L_LAYER) {
                draw_layer(map, layers);
            }
        }
        layers = layers->next;
    }
}
