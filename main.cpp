/* =============================================================================
 Color Difference Tester V2.0
 Hunter Harris | CSCI 111 | Professor Buffardi

 WHAT IT DOES:
 The user enters 2 to 4 colors (RGB plus opacity) and a background color.
 The program reports how different each pair of colors looks and rates it
 (CRITICAL, POOR, GOOD, or EXCELLENT). Results can be saved to a text file.

 HOW IT WORKS (the pipeline each color goes through):
  1. Read RGB (0-255) and opacity (0-100%) from the user, with validation.
  2. Blend the color with the background so transparency is accounted for.
  3. Convert the blended RGB to CIELAB (L, a, b), a color space where
    distance matches how different colors look to a person.
  4. Store the L, a, b values in three vectors (one entry per color).
  5. Compare every pair of colors using the CIE76 Delta E formula.
  6. Rate each Delta E using the cutoff constants below.

 ATTRIBUTION & CITATION:
- Delta E cutoffs (1, 2, 10) come from a general guide at
  https://zschuessler.github.io/DeltaE/learn/ . The author says the ranges
  come from his own testing and not an official standard, so I treat them
  as a starting point.
- The sRGB and CIELAB conversion constants come from the published sRGB and
  CIELAB standards. They are not values I chose.
- Input validation (cin.fail(), cin.clear(), cin.ignore()) is carried over
  from V1.0 and Professor Buffardi's Sanitize Input Algorithm
- Claude (Anthropic's AI assistant) helped me plan the brainstorm the design of V2.0 and
  the CIELAB conversion steps, and reviewed my code when I got stuck at errors.
============================================================================= */
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <cmath> // for the pow and sqrt functions
#include <limits> // for numeric_limits used in cin.ignore
using namespace std;

// These const are for setting up the domain of compareable colors. Aka, it stating how many colors the user can compare (2-4).
const int MIN_COLORS = 2;
const int MAX_COLORS = 4;
// These are the limits for each level of difference.
const double CRITICAL_LIMIT = 1.0;
const double POOR_LIMIT = 2.0;
const double GOOD_LIMIT = 10.0;
// These are the D65 Values for the standard "daylight white" that screens use. If you're curious about these just google "D65 XYZ Tristimulus values" and look at the standard observer.
const double WHITE_X = 0.95047;
const double WHITE_Y = 1.0;
const double WHITE_Z = 1.08883;

// Function prototypes (these functions will have comments defining themselves down in their respective places in the code).
double inputCheck(string prompt, double low, double high);
void convert_to_CIELAB(double red, double green, double blue, double &L, double &a, double &b);
string rateDifference(double deltaE);
void fileReport(vector<double> redValues, vector<double> greenValues, vector<double> blueValues, vector<double> opacityValues, double bgRed, double bgGreen, double bgBlue, vector<double> labL, vector<double> labA, vector<double> labB);
double calculateDeltaE(double L1, double a1, double b1, double L2, double a2, double b2);
bool yesNoCheck(string prompt);
double linearize(double value);
double labF(double t);
double blendChannel(double color, double opacity, double background);

