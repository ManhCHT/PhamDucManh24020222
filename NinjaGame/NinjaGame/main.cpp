#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <string>
#include "Constants.h"
#include "Structs.h"
#include "Game.h"

using namespace std;

// --- Các biến toàn cục (Global variables) ---

// Vị trí và trạng thái của người chơi
int playerX = 100, playerY = 300; // Tọa độ ban đầu của người chơi
int playerHealth = 10; // Máu của người chơi (tối đa 10)
bool gameOver = false; // Cờ báo hiệu trò chơi kết thúc
bool gamePaused = false; // Cờ báo hiệu trò chơi đang tạm dừng
int score = 0; // Điểm số của người chơi

// Trạng thái tấn công gần của người chơi
bool isAttacking = false; // Người chơi có đang tấn công hay không
Uint32 attackStartTime = 0; // Thời điểm bắt đầu tấn công
Uint32 lastAttackTime = 0; // Thời điểm tấn công cuối cùng (dùng để tính cooldown)
int currentDirection = 1; // Hướng hiện tại của người chơi (0-7)

// Trạng thái tàng hình
bool isInvisible = false; // Người chơi có đang tàng hình hay không
Uint32 invisStartTime = 0; // Thời điểm bắt đầu tàng hình
Uint32 lastInvisTime = 0; // Thời điểm tàng hình cuối cùng (dùng để tính cooldown)

// Trạng thái dịch chuyển
Uint32 lastDashTime = 0; // Thời điểm dịch chuyển cuối cùng (dùng để tính cooldown)
int enemyHitCounter = 0; // Đếm số lần kẻ địch đánh trúng người chơi

// Tài nguyên đồ họa (Textures và Frames)
SDL_Texture* playerSpriteSheet = nullptr; // Sprite sheet cho nhân vật chính
SDL_Texture* shootSpriteSheet = nullptr; // Sprite sheet khi bắn phi tiêu
SDL_Texture* dashSpriteSheet = nullptr; // Sprite sheet khi dịch chuyển
SDL_Texture* gameOverTexture = nullptr; // Hình ảnh màn hình game over
SDL_Texture* gamePauseTexture = nullptr; // Hình ảnh màn hình tạm dừng
SDL_Rect playerFrames[TOTAL_FRAMES]; // Các frame của sprite sheet nhân vật chính
SDL_Rect shootFrames[TOTAL_FRAMES]; // Các frame khi bắn phi tiêu
SDL_Rect dashFrames[TOTAL_FRAMES]; // Các frame khi dịch chuyển
SDL_Texture* trailSpriteSheet = nullptr; // Sprite sheet cho hiệu ứng dịch chuyển
SDL_Rect trailFrames[TRAIL_TOTAL_FRAMES]; // Các frame hiệu ứng dịch chuyển
SDL_Texture* playerMeleeSpriteSheet = nullptr; // Sprite sheet cho tấn công cận chiến
SDL_Rect playerMeleeFrames[PLAYER_MELEE_TOTAL_FRAMES]; // Các frame tấn công cận chiến
SDL_Texture* invisSpriteSheet = nullptr; // Sprite sheet khi tàng hình
SDL_Rect invisFrames[TOTAL_FRAMES]; // Các frame khi tàng hình
SDL_Texture* invisAttackSpriteSheet = nullptr; // Sprite sheet tấn công khi tàng hình
SDL_Rect invisAttackFrames[PLAYER_MELEE_TOTAL_FRAMES]; // Các frame tấn công khi tàng hình
SDL_Texture* invisJumpSpriteSheet = nullptr; // Sprite sheet dịch chuyển khi tàng hình
SDL_Rect invisJumpFrames[TOTAL_FRAMES]; // Các frame dịch chuyển khi tàng hình
SDL_Texture* hitSpriteSheet = nullptr; // Sprite sheet hiệu ứng đánh trúng
SDL_Rect hitFrames[HIT_TOTAL_FRAMES]; // Các frame hiệu ứng đánh trúng
int currentHitFrame = 0; // Frame hiện tại của hiệu ứng đánh trúng
SDL_Texture* circleHitSpriteSheet = nullptr; // Sprite sheet hiệu ứng đánh trúng vòng tròn (khi tàng hình)
SDL_Rect circleHitFrames[CIRCLE_HIT_TOTAL_FRAMES]; // Các frame hiệu ứng đánh trúng vòng tròn
int currentCircleHitFrame = 0; // Frame hiện tại của hiệu ứng vòng tròn
Uint32 lastCircleHitFrameTime = 0; // Thời điểm cập nhật frame cuối cùng của vòng tròn
SDL_Texture* enemyMeleeSpriteSheet = nullptr; // Sprite sheet tấn công của kẻ địch
SDL_Rect enemyMeleeFrames[ENEMY_MELEE_TOTAL_FRAMES]; // Các frame tấn công của kẻ địch
SDL_Texture* enemySpriteSheet = nullptr; // Sprite sheet của kẻ địch
SDL_Rect enemyFrames[ENEMY_TOTAL_FRAMES]; // Các frame của kẻ địch
SDL_Texture* enemyDeathSpriteSheet = nullptr; // Sprite sheet khi kẻ địch chết
SDL_Rect enemyDeathFrames[ENEMY_DEATH_TOTAL_FRAMES]; // Các frame khi kẻ địch chết
SDL_Texture* enemySpawnSpriteSheet = nullptr; // Sprite sheet khi kẻ địch xuất hiện
SDL_Rect enemySpawnFrames[ENEMY_SPAWN_TOTAL_FRAMES]; // Các frame khi kẻ địch xuất hiện
SDL_Texture* shurikenSpriteSheet = nullptr; // Sprite sheet của phi tiêu
SDL_Rect shurikenFrames[SHURIKEN_TOTAL_FRAMES]; // Các frame của phi tiêu
int currentShurikenFrame = 0; // Frame hiện tại của phi tiêu
Uint32 lastShurikenFrameTime = 0; // Thời điểm cập nhật frame cuối cùng của phi tiêu
float shurikenRotationAngle = 0; // Góc xoay của phi tiêu
SDL_Texture* heartIconTexture = nullptr; // Hình ảnh biểu tượng trái tim (máu)
SDL_Texture* heartItemTexture = nullptr; // Hình ảnh vật phẩm hồi máu
SDL_Rect heartItemFrames[HEART_ITEM_TOTAL_FRAMES]; // Các frame của vật phẩm hồi máu
int currentHeartItemFrame = 0; // Frame hiện tại của vật phẩm hồi máu
Uint32 lastHeartItemFrameTime = 0; // Thời điểm cập nhật frame cuối cùng của vật phẩm hồi máu
SDL_Texture* backgroundTexture = nullptr; // Hình ảnh nền trò chơi

// Tài nguyên văn bản và âm thanh
TTF_Font* gameFont = nullptr; // Font chữ dùng trong trò chơi
SDL_Texture* scoreTextTexture = nullptr; // Texture hiển thị điểm số
SDL_Texture* restartTextTexture = nullptr; // Texture hiển thị hướng dẫn khởi động lại
SDL_Rect scoreTextRect; // Vùng hiển thị điểm số
SDL_Rect restartTextRect; // Vùng hiển thị hướng dẫn khởi động lại
SDL_Texture* pauseTextTexture = nullptr; // Texture hiển thị hướng dẫn tiếp tục khi tạm dừng
SDL_Rect pauseTextRect; // Vùng hiển thị hướng dẫn tiếp tục
Mix_Music* backgroundMusic = nullptr; // Nhạc nền của trò chơi
Mix_Chunk* gameOverSound = nullptr; // Âm thanh khi trò chơi kết thúc
Mix_Chunk* hitSound = nullptr; // Âm thanh khi đánh trúng
Mix_Chunk* killSound = nullptr; // Âm thanh khi tiêu diệt kẻ địch
Mix_Chunk* jumpSound = nullptr; // Âm thanh khi dịch chuyển
Mix_Chunk* invisSound = nullptr; // Âm thanh khi tàng hình
Mix_Chunk* accept1Sound = nullptr; // Âm thanh xác nhận 1
Mix_Chunk* accept2Sound = nullptr; // Âm thanh xác nhận 2

