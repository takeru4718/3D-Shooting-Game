#include "glut.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include<time.h>
#include<string>
#include<windows.h>
#include <mmsystem.h>

//------�萔------//
#define T 100 
#define PI 3.14159265358979
#define MAX_BULLETS 50 //�ő�e��
#define MAX_ENEMIES 50 //�ő�G��
#define floorX 50
#define floorZ 50
#define objectNumber 30
#define HitObjectNumber 8
#define itemNumber 10
#define GAMECLEARTIME 60 //�N���A�܂ł̎��ԁA�P�ʂ͕b

//------�e��O���ϐ�------//
int gameTime = 0;//�o�߂����Q�[������

float blue[] = { 0.0, 0.8, 1.0, 1.0 };    //��
float green[] = { 0.0, 1.0, 0.0, 1.0 };   //��
float red[] = { 1.0, 0.0, 0.0, 1.0 };     //��
float purple[] = { 1.0,0.0,1.0 };         //��
float yellow[] = { 1.0,1.0,0.0 };         //���F
float cyan[] = { 0,1,1 };                 //���F
float amber[] = { 1.0, 0.5, 0.0, 1.0 };   //�A���o�[
float white[] = { 1.0, 1.0, 1.0, 1.0 };   //��
float black[] = { 0.0, 0.0, 0.0, 1.0 };   //��
float positionR[] = { 1.0, 1.0, 1.0, 0.0 };   //�E��ʒu
float positionL[] = { -1.0, 1.0, 1.0, 0.0 };  //����ʒu

//�e�̑��x
double bulletSpeed = 0.1;

//�G�̑��x
double enemySpeed = 0.15;

//�e�̏����ʒu
double bulletX = 0.0;
double bulletY = 2.0;
double bulletZ = 0.0;

//�J�����̈ʒu
float cameraX = 0.0;
float cameraY = 2.0;
float cameraZ = 0.0;

//�����_�̈ʒu
float targetX = 0.0;
float targetY = 2;
float targetZ = -5;

//�ړ����x
float moveSpeed = 0.1;

//�}�E�X�̍��W
int mouseX = 0;
int mouseY = 0;
int mouseZ = 0;

//�}�E�X���x
float mouseSensitivity = 1;

//�}�E�X��������Ă��邩�ǂ�����\���t���O
bool mousePressed = false;

//�e�`��͈�
float viewPort = 100;

//�ړ������̃x�N�g�����v�Z
float dirX2 = 0;
float dirZ2 = 0;

//���K��
double length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);

//�����̒e�̔��˃��[�g
int bulletRate = 1000;

//�Q�[���N���A
bool gameClear = false;

//�Q�[���I�[�o�[
bool gameOver = false;

//�J�[�\���Ɋւ���ϐ�
bool cursor = false;

//�Q�[���̃X�R�A
 int gameScore = 0;

// �e�̏����i�[����\����
typedef struct {
    double x;
    double y;
    double z;
    double dirX;
    double dirY;
    double dirZ;
    bool active;
} Bullet;

//�G�̏����i�[����\����
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

//�_�̏����i�[����\����
typedef struct {
    double x;
    double y;
    double z;
} Object;

//��Q���̏����i�[����\����
typedef struct {
    double x;
    double y;
    double z;
    float width;
    float height;
    float depth;
} HitObject;

//�A�C�e���̏����i�[����\����
typedef struct {
    float x;
    float y;
    float z;
    float r;
    float k;
    bool active;
}Item;

Bullet bullets[MAX_BULLETS]; // �e�̔z��
Enemy enemies[MAX_ENEMIES]; //�G�̔z��
Object clouds[objectNumber];//�_�̔z��
HitObject objects[HitObjectNumber];//�Ԃ��镨�̂̔z��
Item items[itemNumber];//�A�C�e���̔z��

//�G�̔����ʒu
float spawnPositions[4][2] = {
    {12.5,12.5},
    {12.5,-12.5},
    {-12.5,12.5},
    {-12.5,-12.5}
};

//------�v���g�^�C�v�錾------//
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

//------�֐�------//

// �e������������֐�
void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

