#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>

constexpr float SCREEN_SIZE = 800;
constexpr Rectangle SCREEN_REC = { 0, 0, SCREEN_SIZE, SCREEN_SIZE };

constexpr int TILE_COUNT = 20;
constexpr float TILE_SIZE = SCREEN_SIZE / TILE_COUNT;

constexpr float ENEMY_RADIUS = 10.0f;
constexpr float ENEMY_HEALTH = 20.0f;
constexpr float BULLET_RADIUS = 10.0f;
constexpr float BULLET_SPEED = 300.0f;
constexpr float BULLET_LIFE_TIME = 1.0f;

enum TileType : int
{
    GRASS,    
    DIRT,       
    WAYPOINT,
    TURRET, 
    START,
    COUNT
};

struct Cell
{
    int row;
    int col;
};

constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0 }, Cell{ 0, -1 }, Cell{ 0, 1 } };

inline bool InBounds(Cell cell, int rows = TILE_COUNT, int cols = TILE_COUNT)
{
    return cell.col >= 0 && cell.col < cols && cell.row >= 0 && cell.row < rows;
}

void DrawTile(int row, int col, Color color)
{
    DrawRectangle(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

void DrawTile(int row, int col, int type)
{
    Color colors[4] =
    {
        LIME,
        BEIGE,
        SKYBLUE,
        PINK
    };
    //assert(type >= 0 && type < COUNT);
    DrawTile(row, col, colors[type]);
}

Vector2 TileCenter(int row, int col)
{
    float x = col * TILE_SIZE + TILE_SIZE * 0.5f;
    float y = row * TILE_SIZE + TILE_SIZE * 0.5f;
    return { x, y };
}

typedef struct WaitingTime {
    double startingtime;
    double totaltime = 1.0;



    //Attemped timer system (Sadly Ran out of time)


} WaitingTime;

void StartTimer(WaitingTime* timer, double totaltime) {

    timer->startingtime = GetTime();
    timer->totaltime = totaltime;


}

bool Timerfinish(WaitingTime timer)
{
    return GetTime() - timer.startingtime >= timer.totaltime;

}
bool GetTimer(WaitingTime timer)
{
    return GetTime() - timer.startingtime;

}


   


    std::vector<Cell> FloodFill(Cell start, int tiles[TILE_COUNT][TILE_COUNT], TileType searchValue)
    {
        // "open" = "places we want to search", "closed" = "places we've already searched".
        std::vector<Cell> result;
        std::vector<Cell> open;
        bool closed[TILE_COUNT][TILE_COUNT];
        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                // We don't want to search zero-tiles, so add them to closed!
                closed[row][col] = tiles[row][col] == 0;
            }
        }

    // Add the starting cell to the exploration queue & search till there's nothing left!
    open.push_back(start);
    while (!open.empty())
    {
        // Remove from queue and prevent revisiting
        Cell cell = open.back();
        open.pop_back();
        closed[cell.row][cell.col] = true;

        // Add to result if explored cell has the desired value
        if (tiles[cell.row][cell.col] == searchValue)
            result.push_back(cell);

        // Search neighbours
        for (Cell dir : DIRECTIONS)
        {
            Cell adj = { cell.row + dir.row, cell.col + dir.col };
            if (InBounds(adj) && !closed[adj.row][adj.col] && tiles[adj.row][adj.col] != 0)
                open.push_back(adj);
        }
    }

    return result;
}


    //Turret and Enemy structs with appropriate Vectors/floats.

struct Bullet
{
    Vector2 position = Vector2Zeros;
    Vector2 direction = Vector2Zeros;
    float time = 0.0f;
    bool destroy = false;
};

struct Turret
{
    Vector2 position;
    float range;
};

struct Enemy
{
    Vector2 position;
    int health;
    float speed;
};


//Created New Tile Type for Starting Tile

