/**********************/
#include <ros.h>
#include <geometry_msgs/Twist.h>
#define button 24
/**********************/
class NewHardware : public ArduinoHardware
{
  public:
    NewHardware(): ArduinoHardware(&Serial1, 115200) {};
};
/**********************/
ros::NodeHandle_<NewHardware> nh;
geometry_msgs::Twist vel;
ros::Publisher butter("cmd_vel", &vel);
/**********************/
void setup()
{
  pinMode(button, INPUT);
  nh.initNode();
  nh.advertise(butter);
}
/**********************/
void loop()
{
  if (digitalRead(button) == HIGH) {
    vel.linear.x = 0.1;
    butter.publish( &vel);
    delay(1000);
  }
  else if (digitalRead(button) == LOW) {
    vel.linear.x = 0.0;
    butter.publish( &vel );
    delay(1000);
  }
  nh.spinOnce();
}
/**********************/
