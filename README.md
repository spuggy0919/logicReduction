# logicReduction
This program converts truth table to logical equations.
This simple command line program converts truth table to symbol equations, and if possible, minimizes it. 

### Input terms are 1,0,X, (two bits), Input Limit is 10.
```
    Minimize uitlity Ver 1.0 2015 CopyRight(c) author:spuggy0919@gmail.com\n",      
    Usage:minimize in.txt out.txt [Format]\n
    in.txt input true table\n
    out.txt output logical equation
    Format:
    0 default
    1 C style
    2 Abel style
    3 Verilog style
    4 PALASM style
```
# Example
```
%BCD to 7 segment common +5V Truth Table
%000 gfedcba
%0000 0000001
%0001 1001111
%0010 0010010
%0011 0000110
%0100 1001100
%0101 0100100
%0110 0100000
%0111 0001111
%1000 0000000
%1001 0000100
%
% Input Symbols:3210
%              :pqrs
%Output Symbols:0123456
%              :ABCDEFG
A=qr's'+p'q'r's
B=qr's+qrs'
C=q'rs'
D=qrs+qr's'+p'q'r's
E=s+qr'
F=rs+q'r+p'q's
G=qrs+p'q'r'

```

```
%Xor Truth Table
00 0
01 1
10 1
11 0
```

# Mac os binary logicReduction/bin
```
./logicreduction xor.txt xoro.txt 2
```



# WINDOWS CMD /minimize
```
minimize xor.txt xoro.txt 3
```
