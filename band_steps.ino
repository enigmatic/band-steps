int inputs[5] = {A0,A1,A2,A3,A4};
int testInput = A5;
//int outputs[9] = {12, 11, 10,9,8,7,6,5,4}; //Reversed for easy ribbon cable
int outputs[9] = {4,5,6,7,8,9,10,11,12};
int testLight = 13;

int steps[5][3] = {
  { 0, 0, 1},
  { 1, 2, 3},
  { 3, 4, 5},
  { 5, 6, 7},
  { 7, 8, 8}
};

bool stepState[5] = {false, false, false, false, false};
bool lightState[9];
unsigned long lastDebounceTime[5] = {0,0,0,0,0};  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

void setup() {

  for (int i = 0; i < 5; i++) {
    pinMode(inputs[i], INPUT_PULLUP);
  }

  pinMode(testInput, INPUT_PULLUP);

  for (int i = 0; i < 9; i++) {
    pinMode(outputs[i], OUTPUT);
  }
  pinMode(testLight, OUTPUT);
  clearSteps();
}

void loop() {
  //*//Running
  checkSteps();
  clearSteps();
  for (int i = 0; i < 5; i++) {
    if (stepState[i]) {
      if (millis() - lastDebounceTime[i] > debounceDelay) {
        stepState[i] = false;
      } else {
        lightStep(i);
      }
    }
  }
  triggerLights();
  delay(10); //Add to keep lights triggering
  //*/
}

void testLights() {
  digitalWrite(testLight, HIGH);
  for (int i = 0; i < 5; i++) {
    clearSteps();
    lightStep(i);
    triggerLights();
    delay(1000);
  }

  digitalWrite(testLight, LOW);
  clearSteps();
  triggerLights();
}

void checkSteps() {
  for (int i = 0; i < 5; i++) {
    if (digitalRead(inputs[i]) == LOW) {
      stepState[i] = true;
      lastDebounceTime[i] = millis();
    }
  }

  if (digitalRead(testInput) == LOW) {
    testLights();
  }
}

void clearSteps() {
  for (int i = 0; i < 9; i++) {
    lightState[i] = HIGH;
  }
}

void lightStep(int step) {
  
  for (int i = 0; i < 3; i++) {
    lightState[steps[step][i]] = LOW;
  }
}

void triggerLights() {

  for (int i = 0; i < 9; i++) {
    digitalWrite(outputs[i], lightState[i]);
  }
}
