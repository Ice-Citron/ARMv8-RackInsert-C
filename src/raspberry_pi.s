//choosing io (we choose pin 2 because it is small)
ldr w0, io_0to9
movn w0, pin2_output, lsl pin2_shift
str w0, io_0to9
//loads the register setting address into the w0
//main loop
blink:
    //setting on
    ldr w0, set_0
    orr w0, pin2_on
    str w0, [w0]
    //busy-waiting
    ldr w1, timing
    wait1:
        subs w1, w1, #1
        b.ne wait1
    //setting off
    ldr w0, clr_0
    orr w0, pin2_on
    str w0, [w0]
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
pin2_shift: .int 0x6
pin2_output: .int 0x1
//makes pin2 an input by using OR bitmask
pin2_on: .int 0x4
timing: .int 0x03938700
//IIRC we are using a raspberry pi 3 which runs at 1.2 GHZ
//we will use loop cycles to make the rpi3 wait for 0.1s between each on or off switch
