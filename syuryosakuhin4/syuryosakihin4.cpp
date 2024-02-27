#include "glut.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include<time.h>
#include<string>
#include<windows.h>
#include <mmsystem.h>

//------定数------//
#define T 100 
#define PI 3.14159265358979
#define MAX_BULLETS 50 //最大弾数
#define MAX_ENEMIES 50 //最大敵数
#define floorX 50
#define floorZ 50
#define objectNumber 30
#define HitObjectNumber 8
#define itemNumber 10
#define GAMECLEARTIME 60 //クリアまでの時間、単位は秒

//------各種外部変数------//
int gameTime = 0;//経過したゲーム時間

float blue[] = { 0.0, 0.8, 1.0, 1.0 };    //青
float green[] = { 0.0, 1.0, 0.0, 1.0 };   //緑
float red[] = { 1.0, 0.0, 0.0, 1.0 };     //赤
float purple[] = { 1.0,0.0,1.0 };         //紫
float yellow[] = { 1.0,1.0,0.0 };         //黄色
float cyan[] = { 0,1,1 };                 //水色
float amber[] = { 1.0, 0.5, 0.0, 1.0 };   //アンバー
float white[] = { 1.0, 1.0, 1.0, 1.0 };   //白
float black[] = { 0.0, 0.0, 0.0, 1.0 };   //黒
float positionR[] = { 1.0, 1.0, 1.0, 0.0 };   //右上位置
float positionL[] = { -1.0, 1.0, 1.0, 0.0 };  //左上位置

//弾の速度
double bulletSpeed = 0.1;

//敵の速度
double enemySpeed = 0.15;

//弾の初期位置
double bulletX = 0.0;
double bulletY = 2.0;
double bulletZ = 0.0;

//カメラの位置
float cameraX = 0.0;
float cameraY = 2.0;
float cameraZ = 0.0;

//注視点の位置
float targetX = 0.0;
float targetY = 2;
float targetZ = -5;

//移動速度
float moveSpeed = 0.1;

//マウスの座標
int mouseX = 0;
int mouseY = 0;
int mouseZ = 0;

//マウス感度
float mouseSensitivity = 1;

//マウスが押されているかどうかを表すフラグ
bool mousePressed = false;

//弾描画範囲
float viewPort = 100;

//移動方向のベクトルを計算
float dirX2 = 0;
float dirZ2 = 0;

//正規化
double length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);

//初期の弾の発射レート
int bulletRate = 1000;

//ゲームクリア
bool gameClear = false;

//ゲームオーバー
bool gameOver = false;

//カーソルに関する変数
bool cursor = false;

//ゲームのスコア
 int gameScore = 0;

// 弾の情報を格納する構造体
typedef struct {
    double x;
    double y;
    double z;
    double dirX;
    double dirY;
    double dirZ;
    bool active;
} Bullet;

//敵の情報を格納する構造体
typedef struct {
    double x;
    double y;
    double z;
    double dirX;
    double dirY;
    double dirZ;
    bool active;
    double sita;
} Enemy;

//雲の情報を格納する構造体
typedef struct {
    double x;
    double y;
    double z;
} Object;

//障害物の情報を格納する構造体
typedef struct {
    double x;
    double y;
    double z;
    float width;
    float height;
    float depth;
} HitObject;

//アイテムの情報を格納する構造体
typedef struct {
    float x;
    float y;
    float z;
    float r;
    float k;
    bool active;
}Item;

Bullet bullets[MAX_BULLETS]; // 弾の配列
Enemy enemies[MAX_ENEMIES]; //敵の配列
Object clouds[objectNumber];//雲の配列
HitObject objects[HitObjectNumber];//ぶつかる物体の配列
Item items[itemNumber];//アイテムの配列

//敵の発生位置
float spawnPositions[4][2] = {
    {12.5,12.5},
    {12.5,-12.5},
    {-12.5,12.5},
    {-12.5,-12.5}
};

