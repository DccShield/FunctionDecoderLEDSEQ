//--------------------------------------------------------------------------------
// DCC Smile Function Decoder
// LEDシーケンスデコーダ
// [FunctionDecoderLEDSEQ.ino]
// Copyright (c) 2020 Ayanosuke(Maison de DCC) / Desktop Station
//
// http://maison-dcc.sblo.jp/ http://dcc.client.jp/ http://ayabu.blog.shinobi.jp/
// https://twitter.com/masashi_214
//
// DCC電子工作連合のメンバーです
// https://desktopstation.net/tmi/ https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------

#include <arduino.h>
#include "DccCV.h"
#include "NmraDcc.h"
#include "SeqLight.h"
#include <avr/eeprom.h>   //required by notifyCVRead() function if enabled below

#define DEBUG      //リリースのときはコメントアウトすること
//#define DEBUG_M    //リリースのときはコメントアウトすること 速度・ファンクションパケット表示

//各種設定、宣言

#define PIN_F0_F  3     // D3 PD3,PWM
#define PIN_F0_R  4     // D4 PD4
#define PIN_AUX1  5     // D5 PD5
#define PIN_AUX2  6     // D6 PD6
#define PIN_AUX3  7     // D7 PD7
#define PIN_AUX4  8     // D8 PB0
#define PIN_AUX5  9     // D9 PB1,DIGITAL TR,PWM
#define PIN_AUX6  10    // D10 PB2,DIGITAL TR,PWM
#define PIN_AUX7  11    // D11 PB3,DIGITAL TR,PWM

//void Dccinit(void);

//使用クラスの宣言
NmraDcc   Dcc;
DCC_MSG  Packet;

struct CVPair {
  uint16_t  CV;
  uint8_t Value;
};

CVPair FactoryDefaultCVs [] = {
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DECODER_ADDRESS}, // CV01
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},               // CV09 The LSB is set CV 1 in the libraries .h file, which is the regular address location, so by setting the MSB to 0 we tell the library to use the same address as the primary address. 0 DECODER_ADDRESS
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},          // CV17 XX in the XXYY address
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},          // CV18 YY in the XXYY address
  {CV_29_CONFIG, 128 },   //Make sure this is 0 or else it will be random based on what is in the eeprom which could caue headaches
  {CV_49_F0_FORWARD_LIGHT, 0},  // 通常の点灯
  {CV_dummy,0},
};

void(* resetFunc) (void) = 0;  //declare reset function at address 0

uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);

SeqLight HeadLight = SeqLight(PIN_F0_F, 0);

//void notifyDccReset(uint8_t hardReset );



//------------------------------------------------------------------
// Arduino固有の関数 setup() :初期設定
//------------------------------------------------------------------
void setup()
{
  //ファンクションの割り当てピン初期化

//  pinMode(PIN_F0_F, OUTPUT);
//  digitalWrite(PIN_F0_F, ON);
  HeadLight.OnOff(OFF);

  pinMode(PIN_F0_R, OUTPUT);
  digitalWrite(PIN_F0_R, OFF);
  pinMode(PIN_AUX1, OUTPUT);
  digitalWrite(PIN_AUX1, OFF);
  pinMode(PIN_AUX2, OUTPUT);
  digitalWrite(PIN_AUX2, OFF);
  pinMode(PIN_AUX3, OUTPUT);
  digitalWrite(PIN_AUX3, OFF);
  pinMode(PIN_AUX4, OUTPUT);
  digitalWrite(PIN_AUX4, OFF);
  pinMode(PIN_AUX5, OUTPUT);
  digitalWrite(PIN_AUX5, OFF);
  pinMode(PIN_AUX6, OUTPUT);
  digitalWrite(PIN_AUX6, OFF);
  pinMode(PIN_AUX7, OUTPUT);
  digitalWrite(PIN_AUX7, OFF);

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hello,Smile Function Decoder");
#endif
  
  Dccinit();

  //Reset task
  gPreviousL5 = millis();
}

//---------------------------------------------------------------------
// Arduino main loop
//---------------------------------------------------------------------
void loop() {
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

  if ( (millis() - gPreviousL5) >= 10){ // 100:100msec  10:10msec  Function decoder は 10msecにしてみる。
    HeadLight.stateCheck();
    FunctionProcess();
    gPreviousL5 = millis();
  }
}

