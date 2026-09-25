# Normal Color Vision Distinguisher V1.0    

## Description

**version 1.0**

This program will take in RGB values of 2 or 3 colors, user's choice, and then tell them whether or not they have a distinguishable difference.
This program isn't perfect because it can only handle up to 3 colors at once, and doesn't account for alpha in RGBa.
This program is equipped with some form of fault tolerance in which when an incorrect data type in put into a loop it won't infinitely loop.


## Developer

Hunter Lee Harris

## Example

To run the program, give the following commands:

```
g++ --std=c++11 *.cpp -o cvp
./cvp
```

Here is an example of the program running:

For 2-color mode
```
Hello there user!
What's your name?: H
Great to meet you H, this is designed to tell if colors are different enough to see by the normal human eye.
This program only supports up to 3 colors at once, and of course not just one.
Futhermore, you need the RGB code of each color.
How many colors will you input (pick 2 or 3): 2
Great choice! Proceeding with 2 color mode.
Input each RGB value of the first color (from 0-255) with a space inbetween. DO NOT separate by commas.
0 0 0
Alright then input the second color's RGB values with the same parameters.
255 255 255
Alright just to check the values you entered.
First color values: 0 0 0
Second color values: 255 255 255
Was that correct? (y or n)
Y
STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color one and two.
```
For 3-color mode
```
Hello there user!
What's your name?: H
Great to meet you H, this is designed to tell if colors are different enough to see by the normal human eye.
This program only supports up to 3 colors at once, and of course not just one.
Futhermore, you need the RGB code of each color.
How many colors will you input (pick 2 or 3): 3
Great choice! Proceeding with 3 color mode.
Input each RGB value of the first color (from 0-255) with a space inbetween. DO NOT separate by commas.
0 0 0
Alright then input the second color's RGB values with the same parameters.
1 1 1
Input the third color's RGB values with the same rules.
50 50 50
Alright just to check the values you entered.
First color values: 0 0 0
Second color values: 1 1 1
Third color values: 50 50 50
Was that correct? (y or n)
Y
STATUS: CRITICAL - Color one and two are almost identical. No normal eye can tell the difference.
STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color one and three.
STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color two and three.
browser.cpp:~$ 
```