int main() {
  // Three vectors for storing CIELAB's L*, a*, b* for each color.
  vector<double> labL;
  vector<double> labA;
  vector<double> labB;
  // Four vectors for storing the colors as the user typed them (RGB). These are only used for the file input.
  vector<double> redValues;
  vector<double> greenValues;
  vector<double> blueValues;
  vector<double> opacityValues;
  // Holds one color input at a time. The next color will overwrite these. I understand I would change these to ints but for accuracy of math, I have decided to keep these as doubles.
  double red;
  double blue;
  double green;
  double opacity;
  // Holds the background color RBG values. So far I only store one background value (will make option for more than one BG color in V3). Same reason to use doubles as variables above.
  double bgRed;
  double bgGreen;
  double bgBlue;
  // The store colors after blending with the backgrounds (this is what's converted to CIELAB and MUST be doubles for accuracy).
  double blendedRed;
  double blendedGreen;
  double blendedBlue;
  // ColorMode is just like the old variable from V1, it denotes to the algorithm how many colors should be compared.
  int colorMode = 0;
  // A variable that stores whether or not the user correctly entered their background values.
  bool bgCorrect = false;

  cout << "Welcome to the color difference tester V2!" << endl; //greet user

  colorMode = inputCheck("How many colors would you like to compare? (2-4):", MIN_COLORS, MAX_COLORS); // Defines the colormode as a result of checking the user's input so it's only numreic and is either 2, 3, or 4.

  //background is entered once and shared by every color.
  do { // this loop runs once, and repeats if the user says they messed up.
    bgRed = inputCheck("Background red (0-255): ", 0, 255);
    bgGreen = inputCheck("Background green (0-255): ", 0, 255);
    bgBlue = inputCheck("Background blue (0-255): ", 0, 255);     

    cout << "Background: " << bgRed << " " << bgGreen << " " << bgBlue << endl; // prints out the background the user inputted so they can make sure they did it right.
    bgCorrect = yesNoCheck("Is that correct? (y/n): "); // yes no check so you can restart the loop or continue
    if (!bgCorrect)
    {
      cout << "Okay let's re-enter the background." << endl;
    }
  } while (!bgCorrect);

  cout << "Enter one value at a time." << endl; // I decided to tell the user only 1 value at a time so it looks better and will hopefully reduce confusion.

  for (int i = 1; i <= colorMode; i++) // This starts at i = 1 and not i = 0 so it will print Color 1 and Color 2 and not Color 0 and Color 1.
    {
      bool isCorrect = false;

      do{ // Saying no to this loop will only restart the color your on and not to the beginning (Ex: Color 3 *restart* Color 3 and not Color 3 *restart* Color 1).
        red = inputCheck("Color " + to_string(i) + " red (0-255): ", 0, 255);
        green = inputCheck("Color " + to_string(i) + " green (0-255): ", 0, 255);
        blue = inputCheck("Color " + to_string(i) + " blue (0-255): ", 0, 255);
        opacity = inputCheck("Color " + to_string(i) + " opacity (0-100): ", 0, 100);

        cout << "Color " << i << ": " << red << " " << green << " " << blue << " at " << opacity << "% opacity" << endl;
        isCorrect = yesNoCheck("Is that correct? (y/n): ");
        if (!isCorrect)
        {
          cout << "Okay let's re-enter color " << i << "." << endl;
        }
        } while(!isCorrect);

        // blend with the background first, then convert only the blended result
        // CIELAB can't store transparency so I had to handle it here, before the conversion. 100% opacity just means the color and 0% means the background.
        blendedRed = blendChannel(red, opacity, bgRed);
        blendedGreen = blendChannel(green, opacity, bgGreen);
        blendedBlue = blendChannel(blue, opacity, bgBlue);

        // L, a, b are empty. convert_to_CIELAB uses pass by references to fill them with data.
        double L, a, b;
        convert_to_CIELAB(blendedRed, blendedGreen, blendedBlue, L, a, b);
        
        // Push back obviously adds the values the function filled in
        labL.push_back(L);
        labA.push_back(a);
        labB.push_back(b);

        // These save original input separately so if the user wants a file print out we can tell them the original color and BG values.
        redValues.push_back(red);
        greenValues.push_back(green);
        blueValues.push_back(blue);
        opacityValues.push_back(opacity);
    }

  cout << "Results: " << endl;
  cout << "(the number is the color difference, called Delta E: higher means easier to tell apart)" << endl;
  // Compare every pair of colors only once. i is the first color, j is the second. j is i+1 so j can never be i.
  // The vectors start at index 0 (obviously) so we have to print i + 1 and j + 1 so we don't get something like Color 0 and Color 1.
  for (int i = 0; i < colorMode; i++)
    {
      for (int j = i + 1; j < colorMode; j++)
      {
        double deltaE = calculateDeltaE(labL[i], labA[i], labB[i], labL[j], labA[j], labB[j]);
        cout << "Color " << i + 1 << " vs Color " << j + 1 << ": " << deltaE << " - " << rateDifference(deltaE) << endl;
      }
    }

  // Saving is optional. yesNoCheck returns a bool t or f, so that's why I just put it in a simple if statement
  // File report gets copies of everything (pass by values) because it only reads them.
  if (yesNoCheck("Would you like to save these results to a file? (y/n): "))
  {
    fileReport(redValues, greenValues, blueValues, opacityValues, bgRed, bgGreen, bgBlue, labL, labA, labB);
    cout << "Results saved to color_report.txt" << endl;
  }
  return 0;
}