//---------------------------------------------------------------------
//ファンクション受信によるイベント
//---------------------------------------------------------------------
void FunctionProcess(void){
// F0 受信時の処理
    if(gState_F0 > 0) {                   // DCC F0 コマンドの点灯処理
      if( gDirection == 1){                // Reverse 前進(DCS50Kで確認)
//      digitalWrite(PIN_F0_F, HIGH);
        HeadLight.OnOff(ON);
        digitalWrite(PIN_F0_R, LOW);
      } else {                             // Forward 後進(DCS50Kで確認)
//      digitalWrite(PIN_F0_F, LOW);
        HeadLight.OnOff(OFF);
        digitalWrite(PIN_F0_R, HIGH);
      }
    }
    if(gState_F0 == 0) {
//      digitalWrite(PIN_F0_F, LOW);  
        HeadLight.OnOff(OFF);
        digitalWrite(PIN_F0_R, LOW);
    }

// F1 受信時の処理
    if(gState_F1 == 0){
      digitalWrite(PIN_AUX1, LOW);
    } else {
      digitalWrite(PIN_AUX1, HIGH);
    }
    
// F2 受信時の処理
    if(gState_F2 == 0){
      digitalWrite(PIN_AUX2, LOW);
    } else {
      digitalWrite(PIN_AUX2, HIGH);
    }

// F3 受信時の処理
    if(gState_F3 == 0){
      digitalWrite(PIN_AUX3, LOW);
    } else {
      digitalWrite(PIN_AUX3, HIGH);
    }
    
// F4 受信時の処理
    if(gState_F4 == 0){
      digitalWrite(PIN_AUX4, LOW);
    } else {
      digitalWrite(PIN_AUX4, HIGH);
    }

// F5 受信時の処理
    if(gState_F5 == 0){
      digitalWrite(PIN_AUX5, LOW);
    } else {
      digitalWrite(PIN_AUX5, HIGH);
    }

// F6 受信時の処理
    if(gState_F6 == 0){
      digitalWrite(PIN_AUX6, LOW);
    } else {
      digitalWrite(PIN_AUX6, HIGH);
    }

// F7 受信時の処理
    if(gState_F7 == 0){
      digitalWrite(PIN_AUX7, LOW);
    } else {
      digitalWrite(PIN_AUX7, HIGH);
    }

}

//---------------------------------------------------------------------
//DCC速度信号の受信によるイベント
//extern void notifyDccSpeed( uint16_t Addr, uint8_t Speed, uint8_t ForwardDir, uint8_t MaxSpeed )
//---------------------------------------------------------------------
extern void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps )
{
    uint16_t aSpeedRef = 0;
  //速度値の正規化(255を100%とする処理)
  if( Speed >= 1)
  {
    aSpeedRef = ((Speed - 1) * 255) / SpeedSteps;
  }
  else
  {
    //緊急停止信号受信時の処理 //Nagoden comment 2016/06/11
    #ifdef DEBUG
        Serial.println("***** Emagency STOP **** ");
    #endif
    aSpeedRef = 0;
  
  }

  if ( gDirection != Dir ){
    gDirection = Dir;
  }
  gSpeedRef = Speed;

  #ifdef DEBUG_M
    // デバッグメッセージ
    Serial.print("Speed - ADR: ");
    Serial.print(Addr);
    Serial.print(", SPD: ");
    Serial.print(Speed);
    Serial.print(", DIR: ");
    Serial.print(Dir);
    Serial.print(", aSpeedRef: ");
    Serial.println(aSpeedRef);
  #endif
}


//---------------------------------------------------------------------------
//ファンクション信号受信のイベント
//FN_0_4とFN_5_8は常時イベント発生（DCS50KはF8まで）
//FN_9_12以降はFUNCTIONボタンが押されたときにイベント発生
//前値と比較して変化あったら処理するような作り。
//---------------------------------------------------------------------------
//extern void notifyDccFunc( uint16_t Addr, FN_GROUP FuncGrp, uint8_t FuncState)
extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
  #ifdef DEBUG_M
    // デバッグメッセージ
    Serial.println("notifyDccFunc()");
    Serial.print("Addr:");
    Serial.print(Addr);
    Serial.print(", AddrType:");
    Serial.print(AddrType);
    Serial.print(", FuncGrp: ");
    Serial.print(FuncGrp,HEX);
    Serial.print(", FuncState: ");
    Serial.println(FuncState,HEX);
  #endif
  
  if( FuncGrp == FN_0_4)  // F0〜F4の解析
  {
    if( gState_F0 != (FuncState & FN_BIT_00))
    {
      //Get Function 0 (FL) state
      gState_F0 = (FuncState & FN_BIT_00);
    }
    if( gState_F1 != (FuncState & FN_BIT_01))
    {
      //Get Function 1 state
      gState_F1 = (FuncState & FN_BIT_01);
    }
    if( gState_F2 != (FuncState & FN_BIT_02))
    {
      gState_F2 = (FuncState & FN_BIT_02);
    }
    if( gState_F3 != (FuncState & FN_BIT_03))
    {
      gState_F3 = (FuncState & FN_BIT_03);
    }
    if( gState_F4 != (FuncState & FN_BIT_04))
    {
      gState_F4 = (FuncState & FN_BIT_04);
    }
  }

  if( FuncGrp == FN_5_8)  // F5〜F8の解析
  {
    if( gState_F5 != (FuncState & FN_BIT_05))
    {
      //Get Function 0 (FL) state
      gState_F5 = (FuncState & FN_BIT_05);
    }
    if( gState_F6 != (FuncState & FN_BIT_06))
    {
      //Get Function 1 state
      gState_F6 = (FuncState & FN_BIT_06);
    }
    if( gState_F7 != (FuncState & FN_BIT_07))
    {
      gState_F7 = (FuncState & FN_BIT_07);
    }
    if( gState_F8 != (FuncState & FN_BIT_08))
    {
      gState_F8 = (FuncState & FN_BIT_08);
    }
  }
  if( FuncGrp == FN_9_12)  // F9〜F12の解析
  {
    if( gState_F9 != (FuncState & FN_BIT_09))
    {
      gState_F9 = (FuncState & FN_BIT_09);
    }
    if( gState_F10 != (FuncState & FN_BIT_10))
    {
      gState_F10 = (FuncState & FN_BIT_10);
    }
    if( gState_F11 != (FuncState & FN_BIT_11))
    {
      gState_F11 = (FuncState & FN_BIT_11);
    }
    if( gState_F12 != (FuncState & FN_BIT_12))
    {
      gState_F12 = (FuncState & FN_BIT_12);
    }
  }

  if( FuncGrp == FN_13_20)   // F13〜F20の解析
  {
    if( gState_F13 != (FuncState & FN_BIT_13))
    {
      gState_F13 = (FuncState & FN_BIT_13);
    }
    if( gState_F14 != (FuncState & FN_BIT_14))
    {
      gState_F14 = (FuncState & FN_BIT_14);
    }
    if( gState_F15 != (FuncState & FN_BIT_15))
    {
      gState_F15 = (FuncState & FN_BIT_15);
    }
    if( gState_F16 != (FuncState & FN_BIT_16))
    {
      gState_F16 = (FuncState & FN_BIT_16);
    }
  }
  
}