// Trạng thái game
bool isPaused = false; // Cờ báo hiệu game đang tạm dừng
SDL_Texture* pauseTexture = nullptr; // Texture màn hình tạm dừng (khác với gamePauseTexture)
SDL_Rect pauseRect; // Vùng hiển thị màn hình tạm dừng
SDL_Texture* gameStartTexture = nullptr; // Hình ảnh màn hình bắt đầu trò chơi
SDL_Texture* startTextTexture = nullptr; // Texture hiển thị hướng dẫn bắt đầu
SDL_Rect startTextRect; // Vùng hiển thị hướng dẫn bắt đầu
bool gameStarted = false; // Cờ báo hiệu trò chơi đã bắt đầu

// Hoạt ảnh
int currentTrailFrame = 0; // Frame hiện tại của hiệu ứng dịch chuyển
Uint32 lastTrailFrameTime = 0; // Thời điểm cập nhật frame cuối cùng của hiệu ứng dịch chuyển
bool isShooting = false; // Người chơi có đang bắn phi tiêu hay không
Uint32 shootStartTime = 0; // Thời điểm bắt đầu bắn phi tiêu
bool isDashing = false; // Người chơi có đang dịch chuyển hay không
Uint32 dashStartTime = 0; // Thời điểm bắt đầu dịch chuyển

// Danh sách đối tượng
vector<Projectile> projectiles; // Danh sách các phi tiêu
Uint32 lastShootTime = 0; // Thời điểm bắn phi tiêu cuối cùng (dùng để tính cooldown)
vector<Enemy> enemies; // Danh sách kẻ địch
vector<HealthItem> healthItems; // Danh sách vật phẩm hồi máu
Uint32 lastHealthSpawnTime = 0; // Thời điểm xuất hiện vật phẩm hồi máu cuối cùng
vector<EnemyRespawn> enemyRespawns; // Danh sách thông tin tái sinh kẻ địch
Uint32 lastSpawnTime = 0; // Thời điểm sinh kẻ địch cuối cùng

// --- Định nghĩa các hàm ---

// Constructor cho lớp Enemy (kẻ địch)
Enemy::Enemy(int x_, int y_, int w_, int h_, int speed_, int homeX_, int homeY_, int patrolRange_,
    bool alert_, Uint32 lastMoveTime_, int direction_, bool isAttacking_, Uint32 attackStartTime_,
    Uint32 lastAttackTime_, int attackDirection_, bool isDying_, Uint32 deathStartTime_,
    int currentDeathFrame_, Uint32 lastDeathFrameTime_, bool isSpawning_, Uint32 spawnStartTime_,
    int currentSpawnFrame_, Uint32 lastSpawnFrameTime_)
    : x(x_), y(y_), width(w_), height(h_), speed(speed_), homeX(homeX_), homeY(homeY_),
    patrolRange(patrolRange_), alert(alert_), lastMoveTime(lastMoveTime_), direction(direction_),
    isAttacking(isAttacking_), attackStartTime(attackStartTime_), lastAttackTime(lastAttackTime_),
    attackDirection(attackDirection_), isDying(isDying_), deathStartTime(deathStartTime_),
    currentDeathFrame(currentDeathFrame_), lastDeathFrameTime(lastDeathFrameTime_),
    isSpawning(isSpawning_), spawnStartTime(spawnStartTime_), currentSpawnFrame(currentSpawnFrame_),
    lastSpawnFrameTime(lastSpawnFrameTime_) {
    // Khởi tạo tất cả các thuộc tính của kẻ địch với giá trị truyền vào
}

// Kiểm tra va chạm giữa hai hình chữ nhật
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    // Trả về true nếu hai hình chữ nhật giao nhau
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

// Sinh ngẫu nhiên một kẻ địch
void spawnRandomEnemy(vector<Enemy>& enemies) {
    int randX = rand() % (SCREEN_WIDTH - PLAYER_WIDTH - 100) + 50; // Tọa độ X ngẫu nhiên
    int randY = rand() % (SCREEN_HEIGHT - PLAYER_HEIGHT - 100) + 50; // Tọa độ Y ngẫu nhiên
    int randDirection = rand() % 4; // Hướng ngẫu nhiên (0-3)
    // Thêm một kẻ địch mới vào danh sách với các thuộc tính ban đầu
    enemies.push_back(Enemy(
        randX, randY, PLAYER_WIDTH, PLAYER_HEIGHT, 1, randX, randY, 200,
        false, 0, randDirection, false, 0, 0, 0,
        false, 0, 0, 0, true, SDL_GetTicks(), 0, SDL_GetTicks()
    ));
}

// Sinh ngẫu nhiên một vật phẩm hồi máu
void spawnRandomHealthItem(vector<HealthItem>& healthItems) {
    int randX = rand() % (SCREEN_WIDTH - 15 - 100) + 50; // Tọa độ X ngẫu nhiên
    int randY = rand() % (SCREEN_HEIGHT - 15 - 100) + 50; // Tọa độ Y ngẫu nhiên
    // Thêm một vật phẩm hồi máu mới vào danh sách
    healthItems.push_back({ randX, randY, 15, 15, true, SDL_GetTicks() });
}