// Input Check is a more refined version of the sanitize input function we did in class. Basically you call it with your question along with the lower and upper limits of your numerical answer's domain
// The reason why I didn't just constrain this function to just one domain is because I use it for multiple cases (0-255 for colors, 0-100 for opacity, and 2-4 for colorMode).
// We will only return the original value if it's both numeric and within the bounds. Otherwise we just loop until the user enters proper numbers.
double inputCheck(string prompt, double low, double high) {
  double value = 0; //define the value for the user's input
  bool isValid = false;
  do{
      cout << prompt << endl; // print out the prompt
      cin >> value; // store the users response
      if (cin.fail()) // if user doesn't enter a proper number
      {
        // not a number: reset cin and throw away bad input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "That wasn't a number. Please try again." << endl;
      }
      else // user enters correct number
      {
        // clear leftover characters on the line. Ex: "abc" in "5abc"
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (value < low || value > high) // If number is good but out of bounds
        {
          cout << "Please enter a number from " << low << " to " << high << "." << endl;
        }
        else // If number passes all checks, terminates the loop and let's the return run.
        {
          isValid = true;
        }
      }
    } while (!isValid);
  return value;
}

// Same idea as inputCheck, but just for yes or no questions. This one returns a bool as t or f for loop logic. 
// Fun fact, based on how this works you can type yellow or neon and it would treat that as yes or no.
bool yesNoCheck(string prompt) {
  char answer; // response's first character variable
  bool isValid = false; // loop condition to keep asking user for good input
  bool result = false; // What bool this function returns for the logic it was called for.

  do{
      cout << prompt << endl;
      cin >> answer;
      // clear leftover characters like es in yes or o in no
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      if (answer == 'y' || answer == 'Y')
      {
        result = true;
        isValid = true;
      }
      else if (answer == 'n' || answer == 'N')
      {
          result = false;
          isValid = true;
      }
      else
      {
        cout << "Please enter y or n." << endl;
      }
    } while (!isValid);
  return result;
}

// Step 1 of CIELAB conversion, Linearize. It takes one color channel scaled to 0-1 and returns it as linear light.
// Screens store brightness on a curve so dark shades get more detail, so the stored number isn't proportional to the actual amount of light. This step undoes that curve.
// These precise decimals come from the sRGB formula, they aren't mine.
// If you google the sRGB formula and look at the higher R' values, you'll see where these fancy decimals come from
double linearize(double value) {
  if (value <= 0.04045) // Very dark values use a straight line (a pure power curve misbehaves near 0). 0.04045 is the handoff to the curve.
  {
    return value / 12.92;
  }
  else // 0.055 and 1.055 are offsets that make the curve connect smoothly with the line at that handoff so there's no jumps.
  {
    return pow((value + 0.055) / 1.055, 2.4); // 2.4 is the exponent of the curve. 
  }
}

// Step 3 of CIELAB conversion. Takes one of the XYZ values (already divided by the white point) and returns the "seen" version of it.
// Cube root matches how eyes see brightness, but it gets extremely steep near 0, so very dark values use a straight line instead.
double labF(double t) {
  if (t > 0.008856) // 0.008856 is (6/29) cubed. That's the handoff between the cube root and line.
  {
    return pow(t, 1.0 / 3.0);
  }
  else
  {
    return 7.787 * t + 16.0 / 116.0; // Slope and offset of the line, chosen to meet the cube root at 0.008856 with no jump.
  }
}

