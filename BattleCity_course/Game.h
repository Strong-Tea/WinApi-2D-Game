#pragma once
#include <vector>
#include "DisplayInformation.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <ctime>
#include "Level.h"

enum KeyPress
{
    LEFT, 
    RIGHT, 
    UP, 
    DOWN, 
    KEYNULL
};

class Game
{
private:
    // Window
    HWND hwnd;
    HDC dc;

    // Camera and Display Rect
    static RECT windowRect;
    Camera* cameraOffset;
    
    // Map
    Level* level;
    
    // Player
    Player* player;
    //DWORD PLAYER_TIME_SHOOT;
    BOOL playerCanFire;
    double playerFireTime;
    double playerFireElapsedTime;

    // Enemies
    std::vector<Enemy*> enemies;
    int maxEnemies;
    BOOL enemyShouldSpawn;
    float enemySpawnTime;
    float enemySpawnElapsedTime;

    // Bullets
    std::vector<Bullet*> bullets;

    // Display Information
    DisplayInformation* displayInfo;
   
    // Game
    static Game* pgame;
    const double TARGET_FPS = 60.0; // Target FPS
    const double FRAME_INTERVAL = 1000.0 / TARGET_FPS; // Time per frame in milliseconds
    double fps;

    // Collisions
    CollisionInfo collisionInfo;

    //private functions
    void initWindow(int widthScreen, int heightScreen, PCWSTR lpWindowName, DWORD dwStyle);
    void initGameInformation();
    void initPlayer();
    void initEnemies();
    void initMap();
    void initCamera();

public:

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Game(int screenWidth, int screenHeight);
    void resumeGame();

    virtual ~Game();

    void addBullet(int Mouse_X,int Mouse_Y);

    // game run
    void run();

    // game update
    void update(double deltaTime);

    // update player key input
    void updatePlayerDirection();

    void updateEnemies(double deltaTime);

    void updateBullets(double deltaTime);

    void updateCollision(double deltaTime);

    void render();

    BOOL checkCollisionObjects(const GameObject* obj1, const GameObject* obj2);

};
