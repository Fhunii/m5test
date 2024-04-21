// Last update: 2021-09-26 20:00:00

#include <Arduino.h>
#define LGFX_M5STACK_CORE2
#include <M5Core2.h>
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

int room = 0;

int room0_flag = 0;

int combo = 0;

float circle_x = 0.0F;
float circle_y = 0.0F;

float pitch = 0.0F;
float roll = 0.0F;
float yaw = 0.0F;

float tempitch = 0.0F;
float temroll = 0.0F;
float temyaw = 0.0F;

float repitch = 0.0F;
float reroll = 0.0F;
float reyaw = 0.0F;

int i = 0;
int qi = 0;
int printq = 0;

int quiz_time = 0;
int millis_time = 0;
int limit_time = 0;

int quiz_difficulty = 0;

int quiz_limit = 5000;

int goal_st = 0;

int highscore = 0;

int ans[4] = {0,0,0,0};
int j;

int test = 0;

static LGFX lcd;
static LGFX_Sprite sprite(&lcd);     // スプライト１を生成（描画先：lcd（ディスプレイ））

int question = 0;

string quiz;

void vibration() {

    M5.Axp.SetLDOEnable(3, true);  // 3番をtrueにしてバイブレーション開始
    delay(50);                   // バイブレーションの長さ（ms）はお好みで調整
    M5.Axp.SetLDOEnable(3, false); // 3番をfalseにしてバイブレーション修了

}

void setup()
{
    M5.begin();    // Init M5Core.  M5Coreを初期化する
    M5.IMU.Init(); // Init IMU sensor.  IMUセンサを初期化する
    lcd.init();
    lcd.setBrightness(255);
    srand(time(NULL));

    circle_x = lcd.width() / 2;
    circle_y = lcd.height() / 2;
    
    sprite.setColorDepth(8);
    sprite.createSprite(lcd.width(), lcd.height()); 

}