//------------------------------------------------------------------
// CVをデフォルトにリセット(Initialize cv value)
// Serial.println("CVs being reset to factory defaults");
//------------------------------------------------------------------
void resetCVToDefault()
{
  for (int j = 0; j < FactoryDefaultCVIndex; j++ ) {
    Dcc.setCV( FactoryDefaultCVs[j].CV, FactoryDefaultCVs[j].Value);
  }
};

//------------------------------------------------------------------
// CV8 によるリセットコマンド受信処理
//------------------------------------------------------------------
void notifyCVResetFactoryDefault()
{
  //When anything is writen to CV8 reset to defaults.

  resetCVToDefault();
  delay(1000);  //typical CV programming sends the same command multiple times - specially since we dont ACK. so ignore them by delaying
  resetFunc();
};

//------------------------------------------------------------------
// CV Ackの処理
// そこそこ電流を流さないといけない
//------------------------------------------------------------------
void notifyCVAck(void)
{
//サーボモータを動かすとギミックを壊しかねないのでコメントアウト
//Serial.println("notifyCVAck");
#if 0 
  digitalWrite(O1,HIGH);
  digitalWrite(O2,HIGH);
  digitalWrite(O3,HIGH);
  digitalWrite(O4,HIGH);
  delay( 6 );
  digitalWrite(O4,LOW);
  digitalWrite(O3,LOW);
  digitalWrite(O2,LOW);
  digitalWrite(O1,LOW);
#endif
//MOTOR_Ack();
}

void MOTOR_Ack(void)
{
//  analogWrite(O4, 128);
//  delay( 6 );  
//  analogWrite(O4, 0);
}

//------------------------------------------------------------------
// DCC初期化処理）
//------------------------------------------------------------------
void Dccinit(void)
{

  //DCCの応答用負荷ピン
#if defined(DCCACKPIN)
  //Setup ACK Pin
  pinMode(DccAckPin, OUTPUT);
  digitalWrite(DccAckPin, 0);
#endif

#if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
  if ( Dcc.getCV(CV_MULTIFUNCTION_PRIMARY_ADDRESS) == 0xFF ) {   //if eeprom has 0xFF then assume it needs to be programmed
    notifyCVResetFactoryDefault();
  } else {
    Serial.println("CV Not Defaulting");
  }
#else
  Serial.println("CV Defaulting Always On Powerup");
  notifyCVResetFactoryDefault();
#endif

  // Setup which External Interrupt, the Pin it's associated with that we're using, disable pullup.
  Dcc.pin(0, 2, 0);   // ArduinoNANO D2(PD2)pinをDCC信号入力端子に設定

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_NUMBER, 100,   FLAGS_MY_ADDRESS_ONLY , 0 );

  //Init CVs
  gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;

  //Init CVs
  gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
  gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) + Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );

  //Init CVs
  gCV49_fx = Dcc.getCV( CV_49_F0_FORWARD_LIGHT ) ;
  HeadLight.Change( gCV49_fx);

  //E2P-ROMからCV値を読み込む

}


//------------------------------------------------------------------
// CV値が変化した時の処理（特に何もしない）
//------------------------------------------------------------------
extern void     notifyCVChange( uint16_t CV, uint8_t Value) {
   //CVが変更されたときのメッセージ
  #ifdef DEBUG
    Serial.print("CV "); 
    Serial.print(CV); 
    Serial.print(" Changed to "); 
    Serial.println(Value, DEC);
  #endif
};


//------------------------------------------------------------------
// Resrt処理（特に何もしない）
//------------------------------------------------------------------
void notifyDccReset(uint8_t hardReset )
{
}
