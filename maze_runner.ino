#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motor1Pin1  3     // IN1 on motor driver 1
#define motor1Pin2  4     // IN2 on motor driver 1
#define motor1Pin3  5     // IN3 on motor driver 1
#define motor1Pin4  6     // IN4 on motor driver 1

#define motor2Pin1  8     // IN1 on motor driver 2
#define motor2Pin2  9     // IN2 on motor driver 2
#define motor2Pin3  10    // IN3 on motor driver 2

// Wheel settings 
int oneRotation = 4096;
int wheelCircumference = 219.9114857513;

// Instruction # to start on
int indexOfInstructions = 0;

// List of instructions the robot will follow
String instructions[] = {"F340", "L30", "F170", "R90", "F250", "L60", "F200", "R120", "F750", "R100", "F100", "R40", "F100", "L40", "F100", "L40", "F100","R40","F300"};

// Initialize with pin sequence 1-3-2-4
AccelStepper stepper1(HALFSTEP, motor1Pin1, motor1Pin3, motor1Pin2, motor1Pin4);
AccelStepper stepper2(HALFSTEP, motor2Pin1, motor2Pin3, motor2Pin2, motor2Pin4);

// Max speed
void setMaxMotorSpeed(int amount){
  stepper1.setMaxSpeed(amount);
  stepper2.setMaxSpeed(amount);
}

// Max acceleration
void setMotorAcceleration(int amount){
  stepper1.setAcceleration(amount);
  stepper2.setAcceleration(amount);
}

// Desired Speed
void setMotorSpeed(int amount){
  stepper1.setSpeed(amount);
  stepper2.setSpeed(amount);
}

void moveRightMotor(int stepAmount){
  stepper1.move(stepAmount);
}

void moveLeftMotor(int stepAmount){
  stepper2.move(-stepAmount);
}

// Unit conversion
// The more accurate wheelCircumference is set, the more accurate the instruction list is 
int mmToStep(int mm){
  return mm * (oneRotation / wheelCircumference);
}

void forward(int milim){
  moveRightMotor(mmToStep(milim));
  moveLeftMotor(mmToStep(milim));
}

void turn(int direct, int milim){
  if (direct == 0){
    moveLeftMotor(mmToStep(milim));
    moveRightMotor(-mmToStep(milim));

  } else if (direct == 1){
    moveRightMotor(mmToStep(milim));
    moveLeftMotor(-mmToStep(milim));
  }
}

void evalInstruction(String instruction){
  // Decode string instruction
  String operate = instruction.substring(0, 1);
  int amount = (instruction.substring(1, instruction.length())).toInt();

  if (operate == "F"){
    forward(amount);
  }
  if (operate == "R"){
    turn(0, amount);
  }
  if (operate == "L"){
    turn(1, amount);
  }
}

// Default motor settings
void setup() {
  setMaxMotorSpeed(1000);
  setMotorAcceleration(1000);
  setMotorSpeed(1000);
}

void loop() {  
  if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0)
  {
    // if at target evaluate current instruction and index
    evalInstruction(instructions[indexOfInstructions]);
    indexOfInstructions++;
  }
  // run motors 
  stepper1.run();
  stepper2.run();
}
