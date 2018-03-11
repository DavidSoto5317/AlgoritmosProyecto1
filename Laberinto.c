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
*/
#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"                          // Include abdrive header
#include "ping.h"                             // Include ping header

int irLeft, irRight; //Se inicializan las variables para los sensores laterales Infrared
int sensor_pin = 8; //Se inicializa la variable del pin al que se conecta SIG del Sensor Utrasonico PING

/*
 //Metodo para mover al robot una revolucion de sus ruedas
 */
void move() 
{
  drive_goto(64,64); //(rueda izquierda, rueda derecha)
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
  drive_goto(26,-26); //(rueda izquierda, rueda derecha)
}
/*
 //Metodo que pemite al robot encontrar un camino libre de paredes
 */  
void find_path() 
{
  while(ping_cm(sensor_pin) < 4){ //Mientras el Sensor Ultrasonico PING detecte un objeto a menos de 4 cm
    turn_left(); //Gira a la izquierda
    pause(2000); //Toma una pausa
  }
  move(); //Se mueve una vez que encuentra un camino libre
  pause(2000); //Toma una pausa   
}
/*
 Metodo que permite que el robot siga el Algoritmo de la mano derecha modificado
 @param right El cual indica si es un 1 que no hay nada a la derecha del robot y si es 0 indica que hay algo a su derecha 
 @param left El cual indica si es un 1 que no hay nada a la izquierda del robot y si es 0 indica que hay algo a su izquierda
 */
void follow_right_wall(int right, int left){
    if(right == 1){ //Si no hay nada a su derecha
      turn_right(); //Gira  a la derecha
      pause(2000); //Toma una pausa
      move(); //Se mueve
      pause(2000); //Toma otra pausa
      find_path(); //Encuentra el camino
      pause(2000); //Toma otra pausa
    }
    else if(ping_cm(sensor_pin)> 6){ //Si hay algo a la derecha revisa si lo que esta al frente esta lejos para poder avanzar y no chocar
      move(); //Se mueve
      pause(3000);  //Toma una pausa
    }else{ //En caso de que haya algo a la derecha y al frente
      turn_left(); //Gira a la izquierda
      pause(2000); //Toma una pausa
      find_path(); //Encuentra el camino
      pause(2000); //Toma otra pausa
    }  
}  
int main()                                    // Main function
{
  low(26); //Activa los puertos de digital a analogo en bajo
  low(27); //Activa los puertos de digital a analogo en bajo
  // Add startup code here.
  find_path(); //Encuentra el camino
  
  while(1) //Se activa el ciclo
  { 
    /*
    Parametros de los metodos del Activitybot
    Freqout(puerto al que se conecta, encendido, potencia)
    input(puerto al que se conecta
    */
    freqout(11,1,19000); //Hace que la luz infrared de la izquierda se active con una potencia de 19000
    irLeft = input(10); //Permite que el sensor de la izquierda pueda detectar si la luz infraroja rebota en un objeto cercano devolviendo un 0, en caso de que no lo haga devuelve un 1
    pause(100); //Toma una pausa pequeña
    freqout(1,1,19000); //Hace que la luz infrared de la derecha se active con una potencia de 19000
    irRight = input(2); //Permite que el sensor de la derecha pueda detectar si la luz infraroja rebota en un objeto cercano devolviendo un 0, en caso de que no lo haga devuelve un 1
    pause(100); //Toma otra pausa pequeña
    follow_right_wall(irRight,irLeft);  //Se mandan los parametros de los infred al algoritmo de la mano derecha        

  }  
}
