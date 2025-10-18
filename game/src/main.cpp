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
constexpr float BULLET_RADIUS = 10.0f;
constexpr float BULLET_SPEED = 300.0f;
constexpr float BULLET_LIFE_TIME = 3.0f;

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

struct Turrets
{
    Vector2 position;
    float range;
    int amount;
};




struct BasicEnemy
{


    Vector2 position;
    float speed;
    int currentwaypoint = 0;
    int nextwaypoint = 1;
    int Health;
    
};

struct FastEnemy
{


    Vector2 position;
    float speed;
    int currentwaypoint = 0;
    int nextwaypoint = 1;
    int Health;

};

struct StrongEnemy
{


    Vector2 position;
    float speed;
    int currentwaypoint = 0;
    int nextwaypoint = 1;
    int Health;

};



//ARRAY

int main()
{
    int tiles[TILE_COUNT][TILE_COUNT]
    {
        //col:0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19    row:
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 0
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 1
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 2
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 3
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 4
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 5
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 6
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0 }, // 7
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 8
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 9
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 10
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 11
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 13
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 14
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 15
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 16
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 17
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 18
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 19
    };


    std::vector<Cell> waypoints = FloodFill({ 0, 12 }, tiles, WAYPOINT);


    std::vector<Cell> grass = FloodFill({ 9, 9 }, tiles, GRASS);

    int curr = 0;
    int next = 1;
    

    Vector2 from = TileCenter(grass[curr].row, waypoints[curr].col);

    CheckCollisionCircles(GetMousePosition(), BULLET_RADIUS, from, TILE_SIZE);

    //int curr = 0;
    //int next = curr + 1;
    
    //Vector2 enemyPosition = TileCenter(waypoints[curr].row, waypoints[curr].col);

    float enemySpeed = 250.0f;   // <-- 250 pixels per second

    float fastenemySpeed = 150.0f;

    float minDistance = enemySpeed / 60.0f;
    minDistance *= 1.1f;
    

    float shootTimeCurrent = 0.0f;
    float shootTimeTotal = 1.0f;

    //C = CURRENT TIME 

    //T = TOTAL TIME
    
    //L = TIME LIMIT

    float spawnTimeCurrent1 = 0.0f;

    

    float spawnTimeTotal1 = 1.0f;

   

    float basicEnemySpawnTimeC = 0.0f;
    float basicEnemySpawnTimeT = 1.0f;
    float basicEnemySpawnTimeL = 6.0f;

    float fastEnemySpawnTimeC = 0.0f;
    float fastEnemySpawnTimeT = 8.0f;
    float fastEnemySpawnTimeL = 10.0f;

    float strongEnemySpawnTimeC = 0.0f;
    float strongEnemySpawnTimeT = 10.0f;
    float strongEnemySpawnTimeL = 16.0f;
   
    
  
    //TURRET/BULLET VECTORS
    std::vector<Bullet> bullets;
    std::vector<Turrets> basicturrets;
    std::vector<Turrets> sniperturrets;


    //ENEMY TYPE VECTORS
    std::vector<BasicEnemy> basicenemies;
    std::vector<FastEnemy> fastenemies;
    std::vector<StrongEnemy> strongenemies;


    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            if (tiles[row][col] == TURRET)
            {
                Turrets basicturret;
                basicturret.position = TileCenter(row, col);
                basicturret.range = 250.0f;
                basicturrets.push_back(basicturret);
            }

            bool closed[TILE_COUNT][TILE_COUNT];
            closed[row][col] = tiles[row][col] == 0;
        }
    }





    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            if (tiles[row][col] == TURRET)
            {
                Turrets sniperturret;
                sniperturret.position = TileCenter(row, col);
                sniperturret.range = 1000.0f;
                basicturrets.push_back(sniperturret);
            }

            bool closed[TILE_COUNT][TILE_COUNT];
            closed[row][col] = tiles[row][col] == 0;
        }
    }





    //WHENGAMERUNNING SECTION (CHECKS EVERY FRAME)

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Tower Defense");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();


      

        //BASIC TURRET SPAWNING

        Vector2 Mouseposition = GetMousePosition();
        int MouseY = static_cast<int>(Mouseposition.y / TILE_SIZE);
        int MouseX = static_cast<int>(Mouseposition.x / TILE_SIZE);
        bool TurretMode = true;
        bool MouseCollideWithTile = CheckCollisionCircles(GetMousePosition(), BULLET_RADIUS, from, TILE_SIZE);

        



       
        int turretlimit = 5;

        if (TurretMode)
        {
            if (tiles[MouseY][MouseX] == GRASS && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && basicturrets.size() < turretlimit)
            {


                tiles[MouseY][MouseX]++;
                tiles[MouseY][MouseX]++;
                tiles[MouseY][MouseX]++;

                Turrets basicturret;
                basicturret.position = TileCenter(MouseY, MouseX);
                basicturret.range = 250.0f;
                basicturrets.push_back(basicturret);

              

            }

        }
        if (basicturrets.size() == turretlimit)
        {

            TurretMode = false;
        }
       
        if (basicturrets.size() == turretlimit)
        {

            //SHOOTING

            shootTimeCurrent += dt;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && shootTimeCurrent >= shootTimeTotal)
            {
                shootTimeCurrent = 0.0f;

                // AB = B - A
                Bullet bullet;
                bullet.position = GetMousePosition();
                bullet.direction = Vector2UnitX;

                bullets.push_back(bullet);

            }


            //DETERMINE BULLET DIRECTION
            for (Bullet& bullet : bullets)
            {
                bullet.position += bullet.direction * BULLET_SPEED * dt;
                bullet.time += dt;

                bool expired = bullet.time >= BULLET_LIFE_TIME;
                bool collision = CheckCollisionCircles(bullet.position, BULLET_RADIUS, bullet.position, ENEMY_RADIUS);
                collision |= !CheckCollisionCircleRec(bullet.position, BULLET_RADIUS, SCREEN_REC);

                bullet.destroy = collision || expired;

                if (collision)
                {


                  


                }




                




            }

            //REMOVE BULLETS
            auto bulletsRemoveStart = std::remove_if(bullets.begin(), bullets.end(),
                [](Bullet& bullet)
                {
                    return bullet.destroy;
                });
            bullets.erase(bulletsRemoveStart, bullets.end());





            //BASIC ENEMY SPAWNING

            basicEnemySpawnTimeC += dt;
            if (basicEnemySpawnTimeC > basicEnemySpawnTimeT)
            {
                basicEnemySpawnTimeC = 0.0f;

                BasicEnemy basicEnemy;
                basicEnemy.position = TileCenter(0, 12);
                basicEnemy.speed = enemySpeed;
                basicenemies.push_back(basicEnemy);
                basicEnemy.Health = 2;

            }

            fastEnemySpawnTimeC += dt;
            if (fastEnemySpawnTimeC > fastEnemySpawnTimeT)
            {
                fastEnemySpawnTimeC = 0.0f;

                FastEnemy fastEnemy;
                fastEnemy.position = TileCenter(0, 12);
                fastEnemy.speed = fastenemySpeed;
                fastenemies.push_back(fastEnemy);
                fastEnemy.Health = 1;
            }




            //BASIC ENEMY MOVEMENT

            bool End = false;

            for (BasicEnemy& basicEnemy : basicenemies)
            {
                int& curr = basicEnemy.currentwaypoint;
                int& next = basicEnemy.nextwaypoint;
                bool atEnd;


                if (basicEnemy.Health = 0)
                {





                }



                atEnd = curr == waypoints.size() - 1;

                if (!atEnd)
                {
                    Vector2 from = TileCenter(waypoints[curr].row, waypoints[curr].col);
                    Vector2 to = TileCenter(waypoints[next].row, waypoints[next].col);
                    Vector2 direction = Vector2Normalize(to - from);
                    basicEnemy.position += direction * enemySpeed * dt;

                    // Tolorance depends on enemy speed
                    if (CheckCollisionPointCircle(basicEnemy.position, to, minDistance))
                    {

                        basicEnemy.position = to;

                        curr++;
                        next++;
                        //atEnd = curr == waypoints.size() - 1;
                        //atEnd = curr == waypoints.size() - 1;

                        bool End = false;

                    }






                }










            }


            //FAST ENEMY MOVEMENT
            for (FastEnemy& fastEnemy : fastenemies)
            {
                int& curr = fastEnemy.currentwaypoint;
                int& next = fastEnemy.nextwaypoint;
                bool atEnd;

                atEnd = curr == waypoints.size() - 1;

                if (!atEnd)
                {
                    Vector2 from = TileCenter(waypoints[curr].row, waypoints[curr].col);
                    Vector2 to = TileCenter(waypoints[next].row, waypoints[next].col);
                    Vector2 direction = Vector2Normalize(to - from);
                    fastEnemy.position += direction * enemySpeed * dt;

                    // Tolorance depends on enemy speed
                    if (CheckCollisionPointCircle(fastEnemy.position, to, minDistance))
                    {

                        fastEnemy.position = to;

                        curr++;
                        next++;
                        //atEnd = curr == waypoints.size() - 1;
                        //atEnd = curr == waypoints.size() - 1;
                        bool End = false;
                    }






                }









            }
        }

        //DRAWING
        BeginDrawing();
        ClearBackground(BLACK);

        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                DrawTile(row, col, tiles[row][col]);
            }
        }

        for (const Turrets& turret : basicturrets)
            DrawCircleV(turret.position, 10, WHITE);

   

        for (const BasicEnemy& basicEnemy : basicenemies)
        DrawCircleV(basicEnemy.position, ENEMY_RADIUS, BLUE);
     
        for (const FastEnemy& fastEnemy : fastenemies)
            DrawCircleV(fastEnemy.position, ENEMY_RADIUS, YELLOW);



        for (const Bullet& bullet : bullets)
            DrawCircleV(bullet.position, BULLET_RADIUS, RED);

   /*     if ()
        {
            
            DrawText(TextFormat("YOU LOSE"), 10, 10, 20, RED);
            

        }*/

        DrawText(TextFormat("Bullets: %i", bullets.size()), 10, 10, 20, RED);
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


