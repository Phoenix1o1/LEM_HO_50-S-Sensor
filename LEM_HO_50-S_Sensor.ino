/*
  Read LEM HO 50-S-0100 Current Sensor
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

  //crate a function for floating point value conversions:
float conversion(float x, float in_min, float in_max, float out_min, float out_max) {

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

  //create a function to average readings:
float movingAverage(float value) {
  const byte nvalues = 20;             // Moving average window size
  static byte current = 0;            // Index for current value
  static byte cvalues = 0;            // Count of values read (<= nvalues)
  static float sum = 0;               // Rolling sum
  static float values[nvalues];       //create an array of readings

  sum += value;

  // If the window is full, adjust the sum by deleting the oldest value
  if (cvalues == nvalues)
    sum -= values[current];

  values[current] = value;          // Replace the oldest with the latest

  if (++current >= nvalues)         //return to array start
    current = 0;

  if (cvalues < nvalues)            //move to next array position
    cvalues += 1;

  return sum/cvalues;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float refValue = analogRead(A5) * 10;
  //Serial.println(refValue);
  float sensorValue = analogRead(A4) * 10;
  //Serial.println(sensorValue);
  float diff = (refValue-sensorValue);
  //Serial.println(diff);

  //map analog values to sensor parameters
  //difference between REF and ACTUAL, minimum analog value, maximum analog value, max sensor value, min sensor value
  float amp = conversion(diff,-4100, 4100, 125, -125);
  //average the last measurements
  amp = movingAverage(amp);
  // print out the averaged value:
  Serial.print("AMPERE: ");
  Serial.println(amp);
  //delay
  delay(100);
}