int main()
{
    int tiles[TILE_COUNT][TILE_COUNT]
    {
        //col:0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19    row:
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 }, // 0
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 1
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 2
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 3
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 4
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 5
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 6
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0 }, // 7
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 8
            { 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 9
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 10
            { 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 11
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 13
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 14
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0, 0 }, // 15
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 16
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 17
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 18
            { 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 19
    };

    std::vector<Cell> waypoints = FloodFill({ 0, 12 }, tiles, WAYPOINT);
    int curr = 0;
    int next = curr + 1;

    Vector2 enemyPosition = TileCenter(waypoints[curr].row, waypoints[curr].col);
    float enemySpeed = 250.0f;   // <-- 250 pixels per second
    float minDistance = enemySpeed / 60.0f;
    minDistance *= 1.1f;
    bool atEnd = false;

    float shootTimeCurrent = 0.0f;
    float shootTimeTotal = 1.0f;

    //Enemy and turret vectors

    std::vector<Bullet> bullets;
    std::vector<Turret> turret1;
    std::vector<Enemy> enemy1;


    //Turret Spawning Code 

    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            if (tiles[row][col] == TURRET)
            {
                Turret turret;

                turret.position = TileCenter(row, col);

                turret.range = 1000.0f;

                turret1.push_back(turret);
            }

            bool closed[TILE_COUNT][TILE_COUNT];
            closed[row][col] = tiles[row][col] == 0;
        }
    }

 


    WaitingTime EnemySpawnTimer = { 0 };

    float enemyspawnwaitingtime = 10.0f;

    int enemyhealth = 20.0f;


    //Enemy spawning Code

        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                if (tiles[row][col] == START)
                {
                    StartTimer(&EnemySpawnTimer, enemyspawnwaitingtime);

                    GetTimer(EnemySpawnTimer);

                    if (Timerfinish(EnemySpawnTimer))
                    {
                        for (int i = 1; i < 9; i++)
                        {

                            Enemy enemy;

                            enemy.position = TileCenter(row, col);

                            enemy.speed = enemySpeed;

                            enemy1.push_back(enemy);
                        }
                    }
                }

                bool closed[TILE_COUNT][TILE_COUNT];
                closed[row][col] = tiles[row][col] == 0;
            }
        }

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Tower Defense");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        shootTimeCurrent += dt;
        if (IsKeyDown(KEY_SPACE) && shootTimeCurrent >= shootTimeTotal)
        {
            shootTimeCurrent = 0.0f;

            // AB = B - A
            Bullet bullet;
            bullet.position = GetMousePosition();
            bullet.direction = Vector2Normalize(enemyPosition - bullet.position);
            bullets.push_back(bullet);
        }

        // 1) Update bullet logic (motion + destroy flag)
        for (Bullet& bullet : bullets)
        {
            bullet.position += bullet.direction * BULLET_SPEED * dt;
            bullet.time += dt;

            bool expired = bullet.time >= BULLET_LIFE_TIME;
            bool collision = CheckCollisionCircles(bullet.position, BULLET_RADIUS, enemyPosition, ENEMY_RADIUS);
            collision |= !CheckCollisionCircleRec(bullet.position, BULLET_RADIUS, SCREEN_REC);

            bullet.destroy = collision || expired;
        }
        
        // 2) Remove bullets based on destroy flag
        auto bulletsRemoveStart = std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet& bullet)
            {
                return bullet.destroy;
            });
        bullets.erase(bulletsRemoveStart, bullets.end());

        // Start simple - try to spawn 10 enemies and move them along the path before adding turrets
        // Note - You'll most likely need a 2d for-loop that tests all bullets against all enemies

        if (!atEnd)
        {
            Vector2 from = TileCenter(waypoints[curr].row, waypoints[curr].col);
            Vector2 to = TileCenter(waypoints[next].row, waypoints[next].col);
            Vector2 direction = Vector2Normalize(to - from);
            enemyPosition += direction * enemySpeed * dt;

            // Tolorance depends on enemy speed
            if (CheckCollisionPointCircle(enemyPosition, to, minDistance))
            {
                enemyPosition = to;

                curr++;
                next++;
                atEnd = curr == waypoints.size() - 1;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw entire grid
        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                DrawTile(row, col, tiles[row][col]);
            }
        }

        //Code for drawing turrets/enemies

        for (const Turret& turret : turret1)
            DrawCircleV(turret.position, 10, PINK);
        
        for (const Enemy& enemy : enemy1)
            DrawCircleV(enemy.position, ENEMY_RADIUS, BLUE);

        


        for (const Bullet& bullet : bullets)
            DrawCircleV(bullet.position, BULLET_RADIUS, RED);
        DrawCircleV(enemyPosition, ENEMY_RADIUS, GOLD);
        
        DrawText(TextFormat("Bullets: %i", bullets.size()), 10, 10, 20, RED);
        DrawText(TextFormat("Enemies Remaining: "), 10, 25, 20, RED);
        DrawText(TextFormat("%i", GetFPS()), 770, 10, 20, RED);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

//std::vector<int> numbers{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
//for (int i = 0; i < numbers.size(); i++)
//{
//    if (numbers[i] % 2 == 1)
//    {
//        numbers.erase(numbers.begin() + i);
//        i--;
//    }
//}

// remove_if sorts the array so that all the elements we want to remove are at the end.
// For example, if we start with    { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 },
// We'll end up with                { 2, 2, 4, 4, 1, 1, 3, 3, 5, 5, }
//auto start = std::remove_if(numbers.begin(), numbers.end(), [](int n) { return n % 2 == 1; });
//numbers.erase(start, numbers.end());