//------プロトタイプ宣言------//
void initBullets();
void spawnEnemy(int timer);
void Timer(int timer);
void initEnemies();
void initClouds();
void initItems();
void initObjects();
void checkCollision();
void checkCollisionWithEnemies();
void checkCollisionWithObjects();
void checkCollisionWithItems();
void reload();
void shootBullet();
void DrawCross();
void IncAngle(int timer);
void Prism(int N);
void KeyboardHandler(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void mouse(int button, int state, int x, int y);
void drawEnemy(float x, float y, float z, float radius);
void drawEnemy2(float x, float y, float z, float sita);
void drawCloud(float x, float y, float z);
void drawObject(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
void drawText(const char* text, float x, float y);
void display(void);

//------関数------//

// 弾を初期化する関数
void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

//敵を四方向に生成。ついでにクリア後の処理
void spawnEnemy(int timer) {
    if (timer != 0) return; //0番以外のタイマーは無視
    int numEnemiesToSpawn = 10;//時間毎に発生する敵の数
    for (int i = 0; i < numEnemiesToSpawn; i++) {
        int index = -1;
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            int spawnIndex = rand() % 4;//ランダムに座標を選択

            enemies[index].x = spawnPositions[spawnIndex][0];
            enemies[index].y = 2.0;
            enemies[index].z = spawnPositions[spawnIndex][1];
            enemies[index].active = true;
        }
    }

    //ゲーム終了処理
    gameTime++;

    if (gameTime >= GAMECLEARTIME) {

        if (!gameOver) {
            gameClear = true;
        }
        
    }

    if (!gameClear) {
        //1秒後に再度敵生成をスケジュール
        glutTimerFunc(1000, spawnEnemy, 0);
    }
    else {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].active = false;
        }
    }
    
}

//銃の発射タイマー
void Timer(int timer) {

    if (timer != 2)return;

    if (!gameOver) {
        shootBullet();
    }
    

    glutTimerFunc(bulletRate, Timer, 2);
}

//最初の敵
void initEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }

    srand(time(NULL));
    spawnEnemy(0);
}

//雲の位置設定
void initClouds() {
    for (int i = 0; i < objectNumber; i++) {
        clouds[i].x = (rand() % 61 - 30);
        clouds[i].y = 10;
        clouds[i].z = (rand() % 61 - 30);
    }
}

//アイテムの位置設定
void initItems() {
    for (int i = 0; i < itemNumber; i++) {
        items[i].x = (rand() % 61 - 30);
        items[i].y = 2;
        items[i].z = (rand() % 61 - 30);
        items[i].r = 0.5;//アイテムの半径
        items[i].k = 0;//アイテムの種類を設定。拡張機能の一つとして設定
        items[i].active = true;
    }
}

//物体の位置設定
void initObjects() {

    //一つ目
    objects[0].x = 12.5;
    objects[0].y = 2;
    objects[0].z = 12.5;
    objects[0].width = 5;
    objects[0].height = 5;
    objects[0].depth = 5;

    //２つ目
    objects[1].x = -12.5;
    objects[1].y = 2;
    objects[1].z = 12.5;
    objects[1].width = 5;
    objects[1].height = 5;
    objects[1].depth = 5;

    //3つ目
    objects[2].x = -12.5;
    objects[2].y = 2;
    objects[2].z = -12.5;
    objects[2].width = 5;
    objects[2].height = 5;
    objects[2].depth = 5;

    //4つ目
    objects[3].x = 12.5;
    objects[3].y = 2;
    objects[3].z = -12.5;
    objects[3].width = 5;
    objects[3].height = 5;
    objects[3].depth = 5;

    //外の壁
    //5つ目
    objects[4].x = 35;//25
    objects[4].y = 2;
    objects[4].z = 0;
    objects[4].width = 1;
    objects[4].height = 1;
    objects[4].depth = 50;

    //6つ目
    objects[5].x = -35;
    objects[5].y = 2;
    objects[5].z = 0;
    objects[5].width = 1;
    objects[5].height = 1;
    objects[5].depth = 50;

    //7つ目
    objects[6].x = 0;
    objects[6].y = 2;
    objects[6].z = 35;
    objects[6].width = 50;
    objects[6].height = 1;
    objects[6].depth = 1;

    //8つ目
    objects[7].x = 0;
    objects[7].y = 2;
    objects[7].z = -35;
    objects[7].width = 50;
    objects[7].height = 1;
    objects[7].depth = 1;
}

