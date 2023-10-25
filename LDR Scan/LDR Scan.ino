const int numSamples = 10000;
int samples[numSamples];
int sampleIndex = 0;
long sampleSum = 0;
float runningAverage = 0.0;
int currentState = 0;
unsigned long previousTime = 0;
unsigned long period = 0;

const int numFreqSamples = 50;
float frequencies[numFreqSamples];
int freqIndex = 0;
float frequencySum = 0.0;

const int thresholdFrequency = 50; // Target frequency in Hz
const int frequencyTolerance = 2;  // Frequency tolerance in Hz

unsigned long startTime = 0; // Time when threshold is reached

void setup() {
  pinMode(D2, OUTPUT);  // Set D2 as an output
  digitalWrite(D2, LOW); // Set D2 to LOW initially
  Serial.begin(2000000);
}

void loop() {
  int sensorValue = analogRead(A0);
  
  sampleSum -= samples[sampleIndex];
  samples[sampleIndex] = sensorValue;
  sampleSum += sensorValue;
  sampleIndex = (sampleIndex + 1) % numSamples;
  runningAverage = static_cast<float>(sampleSum) / numSamples;

  if (sensorValue > runningAverage && currentState == 0) {
    currentState = 1;
    unsigned long currentTime = millis();
    period = currentTime - previousTime;
    previousTime = currentTime;

    const float epsilon = 1e-6;
    float currentFrequency = 1000.0 / (period + epsilon);
    
    frequencySum -= frequencies[freqIndex];
    frequencies[freqIndex] = currentFrequency;
    frequencySum += currentFrequency;
    freqIndex = (freqIndex + 1) % numFreqSamples;
    
    float smoothedFrequency = frequencySum / numFreqSamples;
    
    Serial.println(String(currentFrequency) + " " + String(smoothedFrequency));

    // Check if smoothedFrequency is not NaN
    if (!isnan(smoothedFrequency)) {
      // Check if the smoothed frequency is within the target range
      if (abs(smoothedFrequency - thresholdFrequency) <= frequencyTolerance) {
        if (millis() - startTime >= 5000) {
          digitalWrite(D2, HIGH); // Set D2 to HIGH
          while (true) {
            asm("nop");
          }
        }
      } else {
        startTime = millis(); // Reset the start time if the frequency goes out of range
      }
    }
  } else if (sensorValue < runningAverage && currentState == 1) {
    currentState = 0;
  }
}