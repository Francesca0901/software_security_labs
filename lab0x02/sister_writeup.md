# Understand The Result of Readfile
## the first stack
param_1[0]: A **pointer** to a memory block allocated by calloc with the size of **0x400 * 4**.
param_1[1]: The **value of 0x400** passed to the function.
param_1[2]: Initialized with 0.

## the second stack - retstack
param_1[3]: A **pointer** to another memory block allocated by calloc with the size of **0x400 * 4**.
param_1[4]: The **value of 0x400** passed to the function.
param_1[5]: Initialized with 0.

## the heap
param_1[6]: A **pointer** to another memory block allocated by calloc with the size of **0x10000 * 4**.
param_1[7]: The **value of 0x10000** passed to the function.

## the file
param_1[8]: A **pointer** to a memory block allocated by calloc with **the size of the file** being read.
param_1[9]: The **size of the file** being read (sStack_c8.st_size).
param_1[10]: Initialized with 0.

# Abstract Functions
- pop from stack
- pop from retstack
- push stack
- push retstack
- combine2: 
    This function reads two characters from the program, calculates **their difference from the ASCII value of 'a' (0x61)**, and combines these differences into a single unsigned integer. The result is calculated as follows:
    `result = ((first_character - 0x61) * 16) | ((second_character - 0x61) & 0xf)`
    Here, the first character's difference is multiplied by 16 (left-shifted by 4 bits), and the second character's difference is bitwise ANDed with 0xf (to keep only the lower 4 bits). Finally, the two values are combined using the bitwise OR operation.
- combine_4_times:
    This function calls combine2 four times and combines the results into a single unsigned 32-bit integer.

# Observe the Workflow
## Interprete the disassembled C code (the case functions)
Here we realise an interpreter of the c code analyzed by ghidra:
```python
filestring = "olfaaaaaakaapbctnpisooo_smollllirgaagamgbegbcgbegckgbggbfgcigbhgccgblgbkgcggckggiggiggigbagbagblgcagbfgckgbfgbggbegbpgckgbmgcigaogcggcegdggbfgcdgbkgdgrgaajabkaapbbsgijjabbbffanokifrgaagcagdogcbgcbgcbgcaghkggmghagcagghggbggmgggqajaaaaaaqcbaaaaaaqjhaaaaaapdggaagakgddgdkgcagcbgcbgheggdggfghcghcggpggdqajaaaaaaugaagakgfbghhgfbgcaggpggpggpggpggpggpggoqajaaaaaaur"

ip = 0
instructions = ["" for _ in filestring]
stack1 = []
stack2 = []
heap = []

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

for i in range(len(filestring)):
    if instructions[i] == "":
        continue
    print(f"{i:4d}   {filestring[i]}   {instructions[i]}")
```

The code above can help us partially understand the instruction flow of the binary file. But without the right input, it won't show me the right path till the end. 
To avoid this problem, we can realize another similar interpreter, which doesn't really change ip along with the code execution, just to show the intended assembly instruction at each char position.

