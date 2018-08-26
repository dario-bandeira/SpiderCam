//box dimensions in 'steps'
int boxX = 6888;
int boxY = 5166;
int boxZ = 4920;

//initial position of the ball (center of the box)
int axisX = 3444;
int axisY = 2583;
int axisZ = 2460;

//ports of motors
int motors[4][4] = {
  {22,23,24,25},
  {26,27,28,29},
  {30,31,32,33},
  {34,35,36,37}
};

//where the ball has to go
int destination[3] = {x,y,z};

//an array to store the index of active port of each motor
int activePorts[4] = {0,0,0,0}; // motor0, motor1, motor2, motor3

//an array to store the new length of the wires after calculation
float newWiresLength[4] = {0,0,0,0};

//speed of motors spining. The higher, the slower.
int m_speed = 10;

int howManySteps[4] = {0,0,0,0}; //motor0, motor1, motor2, motor3
float leftOuts[4] = {0,0,0,0};

float calculateHypotenuse(float leg1, float leg2){
  float result;
  result = sqrt( pow(leg1, 2) + pow(leg2, 2) );
  return result;
}

float calculateWire(int destination[3]){

  float aux0 = calculateHypotenuse(axisX, axisY);
  float aux1 = calculateHypotenuse(boxX - axisX, axisY);
  float aux2 = calculateHypotenuse(boxX - axisX, boxY - axisY);
  float aux3 = calculateHypotenuse(axisX, boxY - axisY);
  
  newWiresLength[0] = calculateHypotenuse(aux0, axisZ);
  newWiresLength[1] = calculateHypotenuse(aux1, axisZ);
  newWiresLength[2] = calculateHypotenuse(aux2, axisZ);
  newWiresLength[3] = calculateHypotenuse(aux3, axisZ);
}

//defining initial length to wires (equals at the beginning)
calculateWire(destination);
float wiresLength[4] = {
  newWiresLength[0],
  newWiresLength[1],
  newWiresLength[2],
  newWiresLength[3]
};

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

int getSmaller(int a[4]){
  int aux = a[0];
  for(int x=1; x<=3; x++){
    if(a[x] < aux){
      aux = a[x];
    }
  }
  return aux;
}

void goToDestination(){
  calculateWire(destination);

  for(int x=0; x<4; x++){
    howManySteps[x] = newWiresLength[x] - wiresLength[x];
  }

  int smaller = getSmaller(howManySteps);

  int integerPart;
  float decimalPart;

  //cycles...
  for(int x = smaller; x > 0; x--){
    //applying the leftOuts to the amount of steps to give
    for(int y=0; y<4; y++){
      howManySteps[y] += leftOuts[y];
    }
    //emptying the 'leftOuts' array
    leftOuts[] = {0,0,0,0};

    //giving steps on the 4 motors...
    for(int y=0; y<4; y++){
      integerPart = howManySteps[y] / smaller;
      decimalPart = (howManySteps[y] / smaller) - floor(howManySteps[y]);

      leftOuts[y] = decimalPart;
      giveSteps(y, integerPart);
    }

  }//end of cycles
}


//movo os fios
//gravo a posição atual
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
  destination[] = {800,y,z};
  goToDestination();
  destination[] = {6000,y,z};
  goToDestination();
}