// Khởi tạo SDL và các tài nguyên
bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    // Khởi tạo SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL không thể khởi tạo! Lỗi: " << SDL_GetError() << endl;
        return false;
    }

    // Khởi tạo SDL_image để tải ảnh PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image không thể khởi tạo! Lỗi: " << IMG_GetError() << endl;
        SDL_Quit();
        return false;
    }

    // Khởi tạo SDL_ttf để xử lý font chữ
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf không thể khởi tạo! Lỗi: " << TTF_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Khởi tạo SDL_mixer để xử lý âm thanh
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Không thể khởi tạo SDL_mixer! Lỗi: " << Mix_GetError() << endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Tải nhạc nền
    backgroundMusic = Mix_LoadMUS("sound/backgroundmusic.mp3");
    if (!backgroundMusic) cerr << "Không thể tải background music: " << Mix_GetError() << endl;

    // Tải âm thanh game over
    gameOverSound = Mix_LoadWAV("sound/gameover.wav");
    if (!gameOverSound) cerr << "Không thể tải game over sound: " << Mix_GetError() << endl;

    // Tải âm thanh đánh trúng
    hitSound = Mix_LoadWAV("sound/hit.wav");
    if (!hitSound) cerr << "Không thể tải hit sound: " << Mix_GetError() << endl;

    // Tải âm thanh tiêu diệt
    killSound = Mix_LoadWAV("sound/kill.wav");
    if (!killSound) cerr << "Không thể tải kill sound: " << Mix_GetError() << endl;

    // Tải âm thanh dịch chuyển
    jumpSound = Mix_LoadWAV("sound/jump.wav");
    if (!jumpSound) cerr << "Không thể tải jump sound: " << Mix_GetError() << endl;

    // Tải âm thanh tàng hình
    invisSound = Mix_LoadWAV("sound/invis.wav");
    if (!invisSound) cerr << "Không thể tải invis sound: " << Mix_GetError() << endl;

    // Tải âm thanh xác nhận 1
    accept1Sound = Mix_LoadWAV("sound/accept1.wav");
    if (!accept1Sound) cerr << "Không thể tải accept1 sound: " << Mix_GetError() << endl;

    // Tải âm thanh xác nhận 2
    accept2Sound = Mix_LoadWAV("sound/accept2.wav");
    if (!accept2Sound) cerr << "Không thể tải accept2 sound: " << Mix_GetError() << endl;

    // Tải font chữ
    gameFont = TTF_OpenFont("gamefont.ttf", 24);
    if (!gameFont) cerr << "Không thể tải font: " << TTF_GetError() << endl;

    // Đặt chất lượng render để hình ảnh đẹp hơn
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Tạo cửa sổ game
    window = SDL_CreateWindow("Ninja Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << endl;
        return false;
    }

    // Tạo renderer để vẽ đồ họa
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Không thể tạo renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        return false;
    }

    // Tải các texture từ file ảnh
    SDL_Surface* tempSurface;

    // Tải sprite sheet nhân vật chính
    tempSurface = IMG_Load("image/player.png");
    playerSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        playerFrames[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT }; // Khởi tạo các frame
    }

    // Tải sprite sheet bắn phi tiêu
    tempSurface = IMG_Load("image/donphitieu.png");
    shootSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        shootFrames[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
    }

    // Tải sprite sheet dịch chuyển
    tempSurface = IMG_Load("image/jump.png");
    dashSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        dashFrames[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
    }

    // Tải hình ảnh game over
    tempSurface = IMG_Load("image/gameover.png");
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tải hình ảnh tạm dừng
    tempSurface = IMG_Load("image/gamepause.png");
    gamePauseTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tải sprite sheet hiệu ứng dịch chuyển
    tempSurface = IMG_Load("image/tele.png");
    trailSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TRAIL_TOTAL_FRAMES; i++) {
        trailFrames[i] = { i * TRAIL_FRAME_WIDTH, 0, TRAIL_FRAME_WIDTH, TRAIL_FRAME_HEIGHT };
    }

    // Tải sprite sheet tấn công cận chiến
    tempSurface = IMG_Load("image/attack.png");
    playerMeleeSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < PLAYER_MELEE_TOTAL_FRAMES; i++) {
        playerMeleeFrames[i] = { i * PLAYER_MELEE_FRAME_WIDTH, 0, PLAYER_MELEE_FRAME_WIDTH, PLAYER_MELEE_FRAME_HEIGHT };
    }

    // Tải sprite sheet tàng hình
    tempSurface = IMG_Load("image/invis.png");
    invisSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        invisFrames[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
    }

    // Tải sprite sheet tấn công khi tàng hình
    tempSurface = IMG_Load("image/invisattack.png");
    invisAttackSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < PLAYER_MELEE_TOTAL_FRAMES; i++) {
        invisAttackFrames[i] = { i * PLAYER_MELEE_FRAME_WIDTH, 0, PLAYER_MELEE_FRAME_WIDTH, PLAYER_MELEE_FRAME_HEIGHT };
    }

    // Tải sprite sheet dịch chuyển khi tàng hình
    tempSurface = IMG_Load("image/invisjump.png");
    invisJumpSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        invisJumpFrames[i] = { i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
    }

    // Tải sprite sheet hiệu ứng đánh trúng
    tempSurface = IMG_Load("image/hit.png");
    hitSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < HIT_TOTAL_FRAMES; i++) {
        hitFrames[i] = { i * HIT_FRAME_WIDTH, 0, HIT_FRAME_WIDTH, HIT_FRAME_HEIGHT };
    }

    // Tải sprite sheet hiệu ứng đánh trúng vòng tròn
    tempSurface = IMG_Load("image/circlehit.png");
    circleHitSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < CIRCLE_HIT_TOTAL_FRAMES; i++) {
        circleHitFrames[i] = { i * CIRCLE_HIT_FRAME_WIDTH, 0, CIRCLE_HIT_FRAME_WIDTH, CIRCLE_HIT_FRAME_HEIGHT };
    }

    // Tải sprite sheet tấn công của kẻ địch
    tempSurface = IMG_Load("image/enemyattack.png");
    enemyMeleeSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < ENEMY_MELEE_TOTAL_FRAMES; i++) {
        enemyMeleeFrames[i] = { i * ENEMY_MELEE_FRAME_WIDTH, 0, ENEMY_MELEE_FRAME_WIDTH, ENEMY_MELEE_FRAME_HEIGHT };
    }

    // Tải sprite sheet của kẻ địch
    tempSurface = IMG_Load("image/enemy.png");
    enemySpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < ENEMY_TOTAL_FRAMES; i++) {
        enemyFrames[i] = { i * ENEMY_FRAME_WIDTH, 0, ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT };
    }

    // Tải sprite sheet khi kẻ địch chết
    tempSurface = IMG_Load("image/enemyDeath.png");
    enemyDeathSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < ENEMY_DEATH_TOTAL_FRAMES; i++) {
        enemyDeathFrames[i] = { i * ENEMY_DEATH_FRAME_WIDTH, 0, ENEMY_DEATH_FRAME_WIDTH, ENEMY_DEATH_FRAME_HEIGHT };
    }

    // Tải sprite sheet khi kẻ địch xuất hiện
    tempSurface = IMG_Load("image/enemySpawn.png");
    enemySpawnSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < ENEMY_SPAWN_TOTAL_FRAMES; i++) {
        enemySpawnFrames[i] = { i * ENEMY_SPAWN_FRAME_WIDTH, 0, ENEMY_SPAWN_FRAME_WIDTH, ENEMY_SPAWN_FRAME_HEIGHT };
    }

    // Tải sprite sheet phi tiêu
    tempSurface = IMG_Load("image/shuriken.png");
    shurikenSpriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < SHURIKEN_TOTAL_FRAMES; i++) {
        shurikenFrames[i] = { i * SHURIKEN_FRAME_WIDTH, 0, SHURIKEN_FRAME_WIDTH, SHURIKEN_FRAME_HEIGHT };
    }

    // Tải biểu tượng trái tim
    tempSurface = IMG_Load("image/hearticon.png");
    heartIconTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tải vật phẩm hồi máu
    tempSurface = IMG_Load("image/heartitem.png");
    heartItemTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < HEART_ITEM_TOTAL_FRAMES; i++) {
        heartItemFrames[i] = { i * HEART_ITEM_FRAME_WIDTH, 0, HEART_ITEM_FRAME_WIDTH, HEART_ITEM_FRAME_HEIGHT };
    }

    // Tải hình ảnh nền
    tempSurface = IMG_Load("image/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tải hình ảnh tạm dừng (trùng lặp với gamePauseTexture)
    tempSurface = IMG_Load("image/gamepause.png");
    pauseTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tải hình ảnh màn hình bắt đầu
    tempSurface = IMG_Load("image/gamestart.png");
    gameStartTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Tạo texture văn bản hướng dẫn bắt đầu
    SDL_Color textColor = { 255, 255, 255, 255 }; // Màu trắng
    tempSurface = TTF_RenderText_Solid(gameFont, "Nhan   Space   de   bat   dau!", textColor);
    startTextTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    startTextRect.w = tempSurface->w;
    startTextRect.h = tempSurface->h;
    SDL_FreeSurface(tempSurface);

    // Tạo texture văn bản hướng dẫn tiếp tục
    tempSurface = TTF_RenderText_Solid(gameFont, "Nhan   P   de   tiep   tuc!", textColor);
    pauseTextTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    pauseTextRect.w = tempSurface->w;
    pauseTextRect.h = tempSurface->h;
    SDL_FreeSurface(tempSurface);

    // Sinh 5 kẻ địch ban đầu
    const int NUM_ENEMIES = 5;
    for (int i = 0; i < NUM_ENEMIES; i++) {
        spawnRandomEnemy(enemies);
    }

    return true; // Khởi tạo thành công
}

// Đặt lại trạng thái game về ban đầu
void resetGame(int& playerX, int& playerY, vector<Enemy>& enemies, vector<Projectile>& projectiles,
    vector<HealthItem>& healthItems, vector<EnemyRespawn>& enemyRespawns) {
    // Đặt lại vị trí và trạng thái người chơi
    playerX = 100;
    playerY = 300;
    playerHealth = 10;
    score = 0;
    gameOver = false;
    gameStarted = false;
    isPaused = false;
    isAttacking = false;
    isInvisible = false;
    isShooting = false;
    isDashing = false;
    enemyHitCounter = 0;
    currentDirection = 1;
    attackStartTime = 0;
    lastAttackTime = 0;
    invisStartTime = 0;
    lastInvisTime = 0;
    shootStartTime = 0;
    dashStartTime = 0;
    lastShootTime = 0;
    lastDashTime = 0;
    lastSpawnTime = 0;
    lastHealthSpawnTime = 0;

    // Xóa danh sách các đối tượng
    projectiles.clear();
    healthItems.clear();
    enemies.clear();
    enemyRespawns.clear();

    // Sinh lại 5 kẻ địch ban đầu
    const int NUM_ENEMIES = 5;
    for (int i = 0; i < NUM_ENEMIES; i++) {
        spawnRandomEnemy(enemies);
    }
}

