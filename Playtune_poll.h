// Playtune_poll.h

// Here is where you define the pin configuration for playing music. Because we use direct register 
// manipulation for speed, it's a little complicated, so bear with me.

// First, define the number of channels (output pins), which is the maximum number of notes that
// you want to play simultaneously. This can be up to 16, but it's best for efficiency to not make
// it much bigger than what you will actually be using for the most complicated song.

#define MAX_CHANS 8     // maximum number of simultaneous notes

// Next, choose which set of pins in this file you want to use. This should match the processor 
// chosen in the tools/board menu of the Arduino IDE (Integrated Development Environment).

#define TEENSY_3x       // which set of pin definitions, which are below, to use

// Now define the output pin that is wired to the speaker for each channel in the section you chose. 
// The pin numbers are the "virtual" numbers that typically are printed on the board and are used in 
// digitalWrite() functions.  Our channels are numbered in sequence starting with 0. Again, it's best 
// not to assign too many more channels than you really intend to use.

// For Teensy 3.x and Teensy LC microcontrollers, which use an ARM processor, defining pins is not so bad.
// Just define one macro for each channel, as in the following example.

#ifdef TEENSY_LC  // define 8 channels on a Teensy LC
#define CHAN_0_PIN 5
#define CHAN_1_PIN 6
#define CHAN_2_PIN 7
#define CHAN_3_PIN 8
#define CHAN_4_PIN 9
#define CHAN_5_PIN 10
#define CHAN_6_PIN 11
#define CHAN_7_PIN 12
#endif

#ifdef TEENSY_3x  // define 8 channels on a Teensy 3.x
#define CHAN_0_PIN 5
#define CHAN_1_PIN 6
#define CHAN_2_PIN 7
#define CHAN_3_PIN 8
#define CHAN_4_PIN 9
#define CHAN_5_PIN 10
#define CHAN_6_PIN 11
#define CHAN_7_PIN 12
#endif

// For Arduino microcontrollers using an AVX processor, it's a little more complicated.
// For each channel, you need to give the pin number, the data register that it is wired
// to, and the number of the bit in that register that corresponds to the specific pin.
// You can get that information by looking at the schematic for the board, or from one
// of the great cheat-sheets at http://www.pighixxx.com/test/pinoutspg/boards/
// I've included several examples below.

#ifdef ARDUNIO_MICRO  // define 8 channels on an Arduino Micro
#define CHAN_0_PIN 5    // channel 0 outputs on pin 5,
#define CHAN_0_REG C    // pin 5 is wired to register C,
#define CHAN_0_BIT 6    // and is bit 6 in that register

#define CHAN_1_PIN 6
#define CHAN_1_REG D
#define CHAN_1_BIT 7

#define CHAN_2_PIN 7
#define CHAN_2_REG E
#define CHAN_2_BIT 6

#define CHAN_3_PIN 8
#define CHAN_3_REG B
#define CHAN_3_BIT 4

#define CHAN_4_PIN 9
#define CHAN_4_REG B
#define CHAN_4_BIT 5

#define CHAN_5_PIN 10
#define CHAN_5_REG B
#define CHAN_5_BIT 6

#define CHAN_6_PIN 11
#define CHAN_6_REG B
#define CHAN_6_BIT 7

#define CHAN_7_PIN 12
#define CHAN_7_REG D
#define CHAN_7_BIT 6
#endif // Arduino Micro

#ifdef ARDUINO_NANO  // define 8 channels on an Arduino Nano
#define CHAN_0_PIN 5   // channel 0 outputs on pin 5,
#define CHAN_0_REG D   // pin 5 is wired to register D,
#define CHAN_0_BIT 5   // and is bit 5 in that register

#define CHAN_1_PIN 6
#define CHAN_1_REG D
#define CHAN_1_BIT 6

#define CHAN_2_PIN 7
#define CHAN_2_REG D
#define CHAN_2_BIT 7

#define CHAN_3_PIN 8
#define CHAN_3_REG B
#define CHAN_3_BIT 0

#define CHAN_4_PIN 9
#define CHAN_4_REG B
#define CHAN_4_BIT 1

#define CHAN_5_PIN 10
#define CHAN_5_REG B
#define CHAN_5_BIT 2

#define CHAN_6_PIN 11
#define CHAN_6_REG B
#define CHAN_6_BIT 3

#define CHAN_7_PIN 12
#define CHAN_7_REG B
#define CHAN_7_BIT 4
#endif // Arduino Nano

#ifdef ARDUINO_MEGA  // define 8 channels on an Arduino Mega
#define CHAN_0_PIN 53   // channel zero outputs on pin 53,
#define CHAN_0_REG B    // pin 53 is wired to register B,
#define CHAN_0_BIT 0    // and is bit 0 in that register

#define CHAN_1_PIN 51
#define CHAN_1_REG B
#define CHAN_1_BIT 2

#define CHAN_2_PIN 49
#define CHAN_2_REG L
#define CHAN_2_BIT 0

#define CHAN_3_PIN 47
#define CHAN_3_REG L
#define CHAN_3_BIT 2

#define CHAN_4_PIN 45
#define CHAN_4_REG L
#define CHAN_4_BIT 4

#define CHAN_5_PIN 43
#define CHAN_5_REG L
#define CHAN_5_BIT 6

#define CHAN_6_PIN 41
#define CHAN_6_REG G
#define CHAN_6_BIT 0

#define CHAN_7_PIN 39
#define CHAN_7_REG G
#define CHAN_7_BIT 2
#endif // Arduino Mega

// If you want to use an oscillocope to measure how long our interrupt routine
// takes, you can configure a pin that outputs a high when it is running.

#define SCOPE_TEST true // make scope measurements?
#ifdef CORE_TEENSY
#define SCOPE_PIN 13     // board pin
#endif
#ifdef ARDUINO_NANO
#define SCOPE_PIN 4     // board pin
#define SCOPE_REG D     // data register
#define SCOPE_BIT 4     // bit number in that register
#endif
#ifdef ARDUINO_MEGA
#define SCOPE_PIN 4     // board pin
#define SCOPE_REG G     // data register
#define SCOPE_BIT 5     // bit number in that register
#endif

// Finally, here are the prototypes of routines you can call

void tune_start_timer(int);
void tune_playscore(const byte *);
void tune_stopscore(void);
void tune_stop_timer(void);
extern volatile boolean tune_playing;
void tune_speed(unsigned percent);       // adjust playing speed:
 //                                          100 = normal, 50 = slow by 1/2, 200 = doubletime, etc.
