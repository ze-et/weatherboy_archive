# Weatherboy via Processing and Terminal
A functioning recreation of Weatherboy for running on Linux systems, simulating temperature and humidity. Requires Processing (non-Flatpak) for input.

## How to run & use
1. Compile the C++ code. A plain `g++ *.cpp` is enough.
2. Run the compiled program (preferably in a Terminal).
3. Run `proper_simulation` with Processing.
If everything is working correctly, you should see the program print out legible content in the Terminal, while a properly rendered screen is visible
in the Processing window.

Input can be entered via the Processing window. The following keys can be entered:
- Arrow Keys
  - Each key corresponds to joystick input
- Enter
  - Corresponds to a joystick click input
- Tab
  - Saves a numbered screenshot in the sketch folde, starting at `image0.png`. Screenshots may override older screenshots, as the program does not
    save or detect how many screenshots were already taken and will start at 0 either way.
- Escape
  - Ends both the Processing program and the C++ program. Ending the C++ program forcefully without doing this leads to a memory leak, so this is the recommended method.

## Notes on the code
### The IPC between Processing and C++
The original version of this simulation had no way to control the simulation, nor to actively view it in Processing. The initial reasons for this were:
1. Getting input in C++ without blocking is honestly a pain.
2. IPC would be required to get Processing to display what's going on.
I left it at that for a week before I realised that, if I were to implement IPC, reason 1 would not matter; I could just get input via Processing.

From the IPCs I have tried (named pipes, sockets, and shared memory), shared memory ended up winning in the end. Named pipes and sockets both had the
problem of being blocking, so they were out of the question. I did note that there is supposedly a way of making sockets non-blocking, but after a
few days of experimentation I did not figure it out. Shared memory, meanwhile, doesn't have the blocking problem and is probably the simplest to work with.

The shared memory is 1025 bytes in size, storing the pixels of the screen in the first 1024 bytes and the input in the last byte.
In other words, C++ writes to the first 1024 bytes and reads the 1025th byte, and vice versa for Processing.
The last byte has the most significant bit instantly exit the program, while the least significant five bits are from least to most significant: up, down, left, right, click.

### Visual differences in output
There are two differences I can see:
- Rounded rectangles are much rounder. This is likely a difference in the algorithm used for implementing them.
- The small font text renders at a different spot; again, probably due to implementation detais.

### Not futureproof
The code is written with a few clear hacks, e.g. the font size being used to determine whether a font is "legacy" or not in multiple sections.
While I am not too proud of these, I am not planning to expand this simulation, especially since it is so deeply tied to specifically this Arduino project.