// Xử lý sự kiện từ bàn phím
void handleEvents(bool& running, int& playerX, int& playerY) {
    SDL_Event e;
    // Lặp qua tất cả các sự kiện đang chờ xử lý
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false; // Thoát game nếu nhấn nút đóng cửa sổ
        else if (e.type == SDL_KEYDOWN) { // Khi có phím được nhấn
            if (!gameStarted) { // Nếu game chưa bắt đầu
                if (e.key.keysym.sym == SDLK_SPACE) { // Nhấn Space để bắt đầu
                    gameStarted = true;
                    Mix_PlayChannel(-1, accept1Sound, 0); // Phát âm thanh xác nhận
                    Mix_PlayMusic(backgroundMusic, -1); // Phát nhạc nền
                }
            }
            else if (gameOver) { // Nếu game đã kết thúc
                if (e.key.keysym.sym == SDLK_r) { // Nhấn R để chơi lại
                    Mix_PlayChannel(-1, accept1Sound, 0);
                    resetGame(playerX, playerY, enemies, projectiles, healthItems, enemyRespawns);
                    Mix_PlayMusic(backgroundMusic, -1);
                }
                else if (e.key.keysym.sym == SDLK_q) running = false; // Nhấn Q để thoát
            }
            else if (e.key.keysym.sym == SDLK_p && !gameOver) { // Nhấn P để tạm dừng/tiếp tục
                if (!isPaused) Mix_PlayChannel(-1, accept2Sound, 0); // Phát âm thanh khi tạm dừng
                else Mix_PlayChannel(-1, accept1Sound, 0); // Phát âm thanh khi tiếp tục
                isPaused = !isPaused; // Đổi trạng thái tạm dừng
            }
        }
    }

    // Nếu game chưa bắt đầu, đang tạm dừng hoặc đã kết thúc, không xử lý tiếp
    if (!gameStarted || isPaused || gameOver) return;

    // Lấy trạng thái bàn phím hiện tại
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại

    // Xử lý tàng hình (phím I)
    if (keyState[SDL_SCANCODE_I] && !isInvisible && currentTime - lastInvisTime >= INVIS_COOLDOWN) {
        isInvisible = true;
        invisStartTime = currentTime;
        lastInvisTime = currentTime;
        Mix_PlayChannel(-1, invisSound, 0);
    }
    // Kết thúc tàng hình sau thời gian quy định
    if (isInvisible && currentTime - invisStartTime >= INVIS_DURATION) isInvisible = false;

    // Tốc độ di chuyển thay đổi khi tàng hình
    int speed = isInvisible ? PLAYER_SPEED_INVIS : PLAYER_SPEED;
    int dashDistance = isInvisible ? DASH_DISTANCE * 3 / 10 : DASH_DISTANCE;

    // Di chuyển người chơi khi không tấn công
    if (!isAttacking) {
        bool left = keyState[SDL_SCANCODE_A];
        bool right = keyState[SDL_SCANCODE_D];
        bool up = keyState[SDL_SCANCODE_W];
        bool down = keyState[SDL_SCANCODE_S];

        // Di chuyển theo các hướng
        if (left && !right) playerX -= speed;
        else if (right && !left) playerX += speed;
        if (up && !down) playerY -= speed;
        else if (down && !up) playerY += speed;

        // Xác định hướng của người chơi (0-7)
        if (left && !right) {
            if (up && !down) currentDirection = 4; // Trái lên
            else if (down && !up) currentDirection = 6; // Trái xuống
            else currentDirection = 0; // Trái
        }
        else if (right && !left) {
            if (up && !down) currentDirection = 5; // Phải lên
            else if (down && !up) currentDirection = 7; // Phải xuống
            else currentDirection = 1; // Phải
        }
        else if (up && !down) currentDirection = 2; // Lên
        else if (down && !up) currentDirection = 3; // Xuống

        // Giới hạn người chơi trong màn hình
        if (playerX < 0) playerX = 0;
        if (playerX + PLAYER_WIDTH > SCREEN_WIDTH) playerX = SCREEN_WIDTH - PLAYER_WIDTH;
        if (playerY < 0) playerY = 0;
        if (playerY + PLAYER_HEIGHT > SCREEN_HEIGHT) playerY = SCREEN_HEIGHT - PLAYER_HEIGHT;
    }

    // Tấn công cận chiến (phím J)
    if (keyState[SDL_SCANCODE_J] && !isAttacking && currentTime - lastAttackTime > ATTACK_COOLDOWN) {
        isAttacking = true;
        attackStartTime = currentTime;
        lastAttackTime = currentTime;
        Mix_PlayChannel(-1, hitSound, 0);
    }

    // Bắn phi tiêu (phím K, không bắn khi tàng hình)
    if (!isInvisible && keyState[SDL_SCANCODE_K] && currentTime - lastShootTime > SHOOT_COOLDOWN) {
        lastShootTime = currentTime;
        projectiles.push_back({ playerX + PLAYER_WIDTH / 2, playerY + PLAYER_HEIGHT / 2, currentDirection });
        isShooting = true;
        shootStartTime = currentTime;
        Mix_PlayChannel(-1, hitSound, 0);
    }

    // Dịch chuyển (phím L)
    if (keyState[SDL_SCANCODE_L] && (isInvisible || currentTime - lastDashTime >= DASH_COOLDOWN)) {
        lastDashTime = currentTime;
        isDashing = true;
        dashStartTime = currentTime;
        Mix_PlayChannel(-1, jumpSound, 0);
        // Di chuyển theo hướng hiện tại
        switch (currentDirection) {
        case 0: playerX -= dashDistance; break; // Trái
        case 1: playerX += dashDistance; break; // Phải
        case 2: playerY -= dashDistance; break; // Lên
        case 3: playerY += dashDistance; break; // Xuống
        case 4: playerX -= dashDistance / 2; playerY -= dashDistance / 2; break; // Trái lên
        case 5: playerX += dashDistance / 2; playerY -= dashDistance / 2; break; // Phải lên
        case 6: playerX -= dashDistance / 2; playerY += dashDistance / 2; break; // Trái xuống
        case 7: playerX += dashDistance / 2; playerY += dashDistance / 2; break; // Phải xuống
        }
        // Giới hạn người chơi trong màn hình sau khi dịch chuyển
        if (playerX < 0) playerX = 0;
        if (playerX + PLAYER_WIDTH > SCREEN_WIDTH) playerX = SCREEN_WIDTH - PLAYER_WIDTH;
        if (playerY < 0) playerY = 0;
        if (playerY + PLAYER_HEIGHT > SCREEN_HEIGHT) playerY = SCREEN_HEIGHT - PLAYER_HEIGHT;
    }
}

// Cập nhật trạng thái của phi tiêu
void updateProjectiles() {
    const int MAX_DISTANCE = 10 * PLAYER_WIDTH; // Khoảng cách tối đa phi tiêu bay được

    // Cập nhật vị trí của từng phi tiêu
    for (auto& proj : projectiles) {
        if (!proj.active) continue; // Bỏ qua nếu phi tiêu không hoạt động

        // Di chuyển phi tiêu theo hướng
        switch (proj.dir) {
        case 0: proj.x -= proj.speed; break; // Trái
        case 1: proj.x += proj.speed; break; // Phải
        case 2: proj.y -= proj.speed; break; // Lên
        case 3: proj.y += proj.speed; break; // Xuống
        case 4: proj.x -= proj.speed / 2; proj.y -= proj.speed / 2; break; // Trái lên
        case 5: proj.x += proj.speed / 2; proj.y -= proj.speed / 2; break; // Phải lên
        case 6: proj.x -= proj.speed / 2; proj.y += proj.speed / 2; break; // Trái xuống
        case 7: proj.x += proj.speed / 2; proj.y += proj.speed / 2; break; // Phải xuống
        }

        proj.distanceTraveled += proj.speed; // Cập nhật khoảng cách đã bay

        // Vô hiệu hóa phi tiêu nếu vượt quá khoảng cách tối đa hoặc ra ngoài màn hình
        if (proj.distanceTraveled >= MAX_DISTANCE) proj.active = false;
        if (proj.x < 0 || proj.x > SCREEN_WIDTH || proj.y < 0 || proj.y > SCREEN_HEIGHT) proj.active = false;
    }

    // Xóa các phi tiêu không còn hoạt động
    projectiles.erase(remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) { return !p.active; }), projectiles.end());
}