//�G���l�����ɐ����B���łɃN���A��̏���
void spawnEnemy(int timer) {
    if (timer != 0) return; //0�ԈȊO�̃^�C�}�[�͖���
    int numEnemiesToSpawn = 10;//���Ԗ��ɔ�������G�̐�
    for (int i = 0; i < numEnemiesToSpawn; i++) {
        int index = -1;
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            int spawnIndex = rand() % 4;//�����_���ɍ��W��I��

            enemies[index].x = spawnPositions[spawnIndex][0];
            enemies[index].y = 2.0;
            enemies[index].z = spawnPositions[spawnIndex][1];
            enemies[index].active = true;
        }
    }

    //�Q�[���I������
    gameTime++;

    if (gameTime >= GAMECLEARTIME) {

        if (!gameOver) {
            gameClear = true;
        }
        
    }

    if (!gameClear) {
        //1�b��ɍēx�G�������X�P�W���[��
        glutTimerFunc(1000, spawnEnemy, 0);
    }
    else {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].active = false;
        }
    }
    
}

//�e�̔��˃^�C�}�[
void Timer(int timer) {

    if (timer != 2)return;

    if (!gameOver) {
        shootBullet();
    }
    

    glutTimerFunc(bulletRate, Timer, 2);
}

//�ŏ��̓G
void initEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }

    srand(time(NULL));
    spawnEnemy(0);
}

//�_�̈ʒu�ݒ�
void initClouds() {
    for (int i = 0; i < objectNumber; i++) {
        clouds[i].x = (rand() % 61 - 30);
        clouds[i].y = 10;
        clouds[i].z = (rand() % 61 - 30);
    }
}

//�A�C�e���̈ʒu�ݒ�
void initItems() {
    for (int i = 0; i < itemNumber; i++) {
        items[i].x = (rand() % 61 - 30);
        items[i].y = 2;
        items[i].z = (rand() % 61 - 30);
        items[i].r = 0.5;//�A�C�e���̔��a
        items[i].k = 0;//�A�C�e���̎�ނ�ݒ�B�g���@�\�̈�Ƃ��Đݒ�
        items[i].active = true;
    }
}

//���̂̈ʒu�ݒ�
void initObjects() {

    //���
    objects[0].x = 12.5;
    objects[0].y = 2;
    objects[0].z = 12.5;
    objects[0].width = 5;
    objects[0].height = 5;
    objects[0].depth = 5;

    //�Q��
    objects[1].x = -12.5;
    objects[1].y = 2;
    objects[1].z = 12.5;
    objects[1].width = 5;
    objects[1].height = 5;
    objects[1].depth = 5;

    //3��
    objects[2].x = -12.5;
    objects[2].y = 2;
    objects[2].z = -12.5;
    objects[2].width = 5;
    objects[2].height = 5;
    objects[2].depth = 5;

    //4��
    objects[3].x = 12.5;
    objects[3].y = 2;
    objects[3].z = -12.5;
    objects[3].width = 5;
    objects[3].height = 5;
    objects[3].depth = 5;

    //�O�̕�
    //5��
    objects[4].x = 35;//25
    objects[4].y = 2;
    objects[4].z = 0;
    objects[4].width = 1;
    objects[4].height = 1;
    objects[4].depth = 50;

    //6��
    objects[5].x = -35;
    objects[5].y = 2;
    objects[5].z = 0;
    objects[5].width = 1;
    objects[5].height = 1;
    objects[5].depth = 50;

    //7��
    objects[6].x = 0;
    objects[6].y = 2;
    objects[6].z = 35;
    objects[6].width = 50;
    objects[6].height = 1;
    objects[6].depth = 1;

    //8��
    objects[7].x = 0;
    objects[7].y = 2;
    objects[7].z = -35;
    objects[7].width = 50;
    objects[7].height = 1;
    objects[7].depth = 1;
}

