lightsuit
=========

http://lightsuit.tumblr.com




*Howto*
*Build your own light suit!*

This is still a work in progress. In the future, the suits will become smart, and will dance along with us to our tunes. In the meantime, here’s how I made the basics…

*The Hardware*

The light suit comprises a 16x16 grid of ultra-bright white LED’s, an arduino microcontroller, four shift registers, some chicken wire and a bunch of electrical wire. Two mode buttons (up mode, down mode) allow me to interact with the device while we are playing.

*The Physical*

The suit is made of fine plastic chicken wire, which provided me with a pre-made grid to work with. I cut this into a T-Shirt shape, which allowed me to wear it under another T-Shirt while I was on stage. 



The LED’s are connected in a matrix. The cathodes of each row are connected together and the anodes of each column are connected together. Somewhat like this, except twice the size (image adapted from: http://arduino.cc/playground/uploads/Main/Matrix.gif )



This now means that we can turn any one LED on or off by giving +3V (HIGH) to the row and 0V (LOW) to the column. To keep all of the LED’s off, we make sure that the rows are kept low and the columns are kept high. This is nice, but not that useful, as we can’t turn on more than one LED at once without making adjacent LED’s also turn on. We’ll come back to that in a second.

Our first problem is that there aren’t enough output pins on the arduino for the 32 leads that we have (16 for the cathodes, the columns, and 16 for the anodes, the rows).

To solve this I used 4 8-bit shift registers with a latch (I used the 74HC595). This takes serial data in and outputs it as parallel data. This means we only need to use 3 pins on our arduino (Clock, Latch, Data). 

I interfaced with the shift registers using SPI (Serial-Peripheral Interface) which can write to the shift registers much faster than using the Arduino supplied ‘ShiftOut()’ function.

*Wiring*

The anodes and cathodes are connected to the shift registers as follows. This determines the order the bits need to be written to the registers. I connected the wires on the suit to a connector so that I could connect it to my PCB. In hindsight, I would have connected them the other way around, as I now have to reverse all the bits as the SPI library shifts out with the MSB (most significant bit) first. 





*The Code:*

This program takes a matrix bytes (2x16), each bit of which represents a single LED. I wrote a GNU/Octave script which converts a 16x16 pixel black and white (1-bit colour) image into this format:

byte test[16][2]={{B01000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B00000000, B00000000},
{B10000000, B00000000},
{B00000000, B00000000}};

The program sets up two different interrupts, one on each of the buttons (which are connected to pin 2 and pin 3). This allows the buttons to be read even if the program is stuck in a delay() loop somewhere – useful for when I am performing and can’t wait for one of my image sequences to finish.

The other uses FlexiTimer2, which calls the bitmapUpdate() function every 10ms.

For every row, the function reads that row data into the column buffers. This value is ‘not’ed so that each bit is inversed (0->1, 1->0), as this is sent to the anodes.

We cycle through each row one at a time, turning one row on at a time. This goes really fast, so your eye can’t actually see it.

Since the SPI functions write to the registers using the MSB first, and I wired them assuming the LSB is at the end, I had to reverse all the bits before sending it off to the registers.

*Display Routines:*

(visions.ino)

I wrote a bunch of display routines, which allowed me to scroll images, and text in any direction across the screen. There are also a few different spiralling and random routines which look pretty neat.

*Modes:*

The mode buttons cycle through pre-programmed sequences which were used in the performance of the different songs onstage. 