// Cập nhật trạng thái của kẻ địch
void updateEnemies(int playerX, int playerY) {
    Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại
    const int DETECTION_RANGE = 300; // Phạm vi phát hiện người chơi
    const int ATTACK_RADIUS = 62; // Bán kính tấn công khi tàng hình
    const int ENEMY_ATTACK_COOLDOWN = 2000; // Thời gian chờ giữa các lần tấn công của kẻ địch
    const int ENEMY_ATTACK_DURATION = 300; // Thời gian hoạt hình tấn công của kẻ địch

    vector<size_t> deadEnemies; // Danh sách chỉ số của kẻ địch đã chết

    // Lặp qua từng kẻ địch
    for (size_t i = 0; i < enemies.size(); i++) {
        auto& enemy = enemies[i];

        // Xử lý kẻ địch đang chết
        if (enemy.isDying) {
            // Cập nhật frame hoạt hình chết
            if (currentTime - enemy.lastDeathFrameTime >= ENEMY_DEATH_FRAME_DURATION) {
                enemy.currentDeathFrame = (enemy.currentDeathFrame + 1) % ENEMY_DEATH_TOTAL_FRAMES;
                enemy.lastDeathFrameTime = currentTime;
            }
            // Thêm vào danh sách chết nếu hoạt hình kết thúc
            if (currentTime - enemy.deathStartTime >= ENEMY_DEATH_ANIMATION_DURATION) {
                deadEnemies.push_back(i);
            }
            continue;
        }

        // Xử lý kẻ địch đang xuất hiện
        if (enemy.isSpawning) {
            // Cập nhật frame hoạt hình xuất hiện
            if (currentTime - enemy.lastSpawnFrameTime >= ENEMY_SPAWN_FRAME_DURATION) {
                enemy.currentSpawnFrame = (enemy.currentSpawnFrame + 1) % ENEMY_SPAWN_TOTAL_FRAMES;
                enemy.lastSpawnFrameTime = currentTime;
            }
            // Kết thúc xuất hiện khi hoạt hình hoàn tất
            if (currentTime - enemy.spawnStartTime >= ENEMY_SPAWN_ANIMATION_DURATION) {
                enemy.isSpawning = false;
            }
            continue;
        }

        // Tính khoảng cách đến người chơi
        int dx = playerX - enemy.x;
        int dy = playerY - enemy.y;
        int distance = static_cast<int>(sqrt(static_cast<double>(dx * dx + dy * dy)));

        // Kẻ địch phát hiện người chơi nếu không tàng hình và trong phạm vi
        if (!isInvisible) enemy.alert = (distance < DETECTION_RANGE);
        else enemy.alert = false;

        // Cập nhật hướng di chuyển ngẫu nhiên nếu không tấn công
        if (!enemy.isAttacking && currentTime - enemy.lastMoveTime > 500) {
            enemy.lastMoveTime = currentTime;
            enemy.direction = rand() % 4; // Chọn hướng ngẫu nhiên (0-3)
        }

        // Xử lý hành vi của kẻ địch
        if (enemy.alert) { // Nếu phát hiện người chơi
            if (!enemy.isAttacking) {
                // Di chuyển về phía người chơi
                if (enemy.x < playerX) enemy.x += enemy.speed;
                if (enemy.x > playerX) enemy.x -= enemy.speed;
                if (enemy.y < playerY) enemy.y += enemy.speed;
                if (enemy.y > playerY) enemy.y -= enemy.speed;

                // Xác định hướng của kẻ địch dựa trên vị trí người chơi
                if (abs(dx) > abs(dy)) enemy.direction = (dx < 0) ? 0 : 1;
                else enemy.direction = (dy < 0) ? 2 : 3;
            }

            // Tấn công nếu đủ gần và đã qua thời gian cooldown
            if (!enemy.isAttacking && currentTime - enemy.lastAttackTime > ENEMY_ATTACK_COOLDOWN) {
                if (distance < 100) {
                    enemy.isAttacking = true;
                    enemy.attackStartTime = currentTime;
                    enemy.lastAttackTime = currentTime;
                    Mix_PlayChannel(-1, hitSound, 0);
                    // Xác định hướng tấn công
                    if (dx == 0 && dy == 0) enemy.attackDirection = enemy.direction;
                    else if (abs(dx) > abs(dy)) enemy.attackDirection = (dx < 0) ? 0 : 1;
                    else enemy.attackDirection = (dy < 0) ? 2 : 3;
                    enemy.direction = enemy.attackDirection;

                    // Tạo vùng tấn công của kẻ địch
                    SDL_Rect sword;
                    switch (enemy.attackDirection) {
                    case 0: sword = { enemy.x - 48, enemy.y, 48, enemy.height }; break; // Trái
                    case 1: sword = { enemy.x + enemy.width, enemy.y, 48, enemy.height }; break; // Phải
                    case 2: sword = { enemy.x, enemy.y - 48, enemy.width, 48 }; break; // Lên
                    case 3: sword = { enemy.x, enemy.y + enemy.height, enemy.width, 48 }; break; // Xuống
                    }
                    SDL_Rect playerRect = { playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT };
                    // Kiểm tra va chạm với người chơi
                    if (checkCollision(sword, playerRect) || (dx == 0 && dy == 0)) {
                        enemyHitCounter++;
                        if (enemyHitCounter == 2) { // Mất máu khi bị đánh
                            if (playerHealth > 0) {
                                playerHealth--;
                                Mix_PlayChannel(-1, killSound, 0);
                                if (playerHealth <= 0) {
                                    gameOver = true;
                                    Mix_PlayChannel(-1, gameOverSound, 0);
                                }
                            }
                            enemyHitCounter = 0;
                        }
                    }
                }
            }
        }
        else { // Nếu không phát hiện người chơi
            if (!enemy.isAttacking) {
                // Di chuyển ngẫu nhiên
                switch (enemy.direction) {
                case 0: enemy.x -= enemy.speed; break; // Trái
                case 1: enemy.x += enemy.speed; break; // Phải
                case 2: enemy.y -= enemy.speed; break; // Lên
                case 3: enemy.y += enemy.speed; break; // Xuống
                }
            }
            // Giới hạn kẻ địch trong màn hình
            if (enemy.x < 0) enemy.x = 0;
            if (enemy.x + enemy.width > SCREEN_WIDTH) enemy.x = SCREEN_WIDTH - enemy.width;
            if (enemy.y < 0) enemy.y = 0;
            if (enemy.y + enemy.height > SCREEN_HEIGHT) enemy.y = SCREEN_HEIGHT - enemy.height;
        }

        // Kết thúc tấn công sau thời gian quy định
        if (enemy.isAttacking && currentTime - enemy.attackStartTime > ENEMY_ATTACK_DURATION) {
            enemy.isAttacking = false;
        }

        // Kiểm tra va chạm với người chơi hoặc phi tiêu
        SDL_Rect enemyRect = { enemy.x, enemy.y, enemy.width, enemy.height };
        if (isAttacking) {
            if (isInvisible) { // Tấn công vòng tròn khi tàng hình
                int centerX = playerX + PLAYER_WIDTH / 2;
                int centerY = playerY + PLAYER_HEIGHT / 2;
                int dx = (enemy.x + enemy.width / 2) - centerX;
                int dy = (enemy.y + enemy.height / 2) - centerY;
                int distance = static_cast<int>(sqrt(static_cast<double>(dx * dx + dy * dy)));
                if (distance <= ATTACK_RADIUS + enemy.width / 2) {
                    enemy.isDying = true;
                    enemy.deathStartTime = currentTime;
                    enemy.lastDeathFrameTime = currentTime;
                    enemy.currentDeathFrame = 0;
                    score++;
                    Mix_PlayChannel(-1, killSound, 0);
                }
            }
            else { // Tấn công cận chiến thông thường
                SDL_Rect sword;
                switch (currentDirection) {
                case 0: sword = { playerX - 48, playerY, 48, PLAYER_HEIGHT }; break; // Trái
                case 1: sword = { playerX + PLAYER_WIDTH, playerY, 48, PLAYER_HEIGHT }; break; // Phải
                case 2: sword = { playerX, playerY - 48, PLAYER_WIDTH, 48 }; break; // Lên
                case 3: sword = { playerX, playerY + PLAYER_HEIGHT, PLAYER_WIDTH, 48 }; break; // Xuống
                case 4: sword = { playerX - 32, playerY - 32, 48, 48 }; break; // Trái lên
                case 5: sword = { playerX + PLAYER_WIDTH, playerY - 32, 48, 48 }; break; // Phải lên
                case 6: sword = { playerX - 32, playerY + PLAYER_HEIGHT, 48, 48 }; break; // Trái xuống
                case 7: sword = { playerX + PLAYER_WIDTH, playerY + PLAYER_HEIGHT, 48, 48 }; break; // Phải xuống
                }
                if (checkCollision(sword, enemyRect)) {
                    enemy.isDying = true;
                    enemy.deathStartTime = currentTime;
                    enemy.lastDeathFrameTime = currentTime;
                    enemy.currentDeathFrame = 0;
                    score++;
                    Mix_PlayChannel(-1, killSound, 0);
                }
            }
        }

        // Kiểm tra va chạm với phi tiêu
        for (auto& proj : projectiles) {
            if (proj.active) {
                SDL_Rect bullet = { proj.x, proj.y, 22, 22 };
                if (checkCollision(bullet, enemyRect)) {
                    proj.active = false;
                    enemy.isDying = true;
                    enemy.deathStartTime = currentTime;
                    enemy.lastDeathFrameTime = currentTime;
                    enemy.currentDeathFrame = 0;
                    score++;
                    Mix_PlayChannel(-1, killSound, 0);
                }
            }
        }
    }

    // Xử lý vật phẩm hồi máu
    SDL_Rect playerRect = { playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT };
    for (auto& item : healthItems) {
        if (item.active) {
            SDL_Rect itemRect = { item.x, item.y, item.width, item.height };
            // Nhặt vật phẩm nếu va chạm
            if (checkCollision(playerRect, itemRect)) {
                if (playerHealth < 10) {
                    playerHealth = min(playerHealth + 3, 10); // Hồi 3 máu, tối đa 10
                }
                item.active = false;
            }
            // Xóa vật phẩm nếu hết thời gian tồn tại
            else if (currentTime - item.spawnTime >= HEALTH_ITEM_DURATION) {
                item.active = false;
            }
        }
    }
    // Xóa các vật phẩm không còn hoạt động
    healthItems.erase(remove_if(healthItems.begin(), healthItems.end(), [](const HealthItem& item) { return !item.active; }), healthItems.end());

    // Xử lý kẻ địch chết và tái sinh
    if (!deadEnemies.empty()) {
        enemyRespawns.push_back(EnemyRespawn(currentTime, static_cast<int>(deadEnemies.size())));
        for (auto it = deadEnemies.rbegin(); it != deadEnemies.rend(); ++it) {
            enemies.erase(enemies.begin() + *it);
        }
    }
}

