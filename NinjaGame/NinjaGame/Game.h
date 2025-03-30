#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "Constants.h"
#include "Structs.h"

extern int playerX, playerY;
extern int playerHealth;
extern bool gameOver;
extern bool gamePaused;
extern int score;
extern bool isAttacking;
extern Uint32 attackStartTime;
extern Uint32 lastAttackTime;
extern int currentDirection;
extern bool isInvisible;
extern Uint32 invisStartTime;
extern Uint32 lastInvisTime;
extern Uint32 lastDashTime;
extern int enemyHitCounter;
extern SDL_Texture* playerSpriteSheet;
extern SDL_Texture* shootSpriteSheet;
extern SDL_Texture* dashSpriteSheet;
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* gamePauseTexture;
extern SDL_Rect playerFrames[TOTAL_FRAMES];
extern SDL_Rect shootFrames[TOTAL_FRAMES];
extern SDL_Rect dashFrames[TOTAL_FRAMES];
extern SDL_Texture* trailSpriteSheet;
extern SDL_Rect trailFrames[TRAIL_TOTAL_FRAMES];
extern SDL_Texture* playerMeleeSpriteSheet;
extern SDL_Rect playerMeleeFrames[PLAYER_MELEE_TOTAL_FRAMES];
extern SDL_Texture* invisSpriteSheet;
extern SDL_Rect invisFrames[TOTAL_FRAMES];
extern SDL_Texture* invisAttackSpriteSheet;
extern SDL_Rect invisAttackFrames[PLAYER_MELEE_TOTAL_FRAMES];
extern SDL_Texture* invisJumpSpriteSheet;
extern SDL_Rect invisJumpFrames[TOTAL_FRAMES];
extern SDL_Texture* hitSpriteSheet;
extern SDL_Rect hitFrames[HIT_TOTAL_FRAMES];
extern int currentHitFrame;
extern SDL_Texture* circleHitSpriteSheet;
extern SDL_Rect circleHitFrames[CIRCLE_HIT_TOTAL_FRAMES];
extern int currentCircleHitFrame;
extern Uint32 lastCircleHitFrameTime;
extern SDL_Texture* enemyMeleeSpriteSheet;
extern SDL_Rect enemyMeleeFrames[ENEMY_MELEE_TOTAL_FRAMES];
extern SDL_Texture* enemySpriteSheet;
extern SDL_Rect enemyFrames[ENEMY_TOTAL_FRAMES];
extern SDL_Texture* enemyDeathSpriteSheet;
extern SDL_Rect enemyDeathFrames[ENEMY_DEATH_TOTAL_FRAMES];
extern SDL_Texture* enemySpawnSpriteSheet;
extern SDL_Rect enemySpawnFrames[ENEMY_SPAWN_TOTAL_FRAMES];
extern SDL_Texture* shurikenSpriteSheet;
extern SDL_Rect shurikenFrames[SHURIKEN_TOTAL_FRAMES];
extern int currentShurikenFrame;
extern Uint32 lastShurikenFrameTime;
extern float shurikenRotationAngle;
extern SDL_Texture* heartIconTexture;
extern SDL_Texture* heartItemTexture;
extern SDL_Rect heartItemFrames[HEART_ITEM_TOTAL_FRAMES];
extern int currentHeartItemFrame;
extern Uint32 lastHeartItemFrameTime;
extern SDL_Texture* backgroundTexture;
extern TTF_Font* gameFont;
extern SDL_Texture* scoreTextTexture;
extern SDL_Texture* restartTextTexture;
extern SDL_Rect scoreTextRect;
extern SDL_Rect restartTextRect;
extern SDL_Texture* pauseTextTexture;
extern SDL_Rect pauseTextRect;
extern Mix_Music* backgroundMusic;
extern Mix_Chunk* gameOverSound;
extern Mix_Chunk* hitSound;
extern Mix_Chunk* killSound;
extern Mix_Chunk* jumpSound;
extern Mix_Chunk* invisSound;
extern Mix_Chunk* accept1Sound;
extern Mix_Chunk* accept2Sound;
extern bool isPaused;
extern SDL_Texture* pauseTexture;
extern SDL_Rect pauseRect;
extern SDL_Texture* gameStartTexture;
extern SDL_Texture* startTextTexture;
extern SDL_Rect startTextRect;
extern bool gameStarted;
extern int currentTrailFrame;
extern Uint32 lastTrailFrameTime;
extern bool isShooting;
extern Uint32 shootStartTime;
extern bool isDashing;
extern Uint32 dashStartTime;
extern std::vector<Projectile> projectiles;
extern Uint32 lastShootTime;
extern std::vector<Enemy> enemies;
extern std::vector<HealthItem> healthItems;
extern Uint32 lastHealthSpawnTime;
extern std::vector<EnemyRespawn> enemyRespawns;
extern Uint32 lastSpawnTime;

enum PlayerDirection { DOWN, UP, LEFT, RIGHT };

bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void resetGame(int& playerX, int& playerY, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles,
    std::vector<HealthItem>& healthItems, std::vector<EnemyRespawn>& enemyRespawns);
void handleEvents(bool& running, int& playerX, int& playerY);
void updateProjectiles();
void updateEnemies(int playerX, int playerY);
void handleGameOverEvents(bool& running, int& playerX, int& playerY, std::vector<Enemy>& enemies,
    std::vector<Projectile>& projectiles, std::vector<HealthItem>& healthItems,
    std::vector<EnemyRespawn>& enemyRespawns);
void render(SDL_Renderer* renderer, int playerX, int playerY);
bool checkCollision(SDL_Rect a, SDL_Rect b);
void spawnRandomEnemy(std::vector<Enemy>& enemies);
void spawnRandomHealthItem(std::vector<HealthItem>& healthItems);

#endif