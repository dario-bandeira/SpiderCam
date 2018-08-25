//box dimensions in 'steps'
int boxX = 6888;
int boxY = 5166;
int boxZ = 4920;

//initial position of the ball (center of the box)
int axisX = 3444;
int axisY = 2583;
int axisZ = 2460;

float calculateHypotenuse(float leg1, float leg2){
  float result;
  result = sqrt( pow(leg1, 2) + pow(leg2, 2) );
  return result;
}

float calculateWire(int wire){
  float aux = 0;

  // calculating the hypotenuse of the first triangle
  if(wire == 0){
    aux = calculateHypotenuse(axisX, axisY);
  } else if(wire == 1){
    aux = calculateHypotenuse(boxX - axisX, axisY);
  } else if(wire == 2){
    aux = calculateHypotenuse(boxX - axisX, boxY - axisY);
  } else if(wire == 3) {
    aux = calculateHypotenuse(axisX, boxY - axisY);
  } else {
    Serial.println("Wrong wire!");
  }
  
  // calculating the hypotenuse of the second triangle,
  // which is the wire
  aux = calculateHypotenuse(aux, axisZ);
  return aux;
}

//defining initial length to wires (equals at the beginning)
float aux = calculateWire(0);
float wiresLength[4] = {aux,aux,aux,aux};

//ports of motors
int motors[4][4] = {
  {22,23,24,25},
  {26,27,28,29},
  {30,31,32,33},
  {34,35,36,37}
};

//an array to store the index of active port of each motor
int activePorts[4] = {0,0,0,0}; // motor0, motor1, motor2, motor3

//an array to store the new length of the wires after calculation
float newWiresLength[4] = {0,0,0,0};

//speed of motors spining. The higher, the slower.
int m_speed = 10;

void giveSteps(int motor, int steps){
  digitalWrite(motors[motor][activePorts[motor]],LOW); //turn off the current port
  
  if(steps > 0){ //positive step
    
    for(int x = 0; x < steps; x++){    
      if(activePorts[motor] == 3){
        activePorts[motor] = 0;
      } else {
        activePorts[motor] += 1;
      }
      digitalWrite(motors[motor][activePorts[motor]],HIGH);
      delay(m_speed);
      digitalWrite(motors[motor][activePorts[motor]],LOW);
    }
    
  } else if(steps < 0){ //negative step, not 0
    
    for(int x = steps; x > 0; x--){
      if(activePorts[motor] == 0){
        activePorts[motor] = 3;
      } else {
        activePorts[motor] -= 1;
      }
      digitalWrite(motors[motor][activePorts[motor]],HIGH);
      delay(m_speed);
      digitalWrite(motors[motor][activePorts[motor]],LOW);
    }
  }
}


float copyArray(float arrayA[], float arrayB[]){ //previously declared, 4 positions only
  for(int x=0; x<4; x++){
    arrayB[x] = arrayA[x];
  }
}


void setup() {
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(28,OUTPUT);
  pinMode(29,OUTPUT);
  pinMode(30,OUTPUT);
  pinMode(31,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(34,OUTPUT);
  pinMode(35,OUTPUT);
  pinMode(36,OUTPUT);
  pinMode(37,OUTPUT);

  //turning on the initial ports of each motor
  digitalWrite(motors[0][activePorts[0]],HIGH);
  digitalWrite(motors[1][activePorts[1]],HIGH);
  digitalWrite(motors[2][activePorts[2]],HIGH);
  digitalWrite(motors[3][activePorts[3]],HIGH);
  
  Serial.begin(9600);
}

void loop() {
  // moving the ball just on X axis
  // between x = 800 and x = 6000
  while(axisX > 800){
    axisX--;
    for(int x=0; x<4; x++){
      newWiresLength[x] = calculateWire(x);
    }
    
    for(int x=0; x<4; x++){
      giveSteps(x, newWiresLength[x] - wiresLength[x]);
    }
  }

  while(axisX < 6000){
    axisX++;
    for(int x=0; x<4; x++){
      newWiresLength[x] = calculateWire(x);
    }
    
    for(int x=0; x<4; x++){
      giveSteps(x, newWiresLength[x] - wiresLength[x]);
    }
  }
}
