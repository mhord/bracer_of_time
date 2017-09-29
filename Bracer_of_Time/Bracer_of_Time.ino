#include <SimbleeForMobile.h>

#define SFM SimbleeForMobile // a macro to shorten our later library
                             //  calls to a reasonable length.

// Aliases for the pin assignments for each button.
#define BUTTON1 15
#define BUTTON2 12
#define BUTTON3 11
#define BUTTON4 9

// Linearize the output of the LED in 16 steps. Because of the
//  non-linear response of the human eye, an approximation of
//  linear brightness increase of LEDs requires non-linear
//  brightness values.
const int LEDSteps[16] = {255, 191, 154, 128, 107, 90, 76, 64,
                            53,  43,  34,  26,  19, 12,  6,  0};

// Timer variable definitions.
int timer1 = 60;   // timer1 defaults to 60 seconds
int timer2 = 60;   // timer2 defaults to 60 seconds
int timer3 = 300;  // timer3 defaults to 300 seconds (five minutes)
int timer4 = 600;  // timer4 defaults to 600 seconds (ten minutes)

int t1Default = 60;
int t2Default = 60;
int t3Default = 300;
int t4Default = 600;

long t1last = 0;
long t2last = 0;
long t3last = 0;
long t4last = 0;

int t1LEDIndex = 0;
int t2LEDIndex = 0;
int t3LEDIndex = 0;
int t4LEDIndex = 0;

int t1LEDDir = 1;
int t2LEDDir = 1;
int t3LEDDir = 1;
int t4LEDDir = 1;

int t1LEDVal = 255;
int t2LEDVal = 255;
int t3LEDVal = 255;
int t4LEDVal = 255;

long LEDUpdateLast = 0;

// Timer active definitions.
bool t1active = false;
bool t2active = false;
bool t3active = false;
bool t4active = false;

// Flash storage definitions and variables
#define FLASH_PAGE 251

unsigned long *t1p = ADDRESS_OF_PAGE(FLASH_PAGE);
unsigned long *t2p = ADDRESS_OF_PAGE(FLASH_PAGE) + 4;
unsigned long *t3p = ADDRESS_OF_PAGE(FLASH_PAGE) + 8;
unsigned long *t4p = ADDRESS_OF_PAGE(FLASH_PAGE) + 12;

void setup() 
{
  // Our buttons need to be inputs, with pullups.
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  // This is the output pin for the LEDs. I didn't
  //  make a #define for it because this is the only
  //  place we use it.
  pinMode(3, OUTPUT);
  // This function is defined farther down in the
  //  code. It handles writing the current values
  //  of LED brightness to the four LEDs.
  updateLEDs();
  // Serial is only used during debugging.
  Serial.begin(115200);

  // SFM.deviceName and SFM.advertisementData must,
  //  together, contain fewer than 16 characters.
  //  These strings put us at 15. Whew!
  SFM.deviceName = "Bracer of Time";
  SFM.advertisementData = " ";
  SFM.begin();

  // If we have values in the flash memory, we want to
  //  extract them and use those as our defaults. If we
  //  don't have values in flash, the flash value will
  //  read as -1, so we can check that to see whether we
  //  want to use the value in flash or not.
  if ((int)*t1p > 0) t1Default = (int)*t1p;
  if ((int)*t2p > 0) t2Default = (int)*t2p;
  if ((int)*t3p > 0) t3Default = (int)*t3p;
  if ((int)*t4p > 0) t4Default = (int)*t4p;
  timer1 = t1Default;
  timer2 = t2Default;
  timer3 = t3Default;
  timer4 = t4Default;
}

