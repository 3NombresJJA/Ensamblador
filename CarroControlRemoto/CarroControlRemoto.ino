#include <L298N.h>
#include "BluetoothSerial.h"

//Se incluyen las bibliotecas necesarias para controlar un motor con el puente H L298N y para la comunicación Bluetooth.

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define AIN1 2  //Asigna los motor pins
#define BIN1 4
#define AIN2 5
#define BIN2 18
#define PWMA 19
#define PWMB 21

//Se definen los pines del Arduino que se utilizarán para controlar el puente H L298N y otros pines de entrada/salida.

const int offsetA = 1;
const int offsetB = 1;

//Se definen desplazamientos (offsets) para ajustar el comportamiento de los motores.

L298N motor1(PWMA, AIN1, AIN2);
L298N motor2(PWMB, BIN1, BIN2);

//Se inicializan dos motores utilizando la biblioteca L298N.

BluetoothSerial SerialBT;
//Se define el modo de comuncacion por Bluetooth

float Kp = 0;

float Pvalue=1;
//Se inician las variables para saber la direccion en la que debe ir el robot

boolean onoff = false;

//Se inicializa una variable para controlar si el robot debe estar encendido o apagado.

int val, cnt = 0, v[3];
//Se inician las variables que recicibiran los datos por Bluetooth
void setup()
{
  
  Serial.begin(115200);
  //Se comienza la comunicacion serial
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");
  //Se crea el enlaze de comunicacion por Bluetooth
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
  else{
    Serial.println("No conectado");
  }
  if (onoff == true){
    Serial.println("Actu");
    Control_remoto();
  }
  else if(onoff == false){
    motor1.stop();
    motor2.stop();
  }
  delay(20);
}

void Control_remoto(){
//El control es una barra deslizable la cual entrega valores desde 0 hasta 100
    Serial.println(Kp);
    Pvalue = (Kp);
    if (Pvalue<=30){
      Serial.println("Izq");
      motor1.setSpeed(150);
      motor1.forward();
      motor2.setSpeed(50);
      motor2.backward();
      //El auto girara a la izquierda si la barra controladora tiene valores menores a 30
    }
    else if (Pvalue<60){
      Serial.println("Delante");
      motor2.setSpeed(150);
      motor2.forward();
      motor1.setSpeed(150);
      motor1.forward();
       //El auto seguira derecho si la barra controladora tiene valores menores a 60 pero superiores a 30
    }
    else{
      Serial.println("Der");
      motor2.setSpeed(150);
      motor2.forward();
      motor1.setSpeed(50);
      motor1.backward();
       //El auto girara a la derecha si la barra controladora tiene valores superiores a 60
    }

 
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

  // Procesamiento de valores recibidos 

  int a = v[1];
  if (a == 1) {
    Kp = v[2];
  }
  if (a == 7)  {
    onoff = v[2];
  }
}
