#include <Arduino.h>
#include "RMCS2303.h"
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>

RMCS2303 rmcs;


byte slave_id = 6;
int INP_CONTROL_MODE = 513;
int PP_gain = 32;
int PI_gain = 16;
int VF_gain = 32;
int LPR = 41;
int acceleration = 300;
int sp = 80;
int theta = 270;
long int r = (12532 / 360) * theta;  // Put the CPR value  of your motor in place of 12532.
long int current_position;
long int current_speed;


int RATE = (1 / 1000) * 1000;

ros::NodeHandle nh;
std_msgs::Int32 position;
std_msgs::Int32 speed;

void messageCb(const std_msgs::Int32 &state) {

  theta = state.data;
  if (theta == 0)
    r = 0;
  else
    r = (12532 * theta / 360);

  rmcs.Absolute_position(slave_id, r);
  nh.loginfo("data received");
}

ros::Publisher position_pub("position_out", &position);
ros::Publisher speed_pub("speed", &speed);
ros::Subscriber<std_msgs::Int32>
  position_sub("position_in", &messageCb);

void setup() {
//  nh.getHardware()->setBaud(9600);


  rmcs.Serial_selection(0);       //Serial port selection:0-Hardware serial,1-Software serial
  rmcs.Serial0(9600);             //set baudrate for usb serial to monitor data on serial monitor
  rmcs.begin(&Serial1,9600);    //Uncomment if using hardware serial port for mega2560:Serial1,Serial2,Serial3 and set baudrate. Comment this line if Software serial port is in use
  //  rmcs.begin(&myserial,9600);     //Uncomment if using software serial port. Comment this line if using hardware serial.
 
  rmcs.WRITE_PARAMETER(slave_id, INP_CONTROL_MODE, PP_gain, PI_gain, VF_gain, LPR, acceleration, sp);  //Uncomment to write parameters to drive. Comment to ignore.

  // rmcs.Absolute_position(slave_id, 0);  
  nh.initNode();
  nh.subscribe(position_sub);

  nh.advertise(position_pub);
  nh.advertise(speed_pub);
}



void loop() {

  if (nh.connected()) {
    position.data = rmcs.Position_Feedback(slave_id);
    speed.data = rmcs.Speed_Feedback(slave_id);

    position_pub.publish(&position);
    speed_pub.publish(&speed);
  }

  nh.spinOnce();
  delay(RATE);
}