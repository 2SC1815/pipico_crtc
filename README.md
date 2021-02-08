# pipico_crtc
Composite video output with Raspberry pi pico.

The external circuit is simple, with only 11 resistors and 1 transistor, no adjustment required.
A 4-bit R-2R ladder DAC connected to the Raspberry pi pico provides a 16-gradation 256 * 256 graphic display.

The sync frequency and number of scan lines of the composite video output by this program are
based on NTSC used in regions such as the United States and Japan.
If you want to use standards like PAL or SECAM ... Good luck.

Electrical connections:

GND--------+
           |
           R1
           |
GP6---R1---+
           |
           R2
           |
GP7---R1---+
           |
           R2
           |
GP8---R1---+
           |
           R2
           |
GP9---R1---+
           |    3V3 or VBUS
           R3       |
           |        C
GND---R3---+------B(Q1)
           |        E
           |        |
GP10--R4---+        @ OUTPUT

R1: 560 Ω * 5
R2: 270 Ω * 3
R3: 470 Ω * 2
R4: 820 Ω * 1
Q1: 2SC1815 or equivalent * 1

If the input terminal of the monitor is high impedance, the image may be distorted.
In that case, insert a resistor of about 100Ω between the Emitter of Q1 and GND.