// Xử lý sự kiện khi game over (không dùng trong mã hiện tại)
void handleGameOverEvents(bool& running, int& playerX, int& playerY, vector<Enemy>& enemies,
    vector<Projectile>& projectiles, vector<HealthItem>& healthItems,
    vector<EnemyRespawn>& enemyRespawns) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_r: // Chơi lại
                Mix_PlayChannel(-1, accept1Sound, 0);
                Mix_HaltChannel(-1);
                resetGame(playerX, playerY, enemies, projectiles, healthItems, enemyRespawns);
                break;
            case SDLK_q: // Thoát
                Mix_PlayChannel(-1, accept2Sound, 0);
                Mix_HaltChannel(-1);
                running = false;
                break;
            }
        }
    }
}

// Vẽ giao diện game
void render(SDL_Renderer* renderer, int playerX, int playerY) {
    // Xóa màn hình bằng màu đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dừng nhạc nền nếu game over
    if (gameOver && Mix_PlayingMusic()) Mix_HaltMusic();

    SDL_Color textColor = { 255, 255, 255, 255 }; // Màu trắng cho văn bản

    // Vẽ màn hình bắt đầu nếu game chưa khởi động
    if (!gameStarted) {
        SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

        const int GAME_START_WIDTH = 640;
        const int GAME_START_HEIGHT = 288;
        int gameStartX = (SCREEN_WIDTH - GAME_START_WIDTH) / 2;
        int gameStartY = (SCREEN_HEIGHT - GAME_START_HEIGHT) / 2;
        SDL_Rect gameStartRect = { gameStartX, gameStartY, GAME_START_WIDTH, GAME_START_HEIGHT };
        SDL_RenderCopy(renderer, gameStartTexture, nullptr, &gameStartRect);

        startTextRect.x = (SCREEN_WIDTH - startTextRect.w) / 2;
        startTextRect.y = gameStartY + GAME_START_HEIGHT + 20;
        SDL_RenderCopy(renderer, startTextTexture, nullptr, &startTextRect);

        SDL_RenderPresent(renderer);
        return;
    }

    // Vẽ nền game
    SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

    // Nếu game chưa kết thúc, vẽ giao diện chơi
    if (!gameOver) {
        // Vẽ thanh máu
        const int HEALTH_BAR_WIDTH = 200;
        const int HEALTH_BAR_HEIGHT = 20;
        const int HEALTH_UNIT_WIDTH = HEALTH_BAR_WIDTH / 10;
        SDL_Rect healthBarOutline = { 10, 10, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ cho viền
        SDL_RenderDrawRect(renderer, &healthBarOutline);

        // Vẽ các biểu tượng trái tim
        for (int i = 0; i < playerHealth; i++) {
            SDL_Rect heartRect = { 10 + i * HEALTH_UNIT_WIDTH, 10, HEALTH_UNIT_WIDTH - 2, HEALTH_BAR_HEIGHT - 2 };
            SDL_RenderCopy(renderer, heartIconTexture, nullptr, &heartRect);
        }

        // Vẽ điểm số
        string scoreText = "Diem   cua   ban:   " + to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(gameFont, scoreText.c_str(), textColor);
        if (scoreSurface) {
            if (scoreTextTexture) SDL_DestroyTexture(scoreTextTexture);
            scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            scoreTextRect.w = scoreSurface->w;
            scoreTextRect.h = scoreSurface->h;
            scoreTextRect.x = 10;
            scoreTextRect.y = 10 + HEALTH_BAR_HEIGHT + 5;
            SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreTextRect);
            SDL_FreeSurface(scoreSurface);
        }

        // Cập nhật frame hiệu ứng tấn công
        if (isAttacking) {
            Uint32 elapsedTime = SDL_GetTicks() - attackStartTime;
            currentHitFrame = (elapsedTime / (ATTACK_DURATION / HIT_TOTAL_FRAMES)) % HIT_TOTAL_FRAMES;
        }
        else currentHitFrame = 0;

        // Vẽ hiệu ứng tấn công
        if (isAttacking) {
            if (isInvisible) { // Hiệu ứng vòng tròn khi tàng hình
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastCircleHitFrameTime >= (ATTACK_DURATION / CIRCLE_HIT_TOTAL_FRAMES)) {
                    currentCircleHitFrame = (currentCircleHitFrame + 1) % CIRCLE_HIT_TOTAL_FRAMES;
                    lastCircleHitFrameTime = currentTime;
                }

                int centerX = playerX + PLAYER_WIDTH / 2;
                int centerY = playerY + PLAYER_HEIGHT / 2;
                int circleHitX = centerX - CIRCLE_HIT_FRAME_WIDTH / 2;
                int circleHitY = centerY - CIRCLE_HIT_FRAME_HEIGHT / 2;
                SDL_Rect circleHitRect = { circleHitX, circleHitY, CIRCLE_HIT_FRAME_WIDTH, CIRCLE_HIT_FRAME_HEIGHT };
                SDL_RenderCopyEx(renderer, circleHitSpriteSheet, &circleHitFrames[currentCircleHitFrame], &circleHitRect, 0, nullptr, SDL_FLIP_NONE);
            }
            else { // Hiệu ứng kiếm thông thường
                int hitX = playerX, hitY = playerY;
                double angle = 0;
                switch (currentDirection) {
                case 0: hitX = playerX - HIT_FRAME_WIDTH; angle = 180; break; // Trái
                case 1: hitX = playerX + PLAYER_WIDTH; angle = 0; break; // Phải
                case 2: hitY = playerY - HIT_FRAME_HEIGHT; angle = 270; break; // Lên
                case 3: hitY = playerY + PLAYER_HEIGHT; angle = 90; break; // Xuống
                default: hitX = playerX + PLAYER_WIDTH; angle = 0; break;
                }
                SDL_Rect hitRect = { hitX, hitY, HIT_FRAME_WIDTH, HIT_FRAME_HEIGHT };
                SDL_Point center = { HIT_FRAME_WIDTH / 2, HIT_FRAME_HEIGHT / 2 };
                SDL_RenderCopyEx(renderer, hitSpriteSheet, &hitFrames[currentHitFrame], &hitRect, angle, &center, SDL_FLIP_NONE);
            }
            // Kết thúc tấn công sau thời gian quy định
            if (SDL_GetTicks() - attackStartTime > ATTACK_DURATION) isAttacking = false;
        }

        // Kết thúc hoạt hình bắn và dịch chuyển
        if (isShooting && SDL_GetTicks() - shootStartTime > SHOOT_ANIMATION_DURATION) isShooting = false;
        if (isDashing && SDL_GetTicks() - dashStartTime > DASH_ANIMATION_DURATION) isDashing = false;

        // Cập nhật frame hiệu ứng dịch chuyển
        if (isDashing) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastTrailFrameTime >= TRAIL_FRAME_DURATION) {
                currentTrailFrame = (currentTrailFrame + 1) % TRAIL_TOTAL_FRAMES;
                lastTrailFrameTime = currentTime;
            }
        }
        else {
            currentTrailFrame = 0;
            lastTrailFrameTime = 0;
        }

        // Cập nhật frame và góc xoay của phi tiêu
        if (!projectiles.empty()) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastShurikenFrameTime >= SHURIKEN_FRAME_DURATION) {
                currentShurikenFrame = (currentShurikenFrame + 1) % SHURIKEN_TOTAL_FRAMES;
                lastShurikenFrameTime = currentTime;
            }
            shurikenRotationAngle += SHURIKEN_ROTATION_SPEED;
            if (shurikenRotationAngle >= 360) shurikenRotationAngle -= 360;
        }
        else {
            currentShurikenFrame = 0;
            lastShurikenFrameTime = 0;
            shurikenRotationAngle = 0;
        }

        // Vẽ hiệu ứng dịch chuyển
        if (isDashing) {
            int trailX = playerX, trailY = playerY;
            double angle = 0;
            switch (currentDirection) {
            case 0: case 4: case 6: trailX = playerX + PLAYER_WIDTH; angle = 0; break; // Trái
            case 1: case 5: case 7: trailX = playerX - static_cast<int>(TRAIL_FRAME_WIDTH * 1.6); angle = 180; break; // Phải
            case 2: trailY = playerY + PLAYER_HEIGHT; angle = 270; break; // Lên
            case 3: trailY = playerY - static_cast<int>(TRAIL_FRAME_HEIGHT * 1.6); angle = 90; break; // Xuống
            }
            SDL_Rect trailRect = { trailX, trailY, static_cast<int>(TRAIL_FRAME_WIDTH * 1.6), static_cast<int>(TRAIL_FRAME_HEIGHT * 1.6) };
            SDL_Point center = { static_cast<int>(TRAIL_FRAME_WIDTH * 1.6) / 2, static_cast<int>(TRAIL_FRAME_HEIGHT * 1.6) / 2 };
            SDL_RenderCopyEx(renderer, trailSpriteSheet, &trailFrames[currentTrailFrame], &trailRect, angle, &center, SDL_FLIP_NONE);
        }

        // Vẽ nhân vật chính
        SDL_Rect playerRect = { playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT };
        int frameIndex;
        switch (currentDirection) {
        case 0: case 4: case 6: frameIndex = 2; break; // Trái
        case 1: case 5: case 7: frameIndex = 3; break; // Phải
        case 2: frameIndex = 1; break; // Lên
        case 3: frameIndex = 0; break; // Xuống
        default: frameIndex = 0; break;
        }

        // Chọn sprite sheet và frame phù hợp
        SDL_Texture* currentSpriteSheet;
        SDL_Rect* currentFrames;
        if (isAttacking) {
            currentSpriteSheet = isInvisible ? invisAttackSpriteSheet : playerMeleeSpriteSheet;
            currentFrames = isInvisible ? invisAttackFrames : playerMeleeFrames;
        }
        else if (isDashing) {
            currentSpriteSheet = isInvisible ? invisJumpSpriteSheet : dashSpriteSheet;
            currentFrames = isInvisible ? invisJumpFrames : dashFrames;
        }
        else if (isShooting) {
            currentSpriteSheet = shootSpriteSheet;
            currentFrames = shootFrames;
        }
        else {
            currentSpriteSheet = isInvisible ? invisSpriteSheet : playerSpriteSheet;
            currentFrames = isInvisible ? invisFrames : playerFrames;
        }

        // Đặt độ trong suốt khi tàng hình
        SDL_SetTextureAlphaMod(currentSpriteSheet, isInvisible ? 128 : 255);
        SDL_RenderCopyEx(renderer, currentSpriteSheet, &currentFrames[frameIndex], &playerRect, 0, nullptr, SDL_FLIP_NONE);

        // Vẽ phi tiêu
        for (const auto& proj : projectiles) {
            if (proj.active) {
                int shurikenX = proj.x - (static_cast<int>(SHURIKEN_FRAME_WIDTH * 1.6) - 22) / 2;
                int shurikenY = proj.y - (static_cast<int>(SHURIKEN_FRAME_HEIGHT * 1.6) - 22) / 2;
                SDL_Rect shurikenRect = { shurikenX, shurikenY, static_cast<int>(SHURIKEN_FRAME_WIDTH * 1.6), static_cast<int>(SHURIKEN_FRAME_HEIGHT * 1.6) };
                SDL_Point center = { static_cast<int>(SHURIKEN_FRAME_WIDTH * 1.6) / 2, static_cast<int>(SHURIKEN_FRAME_HEIGHT * 1.6) / 2 };
                SDL_RenderCopyEx(renderer, shurikenSpriteSheet, &shurikenFrames[currentShurikenFrame], &shurikenRect, shurikenRotationAngle, &center, SDL_FLIP_NONE);
            }
        }

        // Cập nhật frame vật phẩm hồi máu
        if (!healthItems.empty()) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastHeartItemFrameTime >= HEART_ITEM_FRAME_DURATION) {
                currentHeartItemFrame = (currentHeartItemFrame + 1) % HEART_ITEM_TOTAL_FRAMES;
                lastHeartItemFrameTime = currentTime;
            }
        }
        else {
            currentHeartItemFrame = 0;
            lastHeartItemFrameTime = 0;
        }

        // Vẽ vật phẩm hồi máu
        for (const auto& item : healthItems) {
            if (item.active) {
                SDL_Rect itemRect = { item.x, item.y, item.width, item.height };
                SDL_RenderCopy(renderer, heartItemTexture, &heartItemFrames[currentHeartItemFrame], &itemRect);
            }
        }

        // Vẽ kẻ địch
        for (const auto& enemy : enemies) {
            SDL_Rect enemyRect = { enemy.x, enemy.y, enemy.width, enemy.height };
            int enemyFrameIndex;
            switch (enemy.direction) {
            case 0: enemyFrameIndex = 2; break; // Trái
            case 1: enemyFrameIndex = 3; break; // Phải
            case 2: enemyFrameIndex = 1; break; // Lên
            case 3: enemyFrameIndex = 0; break; // Xuống
            default: enemyFrameIndex = 0; break;
            }

            if (enemy.isDying) { // Vẽ hoạt hình chết
                SDL_RenderCopyEx(renderer, enemyDeathSpriteSheet, &enemyDeathFrames[enemy.currentDeathFrame], &enemyRect, 0, nullptr, SDL_FLIP_NONE);
            }
            else if (enemy.isSpawning) { // Vẽ hoạt hình xuất hiện
                SDL_RenderCopyEx(renderer, enemySpawnSpriteSheet, &enemySpawnFrames[enemy.currentSpawnFrame], &enemyRect, 0, nullptr, SDL_FLIP_NONE);
            }
            else if (enemy.isAttacking) { // Vẽ hoạt hình tấn công
                SDL_RenderCopyEx(renderer, enemyMeleeSpriteSheet, &enemyMeleeFrames[enemyFrameIndex], &enemyRect, 0, nullptr, SDL_FLIP_NONE);

                int hitX = enemy.x, hitY = enemy.y;
                double angle = 0;
                Uint32 enemyElapsedTime = SDL_GetTicks() - enemy.attackStartTime;
                int enemyHitFrame = (enemyElapsedTime / (ATTACK_DURATION / HIT_TOTAL_FRAMES)) % HIT_TOTAL_FRAMES;

                switch (enemy.direction) {
                case 0: hitX = enemy.x - HIT_FRAME_WIDTH; angle = 180; break; // Trái
                case 1: hitX = enemy.x + enemy.width; angle = 0; break; // Phải
                case 2: hitY = enemy.y - HIT_FRAME_HEIGHT; angle = 270; break; // Lên
                case 3: hitY = enemy.y + enemy.height; angle = 90; break; // Xuống
                }

                SDL_Rect hitRect = { hitX, hitY, HIT_FRAME_WIDTH, HIT_FRAME_HEIGHT };
                SDL_Point center = { HIT_FRAME_WIDTH / 2, HIT_FRAME_HEIGHT / 2 };
                SDL_RenderCopyEx(renderer, hitSpriteSheet, &hitFrames[enemyHitFrame], &hitRect, angle, &center, SDL_FLIP_NONE);
            }
            else { // Vẽ trạng thái bình thường
                SDL_RenderCopyEx(renderer, enemySpriteSheet, &enemyFrames[enemyFrameIndex], &enemyRect, 0, nullptr, SDL_FLIP_NONE);
            }
        }

        // Vẽ màn hình tạm dừng
        if (isPaused && gameStarted && !gameOver) {
            const int PAUSE_WIDTH = 624;
            const int PAUSE_HEIGHT = 288;
            int pauseX = (SCREEN_WIDTH - PAUSE_WIDTH) / 2;
            int pauseY = (SCREEN_HEIGHT - PAUSE_HEIGHT) / 2;
            pauseRect = { pauseX, pauseY, PAUSE_WIDTH, PAUSE_HEIGHT };

            SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);

            pauseTextRect.x = (SCREEN_WIDTH - pauseTextRect.w) / 2;
            pauseTextRect.y = pauseY + PAUSE_HEIGHT + 20;
            SDL_RenderCopy(renderer, pauseTextTexture, nullptr, &pauseTextRect);
        }
    }
    else { // Vẽ màn hình game over
        const int GAME_OVER_WIDTH = 512;
        const int GAME_OVER_HEIGHT = 288;
        int gameOverX = (SCREEN_WIDTH - GAME_OVER_WIDTH) / 2;
        int gameOverY = (SCREEN_HEIGHT - GAME_OVER_HEIGHT) / 2;
        SDL_Rect gameOverRect = { gameOverX, gameOverY, GAME_OVER_WIDTH, GAME_OVER_HEIGHT };
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

        // Vẽ điểm số cuối cùng
        string scoreText = "Diem   so   cua   ban:   " + to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(gameFont, scoreText.c_str(), textColor);
        if (scoreSurface) {
            if (scoreTextTexture) SDL_DestroyTexture(scoreTextTexture);
            scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            scoreTextRect.w = scoreSurface->w;
            scoreTextRect.h = scoreSurface->h;
            scoreTextRect.x = (SCREEN_WIDTH - scoreTextRect.w) / 2;
            scoreTextRect.y = gameOverY + GAME_OVER_HEIGHT + 20;
            SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreTextRect);
            SDL_FreeSurface(scoreSurface);
        }

        // Vẽ hướng dẫn chơi lại hoặc thoát
        string instructionText = "Nhan   R   de   choi   lai   hoac   Q   de   thoat";
        SDL_Surface* instructionSurface = TTF_RenderText_Solid(gameFont, instructionText.c_str(), textColor);
        if (instructionSurface) {
            if (restartTextTexture) SDL_DestroyTexture(restartTextTexture);
            restartTextTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
            restartTextRect.w = instructionSurface->w;
            restartTextRect.h = instructionSurface->h;
            restartTextRect.x = (SCREEN_WIDTH - restartTextRect.w) / 2;
            restartTextRect.y = scoreTextRect.y + scoreTextRect.h + 10;
            SDL_RenderCopy(renderer, restartTextTexture, nullptr, &restartTextRect);
            SDL_FreeSurface(instructionSurface);
        }
    }

    // Hiển thị khung hình lên màn hình
    SDL_RenderPresent(renderer);
}