// �e�ƓG�̓����蔻����s���֐�
void checkCollision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].active) {
                    // �e�ƃ^�[�Q�b�g�̋������v�Z
                    double distance = sqrt(pow(bullets[i].x - enemies[j].x, 2) +
                        pow(bullets[i].y - enemies[j].y, 2) +
                        pow(bullets[i].z - enemies[j].z, 2));

                    double totalRadius = 0.005 + 0.1;

                    // ���̋����ȉ��œ����蔻��
                    if (distance < totalRadius) { // �K�X�������K�v����
                        bullets[i].active = false; // �e������
                        enemies[j].active = false; // �^�[�Q�b�g������
                        gameScore += 1000;
                        break;
                    }
                }
            }
        }
    }
}

//�G�ƃv���C���[�̓����蔻����s���֐�
void checkCollisionWithEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // �J�����ƓG�̋������v�Z
            double distance = sqrt(pow(cameraX - enemies[i].x, 2) +
                pow(cameraY - enemies[i].y, 2) +
                pow(cameraZ - enemies[i].z, 2));

            double totalRadius = 0.1;

            // �Փ˔���
            if (distance < totalRadius) {
                // �Q�[���I�[�o�[����
                gameOver = true;

            }
        }
    }
}

//���̂ƃv���C���[�̓����蔻����s���֐�
void checkCollisionWithObjects() {
    for (int i = 0; i < HitObjectNumber; i++) {
        //���̂̒��S���W���v�Z
        float centerX = objects[i].x;
        float centerY = objects[i].y;
        float centerZ = objects[i].z;

        //�v���C���[�ƕ��̂̋������v�Z
        float distanceX = fabs(cameraX - centerX) - objects[i].width;
        float distanceY = fabs(cameraY - centerY) - objects[i].height;
        float distanceZ = fabs(cameraZ - centerZ) - objects[i].depth;

        if (distanceX < 0 && distanceY < 0 && distanceZ < 0) {
            //�Փ˔���

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

//�A�C�e���ƃv���C���[�̓����蔻����s���֐�
void checkCollisionWithItems() {
    for (int i = 0; i < itemNumber; i++) {
        if (items[i].active) {
            // �J�����ƃA�C�e���̋������v�Z
            double distance = sqrt(pow(cameraX - items[i].x, 2) +
                pow(cameraY - items[i].y, 2) +
                pow(cameraZ - items[i].z, 2));

            if (distance < items[i].r) {
                if (items[i].k == 0) {
                    //���˃��[�g�A�b�v
                    bulletRate = bulletRate - 200;

                    if (bulletRate <= 0) {
                        bulletRate = 100;
                    }
                }
                else if(items[i].k==1){
                    //�V�����Z�ǉ��A����͏����Ȃ�
                }
                items[i].active = false;
            }
        }
    }
}

//�����[�h����
void reload() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

// �e�𔭎˂���֐�
void shootBullet() {

    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = cameraX;
            bullets[i].y = cameraY;
            bullets[i].z = cameraZ;

            // �J�����̕����x�N�g�����v�Z
            bullets[i].dirX = targetX - cameraX;
            bullets[i].dirY = targetY - cameraY;
            bullets[i].dirZ = targetZ - cameraZ;

            // �x�N�g���̒�����1�ɐ��K��
            double length = sqrt(bullets[i].dirX * bullets[i].dirX + bullets[i].dirY * bullets[i].dirY + bullets[i].dirZ * bullets[i].dirZ);
            bullets[i].dirX = bullets[i].dirX / length;
            bullets[i].dirY = bullets[i].dirY / length;
            bullets[i].dirZ = bullets[i].dirZ / length;

            bullets[i].active = true;
            break;
        }

    }

    ////�����[�h���̃t���O�𗧂Ă�
    //isReloading = true;

    ////�����[�h���Ԍo�ߌ�Ƀ����[�h���̃t���O�����Z�b�g
    //glutTimerFunc(reloadTime * 1000, resetReload, 0);

}

//��ʂɏ\����\��
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

//�e��G�̈ړ�����
void IncAngle(int timer)
{
    if (timer != 1) return; //1�ԈȊO�̃^�C�}�[�͖���

    //�e����������
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

    //�e�����������[�h
    if (bullets[MAX_BULLETS - 1].active == true) {
        reload();
    }

    //�G����������
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // �G�̕����x�N�g�����v�Z
            enemies[i].dirX = cameraX - enemies[i].x;
            enemies[i].dirZ = cameraZ - enemies[i].z;

            // �x�N�g���̒�����1�ɐ��K��
            double length = sqrt(enemies[i].dirX * enemies[i].dirX + enemies[i].dirZ * enemies[i].dirZ);
            enemies[i].dirX = enemies[i].dirX / length;
            enemies[i].dirZ = enemies[i].dirZ / length;

            //�G�̈ʒu���X�V
            enemies[i].x += enemies[i].dirX * enemySpeed;
            enemies[i].z += enemies[i].dirZ * enemySpeed;

        }
    }
    glutTimerFunc(T, IncAngle, 1);
    glutPostRedisplay();
}

