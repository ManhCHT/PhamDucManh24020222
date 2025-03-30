#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL.h>
#include <vector>

struct Projectile {
    int x, y;
    int dir;
    int speed = 10;
    bool active = true;
    int distanceTraveled = 0;
};

struct Enemy {
    int x, y;
    int width = 58, height = 58;
    int speed = 1;
    int homeX, homeY;
    int patrolRange = 200;
    bool alert = false;
    Uint32 lastMoveTime = 0;
    int direction = 0;
    bool isAttacking = false;
    Uint32 attackStartTime = 0;
    Uint32 lastAttackTime = 0;
    int attackDirection = 0;
    bool isDying = false;
    Uint32 deathStartTime = 0;
    int currentDeathFrame = 0;
    Uint32 lastDeathFrameTime = 0;
    bool isSpawning = false;
    Uint32 spawnStartTime = 0;
    int currentSpawnFrame = 0;
    Uint32 lastSpawnFrameTime = 0;

    Enemy(int x_, int y_, int w_, int h_, int speed_, int homeX_, int homeY_, int patrolRange_,
        bool alert_, Uint32 lastMoveTime_, int direction_, bool isAttacking_, Uint32 attackStartTime_,
        Uint32 lastAttackTime_, int attackDirection_, bool isDying_, Uint32 deathStartTime_,
        int currentDeathFrame_, Uint32 lastDeathFrameTime_, bool isSpawning_, Uint32 spawnStartTime_,
        int currentSpawnFrame_, Uint32 lastSpawnFrameTime_);
};

struct HealthItem {
    int x, y;
    int width = 15, height = 15;
    bool active = true;
    Uint32 spawnTime = 0;
};

struct EnemyRespawn {
    Uint32 deathTime;
    int count;
    EnemyRespawn(Uint32 dt, int c) : deathTime(dt), count(c) {}
};

#endif