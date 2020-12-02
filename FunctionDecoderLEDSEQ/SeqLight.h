//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [SeqLight.h]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------
#ifndef SeqLight_h
#define SeqLight_h

  enum{
    FastOn = 1,
    SlowOn,
    TriangleWave,
    Random,
    MarsLight,
    FlashLight,
    SinglePulesStrobe,
    DoublePulseStrobe,
    MediumPulseStrobo,
    FluorescentLamp,
    BrokenFluorescentLamp,
  };

//  enum{
//        OFF = 0,
//        ON,
//  };


class SeqLight
{
public:
  SeqLight(unsigned char ch,unsigned char mode);
  void Change(unsigned char mode);
  void stateCheck();
  void OnOff(unsigned char sw);
  
private:
  int state;// = ST_IDLE;
  unsigned char port;
  unsigned char mode;
  char updownFlg;

  unsigned char adr = 0;      // アドレス
  
  float pwmRef =0;
  float deltPwm;            // 10msあたりのpwm増加量
  
  int nowPwm;
  int nextPwm;
  int styTime;

  unsigned char (*ptn)[3];
  
  enum{
    ST_IDLE = 0,
    ST_ON,
    ST_OFF,
    ST_FARST,
    ST_STAY,
    ST_RUN,
    ST_NEXT,
  };

  enum{
    DOWN = 0,
    STY,
    UP,
  };

//FX効果
//Cmd,Time,Val,Frq
//I:初期状態,O:出力,S:スイープ,L:ループ,E:終了

const unsigned char ptn1[2][3]={
                          {'I',  0,255},
                          {'E',  0,255}}; //10ms すぐ点灯(Fast ON)

const unsigned char ptn2[3][3]={
                          {'I',  0,  0},
                          {'S',100,255},
                          {'E',  0,255}};//10ms もやっと点灯(slow ON)
unsigned char ptn3[4][3]={
                          {'I',  0,  0},
                          {'S', 50,255},
                          {'S', 50,  0},
                          {'L',  0,  0}}; //10ms 三角波(triangle wave)

unsigned char ptn4[6][3]={
                          {'I',  0,  0},
                          {'O', 10,255},
                          {'O', 10,125},
                          {'O', 10,255},
                          {'O', 10, 80},
                          {'L',  0,  0}}; // ランダム(random)

unsigned char ptn5[6][3]={
                          {'I',  0,  0},
                          {'S', 50,255},
                          {'S', 50,  0},
                          {'S', 50,128},
                          {'S', 50,  0},
                          {'L',  0,  0}};//マーズライト(Mars light)

unsigned char ptn6[4][3]={
                          {'I',  0,  0},
                          {'O', 30,255},
                          {'S', 20,128},
                          {'L',  0,  0}};//フラッシュライト(Flash light)

unsigned char ptn7[4][3]={
                          {'I',  0,  0},
                          {'O', 20,255},
                          {'O', 80,  0},
                          {'L',  0,  0}}; //シングルパルスストロボ(single pules strobe)

unsigned char ptn8[6][3]={
                          {'I',  0,  0},
                          {'O', 10,255},
                          {'O', 20,  0},
                          {'O', 10,255},
                          {'O', 60,  0},
                          {'L',  0,  0}}; //ダブルパルスストロボ(Double pulse strobe)

unsigned char ptn9[6][3]={
                          {'I',  0,  0},
                          {'O', 25,128},
                          {'O', 40,255},
                          {'O', 25,128},
                          {'O', 40,  0},
                          {'L',  0,  0}}; //ミディアムパルスストロボ(medium pulse strobo)

unsigned char ptn10[8][3]={
                          {'I',  0,  0},
                          {'O',  1,255},
                          {'O',  1,  0},
                          {'O',  1,255},
                          {'O', 30,  0},
                          {'O', 10, 50},
                          {'O',100,255},
                          {'E',  0, 0}}; //グロー管蛍光灯(Fluorescent lamp)

unsigned char ptn11[9][3]={
                          {'I',  0,  0},
                          {'O',  0,255},
                          {'O',  0,  0},
                          {'O',  0,255},
                          {'O', 30,  0},
                          {'O', 30, 40},
                          {'O', 50,255},
                          {'O', 70,  0},
                          {'L',  0,  0}}; //グロー管蛍光灯切れそう(broken Fluorescent lamp
  
unsigned char ptn12[4][3]={
                          {'I',  0,  0},
                          {'O',  0,255},
                          {'L',  0,  0}};
  
unsigned char ptn13[8][3]={
                          {'I',  0,  0},
                          {'O', 6,255},
                          {'O', 3,  0},
                          {'O', 6,255},
                          {'O', 3, 0},
                          {'O', 6,255},
                          {'O',63,  0},
                          {'L',  0,  0}}; //トリプルフラッシュ


unsigned char ptn14[14][3]={
                          {'I',  0,  0},
                          {'O', 6,255},
                          {'O', 3,  0},
                          {'O', 3,255},
                          {'O', 3,  0},
                          {'O', 6,255},
                          {'O', 3,  0},
                          {'O', 3,255},
                          {'O', 16,  0},
                          {'O', 6,255},
                          {'O', 10,  0},
                          {'O', 6,255},
                          {'O', 13,  0},
                          {'L',  0,  0}}; //ランダムフォー


};

#endif