// Hàm chính
int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr))); // Khởi tạo hạt giống ngẫu nhiên

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Khởi tạo game, thoát nếu thất bại
    if (!init(window, renderer)) return 1;

    bool running = true; // Cờ điều khiển vòng lặp game
    while (running) {
        Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại

        // Nếu game chưa bắt đầu, chỉ xử lý sự kiện và vẽ màn hình bắt đầu
        if (!gameStarted) {
            handleEvents(running, playerX, playerY);
            render(renderer, playerX, playerY);
            SDL_Delay(16); // Giới hạn 60 FPS
            continue;
        }

        // Xử lý sự kiện
        handleEvents(running, playerX, playerY);

        // Cập nhật trạng thái game nếu không tạm dừng và chưa kết thúc
        if (!gameOver && !isPaused) {
            updateProjectiles(); // Cập nhật phi tiêu
            updateEnemies(playerX, playerY); // Cập nhật kẻ địch

            // Sinh kẻ địch mới theo thời gian
            if (currentTime - lastSpawnTime >= SPAWN_INTERVAL) {
                spawnRandomEnemy(enemies);
                lastSpawnTime = currentTime;
            }

            // Sinh vật phẩm hồi máu theo thời gian
            if (currentTime - lastHealthSpawnTime >= HEALTH_SPAWN_INTERVAL) {
                spawnRandomHealthItem(healthItems);
                lastHealthSpawnTime = currentTime;
            }

            // Xử lý tái sinh kẻ địch
            for (auto it = enemyRespawns.begin(); it != enemyRespawns.end();) {
                if (currentTime - it->deathTime >= ENEMY_RESPAWN_DELAY) {
                    for (int i = 0; i < it->count; i++) spawnRandomEnemy(enemies);
                    it = enemyRespawns.erase(it);
                }
                else ++it;
            }
        }

        // Vẽ khung hình
        render(renderer, playerX, playerY);
        SDL_Delay(16); // Giới hạn 60 FPS
    }

    // Giải phóng tài nguyên trước khi thoát
    SDL_DestroyTexture(playerSpriteSheet);
    SDL_DestroyTexture(shootSpriteSheet);
    SDL_DestroyTexture(dashSpriteSheet);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(gamePauseTexture);
    SDL_DestroyTexture(trailSpriteSheet);
    SDL_DestroyTexture(playerMeleeSpriteSheet);
    SDL_DestroyTexture(invisSpriteSheet);
    SDL_DestroyTexture(invisAttackSpriteSheet);
    SDL_DestroyTexture(invisJumpSpriteSheet);
    SDL_DestroyTexture(hitSpriteSheet);
    SDL_DestroyTexture(circleHitSpriteSheet);
    SDL_DestroyTexture(enemyMeleeSpriteSheet);
    SDL_DestroyTexture(enemySpriteSheet);
    SDL_DestroyTexture(enemyDeathSpriteSheet);
    SDL_DestroyTexture(enemySpawnSpriteSheet);
    SDL_DestroyTexture(shurikenSpriteSheet);
    SDL_DestroyTexture(heartIconTexture);
    SDL_DestroyTexture(heartItemTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(pauseTextTexture);
    SDL_DestroyTexture(gameStartTexture);
    SDL_DestroyTexture(startTextTexture);
    if (scoreTextTexture) SDL_DestroyTexture(scoreTextTexture);
    if (restartTextTexture) SDL_DestroyTexture(restartTextTexture);
    TTF_CloseFont(gameFont);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(gameOverSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(killSound);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(invisSound);
    Mix_FreeChunk(accept1Sound);
    Mix_FreeChunk(accept2Sound);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0; // Thoát chương trình
}