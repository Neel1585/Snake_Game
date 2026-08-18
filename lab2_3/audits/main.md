# Code Smell Audit — `main` (commit `ca45e2d`)

**Branch/Commit**: `main` (`ca45e2d`)  
**Scope**: `game.cpp`, `level.cpp`, `obstacle.cpp`  
**Tool**: Antigravity IDE (Gemini 3.7 Flash) with `detect-code-smells` and `review-accuracy-calibration`  

---

## Summary of Findings

| Smell | Count | Primary Locations | Severity | Confidence |
|---|---|---|---|---|
| **Duplicate Code** | 3 | `game.cpp:30`, `level.cpp:30`, `obstacle.cpp:30` | HIGH | C4 |
| **Shotgun Surgery** | 2 | `game.cpp:65`, `game.cpp:109` | HIGH | C4 |
| **Long Method** | 3 | `game.cpp:142`, `level.cpp:142`, `obstacle.cpp:159` | MEDIUM | C3 |
| **Primitive Obsession** | 3 | `game.cpp:34`, `level.cpp:34`, `obstacle.cpp:34` | MEDIUM | C3 |
| **Data Clumps** | 3 | `game.cpp:94`, `level.cpp:94`, `obstacle.cpp:94` | MEDIUM | C3 |
| **Feature Envy** | 2 | `game.cpp:151`, `level.cpp:151` | MEDIUM | C3 |
| **Inappropriate Intimacy** | 2 | `game.cpp:164`, `level.cpp:164` | MEDIUM | C3 |
| **Divergent Change** | 1 | `game.cpp:109` | HIGH | C3 |
| **Dead Code** | 1 | `game.cpp:27` | LOW | C4 |

---

## Detailed Findings with `file:line` Citations

### 1. Duplicate Code [C4 — Certain]
- `game.cpp:30-108`: Complete `Snake` class is duplicated verbatim in `level.cpp:30-108` and `obstacle.cpp:30-108`.
- `game.cpp:110-128`: `Food` class and random spawning logic is duplicated in `level.cpp:110-128` and `obstacle.cpp:110-130`.
- `game.cpp:8-25`: `getchNonBlocking` terminal input routine is copy-pasted across all three translation units (`level.cpp:8-25`, `obstacle.cpp:8-25`).

### 2. Shotgun Surgery [C4 — Certain]
- `game.cpp:65`: Any change to snake mechanics or multi-entity support forces simultaneous structural edits across `Snake`, `GameBoard::createGrid` (`game.cpp:142`), `GameBoard::display` (`game.cpp:115`), and `GameBoard::handleInput` (`game.cpp:189`).
- `game.cpp:109`: Adding a player or changing game rules requires invasive edits spanning multiple classes across `game.cpp`.

### 3. Long Method [C3 — High Confidence]
- `game.cpp:142-187`: `GameBoard::createGrid()` spans 45+ lines and handles screen clearing, grid character array allocation, food collision, snake segment rendering, boundary/self collision checks, game over UI loop, and reset branching.
- `level.cpp:142-185`: `GameBoard::createGrid()` similarly overloaded with level transition checks.
- `obstacle.cpp:159-205`: Monolithic `GameBoard::createGrid()` mixing obstacle rendering and state advancement.

### 4. Primitive Obsession [C3 — High Confidence]
- `game.cpp:34-40`: `Snake` class uses raw integer primitives (`m1`, `m2`, `dx`, `dy`, `cs`, `hs`) and primitive characters (`hch`, `bch`) for position coordinates, movement directions, and state rather than structured `Position` or `Direction` value objects.
- `level.cpp:34-40`: Identical primitive obsession in `Snake` definition.
- `obstacle.cpp:34-40`: Identical raw primitive fields in `Snake`.

### 5. Data Clumps [C3 — High Confidence]
- `game.cpp:94`: Coordinate pairs `int x, int y` in `Snake::onSnake` and `Snake::hitSnake` (`game.cpp:106`) repeatedly passed together without a cohesive `Point` object.
- `level.cpp:94`: `int x, int y` coordinate pairs repeated across multiple signatures.
- `obstacle.cpp:94`: `int x, int y` repeated without encapsulation.

### 6. Feature Envy [C3 — High Confidence]
- `game.cpp:151-156`: `GameBoard::createGrid()` queries and directly manipulates `snake.cs += 10` and `snake.insert(snake.bch)` instead of invoking a high-level `snake.eat(food)` behavior method on `Snake`.
- `level.cpp:151-156`: Direct manipulation of snake score and growth by `GameBoard`.

### 7. Inappropriate Intimacy [C3 — High Confidence]
- `game.cpp:164-169`: `GameBoard::createGrid()` directly iterates over `snake.head` linked list pointers (`node* tn = snake.head; tn = tn->next;`) accessing raw node coordinates `tn->mi` and `tn->mj`.
- `level.cpp:164-169`: `GameBoard` accesses internal linked list pointers of `Snake`.

### 8. Divergent Change [C3 — High Confidence]
- `game.cpp:109-200`: `GameBoard` class must be modified for rendering changes (ASCII vs unicode), input handling (controls/keybindings), game lifecycle rules, or scoring logic.

### 9. Dead Code [C4 — Certain]
- `game.cpp:27-28`: Unused comment lines `//hello` and `//cheackout` left in source.