void loop() 
{
  // SFM.process() handles the UI processing, if a phone
  //  is connected to the device.
  SFM.process();
  
  // Timer 1 section
  // Start the timer when the button is pressed and the timer
  //  isn't running.
  if ((digitalRead(BUTTON1) == LOW) && (t1active == false)) 
  {
    delay(25);          // Debounce the input.
    t1active = true;    // Start the timer.
    t1last = millis();  // Start counting from now.
    timer1 = t1Default; // Use the current default value for the timer.
    while (digitalRead(BUTTON1) == LOW) 
    {/*Wait for the button to be released*/}
  }

  // Pause the timer if it's running.
  if ((digitalRead(BUTTON1) == LOW) && t1active)
  {
    delay(25);          // Debounce the input.
    t1active = false;   // Stop the timer.
    while (digitalRead(BUTTON1) == LOW) 
    {/*Wait for the button to be released*/}
  }
  // Activates every 1000ms while the timer is running to
  //  keep the time updating.
  if ((millis() - t1last > 1000) && t1active)
  {
    t1last = millis();
    timer1--;
    Serial.println(timer1);
    if (timer1 == 0)
    {
      timer1 = t1Default;
      t1active = false;
      t1LEDVal = 255;
      updateLEDs();
      Serial.println("Timer 1 expired!");
    }
  }
  
  // Timer 2 section
  // Start the timer when the button is pressed and the timer
  //  isn't running.
  if ((digitalRead(BUTTON2) == LOW) && (t2active == false)) 
  {
    delay(25);          // Debounce the input.
    t2active = true;    // Start the timer.
    t2last = millis();  // Start counting from now.
    timer2 = t2Default; // Use the current default value for the timer.
    while (digitalRead(BUTTON2) == LOW) 
    {/*Wait for the button to be released*/}
  }

  // Pause the timer if it's running.
  if ((digitalRead(BUTTON2) == LOW) && t2active)
  {
    delay(25);          // Debounce the input.
    t2active = false;   // Stop the timer.
    while (digitalRead(BUTTON2) == LOW) 
    {/*Wait for the button to be released*/}
  }
  
  // Activates every 1000ms while the timer is running to
  //  keep the time updating.
  if ((millis() - t2last > 1000) && t2active)
  {
    t2last = millis();
    timer2--;
    Serial.println(timer2);
    if (timer2 == 0)
    {
      timer2 = t2Default;
      t2active = false;
      t2LEDVal = 255;
      updateLEDs();
      Serial.println("Timer 2 expired!");
    }
  }
  
  // Timer 3 section
  // Start the timer when the button is pressed and the timer
  //  isn't running.
  if ((digitalRead(BUTTON3) == LOW) && (t3active == false)) 
  {
    delay(25);          // Debounce the input.
    t3active = true;    // Start the timer.
    t3last = millis();  // Start counting from now.
    timer3 = t3Default; // Use the current default value for the timer.
    while (digitalRead(BUTTON3) == LOW) 
    {/*Wait for the button to be released*/}
  }

  // Pause the timer if it's running.
  if ((digitalRead(BUTTON3) == LOW) && t3active)
  {
    delay(25);          // Debounce the input.
    t3active = false;   // Stop the timer.
    while (digitalRead(BUTTON3) == LOW) 
    {/*Wait for the button to be released*/}
  }
  
  // Activates every 1000ms while the timer is running to
  //  keep the time updating.
  if ((millis() - t3last > 1000) && t3active)
  {
    t3last = millis();
    timer3--;
    Serial.println(timer3);
    if (timer3 == 0)
    {
      timer3 = t3Default;
      t3active = false;
      t3LEDVal = 255;
      updateLEDs();
      Serial.println("Timer 3 expired!");
    }
  }
  
  // Timer 4 section
  // Start the timer when the button is pressed and the timer
  //  isn't running.
  if ((digitalRead(BUTTON4) == LOW) && (t4active == false)) 
  {
    delay(25);          // Debounce the input.
    t4active = true;    // Start the timer.
    t4last = millis();  // Start counting from now.
    timer4 = t4Default; // Use the current default value for the timer.
    while (digitalRead(BUTTON4) == LOW) 
    {/*Wait for the button to be released*/}
  }

  // Pause the timer if it's running.
  if ((digitalRead(BUTTON4) == LOW) && t4active)
  {
    delay(25);          // Debounce the input.
    t4active = false;   // Stop the timer.
    while (digitalRead(BUTTON4) == LOW) 
    {/*Wait for the button to be released*/}
  }
  
  // Activates every 1000ms while the timer is running to
  //  keep the time updating.
  if ((millis() - t4last > 1000) && t4active)
  {
    t4last = millis();
    timer4--;
    Serial.println(timer4);
    if (timer4 == 0)
    {
      timer4 = t4Default;
      t4active = false;
      t4LEDVal = 255;
      updateLEDs();
      Serial.println("Timer 4 expired!");
    }
  }
  
  // LED blinking section
  // Updates 10 times a second, to update the LED of any
  //  timer that is running.
  if (millis() - LEDUpdateLast > 100)
  {
    // First, take a note on the current time, so we
    //  know when to next enter this subsection of code.
    LEDUpdateLast = millis();

    // Update the values being displayed on the LEDs.
    updateLEDs();

    // Now, calculate the values that we'll display on
    //  the LEDs next time through the loop.
    // T1 LED section
    if (t1active)
    {
      // Adjust the LED value for this LED by changing
      //  the index we use from the LEDSteps array.
      t1LEDVal = LEDSteps[t1LEDIndex+=t1LEDDir];
      // "Bounce" the direction of adjustment when we
      //  reach one end or the other of the array.
      if (t1LEDIndex == 0)
      {
        t1LEDDir = 1;
      }
      else if (t1LEDIndex == 15)
      {
        t1LEDDir = -1;
      }
    } 
    
    //T2 LED section
    if (t2active)
    {
      t2LEDVal = LEDSteps[t2LEDIndex+=t2LEDDir];
      if (t2LEDIndex == 0)
      {
        t2LEDDir = 1;
      }
      else if (t2LEDIndex == 15)
      {
        t2LEDDir = -1;
      }
    }
    
    //T3 LED section
    if (t3active)
    {
      t3LEDVal = LEDSteps[t3LEDIndex+=t3LEDDir];
      if (t3LEDIndex == 0)
      {
        t3LEDDir = 1;
      }
      else if (t3LEDIndex == 15)
      {
        t3LEDDir = -1;
      }
    }
    
    //T4 LED section
    if (t4active)
    {
      t4LEDVal = LEDSteps[t4LEDIndex+=t4LEDDir];
      if (t4LEDIndex == 0)
      {
        t4LEDDir = 1;
      }
      else if (t4LEDIndex == 15)
      {
        t4LEDDir = -1;
      }
    }
  }
}

