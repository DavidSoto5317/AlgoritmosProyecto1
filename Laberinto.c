/*
  Laberinto
  Hecho por: 
  Javier Carpio
  Maria Fernanda Lopez
  David Soto
  Guatemala 09/Marzo/2018
  Este programa permite que el robot de ActivityBot de Parallax
  pueda encontrar la salida de laberintos a traves del algoritmo de la mano 
  derecha modificado
  Version 2.0
*/
#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"                          // Include abdrive header
#include "ping.h"                             // Include ping header

int irLeft, irRight, irL, irR; //Se inicializan las variables para los sensores laterales Infrared
int sensor_pin = 8; //Se inicializa la variable del pin al que se conecta SIG del Sensor Utrasonico PING
int distancia = 0;
int verificadorRight = 0;
/*
 //Metodo para mover al robot una revolucion de sus ruedas
 */
void move() 
{
  drive_goto(32,32); //(rueda izquierda, rueda derecha)
}
/*
 //Metodo para que el robot gire a la izquierda
 */
void turn_left()
{
  drive_goto(-26,26); //(rueda izquierda, rueda derecha)
}
/*
 //Metodo para que el robot gire a la derecha
 */
void turn_right()
{
  drive_goto(62,10); //(rueda izquierda, rueda derecha)
}
/*
 //Metodo que pemite al robot encontrar un camino libre de paredes
 */  
void find_path() 
{
  while(ping_cm(sensor_pin) < 5){ //Mientras el Sensor Ultrasonico PING detecte un objeto a menos de 4 cm
    turn_left(); //Gira a la izquierda
    pause(500); //Toma una pausa
  }  
}
/*
 Metodo que permite que el robot siga el Algoritmo de la mano derecha modificado
 @param right El cual indica si es un 1 que no hay nada a la derecha del robot y si es 0 indica que hay algo a su derecha 
 @param left El cual indica si es un 1 que no hay nada a la izquierda del robot y si es 0 indica que hay algo a su izquierda
 */
int follow_right_wall(int right, int left){
    if(right == 1){ //Si no hay nada a su derecha
      pause(500);
      turn_right(); //Gira  a la derecha
      pause(500); //Toma una pausa
      return 1;
    }
    else if(ping_cm(sensor_pin)< 5){ //Si hay algo a la derecha revisa si lo que esta al frente esta lejos para poder avanzar y no chocar
      pause(500);
      turn_left();
      pause(500);
      return 2;
    }else{ //En caso de que haya algo a la derecha y al frente
    
    }  
}  
int main()                                    // Main function
{
  // Add startup code here.
  low(26); //Activa los puertos de digital a analogo en bajo
  low(27); //Activa los puertos de digital a analogo en bajo

  find_path(); //Encuentra el camino
    
  while(1) //Se activa el ciclo
  { 
    /*
    Parametros de los metodos del Activitybot
    Freqout(puerto al que se conecta, encendido, potencia)
    input(puerto al que se conecta
    */
    while(ping_cm(sensor_pin) > 5){
      freqout(1,1,38000); //Hace que la luz infrared de la derecha se active con una potencia de 38000
      irRight = input(2); //Permite que el sensor de la derecha pueda detectar si la luz infraroja rebota en un objeto cercano devolviendo un 0, en caso de que no lo haga devuelve un 1
      pause(100); //Toma otra pausa pequeña
      verificadorRight = irRight;
      
      //Avanza continuamente si el obstaculo esta a mas de 4 cm
      if(verificadorRight==0){
        drive_speed(64,64);
        pause(100);
        
      //Se detiene y aplica el algoritmo de la mano derecha ya que detecto que no hay nada a su derecha  
      }else{
        drive_speed(0,0);
        freqout(1,1,38000); //Hace que la luz infrared de la derecha se active con una potencia de 19000
        irRight = input(2); //Permite que el sensor de la derecha pueda detectar si la luz infraroja rebota en un objeto cercano devolviendo un 0, en caso de que no lo haga devuelve un 1
        pause(100); //Toma otra pausa pequeña
        follow_right_wall(irRight,irLeft);  //Se mandan los parametros de los infred al algoritmo de la mano derecha
      }
    }
    //Cuando tenga algo al frente va a revisar nuevamente con el algoritmo de la mano derecha (En este punto lo mas probable es que elija izquierda pero se hace el proceso para verificar que los sensores hayan detectado las cosas bien)
    drive_speed(0,0);
    freqout(1,1,38000); //Hace que la luz infrared de la derecha se active con una potencia de 19000
    irRight = input(2); //Permite que el sensor de la derecha pueda detectar si la luz infraroja rebota en un objeto cercano devolviendo un 0, en caso de que no lo haga devuelve un 1
    pause(100); //Toma otra pausa pequeña
    follow_right_wall(irRight,irLeft);  //Se mandan los parametros de los infred al algoritmo de la mano derecha    
  }
}
