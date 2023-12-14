#include <L298N.h>

#include <QTRSensors.h>

#include "BluetoothSerial.h"

//Se incluyen las bibliotecas necesarias para controlar un motor con el puente H L298N, para trabajar con sensores QTR y para la comunicación Bluetooth.

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define AIN1 2  //Assign the motor pins
#define BIN1 4
#define AIN2 5
#define BIN2 18
#define PWMA 19
#define PWMB 21
//#define STBY 

//Se definen los pines del Arduino que se utilizarán para controlar el puente H L298N y otros pines de entrada/salida.

const int offsetA = 1;
const int offsetB = 1;

//Se definen desplazamientos (offsets) para ajustar el comportamiento de los motores.

L298N motor1(PWMA, AIN1, AIN2);
L298N motor2(PWMB, BIN1, BIN2);

//Se inicializan dos motores utilizando la biblioteca L298N.

QTRSensors qtr;
BluetoothSerial SerialBT;

//Se crea una instancia de objetos para trabajar con sensores QTR y comunicación Bluetooth.

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int threshold[SensorCount];

//Se establece el número de sensores y se definen arreglos para almacenar los valores de los sensores y los umbrales.

float Kp = 0;
float Ki = 0;
float Kd = 0;

//Se inicializan variables para los coeficientes del control PID.

uint8_t multiP = 1;
uint8_t multiI  = 1;
uint8_t multiD = 1;
uint8_t Kpfinal;
uint8_t Kifinal;
uint8_t Kdfinal;
float Pvalue;
float Ivalue;
float Dvalue;

//Se inicializan variables para ajustes adicionales y cálculos del PID.

boolean onoff = false;

//Se inicializa una variable para controlar si el robot debe estar encendido o apagado.

int val, cnt = 0, v[3];

uint16_t position;
int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 230;

void setup()
{
  // Configuración de los sensores QTR y espera durante la calibración.
  // Inicialización del puerto serie y del Bluetooth.
  // Configuración del LED incorporado.
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){32, 33, 25, 26, 27, 14, 12, 13}, SensorCount);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
  
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");
  // analogRead() tarda unos 0.1 ms en un AVR.
  // 0.1 ms por sensor * 4 muestras por sensor leído (por defecto) * 6 sensores
  // * 10 lecturas por llamada a calibrate() = ~24 ms por llamada a calibrate().
  // Llama a calibrate() 400 veces para hacer que la calibración tarde unos 10 segundos.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // apagar el LED de Arduino para indicar que hemos terminado con la calibración
  
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    threshold[i] = (qtr.calibrationOn.minimum[i] + qtr.calibrationOn.maximum[i])/2;
    Serial.print(threshold[i]);
    Serial.print("  ");
  }
  Serial.println();

  delay(1000);
}

void loop()
{
  // Lectura de comandos Bluetooth y procesamiento.
  // Control del robot si está encendido.  
  if (SerialBT.available()){
    while(SerialBT.available() == 0);
    valuesread();
    processing();
  }
  if (onoff == true){
    robot_control();
  }
  else if(onoff == false){
    motor1.stop();
    motor2.stop();
  }
}
void robot_control(){
  // Control del robot basado en la lectura de sensores y aplicación de PID.
  //leer los valores calibrados de los sensores y obtener una medida de la posición de la línea
  //de 0 a 7000 (para una línea blanca, utilice readLineWhite() en su lugar)
  position = qtr.readLineBlack(sensorValues);
  error = 3500 - position;
  Serial.println(position);
  while(sensorValues[0]>=980 && sensorValues[1]>=980 && sensorValues[2]>=980 && sensorValues[3]>=980 && sensorValues[4]>=980 && sensorValues[5]>=980 && sensorValues[6]>=980 && sensorValues[7]>=980){ 
    
  //caso cuando el seguidor de línea abandona la línea

    if(previousError>0){      //Girar a la izquierda si la línea estaba antes a la izquierda
      motor_drive(-230,230);
    }
    else{
      motor_drive(230,-230); // Si no, gira a la derecha
    }
    position = qtr.readLineBlack(sensorValues);
  }
  
  PID_Linefollow(error);
  
}
void PID_Linefollow(int error){

    // Cálculos PID y ajuste de velocidades de los motores.

    P = error;
    I = I + error;
    D = error - previousError;
    
    Pvalue = (Kp/pow(10,multiP))*P;
    Ivalue = (Ki/pow(10,multiI))*I;
    Dvalue = (Kd/pow(10,multiD))*D; 

    float PIDvalue = Pvalue + Ivalue + Dvalue;
    previousError = error;

    lsp = lfspeed - PIDvalue;
    rsp = lfspeed + PIDvalue;

    if (lsp > 255) {
      lsp = 255;
    }
    if (lsp < -255) {
      lsp = -255;
    }
    if (rsp > 255) {
      rsp = 255;
    }
    if (rsp < -255) {
      rsp = -255;
    }
    motor_drive(lsp,rsp);
}

void valuesread()  {

  // Lectura de valores desde Bluetooth.
  
  val = SerialBT.read();
  cnt++;
  v[cnt] = val;
  if (cnt == 2)
    cnt = 0;
}

void  processing() {

  // Procesamiento de valores recibidos y ajuste de parámetros.

  int a = v[1];
  if (a == 1) {
    Kp = v[2];
  }
  if (a == 2) {
    multiP = v[2];
  }
  if (a == 3) {
    Ki = v[2];
  }
  if (a == 4) {
    multiI = v[2];
  }
  if (a == 5) {
    Kd  = v[2];
  }
  if (a == 6) {
    multiD = v[2];
  }
  if (a == 7)  {
    onoff = v[2];
  }
}
void motor_drive(int left, int right){

  // Control de los motores izquierdo y derecho.
  
  if(right>0
  )
  {
    motor2.setSpeed(right);
    motor2.forward();
  }
  else 
  {
    motor2.setSpeed(right);
    motor2.backward();
  }
  
 
  if(left>0)
  {
    motor1.setSpeed(left);
    motor1.forward();
  }
  else 
  {
    motor1.setSpeed(left);
    motor1.backward();
  }
}