//���p��
void Prism(int N)
{
    double angle = 2 * 3.1415 / N;
    int i;
    glPushMatrix();

    glRotated(45, 0, 1, 0);

    glBegin(GL_POLYGON); //���
    glNormal3d(0, +1, 0);
    for (i = 0; i < N; i++)
    {
        glVertex3d(cos(i * angle), +1.0, sin(i * angle));
    }
    glEnd();
    glBegin(GL_POLYGON); //����
    glNormal3d(0, -1, 0);
    for (i = 0; i < N; i++)
    {
        glVertex3d(cos(i * angle), -1.0, sin(i * angle));
    }
    glEnd();
    glBegin(GL_QUAD_STRIP); //����
    for (i = 0; i <= N; i++)
    {
        glNormal3d(cos((i - 0.5) * angle), 0, sin((i - 0.5) * angle));
        glVertex3d(cos(i * angle), +1.0, sin(i * angle));
        glVertex3d(cos(i * angle), -1.0, sin(i * angle));
    }
    glEnd();

    glPopMatrix();
}

//�L�[�{�[�h����
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
            //�ړ������̃x�N�g�����v�Z
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //���K��
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);

            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            //�l�ێ������ق�����������
            cameraX += moveSpeed * (dirX2);
            //cameraY += moveSpeed * (targetY - cameraY);
            cameraZ += moveSpeed * (dirZ2);

            targetX += moveSpeed * (dirX2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ += moveSpeed * (dirZ2);
            break;
        case 's':
            //�ړ������̃x�N�g�����v�Z
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //���K��
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
            //�ړ������̃x�N�g�����v�Z
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //���K��
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);
            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            // �J�������������Ɉړ�
            cameraX -= moveSpeed * (-dirZ2);
            //cameraY += moveSpeed * 0;
            cameraZ -= moveSpeed * (dirX2);

            targetX -= moveSpeed * (-dirZ2);
            //cameraY += moveSpeed * (targetY - cameraY);
            targetZ -= moveSpeed * (dirX2);
            break;
        case 'd':
            //�ړ������̃x�N�g�����v�Z
            dirX2 = targetX - cameraX;
            //float dirY2 = targetY - cameraY;
            dirZ2 = targetZ - cameraZ;

            //���K��
            length2 = sqrt(dirX2 * dirX2 + dirZ2 * dirZ2);
            dirX2 = dirX2 / length2;
            //dirY2 = bullets[i].dirY / length;
            dirZ2 = dirZ2 / length2;

            // �J�������E�����Ɉړ�
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

            //��ŃR�����g�A�E�g����������
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

//�}�E�X�ړ�����
void mouseMove(int x, int y) {
    //�}�E�X�̈ړ��ʂ��v�Z
    float deltaX = x - mouseX;
    float deltaY = y - mouseY;

    //�^�[�Q�b�g�̒l��ێ�
    float a = targetX - cameraX;
    float b = targetZ - cameraZ;

    //�����ړ���
    float redeltaX = deltaX * mouseSensitivity;

    //�����ω��p�x
    float sita = 0.005 * redeltaX;

    targetX = a * cos(sita) - b * sin(sita) + cameraX;
    targetZ = a * sin(sita) + b * cos(sita) + cameraZ;

    if (!cursor) {
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

        // �}�E�X�J�[�\���̍��W���X�V
        mouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        mouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    }
    

    glutPostRedisplay();
}

