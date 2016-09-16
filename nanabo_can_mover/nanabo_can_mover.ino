// 200mlのコーヒー缶を持ち上げて移動させるサンプルプログラム

#include <Arm7Bot.h>
#include <ForceFilter.h>
#include <MedianFilter.h>
#include <PressFilter.h>
#include <PVector.h>

// ロボットアームオブジェクト
Arm7Bot arm;

// バルブ操作ピンと、ポンプ操作ピン
const int VALVE_PIN = 10;
const int PUMP_PIN = 11;

// nanaboサーボ角度配列(DEG)
double angles[SERVO_NUM] =  {90, 145, 60, 90, 90, 90, 0};

// サーボM4の角度を調整して、バキュームが真下を向くようにする
int set_angle4(){
  angles[4] = 130 + 90 - angles[2];
}

// リンクAとリンクBからなる角φ'を得る
int get_phi_dash(){
  return 125 - angles[1] + angles[3];
}

void setup() {
  // シリアル通信の準備をする
  Serial.begin(9600);
  Serial.println("starting");

  // 移動スピードを10（ゆっくり目）にする
  for(int i=0; i<6; i++){
    arm.maxSpeed[i] = 30;
  }

  // nanaboに初期ポーズをとらせる
  Serial.println("moving initial pose");
  set_angle4();
  arm.move(angles);
  delay(1000);

  // サーボM1とサーボM2の角度を調整し、
  // アーム長を25cmに、仰角を45°に設定する
  Serial.println("moving initial2 pose");
  angles[1] = 75;
  angles[2] = 50;
  arm.move(angles);
  delay(3000);

  // バキュームの初期設定を行う
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(VALVE_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
}

// 以下、繰り返し
void loop() {
  // 移動スピードを10（ゆっくり目）にする
  for(int i=0; i<6; i++){
    arm.maxSpeed[i] = 30;
  }

  // アームを左に45°回転させる
  Serial.println("pose left-upper");
  angles[0] -= 45;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // アームの仰角を調整し、先端（バキューム）を缶のある高さまで落とす
  Serial.println("pose left-lower");
  angles[1] -= 20;
  angles[2] += 20;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // バキュームの吸引を開始する
  Serial.println("pump up");
  digitalWrite(VALVE_PIN, LOW);
  digitalWrite(PUMP_PIN, HIGH);
  delay(2000);

  // 仰角を調整し、缶を持ち上げる
  Serial.println("pose left-upper");
  angles[1] += 20;
  angles[2] -= 20;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // アームを右に90°回転させる
  Serial.println("pose right-upper");
  angles[0] += 90;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // アームの仰角を調整し、缶を地面に降ろす
  Serial.println("pose right-lower");
  angles[1] -= 20;
  angles[2] += 20;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // バキュームの吸引を停止する
  Serial.println("pump off");
  digitalWrite(VALVE_PIN, HIGH);
  digitalWrite(PUMP_PIN, LOW);
  delay(2000);

  // 仰角を調整し、缶を離す
  Serial.println("pose right-upper");
  angles[1] += 20;
  angles[2] -= 20;
  set_angle4();
  arm.move(angles);
  delay(2000);

  // アームを左に45°回転させ、初期姿勢に戻る
  Serial.println("pose middle");
  angles[0] -= 45;
  set_angle4();
  arm.move(angles);
  delay(2000);
}

