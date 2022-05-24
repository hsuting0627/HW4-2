#include "mbed.h"
#include "bbcar.h"
BufferedSerial pc(USBTX, USBRX);

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D10), pin6(D11);
DigitalIn encoder(D3);
volatile int steps;
volatile int last;

BBCar car(pin5, pin6, servo_ticker);

DigitalInOut pin1(D4);
DigitalInOut pin2(D5);
DigitalInOut pin3(D6);
DigitalInOut pin4(D7);

int Sensor1;
int Sensor2;
int Sensor3;
int Sensor4;
int wL = 0;
int wR = 0;
int check;
void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}
// main() runs in its own thread in the OS
int main()
{
    ThisThread::sleep_for(500ms);
    encoder_ticker.attach(&encoder_control, 10ms);
    printf("start\n");
    while (true) {
        pin1.output();
        pin2.output();
        pin3.output();
        pin4.output();

        pin1.write(1);
        pin2.write(1);
        pin3.write(1);
        pin4.write(1);

        ThisThread::sleep_for(1ms);

        pin1.input();
        pin2.input();
        pin3.input();
        pin4.input();

        pin1.write(0);
        pin2.write(0);
        pin3.write(0);
        pin4.write(0);

        wait_us(200);
        Sensor1 = pin1.read();
        Sensor2 = pin2.read();
        Sensor3 = pin3.read();
        Sensor4 = pin4.read();
        //printf("%d, %d, %d, %d \n", Sensor1,Sensor2, Sensor3,Sensor4);
        //printf("%d, %d\n", Sensor1 ,Sensor2);
        if (Sensor1 == 1 && Sensor2 == 0 && Sensor3 == 0 && Sensor4 == 0){
            car.turn(30, -0.1);
           // printf("Right a lot\n");
        }
        else if (Sensor1 == 1 && Sensor2 == 1 && Sensor3 == 0 && Sensor4 == 0){
            car.turn(30, -0.3);
            //printf("Right a mid\n");
        }
        else if (Sensor1 == 0 && Sensor2 == 1 && Sensor3 == 0 && Sensor4 == 0){
            car.turn(30, -0.6);     
            //printf("Right a little\n");  
        }
        else if (Sensor1 == 0 && Sensor2 == 1 && Sensor3 == 1 && Sensor4 == 0){
            car.goStraight(20);
            //printf("straight\n");        
        }
        else if (Sensor1 == 0 && Sensor2 == 0 && Sensor3 == 1 && Sensor4 == 0){
            car.turn(30, 0.6);
            //printf("left a little\n");  
        }
        else if (Sensor1 == 0 && Sensor2 == 0 && Sensor3 == 1 && Sensor4 == 1){
            car.turn(30, 0.3);  
            //printf("left a middle\n");  
        }  
        else if (Sensor1 == 0 && Sensor2 == 0 && Sensor3 == 0 && Sensor4 == 1){
            car.turn(30, 0.1);
            //printf("left a lot\n");    
        }  
        else if (Sensor1 == 0 && Sensor2 == 0 && Sensor3 == 0 && Sensor4 == 0) {
            car.stop();
            //printf("stop\n");
        }
        else if (Sensor1 == 1 && Sensor2 == 1 && Sensor3 == 1 && Sensor4 == 1) {
            car.stop();
            if (check == 1) printf("%f \n", steps * 6.5 * 3.14 / 32);
            ThisThread::sleep_for(3s);
            steps = 0;
            check = 1;
            car.goStraight(20);
            ThisThread::sleep_for(2s);
        }
        ThisThread::sleep_for(100ms);
    }
}