// This is the kicker. This one turns one colors RGB into CIELAB: L is lightness from black to white, a is green to red, and b is blue to yellow. Lookup a 3D graph for a better visualization.
// scale 0 to 1 to remove the gamma curve (linearize) -> XYZ then we divide by the white point then we call labF which turns it into fx, fy, and fz and then finally L, a, and b.
// rgb are pass by values and L, a, and b are pass by references so we don't need a return in this case.
void convert_to_CIELAB(double red, double green, double blue, double &L, double &a, double &b) {
  // (Step 1) 255 is the biggest value 8 bit can hold, so this goes from 0-255 to 0-1. 
  double linearRed = linearize(red / 255.0);
  double linearGreen = linearize(green / 255.0);
  double linearBlue = linearize(blue / 255.0);

  // (Step 2) XYZ is how much light a color has. Each line is a mix of R G and B light.
  // The weights come from sRGB standard and are rounded to 4 digits.
  // When you resolve these (if the color is white) X Y and Z will become the white point. (They resolve to the CONST values at the top).
  double x = 0.4124 * linearRed + 0.3576 * linearGreen + 0.1805 * linearBlue;
  double y = 0.2126 * linearRed + 0.7152 * linearGreen + 0.0722 * linearBlue;
  double z = 0.0193 * linearRed + 0.1192 * linearGreen + 0.9505 * linearBlue;

  // (Step 3) Dividing by white point measures each value relative to white (white becomes ~1). Then labF turns it into the seen version.
  double fx = labF(x / WHITE_X);
  double fy = labF(y / WHITE_Y);
  double fz = labF(z / WHITE_Z);

  // (Step 4) These last constants are the CIE's definition of CIELAB, and they're just scaling
  // a comes from gap between fx and fy, b from fy and fz. This shows how far the color leans red vs green or yellow vs blue. 500 and 200 gives us a usable range (~ -128 to +127).
  L = 116.0 * fy - 16.0;
  a = 500.0 * (fx - fy);
  b = 200.0 * (fy - fz);
}

// This function handles the transparency of the colors
// This will blend a partially (or fully) transparent color with the background color
double blendChannel(double color, double opacity, double background) {
  double alpha = opacity / 100.0; // turn 0-100% to 0-1
  return alpha * color + (1.0 - alpha) * background;
}

// This is simple. It's the distance formula for CIELAB on it's 3D graph. It's the CIE76 formula. If you're curious you can google it, it's literally just a 3D distance formula.
double calculateDeltaE(double L1, double a1, double b1, double L2, double a2, double b2) {
  double diffL = L1 - L2;
  double diffA = a1 - a2;
  double diffB = b1 - b2;
  return sqrt(diffL * diffL + diffA * diffA + diffB * diffB);
}

// This one is also simple. This is the categoriser that will put each set of colors in one of 4 classes based on how big their difference is.
// This is the same thing from V1 but the cutoffs are different.
string rateDifference(double deltaE) {
  if (deltaE < CRITICAL_LIMIT)
  {
    return "CRITICAL: These colors are almost identical. Most people with typical color vision cannot tell them apart.";
  }
  else if (deltaE < POOR_LIMIT)
  {
    return "POOR: You would need to look very closely, or place the colors side by side, to tell them apart.";
  }
  else if (deltaE < GOOD_LIMIT)
  {
    return "GOOD: These colors are fairly different and easy to tell apart at a glance.";
  }
  else
  {
    return "EXCELLENT: These colors are VERY different and clearly distinguishable.";
  }
}


// This is a function that will write a file report (if the user wants it).
void fileReport(vector<double> redValues, vector<double> greenValues, vector<double> blueValues, vector<double> opacityValues, double bgRed, double bgGreen, double bgBlue, vector<double> labL, vector<double> labA, vector<double> labB) {
  ofstream outFile("color_report.txt", ios::app);

  if (!outFile) // Can't open file, don't do anything and break out
  {
    cout << "Sorry, the report file could not be opened. Nothing was saved." << endl;
    return;
  }

  int count = redValues.size(); // All vectors are same length. So we get the length of one to tell how many colors their are

  outFile << "==============================" << endl; // A little header
  outFile << "Color Difference Report" << endl; // Title
  outFile << "Background: " << bgRed << " " << bgGreen << " " << bgBlue << endl << endl; // Prints out the background used for all colors

  outFile << "Colors entered: " << endl; // Prints out colors and original values
  for (int i = 0; i < count; i++)
  {
    outFile << "Color " << i + 1 << ": " << redValues[i] << " " << greenValues[i] << " " << blueValues[i] << " at " << opacityValues[i] << "% opacity" << endl;
  }

  outFile << endl << "Comparisons:" << endl; // Comparison section
  for (int i = 0; i < count; i++) // This nested loop is identical to the earlier one except it just uses count
  {
    for (int j = i + 1; j < count; j++)
    {  
      double deltaE = calculateDeltaE(labL[i], labA[i], labB[i], labL[j], labA[j], labB[j]);
      outFile << "Color " << i + 1 << " vs Color " << j + 1 << ": " << deltaE << " - " << rateDifference(deltaE) << endl; // Prints comparisons (the same terminal output).  
    }
  }

  outFile << endl << "Note: results assume typical color vision. (Color blind options coming in V3!)" << endl << endl;
  outFile.close();
}