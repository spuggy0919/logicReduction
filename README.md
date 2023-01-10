# logicReduction
This program converts true table to logic equations.
This simple command line program convert true table to symbol equations, and if possible, miniize it. 
```
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
'''
# Example
'''
%Xor True Table
00 0
01 1
10 1
11 0
'''

# mac os in /bin
```
./logicreduction xor.txt xoro.txt 2
```



# WINDOWS CMD /minimize
```
minimize xor.txt xoro.txt 3
```
