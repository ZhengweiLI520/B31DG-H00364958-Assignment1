// Definition of constants to determine signal duration
#define A 1200
#define B 900
#define C 13
#define D 4500


// Naming the I/O pins - Modify these according to your ESP32-C3 pinout
#define SignalA 2
#define SignalB 3
#define Button1 18
#define Button2 19
#define LedA 2// Define pin for LED controlled by SignalA (same as SignalA)
#define LedB 3 // Define pin for LED controlled by SignalB (same as SignalB)
#define OUTPUT_ENABLE 18// Define pin for OUTPUT_ENABLE (same as Button1)
#define OUTPUT_SELECT 19 // Define pin for OUTPUT_SELECT (same as Button2)

int signalA_enabled = 0;
int outputSelect_enabled = 0;
int last_switch1_state = 0;
int last_switch2_state = 0;

void setup() {
   // Set Pin Modes
  pinMode(Button1, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(Button2, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(SignalA, OUTPUT);
  pinMode(SignalB, OUTPUT);
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(OUTPUT_ENABLE, OUTPUT);
  pinMode(OUTPUT_SELECT, OUTPUT);
}
void loop() {
  bool switch1 = digitalRead(Button1); // Read button 1 status
  bool switch2 = digitalRead(Button2); // Read button 2 status
// If button 1 status changes, and it's currently 1
  if (switch1 == 1 && last_switch1_state == 0) {
    // Toggle SignalA output status
    signalA_enabled = !signalA_enabled;
  }

  last_switch1_state = switch1;

  // If button 2 status changes, and it's currently 1
  if (switch2 == 1 && last_switch2_state == 0) {
    // Toggle OUTPUT_SELECT status
    outputSelect_enabled = 1-outputSelect_enabled;
  }
  last_switch2_state = switch2;

  if (signalA_enabled) {
    int a = A; // Local pulse duration variable
    char i; // Incremental counter
    char c=C; // Local pulse quantity variable
    int t;
    if (outputSelect_enabled==1) {
      
      t=-50; // Example value, replace with your alternative waveform pulse quantity
    } else {
      t=50; // Normal waveform pulse quantity
    }

    // Create Signal B Trigger waveform
    digitalWrite(SignalB, HIGH);
    delayMicroseconds(50);
    digitalWrite(SignalB, LOW);

    // Create iterative sequence of pulses for Signal A
    for (i = 0; i < c; i++) {
      // Create rectangular pulse
      digitalWrite(SignalA, HIGH);
      digitalWrite(LedA, HIGH); // Turn on LED controlled by SignalA
      delayMicroseconds(a);
      digitalWrite(SignalA, LOW);
      digitalWrite(LedA, LOW); // Turn off LED controlled by SignalA
      delayMicroseconds(B);
      a += t; // Increase the pulse duration for every next iteration
    }
    delayMicroseconds(D); // Wait some time here
  } 

  else  {
    digitalWrite(SignalA, LOW); // Ensure SignalA is low if disabled
    digitalWrite(LedA, LOW); // Ensure LED controlled by SignalA is off if SignalA is disabled
  }
  

   } 
