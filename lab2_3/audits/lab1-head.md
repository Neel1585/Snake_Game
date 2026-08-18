# Code Smell Audit — `lab1-head` (commit `5775ca5`)

**Branch/Commit**: `feat/multiplayer` (`5775ca5`)  
**Scope**: `game.cpp`, `level.cpp`, `obstacle.cpp`  
**Tool**: Antigravity IDE (Gemini 3.7 Flash) with `detect-code-smells` and `review-accuracy-calibration`  

---

## Summary of Findings

| Smell | Count | Primary Locations | Severity | Confidence |
|---|---|---|---|---|
| **Duplicate Code** | 7 | `game.cpp:272`, `game.cpp:290`, `game.cpp:305`, `game.cpp:325`, `game.cpp:380`, `level.cpp:30`, `obstacle.cpp:30` | HIGH | C4 |
| **Shotgun Surgery** | 2 | `game.cpp:211`, `game.cpp:261` | HIGH | C4 |
| **Long Method** | 3 | `game.cpp:261`, `level.cpp:142`, `obstacle.cpp:159` | HIGH | C3 |
| **Divergent Change** | 2 | `game.cpp:203`, `level.cpp:203` | HIGH | C3 |
| **Primitive Obsession** | 3 | `game.cpp:69`, `level.cpp:69`, `obstacle.cpp:69` | MEDIUM | C3 |
| **Data Clumps** | 3 | `game.cpp:118`, `level.cpp:118`, `obstacle.cpp:118` | MEDIUM | C3 |
| **Feature Envy** | 2 | `game.cpp:273`, `level.cpp:273` | MEDIUM | C3 |
| **Inappropriate Intimacy** | 2 | `game.cpp:290`, `level.cpp:290` | MEDIUM | C3 |
| **Dead Code** | 1 | `game.cpp:56` | LOW | C4 |

---

## Detailed Findings with `file:line` Citations

### 1. Duplicate Code [C4 — Certain]
- `game.cpp:272-284`: Food collision and consumption logic is duplicated verbatim for `snake` and `snake2` (`if (snake.m1 == food.fi ...)` followed by `else if (snake2.m1 == food.fi ...)`).
- `game.cpp:290-302`: Linked list traversal rendering segments to `arr` is repeated once for `snake.head` and again for `snake2.head`.
- `game.cpp:305-306`: Collision detection logic (`wallHit`, `hitSnake`, `onSnake`) is copy-pasted with identical structure for `s1Hit` and `s2Hit`.
- `game.cpp:325-329`: Reset and re-initialization sequence duplicated for `snake` and `snake2`.
- `game.cpp:380-385`: Update loop advances and changes indexes separately for each snake instance.
- `level.cpp:30-108`, `obstacle.cpp:30-108`: File-level duplication across binaries remains untouched.

### 2. Shotgun Surgery [C4 — Certain]
- `game.cpp:211`: Adding the second player forced edits in 7 distinct places: member declarations (`game.cpp:211`), constructor (`game.cpp:215`), `display` (`game.cpp:239`), `createGrid` (`game.cpp:272`), `handleInput` (`game.cpp:367`), `run` (`game.cpp:382`), and `Food::newFood` signature (`game.cpp:193`).
- `game.cpp:261`: Adding a hypothetical third player would require touching over 10 distinct blocks in `GameBoard`.

### 3. Long Method [C3 — High Confidence]
- `game.cpp:261-337`: `GameBoard::createGrid()` expanded to 77 lines, performing screen clearing, dual-snake food eating, dual-snake segment rendering, dual-snake collision resolution, game over screen display, input polling, and resetting.
- `level.cpp:142-185`: Unchanged long method in `level.cpp`.
- `obstacle.cpp:159-205`: Unchanged long method in `obstacle.cpp`.

### 4. Divergent Change [C3 — High Confidence]
- `game.cpp:203-390`: `GameBoard` class now combines multi-player state, multi-player input mapping (arrow keys vs WASD), collision resolution, and grid rendering.

### 5. Primitive Obsession [C3 — High Confidence]
- `game.cpp:69-72`: `Snake` fields (`m1, m2, dx, dy, hch, bch, cs, hs`) remain un-encapsulated primitive ints and chars.
- `level.cpp:69-72`: Unchanged primitive obsession in `level.cpp`.
- `obstacle.cpp:69-72`: Unchanged primitive obsession in `obstacle.cpp`.

### 6. Data Clumps [C3 — High Confidence]
- `game.cpp:118`: `int x, int y` coordinate pairs repeated across `Snake::onSnake` and `Snake::hitSnake` (`game.cpp:132`).
- `level.cpp:118`: Repeated `int x, int y` parameter pairs.
- `obstacle.cpp:118`: Repeated `int x, int y` parameter pairs.

### 7. Feature Envy [C3 — High Confidence]
- `game.cpp:273-282`: `GameBoard::createGrid()` directly mutates score (`snake.cs += 10`, `snake2.cs += 10`) and inserts body characters (`snake.insert(snake.bch)`) instead of delegating to snake methods.
- `level.cpp:273-282`: Direct mutation of snake state from `GameBoard`.

### 8. Inappropriate Intimacy [C3 — High Confidence]
- `game.cpp:290-302`: `GameBoard::createGrid()` traverses private-style linked list node pointers (`tn = tn->next; tn->mi; tn->mj`) of both snakes directly.
- `level.cpp:290-302`: Direct inspection of snake node internals.

### 9. Dead Code [C4 — Certain]
- `game.cpp:56-57`: Unused comments `//hello` and `//cheackout` retained in source.