// 弾と敵の当たり判定を行う関数
void checkCollision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].active) {
                    // 弾とターゲットの距離を計算
                    double distance = sqrt(pow(bullets[i].x - enemies[j].x, 2) +
                        pow(bullets[i].y - enemies[j].y, 2) +
                        pow(bullets[i].z - enemies[j].z, 2));

                    double totalRadius = 0.005 + 0.1;

                    // 一定の距離以下で当たり判定
                    if (distance < totalRadius) { // 適宜調整が必要かも
                        bullets[i].active = false; // 弾を消す
                        enemies[j].active = false; // ターゲットを消す
                        gameScore += 1000;
                        break;
                    }
                }
            }
        }
    }
}

//敵とプレイヤーの当たり判定を行う関数
void checkCollisionWithEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // カメラと敵の距離を計算
            double distance = sqrt(pow(cameraX - enemies[i].x, 2) +
                pow(cameraY - enemies[i].y, 2) +
                pow(cameraZ - enemies[i].z, 2));

            double totalRadius = 0.1;

            // 衝突判定
            if (distance < totalRadius) {
                // ゲームオーバー処理
                gameOver = true;

            }
        }
    }
}

//物体とプレイヤーの当たり判定を行う関数
void checkCollisionWithObjects() {
    for (int i = 0; i < HitObjectNumber; i++) {
        //物体の中心座標を計算
        float centerX = objects[i].x;
        float centerY = objects[i].y;
        float centerZ = objects[i].z;

        //プレイヤーと物体の距離を計算
        float distanceX = fabs(cameraX - centerX) - objects[i].width;
        float distanceY = fabs(cameraY - centerY) - objects[i].height;
        float distanceZ = fabs(cameraZ - centerZ) - objects[i].depth;

        if (distanceX < 0 && distanceY < 0 && distanceZ < 0) {
            //衝突発生

            if (distanceX > distanceY && distanceX > distanceZ) {

                if (cameraX > centerX) {
                    cameraX = centerX + objects[i].width;
                    
                }
                else {
                    cameraX = centerX - objects[i].width;
                    
                }
            }
            else if (distanceY > distanceZ) {

                if (cameraY > centerY) {
                    cameraY = centerY + objects[i].height;
                    
                }
                else {
                    cameraY = centerY - objects[i].height;
                   
                }
            }
            else {

                if (cameraZ > centerZ) {
                    cameraZ = centerZ + objects[i].depth;
                   
                }
                else {
                    cameraZ = centerZ - objects[i].depth;
                    
                }
            }

        }

    }
}

//アイテムとプレイヤーの当たり判定を行う関数
void checkCollisionWithItems() {
    for (int i = 0; i < itemNumber; i++) {
        if (items[i].active) {
            // カメラとアイテムの距離を計算
            double distance = sqrt(pow(cameraX - items[i].x, 2) +
                pow(cameraY - items[i].y, 2) +
                pow(cameraZ - items[i].z, 2));

            if (distance < items[i].r) {
                if (items[i].k == 0) {
                    //発射レートアップ
                    bulletRate = bulletRate - 200;

                    if (bulletRate <= 0) {
                        bulletRate = 100;
                    }
                }
                else if(items[i].k==1){
                    //新しい技追加、今回は書かない
                }
                items[i].active = false;
            }
        }
    }
}

//リロード処理
void reload() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

// 弾を発射する関数
void shootBullet() {

    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = cameraX;
            bullets[i].y = cameraY;
            bullets[i].z = cameraZ;

            // カメラの方向ベクトルを計算
            bullets[i].dirX = targetX - cameraX;
            bullets[i].dirY = targetY - cameraY;
            bullets[i].dirZ = targetZ - cameraZ;

            // ベクトルの長さを1に正規化
            double length = sqrt(bullets[i].dirX * bullets[i].dirX + bullets[i].dirY * bullets[i].dirY + bullets[i].dirZ * bullets[i].dirZ);
            bullets[i].dirX = bullets[i].dirX / length;
            bullets[i].dirY = bullets[i].dirY / length;
            bullets[i].dirZ = bullets[i].dirZ / length;

            bullets[i].active = true;
            break;
        }

    }

    ////リロード中のフラグを立てる
    //isReloading = true;

    ////リロード時間経過後にリロード中のフラグをリセット
    //glutTimerFunc(reloadTime * 1000, resetReload, 0);

}

//画面に十字を表示
void DrawCross() {
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    int crossSize = 20;

    glColor3f(1.0f, 0.0f, 0.0f);//RGB
    glBegin(GL_LINES);
    glVertex2f(centerX - crossSize, centerY);
    glVertex2f(centerX + crossSize, centerY);
    glVertex2f(centerX, centerY - crossSize);
    glVertex2f(centerX, centerY + crossSize);
    glEnd();
}

