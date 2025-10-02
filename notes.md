# Todo

- [ ] TileRenderSystem
- [ ] Static objects i.e. don't have rigid body, can't collide with each other
- [ ] Make collision layer for ground, ceiling, wall etc.
- [ ] Make gameplay object layer - same as above but with extra behaviour like EnemyAI, Collectible etc.
- [ ] Make UI layer

# Research

- Broad‑phase vs. narrow‑phase
    - Broad‑phase – Use a spatial hash or uniform grid that indexes collidable entities. Since most static terrain isn’t an entity, you’ll only insert the relatively few obstacle entities (walls, platforms, moving objects).
    - Narrow‑phase – When two entities’ broad‑phase cells overlap, run AABB checks using their BoxColliders.

- Tile‑based collision fallback
    - If you ever need pixel‑perfect terrain collision (e.g., slopes), you can still query the TileMap directly:
    - Then, during physics resolution, you can sample the tile underneath the entity and push it out if it’s solid. This hybrid approach lets you keep most terrain as data while still supporting fine‑grained collision where needed.

```c
bool isSolid(int tileX, int tileY) {
    int gid = tileMap.getGid(tileX, tileY);
    return solidGids.contains(gid);
}
```

