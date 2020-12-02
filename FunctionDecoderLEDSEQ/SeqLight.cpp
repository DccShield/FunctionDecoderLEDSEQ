//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [SeqLight.cpp]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// SeqLightクラス
//--------------------------------------------------------------------------------
#include <Arduino.h>
#include"SeqLight.h"

// コンストラクタ
SeqLight::SeqLight( unsigned char ch, unsigned char mode)
{
  port = ch;
  pinMode(port, OUTPUT);
  state = ST_IDLE;
  
  switch(mode){
    case 0: ptn = ptn1;
    break;
    case 1: ptn = ptn2;
    break;
    case 2: ptn = ptn3;
    break;
    case 3: ptn = ptn4;    
    break;
    case 4: ptn = ptn5;
    break;
    case 5: ptn = ptn6;
    break;
    case 6: ptn = ptn7;
    break;
    case 7: ptn = ptn8;
    break;
    case 8: ptn = ptn9;
    break;
    case 9: ptn = ptn10;
    break;
    case 10: ptn = ptn11;
    break;
    case 11: ptn = ptn12;
    break;
    case 12: ptn = ptn13;
    break;
    case 13: ptn = ptn14;
    break;
    default: ptn = ptn1;
    break;
  }
}  

void SeqLight::Change(unsigned char mode)
{
  switch(mode){
    case 0: ptn = ptn1;
    break;
    case 1: ptn = ptn2;
    break;
    case 2: ptn = ptn3;
    break;
    case 3: ptn = ptn4;    
    break;
    case 4: ptn = ptn5;
    break;
    case 5: ptn = ptn6;
    break;
    case 6: ptn = ptn7;
    break;
    case 7: ptn = ptn8;
    break;
    case 8: ptn = ptn9;
    break;
    case 9: ptn = ptn10;
    break;
    case 10: ptn = ptn11;
    break;
    case 11: ptn = ptn12;
    break;
    case 12: ptn = ptn13;
    break;
    case 13: ptn = ptn14;
    break;
    default: ptn = ptn1;
    break;
  }
}  

void SeqLight::OnOff(unsigned char sw)
{
  static unsigned char presw = 0;

  if(presw == sw) // 変化がなかったら抜ける（ずっとstate書き換えられてしまう）
    return;

  if( sw == 0 )
    state = ST_OFF;
  else if( sw == 1)
    state = ST_ON;

  presw = sw;
}

//---------------------------------------------------------------------
// FX効果ステートマシン
// 10ms周期で起動
// unsigned chart ptn[4][5]{{'I',0,0,1},{'S',20,255,1},{'S',40,0,1},{'E',0,0,1}};
//---------------------------------------------------------------------
void SeqLight::stateCheck(){

//  static unsigned char adr = 0;      // アドレス
 // static int timeUp = 0;    // 時間
 // static float delt_v = 0;  // 100msあたりの増加量 

#if 0
char t[20];
if(state != 0){
sprintf(t,"ch:%d , state:%d",port,state);
Serial.println(t);   
}
#endif

//Serial.print("state:");    
//Serial.println(state);   

  switch(state){
    case ST_IDLE:
                break;

     case ST_ON:
                adr = 0;
 //               timeUp = 0;
                pwmRef = 0;
                analogWrite( port, pwmRef );          
                state = ST_FARST;
                break;
    case ST_OFF:
                adr = 0;
 //              timeUp = 0;
                pwmRef = 0;
                analogWrite( port, 0 );          
                state = ST_IDLE;
                break;
 
    case ST_FARST: //3
                pwmRef = ptn[adr][2];
                analogWrite( port, pwmRef ); 
                nowPwm = pwmRef;
                //adr++;
                nextPwm = ptn[adr+1][2];
                styTime = ptn[adr+1][1];
#if 0
char t[60],s[10];
dtostrf(deltPwm, 3, 1, s);
sprintf(t,"1: port:%d , adr:%d , nextPwm:%d , deltPwm %s , time %d",port ,adr,nextPwm,s,ptn[adr+1][1]);
Serial.println(t);                  
dtostrf(pwmRef, 3, 1, t);
Serial.println(t);
#endif                
                if(styTime == 0 || ptn[adr+1][0]=='O' ){
                  deltPwm = nextPwm - nowPwm;
                } else
                  deltPwm = (nextPwm - nowPwm) / (float)styTime;             

//dtostrf(deltPwm, 3, 1, t);
//Serial.println(t);
                
                if(nextPwm - nowPwm == 0){
                  updownFlg = STY;
                  //styTime = ptn[adr][1];
                  state = ST_STAY;
                  break;
                } else if(nextPwm - nowPwm < 0)
                  updownFlg = DOWN;
                else
                  updownFlg = UP;
                if(ptn[adr+1][0]=='O')
                  state = ST_STAY;                
                else
                  state = ST_RUN;
                break;
                
    case ST_STAY://4
                  styTime --;
                  if(styTime <= 0)
                    state = ST_NEXT;
                  break;

    case ST_RUN://5
                  pwmRef = pwmRef + deltPwm;
#if 0
dtostrf(deltPwm, 3, 1, s);
sprintf(t,"2: port:%d , adr:%d ,  nextPwm:%d , deltPwm %s , time %d",port , adr,nextPwm,s,ptn[adr][1]);
Serial.println(t);
dtostrf(pwmRef, 3, 1, t);
Serial.println(t);   
#endif
                  if((updownFlg == DOWN) && (pwmRef <= nextPwm)){
                    analogWrite( port,(int)nextPwm);
                    state = ST_NEXT;
                  } else if((updownFlg == UP) && (pwmRef >= nextPwm)){
                    analogWrite( port,(int)nextPwm);
                    state = ST_NEXT;
                  } else {
                    analogWrite( port,(int)pwmRef);
                  }
                  if(ptn[adr][0]=='O')
                    state = ST_STAY;
                  break;
    case ST_NEXT://6
                  adr++;
//sprintf(t,"3: port:%d , adr:%d , %x",port,adr,ptn[adr][0]);
//Serial.println(t);

                  
                  if(ptn[adr][0]=='E'){
                    state = ST_IDLE;
                    break;
                  }
                  if(ptn[adr][0]=='L'){
                    adr = 0;
                    state = ST_FARST;
                    break;
                  }
                  
                  nowPwm = ptn[adr-1][2];
                  nextPwm = ptn[adr][2];
                  styTime = ptn[adr][1];
                if(styTime == 0|| ptn[adr][0]=='O'){
                  deltPwm = nextPwm - nowPwm;
                } else
                  deltPwm = (nextPwm - nowPwm) / (float)styTime;


//                  deltPwm = (nextPwm - nowPwm) / (float)ptn[adr][0];
                  if(nextPwm - nowPwm == 0){
                    updownFlg = STY;
                    styTime = ptn[adr][1];
                    state = ST_STAY;
                    break;
                  }
                  else if(nextPwm - nowPwm < 0)
                    updownFlg = DOWN;
                  else
                    updownFlg = UP;
                  state = ST_RUN;
                  break;
    default:
                  break;
  }
}