//弾や敵の移動処理
void IncAngle(int timer)
{
    if (timer != 1) return; //1番以外のタイマーは無視

    //弾が動く処理
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x += bullets[i].dirX * bulletSpeed;
            bullets[i].y += bullets[i].dirY * bulletSpeed;
            bullets[i].z += bullets[i].dirZ * bulletSpeed;

            if (bullets[i].x<-viewPort || bullets[i].x>viewPort || bullets[i].y<-viewPort || bullets[i].y>viewPort || bullets[i].z<-viewPort || bullets[i].z>viewPort) {
                bullets[i].active = false;
            }
        }
    }

    //弾を自動リロード
    if (bullets[MAX_BULLETS - 1].active == true) {
        reload();
    }

    //敵が動く処理
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // 敵の方向ベクトルを計算
            enemies[i].dirX = cameraX - enemies[i].x;
            enemies[i].dirZ = cameraZ - enemies[i].z;

            // ベクトルの長さを1に正規化
            double length = sqrt(enemies[i].dirX * enemies[i].dirX + enemies[i].dirZ * enemies[i].dirZ);
            enemies[i].dirX = enemies[i].dirX / length;
            enemies[i].dirZ = enemies[i].dirZ / length;

            //敵の位置を更新
            enemies[i].x += enemies[i].dirX * enemySpeed;
            enemies[i].z += enemies[i].dirZ * enemySpeed;

        }
    }
    glutTimerFunc(T, IncAngle, 1);
    glutPostRedisplay();
}

//多角柱
void Prism(int N)
{
    double angle = 2 * 3.1415 / N;
    int i;
    glPushMatrix();

    glRotated(45, 0, 1, 0);

    glBegin(GL_POLYGON); //上面
    glNormal3d(0, +1, 0);
    for (i = 0; i < N; i++)
    {
        glVertex3d(cos(i * angle), +1.0, sin(i * angle));
    }
    glEnd();
    glBegin(GL_POLYGON); //下面
    glNormal3d(0, -1, 0);
    for (i = 0; i < N; i++)
    {
        glVertex3d(cos(i * angle), -1.0, sin(i * angle));
    }
    glEnd();
    glBegin(GL_QUAD_STRIP); //側面
    for (i = 0; i <= N; i++)
    {
        glNormal3d(cos((i - 0.5) * angle), 0, sin((i - 0.5) * angle));
        glVertex3d(cos(i * angle), +1.0, sin(i * angle));
        glVertex3d(cos(i * angle), -1.0, sin(i * angle));
    }
    glEnd();

    glPopMatrix();
}

//キーボード処理
void KeyboardHandler(unsigned char key, int x, int y)
{

    if (key == ' ') {
        exit(0);
    }

    if (key == VK_ESCAPE) {
        if (!cursor) {
            cursor = true;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        }
        else {
            cursor = false;
            glutSetCursor(GLUT_CURSOR_NONE);
        }
    }
        

    if (!gameOver) {
        switch (key) {
        case 'w':
            //移動方向のベクトルを計算
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //正規化
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);

            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            //値保持したほうがいいかも
            cameraX += moveSpeed * (dirX2);
            //cameraY += moveSpeed * (targetY - cameraY);
            cameraZ += moveSpeed * (dirZ2);

            targetX += moveSpeed * (dirX2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ += moveSpeed * (dirZ2);
            break;
        case 's':
            //移動方向のベクトルを計算
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //正規化
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);
            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            cameraX -= moveSpeed * (dirX2);
            //cameraY -= moveSpeed * (targetY - cameraY);
            cameraZ -= moveSpeed * (dirZ2);

            targetX -= moveSpeed * (dirX2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ -= moveSpeed * (dirZ2);
            break;
        case 'a':
            //移動方向のベクトルを計算
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //正規化
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);
            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            // カメラを左方向に移動
            cameraX -= moveSpeed * (-dirZ2);
            //cameraY += moveSpeed * 0;
            cameraZ -= moveSpeed * (dirX2);

            targetX -= moveSpeed * (-dirZ2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ -= moveSpeed * (dirX2);
            break;
        case 'd':
            //移動方向のベクトルを計算
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //正規化
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);
            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            // カメラを右方向に移動
            cameraX += moveSpeed * (-dirZ2);
            //cameraY -= moveSpeed * 0;
            cameraZ += moveSpeed * (dirX2);

            targetX += moveSpeed * (-dirZ2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ += moveSpeed * (dirX2);
            break;
        case 'q':
            cameraY += 0.1;
            targetY += 0.1;

            break;
        case 'e':
            cameraY -= 0.1;
            targetY -= 0.1;

            //後でコメントアウトを消すこと
            if (cameraY < 2) {
                cameraY = 2;
                targetY = 2;
            }
            break;
        case 'r':
            reload();
            break;
        }

    }

    glutPostRedisplay();
}

