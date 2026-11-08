# Normal Color Vision Distinguisher V2.0    

## Description

**version 2.0**

This program takes in 2-4 colors and their RGB values and 1 background RGB value. Then converts it to CIELAB and tells you the difference of the pairs of colors
then it relay you the results and give you the option to print it out in a file in the form of a txt. This isn't the perfect one yet, there's still limitations
because of CIE 76, the main equation I use, and limit to only 1 background color. This and many other things will be fixed/added in V3.0 in the future.
I decided to remove the naming and focus more on logic, accuracy, and usuability.

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
Welcome to the color difference tester V2!
How many colors would you like to compare? (2-4):
2
Background red (0-255):
255
Background green (0-255):
255
Background blue (0-255):
255
Background: 255 255 255
Is that correct? (y/n):
y
Enter one value at a time.
Color 1 red (0-255):
0
Color 1 green (0-255):
0
Color 1 blue (0-255):
0
Color 1 opacity (0-100):
100
Color 1: 0 0 0 at 100% opacity
Is that correct? (y/n):
y
Color 2 red (0-255):
255
Color 2 green (0-255):
255
Color 2 blue (0-255):
255
Color 2 opacity (0-100):
100
Color 2: 255 255 255 at 100% opacity
Is that correct? (y/n):
y
Results:
(the number is the color difference, called Delta E: higher means easier to tell apart)
Color 1 vs Color 2: 100 - EXCELLENT: These colors are VERY different and clearly distinguishable.
Would you like to save these results to a file? (y/n):
y
Results saved to color_report.txt

The saved color_report.txt for that run:

==============================
Color Difference Report
Background: 255 255 255

Colors entered: 
Color 1: 0 0 0 at 100% opacity
Color 2: 255 255 255 at 100% opacity

Comparisons:
Color 1 vs Color 2: 100 - EXCELLENT: These colors are VERY different and clearly distinguishable.

Note: results assume typical color vision. (Color blind options coming in V3!)
```
3-color mode with transparency (the same red at 100%, 50%, and 0% opacity over a white background)
```
Welcome to the color difference tester V2!
How many colors would you like to compare? (2-4):
3
Background red (0-255):
255
Background green (0-255):
255
Background blue (0-255):
255
Background: 255 255 255
Is that correct? (y/n):
y
Enter one value at a time.
Color 1 red (0-255):
255
Color 1 green (0-255):
0
Color 1 blue (0-255):
0
Color 1 opacity (0-100):
100
Color 1: 255 0 0 at 100% opacity
Is that correct? (y/n):
y
Color 2 red (0-255):
255
Color 2 green (0-255):
0
Color 2 blue (0-255):
0
Color 2 opacity (0-100):
50
Color 2: 255 0 0 at 50% opacity
Is that correct? (y/n):
y
Color 3 red (0-255):
255
Color 3 green (0-255):
0
Color 3 blue (0-255):
0
Color 3 opacity (0-100):
0
Color 3: 255 0 0 at 0% opacity
Is that correct? (y/n):
y
Results:
(the number is the color difference, called Delta E: higher means easier to tell apart)
Color 1 vs Color 2: 56.5496 - EXCELLENT: These colors are VERY different and clearly distinguishable.
Color 1 vs Color 3: 114.559 - EXCELLENT: These colors are VERY different and clearly distinguishable.
Color 2 vs Color 3: 62.2935 - EXCELLENT: These colors are VERY different and clearly distinguishable.
Would you like to save these results to a file? (y/n):
n
```
Example of bad input protection
```
Welcome to the color difference tester V2!
How many colors would you like to compare? (2-4):
abc
That wasn't a number. Please try again.
How many colors would you like to compare? (2-4):
9
Please enter a number from 2 to 4.
How many colors would you like to compare? (2-4):
2
Background red (0-255):
256
Please enter a number from 0 to 255.
Background red (0-255):
255
```