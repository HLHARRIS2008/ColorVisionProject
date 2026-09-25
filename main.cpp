#include <iostream>
#include <cmath>
#include <string>
#include <limits> // found this nice library to prevent the user from entering non-numeric inputs into my dang loop so it won't crash

using namespace std;

int main(){
    // define dem variables
    float firstR;
    float secondR;
    float thirdR;
    float firstG;
    float secondG;
    float thirdG;
    float firstB;
    float secondB;
    float thirdB;
    string userName;
    int colorOption;
    double dist12;
    double dist13;
    double dist23;

    // greet the user
    cout << "Hello there user!\nWhat's your name?: ";
    cin >> userName;
    cout << "Great to meet you " << userName << ", this is designed to tell if colors are different enough to see by the normal human eye." << endl;
    cout << "This program only supports up to 3 colors at once, and of course not just one.\nFuthermore, you need the RGB code of each color." << endl;

    // some loop just incase someone misreads it or inputs incorrectly
    do{
        cout << "How many colors will you input (pick 2 or 3): ";
        cin >> colorOption;

        // =========================================================================
        // ATTRIBUTION & CITATION:
        // Input validation technique (cin.fail(), cin.clear(), cin.ignore()) 
        // This was referenced from standard C++ documentation and gemini AI assistant guidance to 
        // prevent infinite loops caused by input that isn't integers
        // I read that if someone input an integer into something like this it could cause it to infinitely loop.
        // I needed a bit of help on the syntax since I've never used cin.ignore
        // =========================================================================
        if (cin.fail()) {
            cin.clear(); // Reset cin's error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the bad input from stream.
            colorOption = 0; // Reset variable so loop condition stays true
            cout << "Invalid choice. Please enter a number (2 or 3) and not words or letters." << endl;
        }
        else if (colorOption != 2 && colorOption != 3)
        {
            cout << "Invalid choice. Please only enter 2 or 3 and not in word form." << endl;
        }
    }while(colorOption != 2 && colorOption != 3);

    // notifies user of selected mode
    cout << "Great choice! Proceeding with " << colorOption << " color mode." << endl;

    //2 color mode body
    if (colorOption == 2)
    {
        char reset = 'Y';
        do { // this loop will be here just incase someone mistyped the RGB color codes.
            cout << "Input each RGB value of the first color (from 0-255) with a space inbetween. DO NOT separate by commas." << endl;
            cin >> firstR >> firstG >> firstB;
            cout << "Alright then input the second color's RGB values with the same parameters." << endl;
            cin >> secondR >> secondG >> secondB;

            // RGB Input Validation Armor
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "\nOops! You entered a letter or symbol instead of a number." << endl;
                cout << "Okay let's start over from the beginning at 2 color mode.\n" << endl;
                reset = 'n'; 
                continue; // Skips the rest of the loop and restarts
            }

            cout << "Alright just to check the values you entered." << endl;
            cout << "First color values: " << firstR << " " << firstG << " " << firstB << endl;
            cout << "Second color values: " << secondR << " " << secondG << " " << secondB << endl;
            cout << "Was that correct? (y or n)" << endl;
            cin >> reset;

            // checks and tells the user if the RGB codes were outside the 0-255 range
            if (firstR > 255 || firstG > 255 || firstB > 255 || secondR > 255 || secondG > 255 || secondB > 255
            || firstR < 0 || firstG < 0 || firstB < 0 || secondR < 0 || secondG < 0 || secondB < 0)
            {
                cout << "Hey there buddy. Only input the RGB codes from 0-255 in number form." << endl;
                reset = 'N';
            }

            if (reset == 'n' || reset == 'N')
            {
                cout << "Okay let's start over from the beginning at 2 color mode." << endl;
            }
        }while(reset == 'n' || reset == 'N');
        // intensive math time. This is euclidean difference math. See ReadMe for details.
        dist12 = sqrt(((firstR - secondR) * (firstR - secondR)) + ((firstG - secondG) * (firstG - secondG)) + ((firstB - secondB) * (firstB - secondB)));
        if (dist12 < 4.0)
        {
            cout << "STATUS: CRITICAL - Color one and two are almost identical. No normal eye can tell the difference." << endl;
        }
        else if (dist12 < 12.0)
        {
            cout << "STATUS: POOR - You would need to have color one and two put side by side to tell the difference." << endl;
        }
        else if (dist12 < 45.0)
        {
            cout << "STATUS: GOOD - Color one and two are fairly different and easily distinguishable for normal vision." << endl;
        }
        else 
        {
            cout << "STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color one and two." << endl;
        }
    }
    // 3 color mode body
    if (colorOption == 3)
    {
        char reset = 'Y';
        do { // this loop will be here just incase someone mistyped the RGB color codes.
            cout << "Input each RGB value of the first color (from 0-255) with a space inbetween. DO NOT separate by commas." << endl;
            cin >> firstR >> firstG >> firstB;
            cout << "Alright then input the second color's RGB values with the same parameters." << endl;
            cin >> secondR >> secondG >> secondB;
            cout << "Input the third color's RGB values with the same rules." << endl;
            cin >> thirdR >> thirdG >> thirdB;
            
            // RGB Input Validation Armor
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "\nOops! You entered a letter or symbol instead of a number." << endl;
                cout << "Okay let's start over from the beginning at 3 color mode.\n" << endl;
                reset = 'n'; 
                continue; // Skips the rest of the loop and restarts
            }
            cout << "Alright just to check the values you entered." << endl;
            cout << "First color values: " << firstR << " " << firstG << " " << firstB << endl;
            cout << "Second color values: " << secondR << " " << secondG << " " << secondB << endl;
            cout << "Third color values: " << thirdR << " " << thirdG << " " << thirdB << endl;
            cout << "Was that correct? (y or n)" << endl;
            cin >> reset;

            // checks and tells the user if the RGB codes were outside the 0-255 range
            if (firstR > 255 || firstG > 255 || firstB > 255 || secondR > 255 || secondG > 255 || secondB > 255 || thirdR > 255 || thirdG > 255 || thirdB > 255 
            || firstR < 0 || firstG < 0 || firstB < 0 || secondR < 0 || secondG < 0 || secondB < 0 || thirdR < 0 || thirdG < 0 || thirdB < 0)
            {
                cout << "Hey there buddy. Only input the RGB codes from 0-255 in number form." << endl;
                reset = 'n';
            }

            if (reset == 'n' || reset == 'N')
            {
                cout << "Okay let's start over from the beginning at 3 color mode." << endl;
            }
        }while(reset == 'n' || reset == 'N');
        // even fancier math since now we gotta compare 3 values instead of just 2
        dist12 = sqrt(((firstR - secondR) * (firstR - secondR)) + ((firstG - secondG) * (firstG - secondG)) + ((firstB - secondB) * (firstB - secondB)));
        dist13 = sqrt(((firstR - thirdR) * (firstR - thirdR)) + ((firstG - thirdG) * (firstG - thirdG)) + ((firstB - thirdB) * (firstB - thirdB)));
        dist23 = sqrt(((secondR - thirdR) * (secondR - thirdR)) + ((secondG - thirdG) * (secondG - thirdG)) + ((secondB - thirdB) * (secondB - thirdB)));
        if (dist12 < 4.0)
        {
            cout << "STATUS: CRITICAL - Color one and two are almost identical. No normal eye can tell the difference." << endl;
        }
        else if (dist12 < 12.0)
        {
            cout << "STATUS: POOR - You would need to have color one and two put side by side to tell the difference." << endl;
        }
        else if (dist12 < 45.0)
        {
            cout << "STATUS: GOOD - Color one and two are fairly different and easily distinguishable for normal vision." << endl;
        }
        else 
        {
            cout << "STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color one and two." << endl;
        }
        if (dist13 < 4.0)
        {
            cout << "STATUS: CRITICAL - Color one and three are almost identical. No normal eye can tell the difference." << endl;
        }
        else if (dist13 < 12.0)
        {
            cout << "STATUS: POOR - You would need to have color one and three put side by side to tell the difference." << endl;
        }
        else if (dist13 < 45.0)
        {
            cout << "STATUS: GOOD - Color one and three are fairly different and easily distinguishable for normal vision." << endl;
        }
        else 
        {
            cout << "STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color one and three." << endl;
        }
        if (dist23 < 4.0)
        {
            cout << "STATUS: CRITICAL - Color two and three are almost identical. No normal eye can tell the difference." << endl;
        }
        else if (dist23 < 12.0)
        {
            cout << "STATUS: POOR - You would need to have color two and three put side by side to tell the difference." << endl;
        }
        else if (dist23 < 45.0)
        {
            cout << "STATUS: GOOD - Color two and three are fairly different and easily distinguishable for normal vision." << endl;
        }
        else 
        {
            cout << "STATUS: EXCELLENT - Giant difference. You'd have to either be color blind or just plain old blind to not see the difference between color two and three." << endl;
        }
    }
    return 0;
}