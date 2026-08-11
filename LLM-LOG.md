# LLM Setup
- **Model**: Gemini 3.1 Pro (High)
- **Tool / harness**: Antigravity IDE Agent
- **IDE / editor**: Antigravity IDE (VS Code based)
- **Did you paste this assignment document into it?**: Yes - I pasted the full assignment document at the end to get instructions on submitting.

# The Session

**Prompt 1**: "make it windows , ubuntu, mac compatible"
*(I had already implemented the second snake locally, but needed the LLM to help make the terminal calls POSIX/Windows compatible).*
**Agent Reply**: Provided the updated `#include <sys/time.h>` and `#include <sys/types.h>` and fixed the `system("./obstacle.out")` vs `system("obstacle.exe")` logic in `level.cpp` and `obstacle.cpp`. It also created a `Makefile` for Ubuntu/Mac compatibility.
**Working Code**: Yes, it compiled successfully.

**Prompt 2**: "tell me how to check all environement like g++ etc , on my local computer to run this proect on window"
**Agent Reply**: Instructed me to use `g++ --version` and `mingw32-make --version`.

**Prompt 3**: "is it correct ?" (Pasted output of my compiler versions)
**Agent Reply**: Confirmed my environment was successfully set up.

**Prompt 4**: "game ran succesfulluy but after 3-4 second my terminal automatically close . and game diappear , so go through codebase and find any error or bugs and fix those"
**Agent Reply**: Found a critical C++ uninitialized memory bug in `game.cpp`, `level.cpp`, and `obstacle.cpp` inside the `insert(char data)` function which was causing a Segmentation Fault when redrawing the grid. 
It modified `insert(char data)` to correctly initialize `n->mi` and `n->mj` to the tail node's positions. It also fixed a memory leak in the `reset()` functions.
**Working Code**: Yes, this fixed the random crashes entirely.

**Prompt 5**: "now give winodow cmd commadn to run thi"
**Agent Reply**: Instructed to run `game.out` or `game.exe`.

**Prompt 6**: "1:Make it multiplayer Lab Assignment 3: Make it multiplayer... i made all changes and currently i am in feat/multiplayer branch what should i do now give me steps"
**Agent Reply**: Provided the git commands to commit, push, and generate the required PR metrics for the assignment.

**Summary**:
- **Total prompts to working code**: 4 (primarily for fixing bugs and cross-platform compilation, not generating the initial multiplayer logic).
- **What code I gave it**: The entire workspace (`game.cpp`, `level.cpp`, `obstacle.cpp`) was visible to the IDE Agent so it could hunt for bugs itself.
