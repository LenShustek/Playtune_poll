*** 13 November 2022: A major improvement to allow tables to be in Flash ROM
*** memory when the polling interval is known at compile time. That saves
*** about 1200 bytes of RAM, which is a big deal on a Nano with 2K! 
*** We do this when DO_CONSTANT_POLLTIME is set to 1, which is now the default.


     Playtune_poll

     An Arduino Tune Generator library that uses interrupt polling
     to play a polyphonic musical score.


                               About Playtune

   Playtune is a family of music players for Arduino-like microcontrollers. They
   each intepret a bytestream of commands that represent a polyphonic musical
   score, and play it using different techniques.

   The original Playtune that was first released in 2011 uses a separate hardware timer
   to generate a square wave for each note played simultaneously. The timers run at twice
   the frequency of the note being played, and the interrupt routine flips the output bit.
   It can play only as many simultaneous notes as there are timers available.
   https://github.com/LenShustek/arduino-playtune

   This second ("polling") version uses only one hardware timer that interrupts often, by
   default at 20 Khz, or once every 50 microseconds. The interrupt routine determines
   which, if any, of the currently playing notes need to be toggled.
   https://github.com/LenShustek/playtune_poll

   This third version also uses only one hardware timer interrupting frequently, but uses
   the hardware digital-to-analog converter on high-performance microntrollers like the
   Teensy to generate an analog wave that is the sum of stored samples of sounds. The
   samples scaled to the right frequency and volume, and any number of instrument samples
   can be used and mapped to MIDI patches. It currently only supports Teensy.
   https://github.com/LenShustek/playtune_samp

   The fourth version is an audio object for the PJRC Audio Library.
   https://www.pjrc.com/teensy/td_libs_Audio.html
   It allows up to 16 simultaneous sound generators that are internally mixed, at
   the appropriate volume, to produce one monophonic audio stream.
   Sounds are created from sampled one-cycle waveforms for any number of instruments,
   each with its own attack-hold-decay-sustain-release envelope. Percussion sounds
   (from MIDI channel 10) are generated from longer sampled waveforms of a complete
   instrument strike. Each generator's volume is independently adjusted according to
   the MIDI velocity of the note being played before all channels are mixed.
   www.github.com/LenShustek/Playtune_synth

   The fifth version is for the Teensy 3.1/3.2, and uses the four Periodic Interval
   Timers in the Cortex M4 processor to support up to 4 simultaneous notes.
   It uses less CPU time than the polling version, but is limited to 4 notes at a time.
   (This was written to experiment with multi-channel multi-Tesla Coil music playing,
   where I use Flexible Timer Module FTM0 for generating precise one-shot pulses.
   But I ultimately switched to the polling version to play more simultaneous notes.)
   www.github.com/LenShustek/Playtune_Teensy

   ***** Details about this version: Playtune_poll

   The advantage of this polling scheme is that you can play more simultaneous notes
   than there are hardware timers. The disadvantages are that it takes more CPU power,
   and that notes at higher frequencies have a little bit of jitter that adds some
   low frequency "undertones". That problem disappears if you use a microcontroller
   like the Teensy, which uses a fast 32-bit ARM processor instead of the slow 8-bit
   AVR processors used by most Arduinos so the interrupt frequency can be higher.

   The interrupt routine must be really fast, regardless of how many notes are playing
   and being toggled. We try hard to be efficient by using the following techniques:
    - Do division incrementally by only one subtraction per channel at each interrupt.
    - Use direct hardware register bit manipulation instead of digitalWrite().
      (That requires assigning output pins at compile time, not at runtime.)
    - Unroll the loop for checking on channel transitions at compile time.
      (If you are unfamiliar with loop unrolling, see https://en.wikipedia.org/wiki/Loop_unrolling)

   Once a score starts playing, all of the processing happens in the interrupt routine.
   Any other "real" program can be running at the same time as long as it doesn't
   use the timer or the output pins that Playtune is using.

   The easiest way to hear the music is to connect each of the output pins to a resistor
   (500 ohms, say).  Connect other ends of the resistors together and then to one
   terminal of an 8-ohm speaker.  The other terminal of the speaker is connected to
   ground.  No other hardware is needed!  An external amplifier is good, though.

   By default there is no volume modulation.  All tones are played at the same volume, which
   makes some scores sound strange.  This is definitely not a high-quality synthesizer.

   There are two optional features you can enable at compile time:

   (1) If the DO_VOLUME compile-time switch is set to 1, then we interpret MIDI "velocity"
   information and try to modulate the volume of the output by controlling the duty cycle of the
   square wave. That doesn't allow much dynamic range, though, so we make the high pulse of
   the square wave vary in duration only from 1 to 10 interrupt-times wide.

   There's a mapping table from MIDI velocity to pulse width that you can play with. The
   MIDI standard actually doesn't specify how velocity is to be interpreted; for an interesting
   emprical study of what synthesizers actually do, see this paper by Roger Dannenberg of CMU:
   http://www.cs.cmu.edu/~rbd/papers/velocity-icmc2006.pdf

   In order for volume modulation to work, the bytestream must include volume information,
   which can be generated by Miditones with the -v option. (It's best to also use -d, so
   that the self-describing header is generated.)

   (2) If the DO_PERCUSSION compile-time switch is also set to 1, then we interpret notes on
   MIDI channel 9 (10 if you start counting with 1) to be percussion sounds, which we play
   with a high-frequency chirp that is dithered with a pseudo-random number. Tables set
   the frequency and duration of the chirp differently for the various notes that represent
   different percussion instruments. It will only play one percussion sound at a time.

   In order for this to work, the bytestream must have the notes on the percussion track
   relocated from 0..127 to 128..255, which Miditones will do with the -pt option in
   addition to the -v option.  (Again, it best to also use -d.)


******  Using Playtune_poll  *****

   Unlike the original Playtune, this is not configured as a library because we must
   make compile-time changes for pin assignments. You should create a sketch directory
   with the following files in it:

     Playtune_poll.ino       This file, which has most of the code
     Playtune_poll.h         The header file, which defines the output pin configuration
     Playtune_poll_test.ino  The main program, which contains the score(s) you wish to
                              play, and any other code you want to run.

   In order to assign the output pins at compile time, you must add lines to the
   Playtune_poll.h file. How you do that depends on the kind of microcontroller you
   are using; see the instructions and examples in that file.

   You can define up to MAX_CHANS channels, which is 8 by default. You can
   increase MAX_CHANS up to 16. There is some inefficiency if MAX_CHANS is much
   larger than the number of channels actually being used.

   We also use the TimerOne library files, which you can get at
   http://playground.arduino.cc/Code/Timer1 and put into your Arduino library
   directory, or just put in the directory with the other files.

   There are seven public functions and one public variable that you can use
   in your runtime code in Playtune_poll_test.ino.

   void tune_start_timer(int microseconds)

    This is optional, and is available only if DO_CONSTANT_POLLTIME is set to 0.
    Call it to set how often notes should be checked for transitions,
    from 5 to 50 microseconds. If you don't call it, we'll pick something that seems
    appropriate from the type of microcontroller and the frequency it's running at.

   void tune_playscore(byte *score)

     Call this pointing to a "score bytestream" to start playing a tune.  It will
     only play as many simultaneous notes as you have defined output pins; any
     more will be ignored.  See below for the format of the score bytestream.

   boolean tune_playing

     This global variable will be "true" if a score is playing, and "false" if not.
     You can use this to see when a score has finished.

   void tune_speed(unsigned int percent)

    This changes playback speed to the specified percent of normal.
    The minimum is percent=20 (5x slowdown),
    and the maximum is percent=500 (5x speedup).

   void tune_stopscore()

     This will stop a currently playing score without waiting for it to end by itself.

   void tune_stop_timer()

     This stops playing and also stops the timer interrupt.

   void tune_resumescore (bool init_pins)

     The resumes playing a score that was stopped with tune_stopscore() or tune_stop_timer().
     If the I/O pins need to be reinitialized because they were used by something else in the
     meantime, pass TRUE for init_pins.

   *****  The score bytestream  *****

   The bytestream is a series of commands that can turn notes on and off, and can
   start a waiting period until the next note change.  Here are the details, with
   numbers shown in hexadecimal.

   If the high-order bit of the byte is 1, then it is one of the following commands:

     9t nn  Start playing note nn on tone generator t.  Generators are numbered
            starting with 0.  The notes numbers are the MIDI numbers for the chromatic
            scale, with decimal 60 being Middle C, and decimal 69 being Middle A
            at 440 Hz.  The highest note is decimal 127 at about 12,544 Hz. except
            that percussion notes (instruments, really) range from 128 to 255.

            [vv]  If DO_VOLUME is set to 1, then we expect another byte with the velocity
            value from 1 to 127. You can generate this from Miditones with the -v switch.
            Everything breaks if vv is present with DO_VOLIUME set 0, and vice versa!

     8t     Stop playing the note on tone generator t.

     Ct ii  Change tone generator t to play instrument ii from now on. This version of
            Playtune ignores instrument information if it is present.

     F0     End of score: stop playing.

     E0     End of score: start playing again from the beginning.

   If the high-order bit of the byte is 0, it is a command to wait.  The other 7 bits
   and the 8 bits of the following byte are interpreted as a 15-bit big-endian integer
   that is the number of milliseconds to wait before processing the next command.
   For example,

     07 D0

   would cause a wait of 0x07d0 = 2000 decimal millisconds or 2 seconds.  Any tones
   that were playing before the wait command will continue to play.

   The score is stored in Flash memory ("PROGMEM") along with the program, because
   there's a lot more of that than data memory.


*  *****  Where does the score data come from?  *****

   Well, you can write the score by hand from the instructions above, but that's
   pretty hard.  An easier way is to translate MIDI files into these score commands,
   and I've written a program called "Miditones" to do that.  See the separate
   documentation for that program, which is also open source at
   https://github.com/lenshustek/miditones


*  *****  Nostalgia from me  *****

   Writing Playtune was a lot of fun, because it essentially duplicates what I did
   as a graduate student at Stanford University in about 1973.  That project used the
   then-new Intel 8008 microprocessor, plus three hardware square-wave generators that
   I built out of 7400-series TTL.  The music compiler was written in Pascal and read
   scores that were hand-written in a notation I made up, which looked something like
   this:     C  Eb  4G  8G+  2R  +  F  D#
   This was before MIDI had been invented, and anyway I wasn't a pianist so I would
   not have been able to record my own playing.  I could barely read music well enough
   to transcribe scores, but I slowly did quite a few of them. MIDI is better!

   Len Shustek, originally 4 Feb 2011,
   but updated for the polling version in July 2016.
