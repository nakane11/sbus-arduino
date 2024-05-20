/************************************************************/
/*  Arduino用S.BUS通信プログラム                               */
/*  　　--Arduinoから外付け論理反転回路を介してS.BUSサーボに接続     */
/*     --target_angle配列にIDごとの目標角度を格納                */
/*     --S.BUS_ID 1~6 の動作に対応　                           */
/************************************************************/
#define SBUS_SPEED    100000     //SBUS通信速度（9600 or 100000）
#define Peri1         1000       //動作周期設定

char sbus_data[25] = {
  0x0f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00
};
short  sbus_servo_id[16];                            //送信用指令角度格納配列（SBUS用変換後）

void setup() {
  Serial.begin(SBUS_SPEED, SERIAL_8E2); // 100kbps 1データ8bit　パリティEven StopBit:2bit
}

void loop() { 
  static int timer1 = 0, phase1 = 0;
  int i = 0, n;

  if ( (Peri1 < timer1++) && phase1 == 0) {
    
    /*IDごとの角度入力*/
    setServoAngle(0, 0.0);
    setServoAngle(1, 0.0);
    setServoAngle(2, 0.0);
    setServoAngle(3, 0.0);
    setServoAngle(4, 0.0);
    setServoAngle(5, 0.0);
    
    sendSbusData();          

    /*フラグ、タイマークリア*/
    timer1 = 0;
    phase1 = 1;
  } else if ( (Peri1 < timer1) && phase1 == 1) {
    
    /*IDごとの角度入力*/
    setServoAngle(0, -90.0);
    setServoAngle(1, 90.0);
    setServoAngle(2, 30.0);
    setServoAngle(3, -30.0);
    setServoAngle(4, 60.0);
    setServoAngle(5, -60.0);
    
    sendSbusData();

    /*フラグ、タイマークリア*/
    timer1 = 0;
    phase1 = 0;
  }
  delay(1);        // 1ms_wait
}

void sendSbusData(void){
    /* sbus送信用に変換した目標角度をS.BUS送信用バッファに詰め込む  */
    sbus_data[0] = 0x0f;
    sbus_data[1] =  sbus_servo_id[0] & 0xff;
    sbus_data[2] = ((sbus_servo_id[0] >> 8) & 0x07 ) | ((sbus_servo_id[1] << 3 ) );
    sbus_data[3] = ((sbus_servo_id[1] >> 5) & 0x3f ) | (sbus_servo_id[2]  << 6);
    sbus_data[4] = ((sbus_servo_id[2] >> 2) & 0xff ) ;
    sbus_data[5] = ((sbus_servo_id[2] >> 10) & 0x01 ) | (sbus_servo_id[3] << 1 )   ;
    sbus_data[6] = ((sbus_servo_id[3] >> 7) & 0x0f ) | (sbus_servo_id[4]  << 4 )   ;
    sbus_data[7] = ((sbus_servo_id[4] >> 4) & 0x7f ) | (sbus_servo_id[5]  << 7 )   ;
    sbus_data[8] = ((sbus_servo_id[5] >> 1) & 0xff ) ;
    sbus_data[9] = ((sbus_servo_id[5] >> 9) & 0x03 ) ;

    /* sbus_dataを25bit分送信 */  
    Serial.write(sbus_data, 25); 
}

void setServoAngle(int id, float angle) {
  if (id < 0 || id >= 16) return; // IDが範囲外の場合は何もしない
  sbus_servo_id[id] = (int)(  10.667 * (double)(angle + 90.0) + 64);
}