# midi-reaper
Arduino sketch that looks for certain MIDI sequences and uses them to switch a logical output on/off

The inspiration for the project was to power a 'Record' light for a studio, such that when you press record on 
Reaper (a DAW application), it would automatically switch on a record light for the talent.

The application looks for two different sequences:
  90 5F xx      where xx is 7F (on)  and 00 (off)
  B0 2C yy      where yy is 45 (on)  and 05 (off)

When these sequences are detected, digital output 3 goes on and output 4 off (onboard Arduino LED also goes on), or for the
off sequence, 3 is off, LED is off and 4 is on.  We used an Arduino Pro Mini (for compactness), but probably any Arduino
model will work.

The MIDI interface uses software serial, which seems to work well at the 31250 bps rate of MIDI.  The hardware serial
is used to send diagnostics in real-time, so you can watch what MIDI sequences are arriving on an attached PC.
 