//マウス移動処理
void mouseMove(int x, int y) {
    //マウスの移動量を計算
    float deltaX = x - mouseX;
    float deltaY = y - mouseY;

    //ターゲットの値を保持
    float a = targetX - cameraX;
    float b = targetZ - cameraZ;

    //調整移動量
    float redeltaX = deltaX * mouseSensitivity;

    //微少変化角度
    float sita = 0.005 * redeltaX;

    targetX = a * cos(sita) - b * sin(sita) + cameraX;
    targetZ = a * sin(sita) + b * cos(sita) + cameraZ;

    if (!cursor) {
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

        // マウスカーソルの座標を更新
        mouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        mouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    }
    

    glutPostRedisplay();
}

//マウスクリック処理
void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    if (gameClear||gameOver) {
        gameOver = false;
        gameClear = false;
        gameTime = 0;
        bulletRate = 1000;
        gameScore = 0;

        //アイテムの描画
        for (int i = 0; i < itemNumber; i++) {
            items[i].active = true;
        }

        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].active = false;
        }

        srand(time(NULL));
        spawnEnemy(0);
    
    }

    glutPostRedisplay();
}

//紫の球を生成。アイテムの見た目
void drawEnemy(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, purple);
    glLightfv(GL_LIGHT0, GL_AMBIENT, purple);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