void loop()
{       
    
    M5.update();
    if (room == -1) {
        combo = 0;
        pitch = 0.0F;
        roll = 0.0F;
        yaw = 0.0F;
        tempitch = 0.0F;
        temroll = 0.0F;
        temyaw = 0.0F;
        repitch = 0.0F;
        reroll = 0.0F;
        reyaw = 0.0F;
        i = 0;
        qi = 0;
        printq = 0;
        quiz_time = 0;
        millis_time = 0;
        limit_time = 0;
        quiz_difficulty = 0;
        goal_st = 0;
        test = 0;
        quiz = "";
        question = 0;
        M5.Lcd.fillScreen(BLACK);
        circle_x = lcd.width() / 2;
        circle_y = lcd.height() / 2;
        room0_flag = 0;
        room = 0;
        for (j = 0; j < 4; j++)
            ans[j] = 0;
    }
    if (room == 0) {
        if (room0_flag == 0) {
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.setTextSize(3);
            M5.Lcd.printf("select difficulty level.\nhighscore:");
            M5.Lcd.printf("%d", highscore);
            M5.Lcd.printf("\n\n    Difficulty\n\n\n   A <<< B <<< C");
            room0_flag = 1;

        }
        if (M5.BtnA.isPressed()) {

            room = 1;
            quiz_difficulty = 1;
            M5.Lcd.fillScreen(BLACK);
            room0_flag = 0;
            
        }
        if (M5.BtnB.wasReleased()) {

            room = 1;
            quiz_difficulty = 2;
            M5.Lcd.fillScreen(BLACK);
            room0_flag = 0;
            
        }
        if (M5.BtnC.isPressed()) {

            room = 1;
            quiz_difficulty = 3;
            M5.Lcd.fillScreen(BLACK);
            room0_flag = 0;

        }

    }
    if (room == 1){
        if (room0_flag == 0){

            M5.Lcd.setCursor(0, 0);
            M5.Lcd.setTextSize(3);
            M5.Lcd.print("Press B button to start.\nPress A button to reset pitch.\n        BBB\n        BBB\n        BBB\n       BBBBB\n        BBB\n   A     B     C");
            room0_flag = 1;

        }
        if (M5.BtnB.isPressed()) {

            room = 2;
            M5.Lcd.fillScreen(BLACK);
            quiz_time = millis();
            
        }

    }

    if (room == 2){
        if (M5.BtnA.isPressed()) {

            M5.Lcd.fillScreen(GREEN);
            M5.Lcd.setCursor(0,0);
            M5.Lcd.print("resetting");    
            M5.Axp.SetLDOEnable(3, true);

            circle_x = lcd.width() / 2;
            circle_y = lcd.height() / 2;

        }

        if (M5.BtnA.wasReleased()){

            M5.IMU.getAhrsData(
            &tempitch, &temroll,&temyaw);

            M5.Lcd.fillScreen(BLACK);
            M5.Axp.SetLDOEnable(3, false);

        }

        repitch = pitch - tempitch ;
        reroll = roll - temroll;
        reyaw = yaw - temyaw;

        M5.IMU.getAhrsData(
            &pitch, &roll,
            &yaw); // Stores the inertial sensor attitude.  存储惯性传感器的姿态
        sprite.fillScreen(BLACK);
        sprite.setCursor(0, 0);

        if (millis() - quiz_time > quiz_limit){

            printq = 4;
            
        }

        quiz_limit = (10 /((combo / 10) + 1)) * (10 / quiz_difficulty) * 70 ;

        if (printq == 4){

            sprite.setCursor(20, 65);
            sprite.setTextSize(5);
            sprite.printf("GAME OVER");
            sprite.setCursor(50, 20);
            sprite.setTextSize(2);
            sprite.printf("press B to restart.");
            limit_time = quiz_limit;
            if (highscore <= combo)
                highscore = combo;

            if (M5.BtnB.wasReleased()) 
                room = -1;
        }

        if (printq != 4) {
            
            limit_time = millis() - quiz_time;
            
            if (printq == 2) {
                if (ans[goal_st % 4] == 1) { //正解
                    printq = 3;
                    srand(time(NULL));
                    question = 0 + (int)(rand() * (quiz_difficulty * 5 - 0 + 1.0) / (1.0 + RAND_MAX));
                    quiz_time = millis();
                    millis_time = 0;
                    vibration();
                    combo += 1;
                } else { //不正解
                    printq = 4;
                    M5.Axp.SetLDOEnable(3, true);  // 3番をtrueにしてバイブレーション開始
                    delay(3000);                   // バイブレーションの長さ（ms）はお好みで調整
                    M5.Axp.SetLDOEnable(3, false); // 3番をfalseにしてバイブレーション修了
                }
            }

            if (printq == 3) {
                quiz = "";
                for (j = 0; j < 4; j++)
                    ans[j] = 0;
                printq = 0;
            }

            for (qi = 0; qi < (question / 4); qi++) {
                if (printq == 0) {
                    quiz += "\n     not";
                    for (j = 0; j < 4; j++)
                        ans[j] = !ans[j];
                }
            }

            if (question % 4 == 0) {
                if (printq == 0) {
                    quiz += "\n     up";
                    ans[0] = !ans[0];
                    printq = 1;
                }
            }
            
            if (question % 4 == 1) {
                if (printq == 0) {
                    quiz += "\n     down";
                    ans[1] = !ans[1];  
                    printq = 1;
                }   
            }

            if (question % 4 == 2) {
                if (printq == 0) {
                    quiz += "\n     right";
                    ans[2] = !ans[2]; 
                    printq = 1;
                }      
            }

            if (question % 4 == 3) {
                if (printq == 0) {
                    quiz += "\n     left";
                    ans[3] = !ans[3]; 
                    printq = 1;
                }
            }

            // 円の座標の指定
            circle_x += repitch / 2 ;
            circle_y += reroll / 2 ;
            if (circle_y <= 0) {
                circle_y += lcd.height();
                goal_st = 0;
                printq = 2;
            }
            if (circle_y >= lcd.height()) {
                circle_y += -lcd.height();
                goal_st = 1;
                printq = 2;
            } 
            if (circle_x >= lcd.width()) {
                circle_x += -lcd.width();
                goal_st = 2;
                printq = 2;
            }
            if (circle_x <= 0) {
                circle_x = lcd.width();
                goal_st = 3;
                printq = 2;
            }
            sprite.fillRect(lcd.width() * limit_time/quiz_limit/2, lcd.height() * limit_time/quiz_limit/2, 
            lcd.width() * (quiz_limit-limit_time)/quiz_limit, lcd.height() * (quiz_limit-limit_time)/quiz_limit, RED);
        }
        // sprite.setTextSize(5);
        // sprite.printf("%d",printq);
        // sprite.setTextSize(1);
        // sprite.setCursor(0, 120);
        // sprite.printf("up_j,  down_j,  right_j,  left_j");
        // sprite.setCursor(0, 20);
        // sprite.printf("%d  %d  %d  %d",ans[0], ans[1], ans[2], ans[3]);
        // sprite.setCursor(0, 165);
        // sprite.printf("%5.2f  %5.2f  %5.2f ", repitch, reroll, reyaw);
        
        sprite.setCursor(15, 95);
        sprite.setTextSize(3);
        sprite.printf(quiz.c_str());
        sprite.setCursor(15, 10);
        sprite.setTextSize(3);
        
        sprite.printf("%d",quiz_limit - (limit_time));
        sprite.setCursor(lcd.width()-50, 10);
        sprite.setTextSize(3);
        
        sprite.printf("%d",combo);
        // sprite.setCursor(60, 50);
        // sprite.setTextSize(3);
        // sprite.printf("%d",test);
        
        sprite.fillCircle(circle_x, circle_y, 6, BLUE);
        sprite.pushSprite(0,0);

        i += 1 ;
        
        // delay(10); // Delay 10ms.  延迟10ms
    }
}