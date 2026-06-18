ldr w2, io_0to9
ldr w0, [x2]
ldr w3, pin2_clear_bitmask
and w0, w0, w3
ldr w3, pin2_output_set
orr w0, w0, w3
str w0, [x2]
blink:
    ldr w2, set_0
    ldr w0, pin2_set_hilo
    str w0, [x2]
    ldr w1, timing
    wait1:
        subs w1, w1, #1
        b.ne wait1
    ldr w2, clr_0
    ldr w0, pin2_set_hilo
    str w0, [x2]
    ldr w1, timing
    wait2:
        subs w1, w1, #1
        b.ne wait2
    b blink
io_0to9: .int 0x3f200000
set_0: .int 0x3f20001c
clr_0: .int 0x3f200028
timing: .int 0x03938700
pin2_set_hilo: .int 0x4
pin2_clear_bitmask: .int 0xFFFFFE3F
pin2_output_set: .int 0x40