//�}�E�X�N���b�N����
void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    if (gameClear||gameOver) {
        gameOver = false;
        gameClear = false;
        gameTime = 0;
        bulletRate = 1000;
        gameScore = 0;

        //�A�C�e���̕`��
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

//���̋��𐶐��B�A�C�e���̌�����
void drawEnemy(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, purple);
    glLightfv(GL_LIGHT0, GL_AMBIENT, purple);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

//�G�L�����̌�����
void drawEnemy2(float x, float y, float z, float sita) {

    glPushMatrix();

    glTranslated(0, -0.1, 0);

    //��
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cyan);
    glLightfv(GL_LIGHT0, GL_AMBIENT, cyan);
    glTranslated(0, 0.1, 0);

    //��]����
    glRotated(sita, 0, 1, 0);

    glScaled(0.7, 0.7, 0.7);
    glutSolidSphere(0.1, 20, 20);

    //��
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

    //��
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
    //����
    glPushMatrix();
    glTranslatef(x, y, z);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cyan);
    glLightfv(GL_LIGHT0, GL_AMBIENT, cyan);

    //��]
    glRotated(sita, 0, 1, 0);

    glScaled(0.75, 1.25, 0.75);//1,0.3,1
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    glPushMatrix();

    //��
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

    //��
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

    //��
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

    //��
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

//�_�̌�����
void drawCloud(float x, float y, float z) {

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glTranslated(x, y, z); //y��10���炢
    glScaled(2, 0.5, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

}

//��Q��
void drawObject(float x, float y, float z, float sizeX, float sizeY, float sizeZ) {

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT0, GL_AMBIENT, yellow);
    glTranslated(x, y, z); //y��10���炢
    glScaled(sizeX, sizeY, sizeZ);
    Prism(4);
    glPopMatrix();

}

//�����̕\��
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

//�\���p�̊֐�
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

    //�A�C�e���̕`��
    for (int i = 0; i < itemNumber; i++) {
        if (items[i].active == true) {
            drawEnemy(items[i].x, items[i].y, items[i].z, items[i].r);
        }
        
    }

    //�����Ȃ��`��
    for (int i = 0; i < objectNumber; i++) {
        drawCloud(clouds[i].x, clouds[i].y, clouds[i].z);
    }

    //�����蔻��̂��镨�̂̕`��

    glPushMatrix();

    for (int i = 0; i < HitObjectNumber; i++) {
        //�X�e�[�W��̕���
        drawObject(objects[i].x, objects[i].y, objects[i].z, objects[i].width, objects[i].height, objects[i].depth);

    }

    glPopMatrix();

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // �e��`��
            glPushMatrix();
            glLightfv(GL_LIGHT0, GL_POSITION, positionR);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
            glLightfv(GL_LIGHT0, GL_AMBIENT, red);
            glTranslated(bullets[i].x, bullets[i].y, bullets[i].z);
            glutSolidSphere(0.005, 20, 20); // ���̂�`��
            glPopMatrix();
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {

        if (enemies[i].active) {

            enemies[i].sita = atan2(enemies[i].dirX, enemies[i].dirZ) * 180 / PI;

            drawEnemy2(enemies[i].x, enemies[i].y, enemies[i].z, enemies[i].sita);
        }

    }

    //��
    glPushMatrix();
    glShadeModel(GL_FLAT);
    glLightfv(GL_LIGHT0, GL_POSITION, positionR);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT0, GL_AMBIENT, yellow);
    glTranslated(0, 1, 0);
    glScaled(floorX, 0.01, floorZ);
    Prism(4);
    glPopMatrix();

    // 2D�̕`��
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // 2D�̕`��R�[�h�������ɏ��� 

    DrawCross();//�\���̕`��

    if (gameClear) {
       
        //�e�L�X�g�̕\��
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

        //�e�L�X�g�̕\��
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

//���C���֐�
int main(int argc, char** argv)
{

    //���y�Đ�
    PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

    glutInit(&argc, argv);

    glutInitWindowPosition(200, 0);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

    glutCreateWindow("3D shooting game");

    glClearColor(1.0f, 0.75f, 0.8f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 0.1, 1000.0);//����p�A�A�X�y�N�g��A�ǂ�����A�ǂ��܂ŕ\��

    if (!cursor) {
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

        // �}�E�X�J�[�\���̍��W���X�V
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

