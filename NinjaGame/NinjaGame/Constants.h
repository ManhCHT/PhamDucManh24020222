#ifndef CONSTANTS_H
#define CONSTANTS_H

// Kích thước màn hình
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 650;

// Kích thước và tốc độ của nhân vật
const int PLAYER_WIDTH = 58;
const int PLAYER_HEIGHT = 58;
const int PLAYER_SPEED = 5;
const int PLAYER_SPEED_INVIS = 7;

// Trạng thái tấn công gần
const int ATTACK_DURATION = 300;
const int ATTACK_COOLDOWN = 500;

// Trạng thái tàng hình
const int INVIS_DURATION = 3000;
const int INVIS_COOLDOWN = 15000;

// Cơ chế dịch chuyển
const int DASH_DISTANCE = PLAYER_WIDTH * 5;
const int DASH_COOLDOWN = 3000;

// Kích thước khung hình sprite
const int FRAME_WIDTH = 36;
const int FRAME_HEIGHT = 36;
const int TOTAL_FRAMES = 4;

// Trail effect
const int TRAIL_FRAME_WIDTH = 36;
const int TRAIL_FRAME_HEIGHT = 36;
const int TRAIL_TOTAL_FRAMES = 3;
const int TRAIL_FRAME_DURATION = 100;

// Hit effect
const int HIT_FRAME_WIDTH = 48;
const int HIT_FRAME_HEIGHT = 58;
const int HIT_TOTAL_FRAMES = 3;

// Circle hit effect (tàng hình)
const int CIRCLE_HIT_FRAME_WIDTH = 124;
const int CIRCLE_HIT_FRAME_HEIGHT = 124;
const int CIRCLE_HIT_TOTAL_FRAMES = 4;

// Enemy melee
const int ENEMY_MELEE_FRAME_WIDTH = 36;
const int ENEMY_MELEE_FRAME_HEIGHT = 36;
const int ENEMY_MELEE_TOTAL_FRAMES = 4;

// Enemy
const int ENEMY_FRAME_WIDTH = 36;
const int ENEMY_FRAME_HEIGHT = 36;
const int ENEMY_TOTAL_FRAMES = 4;

// Enemy death
const int ENEMY_DEATH_FRAME_WIDTH = 36;
const int ENEMY_DEATH_FRAME_HEIGHT = 36;
const int ENEMY_DEATH_TOTAL_FRAMES = 2;
const int ENEMY_DEATH_FRAME_DURATION = 200;
const int ENEMY_DEATH_ANIMATION_DURATION = ENEMY_DEATH_FRAME_DURATION * ENEMY_DEATH_TOTAL_FRAMES;

// Enemy spawn
const int ENEMY_SPAWN_FRAME_WIDTH = 36;
const int ENEMY_SPAWN_FRAME_HEIGHT = 36;
const int ENEMY_SPAWN_TOTAL_FRAMES = 2;
const int ENEMY_SPAWN_FRAME_DURATION = 200;
const int ENEMY_SPAWN_ANIMATION_DURATION = ENEMY_SPAWN_FRAME_DURATION * ENEMY_SPAWN_TOTAL_FRAMES;

// Shuriken
const int SHURIKEN_FRAME_WIDTH = 16;
const int SHURIKEN_FRAME_HEIGHT = 16;
const int SHURIKEN_TOTAL_FRAMES = 2;
const int SHURIKEN_FRAME_DURATION = 100;
const float SHURIKEN_ROTATION_SPEED = 10.0f;

// Heart item
const int HEART_ITEM_FRAME_WIDTH = 15;
const int HEART_ITEM_FRAME_HEIGHT = 15;
const int HEART_ITEM_TOTAL_FRAMES = 4;
const int HEART_ITEM_FRAME_DURATION = 500;

// Game intervals
const Uint32 HEALTH_SPAWN_INTERVAL = 30000;
const Uint32 HEALTH_ITEM_DURATION = 7000;
const Uint32 SPAWN_INTERVAL = 30000;
const Uint32 ENEMY_RESPAWN_DELAY = 3000;
const int SHOOT_COOLDOWN = 700;
const int SHOOT_ANIMATION_DURATION = 300;
const int DASH_ANIMATION_DURATION = 300;

const int PLAYER_MELEE_FRAME_WIDTH = 36;  
const int PLAYER_MELEE_FRAME_HEIGHT = 36;
const int PLAYER_MELEE_TOTAL_FRAMES = 4;

#endif