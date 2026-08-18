# Ubiquitous Language

## Game Entities

| Term | Definition | Aliases to avoid | In code |
|---|---|---|---|
| **Snake** | A player-controlled elongated entity moving continuously across the grid, comprising a head and trailing body segments. | Worm, player avatar | `Snake` — `game.cpp:65` |
| **Segment** | An individual unit/node of a snake's trailing body following the head. | Node, body part, tail block | `node` — `game.cpp:58` ⚠ |
| **Food** | A collectible consumable item randomly spawned on an unoccupied cell that increments snake length and score upon collection. | Fruit, apple, dot | `Food` — `game.cpp:182` |
| **Board** | The bounded 2D rectangular play area enclosing all active game elements and boundaries. | Grid, arena, map | `GameBoard` — `game.cpp:203` |
| **Obstacle** | A static interior barrier hazard that triggers collision and terminates the round upon snake contact. | Wall, block, hurdle | `Obstacle` — `obstacle.cpp:147` |

## Mechanics & State

| Term | Definition | Aliases to avoid | In code |
|---|---|---|---|
| **Collision** | An event where a snake's head coordinates intersect with a boundary, obstacle, self-segment, or opponent snake. | Crash, death, hit | `hitSnake` — `game.cpp:132` ⚠ |
| **Score** | The numeric point tally accumulated by a snake through eating food. | Points, count | `cs` (current), `hs` (high) — `game.cpp:72` ⚠ |
| **Direction** | The current 2D movement vector (Up, Down, Left, Right) applied to the snake head each tick. | Heading, velocity | `dx`, `dy` — `game.cpp:70` ⚠ |
| **Tick** | A discrete fixed-duration update cycle during which user input is processed and game state advances. | Frame, step, loop iteration | `this_thread::sleep_for(chrono::milliseconds(200))` — `game.cpp:387` |

## Participants & Controls

| Term | Definition | Aliases to avoid | In code |
|---|---|---|---|
| **Player** | A human participant navigating a snake using assigned input bindings. | User, gamer, actor | *(absent)* — `game.cpp:340` ⚠ |
| **Controls** | The dedicated keyboard mapping assigned to each player to update snake direction. | Keybinds, input map | `handleInput` — `game.cpp:340` |

## Relationships

- A **Board** contains one or more **Snakes**, exactly one active **Food**, and zero or more **Obstacles**.
- A **Snake** is controlled by exactly one **Player** through dedicated **Controls**.
- A **Snake** consists of one head **Segment** and zero or more trailing body **Segments**.
- A **Collision** involves one snake head and either the **Board** perimeter, an **Obstacle**, or a **Snake** **Segment**.
- Each **Tick** updates the position of every **Snake** according to its active **Direction**.

## Example dialogue

> **Dev:** "When a **Player** presses a direction key, do we update the **Snake** immediately?"
> **Domain expert:** "No — the input only changes the **Snake**'s pending **Direction**. The actual position changes on the next **Tick**."
> **Dev:** "If two **Snakes** collide with each other on the same **Tick**, how is **Collision** resolved?"
> **Domain expert:** "If both snake heads enter the same cell or enter each other's body on the same **Tick**, both suffer a **Collision** simultaneously."

## Flagged ambiguities

- "node" was used in code (`game.cpp:58`) to represent a snake's body segment, confusing linked list data structure implementation details with the domain entity **Segment**.
- "m1, m2" was used in code (`game.cpp:69`) for snake head coordinates `(row, col)`, while food uses "fi, fj" (`game.cpp:185`) and node uses "mi, mj" (`game.cpp:60`). A unified **Position** or **Coordinate** concept is missing.
- "level" and "obstacle" were split into separate binary entrypoints (`level.cpp`, `obstacle.cpp`) rather than modeling **Obstacle** and progression as configurable **Board** settings.

## Code drift

Places where the code's name for a concept diverges from the domain term.

| Canonical term | Called in code | Location | Note |
|---|---|---|---|
| **Segment** | `node` | `game.cpp:58` | Data structure detail leaked into domain entity name |
| **Direction** | `dx`, `dy`, `hch` | `game.cpp:70-71` | Decomposed into raw coordinate deltas and character glyphs |
| **Score** | `cs`, `hs` | `game.cpp:72` | Cryptic abbreviations for current score and high score |
| **Player** | *(absent)* | `game.cpp:65` | GameBoard hardcoded a single snake directly with no Player abstraction |
| **Collision** | `hitSnake` / inline checks | `game.cpp:132`, `game.cpp:305` | Wall and body checks scattered across multiple methods |
