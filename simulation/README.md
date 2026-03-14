# Weatherboy in Terminal
A close-enough attempt at getting the program to run without an Arduino board. Done mainly to get more screenshots before uploading the project to this repo. 

## Notes on the code
### Visual differences in output
There are two differences I can see:
- Rounded rectangles are much rounder. This is likely a difference in the algorithm used for implementing them.
- Text renders one pixel lower.

### Ideas versus reality
While I initially planned this to merely be a header file replacement, this ended up not happening; some pieces of code in the project itself got replaced, hence why an altered copy of it is included here.

I also wanted to make input properly work at first, allowing proper navigation of the UI. This ended up not happening because of the difficulties of getting non-halting inputs in plain CLI C++.

### A lack of futureproofing
The code is hacked together if it isn't clear: For example, the SSD1306 drawBitmap function flips x and y if a font has width 8 and height 5.
This is because the default font's bitmap doesn't store its data LtR, UtD, requiring a special case for it to render properly.
A similar hack is done in the print(char) function, reducing the spacing of fonts with width 8 because the aforementioned hack causes them to render too far apart otherwise.

These quick-and-dirty fixes are unfortunate, but I always intended for this to be a quick project that is about getting the screenshots and nothing more.
I wouldn't have done them had I wanted to keep working on it.
