import sys

filestring = "olfaaaaaakaapbctnpisooo_smollllirgaagamgbegbcgbegckgbggbfgcigbhgccgblgbkgcggckggiggiggigbagbagblgcagbfgckgbfgbggbegbpgckgbmgcigaogcggcegdggbfgcdgbkgdgrgaajabkaapbbsgijjabbbffanokifrgaagcagdogcbgcbgcbgcaghkggmghagcagghggbggmgggqajaaaaaaqcbaaaaaaqjhaaaaaapdggaagakgddgdkgcagcbgcbgheggdggfghcghcggpggdqajaaaaaaugaagakgfbghhgfbgcaggpggpggpggpggpggpggoqajaaaaaaur"
ip = 0
instructions = ["" for _ in filestring]
stack1 = []
stack2 = []
heap = []
result = []

def combine2():
    global ip
    a = filestring[ip]
    b = filestring[ip + 1]
    ip += 2
    return (ord(a) - ord('a')) * 0x10 | ((ord(b) - ord('a')) & 0xf) & 0xff

def combine2_signed():
    x = combine2()
    if x < 128:
        return x
    return x - 256

def combine2_four_times():
    v = [combine2() for i in range(4)]
    return v[0] + 0x100 * v[1] + 0x10000 * v[2] + 0x1000000 * v[3]

def combine2_four_times_signed():
    x = combine2_four_times()
    if x < 0x80000000:
        return x
    return x - 0x100000000


while ip < len(filestring):
    a = filestring[ip]
    print("ip:" + str(ip) + " : " + str(a))
    ipp = ip
    ip += 1
    if a == 'a':
        var1 = stack1.pop()
        var2 = stack1.pop()
        stack1.append(var1 + var2)
        instructions[ipp] = "PUSH(POP() + POP())"
    if a == 'b':
        var1 = stack1.pop()
        var2 = stack1.pop()
        stack1.append(var2 - var1)
        instructions[ipp] = "PUSH(-1 * POP() + POP())"
    if a == 'c':
        var1 = stack1.pop()
        var2 = stack1.pop()
        stack1.append(var1 * var2)
        instructions[ipp] = "PUSH(POP() * POP())"
    if a == 'd':
        var1 = stack1.pop()
        stack1.append(~var1)
        instructions[ipp] = "PUSH(~POP())"
    if a == 'e':
        var1 = stack1.pop()
        var2 = stack1.pop()
        stack1.append(var2 < var1)
        instructions[ipp] = "PUSH(if POP() > POP() 1 else 0)"
    if a == 'f':
        var1 = stack1.pop()
        stack1.append(var1 == 0)
        instructions[ipp] = "PUSH(POP() if POP() == 0)"
    if a == 'g':
        var3 = combine2()
        stack1.append(var3 & 0xff)
        instructions[ipp] = f"PUSH({var3 & 0xff})"
    if a == 'h':
        var3 = combine2_four_times()
        stack1.append(var3)
        instructions[ipp] = f"PUSH({var3})"
    if a == 'i':
        stack1.pop()
        instructions[ipp] = "POP()"
    if a == 'j':
        var1 = combine2()
        var2 = (var1 & 0xff)
        loc = len(stack1) - 1 - var2
        value_to_move = stack1[loc]
        if var1 != 0:
            for i in range(loc, len(stack1) - 1):
                stack1[i] = stack1[i + 1]
        stack1.append(value_to_move)
        instructions[ipp] = f"MOVE {loc} TO THE END"
    if a == 'k':
        var1 = combine2()
        loc = len(stack1) - 1 - (var1 & 0xff)
        # loc &= 0xffffffff
        stack1.append(stack1[loc])
        instructions[ipp] = f"PUSH(STACK[{loc}])"
    if a == 'l':
        var1 = stack1.pop()
        var2 = stack1.pop()
        heap[var1] = var2
        instructions[ipp] = "HEAP[POP()] = POP()"
    if a == 'm':
        var1 = stack1.pop()
        stack1.append(heap[var1])
        instructions[ipp] = "PUSH(HEAP[POP()])"
    if a == 'n':
        ip += combine2_signed()
        instructions[ipp] = f"INDEX MOVE TO {ip}"
    if a == 'o':
        var1 = combine2_four_times()
        ip = var1
        instructions[ipp] = f"INDEX MOVE TO {ip}"
    if a == 'p':
        var1 = combine2_signed()
        var2 = stack1.pop()
        if var2 == 0:
            ip += var1
            instructions[ipp] = f"IF POP() == 0 THEN MOVE INDEX TO {ip}"
    if a == 'q':
        ip = combine2_four_times()
        stack2.append(ip)
        instructions[ipp] = f"INDEX MOVE TO {ip}"
    if a == 'r':
        ip = stack2.pop() 
        instructions[ipp] = f"INDEX MOVE TO {ip}"
    if a == 's':
        char_read = sys.stdin.read(1)
        stack1.append(ord(char_read) & 0xff)
        instructions[ipp] = "PUSH(INPUT)"
    if a == 't':
        res = chr(stack1.pop())
        result.append(res)
        print(res)
        instructions[ipp] = "PRINT(POP())"
    if a == 'u':
        break
        instructions[ipp] = "RETURN"
    print(instructions[ipp])
    print(str(stack1))



    
