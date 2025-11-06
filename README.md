# 🐍 Snake Game (C++ Terminal Edition)

A fun and fully functional **Snake Game** built completely in **C++**, designed to run inside the **Linux terminal** using low-level system functions like `termios`, `select`, and `unistd`.  
This project is divided into **two playable levels**:
- **Level 1 (`level.cpp`)** – Classic Snake Game.
- **Level 2 (`obstacle.cpp`)** – Advanced Snake Game with obstacles.

---

## 🚀 Features

- 🧭 **Real-time input** (non-blocking key detection).
- 🎮 **Smooth snake movement** within a bordered grid.
- 🍎 **Random food generation** avoiding snake collision.
- 🐍 **Snake growth** on eating food.
- 🧱 **Walls and obstacles** (in advanced level).
- 💾 **Score & High Score tracking**.
- 🔄 **Restart or Exit option** after game over.
- 🌈 **Emoji-based visuals** for an engaging terminal experience.

---

## 🧩 LEVEL 1: `level.cpp` — The Classic Snake Game

### 🎯 Description
This file implements the **simple version** of the game.  
You control a snake in a **20x50 grid**, eat apples (`🍎`), and try to score as high as possible without hitting the wall or your own body.  
Each apple increases your score by **10** and lengthens the snake.

---

### ⚙️ Core Components

| Component | Description |
|------------|--------------|
| **`getchNonBlocking()`** | Reads keyboard input in real-time without pausing the game. |
| **`struct node`** | Represents one segment (part) of the snake’s body. |
| **`class Snake`** | Handles snake body, head position, movement, and growth. |
| **`class Food`** | Randomly places food in the grid ensuring it doesn’t overlap with the snake. |
| **`class GameBoard`** | Manages gameplay: input handling, drawing grid, scoring, and collision detection. |

---

### 🕹️ Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move Up |
| `S` / `↓` | Move Down |
| `A` / `←` | Move Left |
| `D` / `→` | Move Right |
| `X` | Exit game after Game Over |

---

### 🎨 Symbols Used

| Symbol | Meaning |
|---------|----------|
| 🧱 | Wall Border |
| 🍎 | Food |
| 🐸 | Snake Head |
| 🟩 | Snake Body |
| ⬜ | Empty Cell |

---

### 🧮 Scoring System

| Event | Points |
|--------|--------|
| Eating an Apple | +10 |
| Colliding with Wall/Self | Game Over |
| After Game Over | Press any key to Restart or `X` to Exit |

The **High Score** is maintained throughout the session, and if you reach a score of **100**, you can move to the next advanced level.

---

### ⚙️ Game Logic Flow

1. **Initialize Board:**  
   A 20x50 grid is drawn with borders (`🧱`).

2. **Snake Creation:**  
   Starts at the center of the grid, represented by a linked list of nodes.

3. **Food Generation:**  
   Randomly placed, avoiding the snake’s position.

4. **Input Handling:**  
   - Non-blocking mode ensures smooth movement.  
   - Direction changes occur instantly upon keypress.

5. **Movement:**  
   - The snake moves one step in the current direction every 200ms.  
   - Eating food extends its length.

6. **Collision Detection:**  
   - If the snake hits the wall or itself → Game Over.

7. **Restart or Exit:**  
   After Game Over, the player can restart or exit easily.

---

## ⚔️ LEVEL 2: `obstacle.cpp` — The Advanced Obstacle Challenge

### 🧱 Description
This version extends the classic snake game by introducing **obstacles** in the play area.  
You must now avoid walls, your own body, and **static obstacles** positioned throughout the grid.

This level requires more attention and precision, testing your reflexes and planning.

---

### ⚙️ Additional Features

| Feature | Description |
|----------|--------------|
| **`obsarr[28][2]`** | Stores fixed obstacle coordinates in the grid. |
| **Obstacle Rendering** | Obstacles (`🧱`) are displayed inside the grid in addition to border walls. |
| **Collision Check Update** | Snake now also checks collision with obstacles. |
| **Enhanced Food Placement** | Food cannot spawn on snake or obstacle positions. |
| **Difficulty Increase** | Requires more careful navigation and strategic path planning. |

---

### 🧩 Key Logical Enhancements

- **Obstacle-Aware Movement:**  
  Before each move, the program checks if the next head position is part of an obstacle.

- **Improved Food Generation:**  
  Uses randomization that excludes both snake coordinates and obstacle positions.

- **Same Snake Logic Reused:**  
  The `Snake` and `Food` classes from Level 1 are reused with added obstacle-awareness in collision and food placement functions.

---

### 🎯 Level Comparison

| Feature | Level 1 (`level.cpp`) | Level 2 (`obstacle.cpp`) |
|----------|------------------------|---------------------------|
| Gameplay Type | Classic | Advanced with Obstacles |
| Grid Size | 20x50 | 20x50 |
| Obstacles | ❌ None | ✅ 28 Fixed Obstacles |
| Difficulty | Easy | Hard |
| Score Goal | Reach 100 to Advance | Continue Until Game Over |
| Food Spawn Rules | Avoids Snake Body | Avoids Snake + Obstacles |
| Collision Checks | Walls + Self | Walls + Self + Obstacles |

---

## 💻 Technical Implementation

1. **Real-time Input:**  
   `termios` and `select()` handle instant key detection without halting program execution.

2. **Display:**  
   Grid updated continuously using emojis for better visual clarity in terminal.

3. **Timing Control:**  
   `std::this_thread::sleep_for(std::chrono::milliseconds(200));` ensures smooth frame updates.

4. **Cross-Platform Note:**  
   - Designed for **Linux/macOS terminals** (not Windows CMD).  
   - Uses `<unistd.h>` and `<termios.h>` which are POSIX-specific.

---

## 🧪 How to Compile & Run

### 🔧 Step 1: Compile
```bash
g++ level.cpp -o level
g++ obstacle.cpp -o obstacle