// UI Element object handles
// We could have put this stuff up at the top, but I
//  wanted it closer to the UI function.
uint8_t t1Input;
uint8_t t2Input;
uint8_t t3Input;
uint8_t t4Input;

uint8_t getValuesButton;
uint8_t updateButton;

// This function is a Simblee library function that
//  defines the UI elements that we'll see on the phone.
void ui()
{
  SFM.beginScreen();
  // We need to re-fetch these values every time we
  //  reconnect to the phone, in case they changed.
  if ((int)*t1p > 0) t1Default = (int)*t1p;
  if ((int)*t2p > 0) t2Default = (int)*t2p;
  if ((int)*t3p > 0) t3Default = (int)*t3p;
  if ((int)*t4p > 0) t4Default = (int)*t4p;
  timer1 = t1Default;
  timer2 = t2Default;
  timer3 = t3Default;
  timer4 = t4Default;

  // These are the text boxes that display the name of
  //  the timer the text field will be controlling.
  SFM.drawText(40,80, "Timer 1:");
  SFM.drawText(40,120, "Timer 2:");
  SFM.drawText(40,160, "Timer 3:");
  SFM.drawText(40,200, "Timer 4:");

  // These are the text fields that allow the user to input
  //  a number to be used for the default value of each timer.
  int temp = -1;
  t1Input = SFM.drawTextField(100, 70, 50, temp);
  t2Input = SFM.drawTextField(100, 110, 50, temp);
  t3Input = SFM.drawTextField(100, 150, 50, temp);
  t4Input = SFM.drawTextField(100, 190, 50, temp);

  // Define two buttons: one to get the values from flash and
  //  populate the text fields, and one to store the values to
  //  flash.
  getValuesButton = SFM.drawButton(40,240,150, "Get settings");
  updateButton = SFM.drawButton(40, 300, 150, "Store settings");
  SFM.endScreen();
}