//JUNK CODE

  // Start simple- try to spawn 10 enemies and move them along the path before adding turrets
        // Note - You'll most likely need a 2d for-loop that tests all bullets against all enemies

    /*     for (int row = 0; row < TILE_COUNT; row++)
            {
                for (int col = 0; col < TILE_COUNT; col++)
                {
                    if (tiles[row][col] == START)
                    {
                        for (int i = 1; i < 9; i++)
                        {
                            if (shootTimeCurrent >= shootTimeTotal)
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
            }*/



            //POSSIBLE ENEMY VARIANT CODE
             /*   spawnTimeCurrent += dt;
                if (spawnTimeCurrent >= spawnTimeTotal)
                {
                    spawnTimeCurrent = 0.0f;

                    for (int i = 0; i < 3; i++)
                    {
                        Enemy enemy;
                        enemy.position = TileCenter(0 + i, 12);
                        enemy.speed = enemySpeed;
                        enemies.push_back(enemy);
                    }
                }*/

                //bullet.direction = Vector2Normalize(enemy.position - bullet.position);
//bullets.push_back(bullet);

//Bullet bulletT;
//bulletT.position = TileCenter(TURRET, TURRET);
//bulletT.direction = Vector2Normalize(enemyPosition - bulletT.position);
//bullets.push_back(bulletT);

