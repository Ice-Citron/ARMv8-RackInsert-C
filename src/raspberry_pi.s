ldr w2, io_0to9
ldr w0, [x2]
and w0, w0, #0xFFFFFE3F
orr w0, w0, #0x40
str w0, [x2]
blink:
    ldr w2, set_0
    mov w0, #0x4
    str w0, [x2]
    ldr w1, timing
    wait1:
        subs w1, w1, #1
        b.ne wait1
    ldr w2, clr_0
    mov w0, #0x4
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