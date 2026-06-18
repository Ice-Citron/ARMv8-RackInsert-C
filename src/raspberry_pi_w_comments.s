//choosing io (we choose pin 2 because it is small)
ldr w2, io_0to9
ldr w0, [x2]
//uses bitmask to clear pin2
and w0, w0, #0xFFFFFE3F
//sets pin2 as an output
orr w0, w0, #0x40
str w0, [x2]
//loads the register setting address into the w0
//main loop
blink:
    //setting on
    ldr w2, set_0
    //sets pin2 high
    mov w0, #0x4
    str w0, [x2]
    //busy-waiting
    ldr w1, timing
    wait1:
        subs w1, w1, #1
        b.ne wait1
    //setting off
    ldr w2, clr_0
    //sets pin2 low
    mov w0, #0x4
    str w0, [x2]
    //busy-waiting
    ldr w1, timing
    wait2:
        subs w1, w1, #1
        b.ne wait2
    b blink
//----------------------------------------------------------
//constant pool 
//using GPIO Pin 2 to control the LED
io_0to9: .int 0x3f200000
set_0: .int 0x3f20001c
clr_0: .int 0x3f200028
timing: .int 0x03938700
//IIRC we are using a raspberry pi 3 which runs at 1.2 GHZ
//we will use loop cycles to make the rpi3 wait for 0.1s between each on or off switch