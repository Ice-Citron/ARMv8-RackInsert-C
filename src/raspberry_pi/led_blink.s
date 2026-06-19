ldr w2, io_10to19
ldr w0, [x2]
ldr w3, gpio17_clear_bitmask
and w0, w0, w3
ldr w3, gpio17_output_set
orr w0, w0, w3
str w0, [x2]
blink:
    ldr w2, set_10
    ldr w0, gpio17_set_hilo
    str w0, [x2]
    ldr w1, timing
    wait1:
        subs w1, w1, #1
        b.ne wait1
    ldr w2, clr_10
    ldr w0, gpio17_set_hilo
    str w0, [x2]
    ldr w1, timing
    wait2:
        subs w1, w1, #1
        b.ne wait2
    b blink
io_10to19: 
    .int 0x3f200004
set_10: 
    .int 0x3f20001c
clr_10: 
    .int 0x3f200028
timing: 
    .int 0x000fffff
gpio17_set_hilo: 
    .int 0x00020000
gpio17_clear_bitmask: 
    .int 0xff1fffff
gpio17_output_set: 
    .int 0x00200000