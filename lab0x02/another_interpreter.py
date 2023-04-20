import sys

instructions = "olfaaaaaakaapbctnpisooo_smollllirgaagamgbegbcgbegckgbggbfgcigbhgccgblgbkgcggckggiggiggigbagbagblgcagbfgckgbfgbggbegbpgckgbmgcigaogcggcegdggbfgcdgbkgdgrgaajabkaapbbsgijjabbbffanokifrgaagcagdogcbgcbgcbgcaghkggmghagcagghggbggmgggqajaaaaaaqcbaaaaaaqjhaaaaaapdggaagakgddgdkgcagcbgcbgheggdggfghcghcggpggdqajaaaaaaugaagakgfbghhgfbgcaggpggpggpggpggpggpggoqajaaaaaaur"
ip = 0
translated = ["" for _ in instructions]

def read_2_instructions_as_nr():
    global ip
    a = instructions[ip]
    b = instructions[ip + 1]
    ip += 2
    return (ord(a) - ord('a')) * 0x10 | ((ord(b) - ord('a')) & 0xf) & 0xff

def read_2_instr_signed():
    x = read_2_instructions_as_nr()
    if x < 128:
        return x
    return x - 256

def read_8_instructions_as_nr():
    v = [read_2_instructions_as_nr() for i in range(4)]
    return v[0] + 0x100 * v[1] + 0x10000 * v[2] + 0x1000000 * v[3]

def read_8_instr_signed():
    x = read_8_instructions_as_nr()
    if x < 0x80000000:
        return x
    return x - 0x100000000


while ip < len(instructions):
    a = instructions[ip]
    loc = ip
    ip += 1
    if a == 'a':
        translated[loc] = "PUSH(POP() + POP())"
    if a == 'b':
        translated[loc] = "PUSH(-1 * POP() + POP())"
    if a == 'c':
        translated[loc] = "PUSH(POP() * POP())"
    if a == 'd':
        translated[loc] = "PUSH(~POP())"
    if a == 'e':
        translated[loc] = "PUSH(1 if POP() > POP() else 0)"
    if a == 'f':
        translated[loc] = "PUSH(POP() if POP() == 0)"
    if a == 'g':
        x = read_2_instructions_as_nr()
        translated[loc] = f"PUSH({x})"
    if a == 'h':
        x = read_8_instructions_as_nr()
        translated[loc] = f"PUSH({x})"
    if a == 'i':
        translated[loc] = "POP()"
    if a == 'j':
        x = read_2_instructions_as_nr()
        translated[loc] = f"STACK MOVE {x}"
    if a == 'k':
        x = read_2_instructions_as_nr()
        translated[loc] = f"PUSH(STACK[{x}])"
    if a == 'l':
        translated[loc] = "HEAP[POP()] = POP()"
    if a == 'm':
        translated[loc] = "PUSH(HEAP[POP()])"
    if a == 'n':
        x = read_2_instr_signed()
        translated[loc] = f"JUMP TO {ip + x}"
    if a == 'o':
        x = read_8_instr_signed()
        translated[loc] = f"JUMP TO {x}"
    if a == 'p':
        x = read_2_instr_signed()
        translated[loc] = f"IF POP() == 0 THEN JUMP TO {ip + x}"
    if a == 'q':
        x = read_8_instr_signed()
        translated[loc] = f"CALL {x}"
    if a == 'r':
        translated[loc] = "RET"
    if a == 's':
        translated[loc] = f"PUSH(INPUT)"
    if a == 't':
        translated[loc] = f"PRINT POP()"
    if a == 'u':
        translated[loc] = "END"


for i in range(len(instructions)):
    if translated[i] == "":
        continue
    print(f"{i:4d}   {instructions[i]}   {translated[i]}")
    