// This is a Simblee library function which handles events caused
//  by objects in the UI. We have two types of events in this
//  application: text field entry events and button events.
void ui_event(event_t &event)
{
  // First, handle the text field entry events. These occur when
  //  the "enter" key is pressed while the cursor is in a text
  //  field.
  if (event.id == t1Input)
  {
    t1Default = event.value;
    Serial.println(event.value);
  }
  if (event.id == t2Input)
  {
    t2Default = event.value;
    Serial.println(event.value);
  }
  if (event.id == t3Input)
  {
    t3Default = event.value;
    Serial.println(event.value);
  }
  if (event.id == t4Input)
  {
    t4Default = event.value;
    Serial.println(event.value);
  }

  // Now, the update stored values button. This records
  //  the values in the fields to flash memory so they
  //  persist through power loss or reset.
  if (event.id == updateButton)
  {
    int rc = flashPageErase(FLASH_PAGE);
    Serial.println(rc);
    rc = flashWrite(t1p, (unsigned long)t1Default);
    Serial.println(rc);
    rc = flashWrite(t2p, (unsigned long)t2Default);
    Serial.println(rc);
    rc = flashWrite(t3p, (unsigned long)t3Default);
    Serial.println(rc);
    rc = flashWrite(t4p, (unsigned long)t4Default);
    Serial.println(rc);
    Serial.println(*t1p);
    Serial.println(*t2p);
    Serial.println(*t3p);
    Serial.println(*t4p);
    timer1 = t1Default;
    timer2 = t2Default;
    timer3 = t3Default;
    timer4 = t4Default;
  }

  // This button fetches the current values and puts them
  //  into the text fields.
  if (event.id == getValuesButton)
  {
    SFM.updateValue(t1Input, timer1);
    SFM.updateValue(t2Input, timer2);
    SFM.updateValue(t3Input, timer3);
    SFM.updateValue(t4Input, timer4);
  }
}

// Put the current intensity on each LED.
void updateLEDs()
{
  RGB_Show(t1LEDVal,0,0,3);
  RGB_Show(t2LEDVal,t2LEDVal,0,3);
  RGB_Show(0,0,t3LEDVal,3);
  RGB_Show(0,t4LEDVal,0,3);
}

// Handle all the timing-critical stuff required to send
//  data to the LEDs.
void RGB_Show(uint8_t r, uint8_t g, uint8_t b, uint32_t ledPin)
{
    uint8_t rgb[3] = {g, r, b};
    uint8_t *p = rgb;
    uint8_t *end = p + 3;

    while (p < end)
    {
        uint8_t pix = *p++;
        for (uint8_t mask = 0x80; mask; mask >>= 1)
        {
            if (pix & mask)
            {
                // T1H 760ns
                NRF_GPIO->OUTSET = (1UL << ledPin);
                NRF_GPIO->OUTSET = (1UL << ledPin);
                NRF_GPIO->OUTSET = (1UL << ledPin);

                // T1L 660ns
                NRF_GPIO->OUTCLR = (1UL << ledPin);
            }
            else
            { 
                // T0H 380ns
                NRF_GPIO->OUTSET = (1UL << ledPin);

                // T0L 840ns
                NRF_GPIO->OUTCLR = (1UL << ledPin);
                NRF_GPIO->OUTCLR = (1UL << ledPin);
                NRF_GPIO->OUTCLR = (1UL << ledPin);
                NRF_GPIO->OUTCLR = (1UL << ledPin);
            }
        }
    } 
    NRF_GPIO->OUTCLR = (1UL << ledPin);
}