//敵キャラの見た目
void drawEnemy2(float x, float y, float z, float sita) {

    glPushMatrix();

    glTranslated(0, -0.1, 0);

    //頭
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cyan);
    glLightfv(GL_LIGHT0, GL_AMBIENT, cyan);
    glTranslated(0, 0.1, 0);

    //回転処理
    glRotated(sita, 0, 1, 0);

    glScaled(0.7, 0.7, 0.7);
    glutSolidSphere(0.1, 20, 20);

    //目
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glTranslated(0.025, 0.02, 0.08);
    glutSolidSphere(0.02, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glTranslated(-0.025, 0.02, 0.08);
    glutSolidSphere(0.02, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glTranslated(0.025, 0.02, 0.1);
    glutSolidSphere(0.005, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glTranslated(-0.025, 0.02, 0.1);
    glutSolidSphere(0.005, 20, 20);
    glPopMatrix();

    //口
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT0, GL_AMBIENT, red);
    glTranslated(0, -0.03, 0.09);
    glRotated(90, 0, 0, 1);
    glScaled(0.01, 0.02, 0.01);
    Prism(6);
    glPopMatrix();


    glPopMatrix();


    glPushMatrix();

    glTranslated(0, -0.075, 0);
    //胴体
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cyan);
    glLightfv(GL_LIGHT0, GL_AMBIENT, cyan);

    //回転
    glRotated(sita, 0, 1, 0);

    glScaled(0.75, 1.25, 0.75);//1,0.3,1
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    glPushMatrix();

    //手
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    glRotated(sita, 0, 1, 0);

    glTranslated(-0.08, 0, 0);//-0.1,-0.075,0
    glRotated(-5, 0, 0, 1);
    glScaled(0.01, 0.075, 0.01);
    Prism(6);
    glPopMatrix();

    //手
    glPushMatrix();
    glTranslated(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    glRotated(sita, 0, 1, 0);

    glTranslated(0.08, 0, 0);
    glRotated(5, 0, 0, 1);
    glScaled(0.01, 0.075, 0.01);
    Prism(6);
    glPopMatrix();

    glPopMatrix();

    //足
    glPushMatrix();
    glTranslated(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    glRotated(sita, 0, 1, 0);

    glTranslated(-0.02, -0.09, 0);
    glScaled(0.01, 0.09, 0.01);
    Prism(6);
    glPopMatrix();

    //足
    glPushMatrix();
    glTranslated(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    glRotated(sita, 0, 1, 0);

    glTranslated(0.02, -0.09, 0);
    glScaled(0.01, 0.09, 0.01);
    Prism(6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);

    glRotated(sita, 0, 1, 0);

    glTranslated(0.0, -0.8, 0);
    glScaled(0.09, 0.01, 0.09);
    Prism(6);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

//雲の見た目
void drawCloud(float x, float y, float z) {

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glTranslated(x, y, z); //yは10くらい
    glScaled(2, 0.5, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

}

//障害物
void drawObject(float x, float y, float z, float sizeX, float sizeY, float sizeZ) {

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT0, GL_AMBIENT, yellow);
    glTranslated(x, y, z); //yは10くらい
    glScaled(sizeX, sizeY, sizeZ);
    Prism(4);
    glPopMatrix();

}

//文字の表示
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

//表示用の関数
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, 0, 1, 0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    checkCollision();
    checkCollisionWithEnemies();
    checkCollisionWithObjects();
    checkCollisionWithItems();

    //アイテムの描画
    for (int i = 0; i < itemNumber; i++) {
        if (items[i].active == true) {
            drawEnemy(items[i].x, items[i].y, items[i].z, items[i].r);
        }
        
    }

    //動かない描写
    for (int i = 0; i < objectNumber; i++) {
        drawCloud(clouds[i].x, clouds[i].y, clouds[i].z);
    }

    //当たり判定のある物体の描写

    glPushMatrix();

    for (int i = 0; i < HitObjectNumber; i++) {
        //ステージ上の物体
        drawObject(objects[i].x, objects[i].y, objects[i].z, objects[i].width, objects[i].height, objects[i].depth);

    }

    glPopMatrix();

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // 弾を描画
            glPushMatrix();
            glLightfv(GL_LIGHT0, GL_POSITION, positionR);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
            glLightfv(GL_LIGHT0, GL_AMBIENT, red);
            glTranslated(bullets[i].x, bullets[i].y, bullets[i].z);
            glutSolidSphere(0.005, 20, 20); // 球体を描画
            glPopMatrix();
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {

        if (enemies[i].active) {

            enemies[i].sita = atan2(enemies[i].dirX, enemies[i].dirZ) * 180 / PI;

            drawEnemy2(enemies[i].x, enemies[i].y, enemies[i].z, enemies[i].sita);
        }

    }

    //環境
    glPushMatrix();
    glShadeModel(GL_FLAT);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT0, GL_AMBIENT, yellow);
    glTranslated(0, 1, 0);
    glScaled(floorX, 0.01, floorZ);
    Prism(4);
    glPopMatrix();

    // 2Dの描画
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // 2Dの描画コードをここに書く 

    DrawCross();//十字の描画

    if (gameClear) {
       
        //テキストの表示
        int x = 100;
        int y = 100;
        const char* text = "GAME CLEAR!! Restart on mouse left button click ";

        glColor3f(0, 0, 0);

        glRasterPos2f(x, y);

        for (int i = 0; i < strlen(text); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
    }

    if (gameOver) {

        //テキストの表示
        int x = 100;
        int y = 100;
        const char* text = "GAME OVER... Restart on mouse left button click";

        glColor3f(0, 0, 0);

        glRasterPos2f(x, y);

        for (int i = 0; i < strlen(text); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        }
    }

    char text2[50];

    sprintf_s(text2, "Score:%d", gameScore);
    glColor3f(0, 0, 0);
    drawText(text2, 10, 600);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();
}

//メイン関数
int main(int argc, char** argv)
{

    //音楽再生
    PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

    glutInit(&argc, argv);

    glutInitWindowPosition(200, 0);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

    glutCreateWindow("3D shooting game");

    glClearColor(1.0f, 0.75f, 0.8f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 0.1, 1000.0);//視野角、アスペクト比、どこから、どこまで表示

    if (!cursor) {
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

        // マウスカーソルの座標を更新
        mouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        mouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    }
    

    initBullets();
    initEnemies();
    initClouds();
    initObjects();
    initItems();

    glutTimerFunc(T, IncAngle, 1);

    glutTimerFunc(bulletRate, Timer, 2);
    
    glutSetCursor(GLUT_CURSOR_NONE);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(KeyboardHandler);

    glutMainLoop();
}