## Strange Series of PUSH
If we print the workflow, we can easily find four bunches of `g` execution, which represent a sequence of `push` instructions on stack1.
```shell                        
184   g          PUSH(32)                           
187   g          PUSH(62)                           
190   g          PUSH(33)                           
193   g          PUSH(33)                           
196   g          PUSH(33)                           
199   g          PUSH(32)                           
202   g          PUSH(122)                          
205   g          PUSH(108)                          
208   g          PUSH(112)                          
211   g          PUSH(32)                           
214   g          PUSH(103)                          
217   g          PUSH(97)                           
220   g          PUSH(108)                          
223   g          PUSH(102)                           
[32, 62, 33, 33, 33, 32, 122, 108, 112, 32, 103, 97, 108, 102]
```
```shell                   
259   g          PUSH(10)                           
262   g          PUSH(51)                           
265   g          PUSH(58)                           
268   g          PUSH(32)                           
271   g          PUSH(33)                           
274   g          PUSH(33)                           
277   g          PUSH(116)                          
280   g          PUSH(99)                           
283   g          PUSH(101)                          
286   g          PUSH(114)                          
289   g          PUSH(114)                          
292   g          PUSH(111)                          
295   g          PUSH(99) 
[10, 51, 58, 32, 33, 33, 116, 99, 101, 114, 114, 111, 99]                          
```
```shell
 36   g          PUSH(12) 
 39   g          PUSH(20) 
 42   g          PUSH(18) 
 45   g          PUSH(20) 
 48   g          PUSH(42) 
 51   g          PUSH(22) 
 54   g          PUSH(21) 
 57   g          PUSH(40) 
 60   g          PUSH(23) 
 63   g          PUSH(34) 
 66   g          PUSH(27) 
 69   g          PUSH(26) 
 72   g          PUSH(38) 
 75   g          PUSH(42) 
 78   g          PUSH(104)
 81   g          PUSH(104)
 84   g          PUSH(104)
 87   g          PUSH(16) 
 90   g          PUSH(16) 
 93   g          PUSH(27) 
 96   g          PUSH(32) 
 99   g          PUSH(21) 
102   g          PUSH(42) 
105   g          PUSH(21) 
108   g          PUSH(22) 
111   g          PUSH(20) 
114   g          PUSH(31) 
117   g          PUSH(42) 
120   g          PUSH(28) 
123   g          PUSH(40) 
126   g          PUSH(14) 
129   g          PUSH(38) 
132   g          PUSH(36) 
135   g          PUSH(54) 
138   g          PUSH(21) 
141   g          PUSH(35) 
144   g          PUSH(26) 
147   g          PUSH(54) 
[12, 20, 18, 20, 42, 22, 21, 40, 23, 34, 27, 26, 38, 42, 104, 104, 104, 16, 16, 27, 32, 21, 42, 21, 22, 20, 31, 42, 28, 40, 14, 38, 36, 54, 21, 35, 26, 54]
```
```shell
308   g          PUSH(0)  
311   g          PUSH(10) 
314   g          PUSH(81) 
317   g          PUSH(119)
320   g          PUSH(81) 
323   g          PUSH(32) 
326   g          PUSH(111)
329   g          PUSH(111)
332   g          PUSH(111)
335   g          PUSH(111)
338   g          PUSH(111)
341   g          PUSH(111)
344   g          PUSH(110)
[10, 81, 119, 81, 32, 111, 111, 111, 111, 111, 111, 110]
```

I tried to translate each stack status in string:
```python
x = [32, 62, 33, 33, 33, 32, 122, 108, 112, 32, 103, 97, 108, 102]
# x = [10, 51, 58, 32, 33, 33, 116, 99, 101, 114, 114, 111, 99]
# x = [12, 20, 18, 20, 42, 22, 21, 40, 23, 34, 27, 26, 38, 42, 104, 104, 104, 16, 16, 27, 32, 21, 42, 21, 22, 20, 31, 42, 28, 40, 14, 38, 36, 54, 21, 35, 26, 54]
# x = [10, 81, 119, 81, 32, 111, 111, 111, 111, 111, 111, 110]
result = ''.join(chr(n) for n in x if n > 0)
result = result[::-1]
print(result)
```

And I got results below:
```shell
[32, 62, 33, 33, 33, 32, 122, 108, 112, 32, 103, 97, 108, 102] --> flag plz !!!>
[10, 81, 119, 81, 32, 111, 111, 111, 111, 111, 111, 110] --> noooooo QwQ
[10, 51, 58, 32, 33, 33, 116, 99, 101, 114, 114, 111, 99] --> correct!! :3
```
Only `[12, 20, 18, 20, 42, 22, 21, 40, 23, 34, 27, 26, 38, 42, 104, 104, 104, 16, 16, 27, 32, 21, 42, 21, 22, 20, 31, 42, 28, 40, 14, 38, 36, 54, 21, 35, 26, 54]` generated nonsense. Then I guessed this is a part of our flag generation.

Between `flag plz !!!>` and our suspected flag, there is a loop that caught my attention:
```
163   s          PUSH(INPUT)                   
164   g          PUSH(137)                     
167   j          MOVE 1 TO THE END               
170   b          PUSH(-1 * POP() + POP())      
171   b          PUSH(-1 * POP() + POP())      
172   f          PUSH(POP() if POP() == 0)     
173   f          PUSH(POP() if POP() == 0)     
174   a          PUSH(POP() + POP())           
175   n          INDEX MOVE TO 156                   
```
This means it gets our input char and calculates the result of `137 - INPUT`, then compares the result with the top of the stack to check if they are the same. This is a strong signal of flag comparison.

So we can simply reproduce this arithmetic and observe the result:
```python
In [18]: x = [12, 20, 18, 20, 42, 22, 21, 40, 23, 34, 27, 26, 38, 42, 104, 104, 104, 16, 16, 27, 32, 21, 42, 21, 22, 20, 31, 42, 28, 40, 14, 38, 36, 54, 21, 35, 26, 54]

In [19]: result = ''.join(chr(137 - n) for n in x if n > 0)

In [20]: result = result[::-1]

In [21]: print(result)
SoftSec{am_just_tinyy!!!_congrats_uwu}
```

Then we can get the flag.