//Vector2 Overlaptile = CheckCollisionCircles(GetMousePosition(), TILE_SIZE);
        //}

        // Turret Shooting
        //if (IsKeyDown(KEY_SPACE) && shootTimeCurrent >= shootTimeTotal)
        //{
        //    shootTimeCurrent = 0.0f;
        //    // AB = B - A
        //    Bullet bulletT;
        //    bulletT.position = 
        //    bulletT.direction = Vector2Normalize(enemyPosition - bulletT.position);
        //    bullets.push_back(bulletT);
        //}

        // 1) Update bullet logic (motion + destroy flag)

  //Turret Spawning Code 
//    for (int row = 0; row < TILE_COUNT; row++)
//    {
//        for (int col = 0; col < TILE_COUNT; col++)
//        {
//            if (tiles[row][col] == TURRET)
//            {
//                Turret turret;
//                turret.position = TileCenter(row, col);
//                turret.range = 1000.0f;
//                turrets.push_back(turret);
//            }
//
//            bool closed[TILE_COUNT][TILE_COUNT];
//            closed[row][col] = tiles[row][col] == 0;
//        }
//    }
//
//    Vector2 TurretClickTile = GetMousePosition();
//
//    int TurretCount = 3;
//
//    if (TurretCount < 5 && IsKeyDown(KEY_SPACE))
//    {
//        Turret turret;
//    turret.range = 1000.0f;
//    turrets.push_back(turret);
//}

    //SPAWNS ENEMY
    //for (int row = 0; row < TILE_COUNT; row++)
    //{
    //    for (int col = 0; col < TILE_COUNT; col++)
    //    {
    //        if (tiles[row][col] == START)
    //        {
    //            for (int i = 1; i < 9; i++)
    //            {
    //                if (shootTimeCurrent >= shootTimeTotal)
    //                {
    //                    Enemy enemy;
    //                    enemy.position = TileCenter(row, col);
    //                    enemy.speed = enemySpeed;
    //                    enemy1.push_back(enemy);
    //                }
    //            }
    //        }
    //
    //        bool closed[TILE_COUNT][TILE_COUNT];
    //        closed[row][col] = tiles[row][col] == 0;
    //    }
    //}