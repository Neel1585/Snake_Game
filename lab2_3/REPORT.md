# Lab 2_3 — Group A2_3

## 1. Tool and install route — [3]

| | |
|---|---|
| Agent used for run 2 | Antigravity IDE (Gemini 3.7 Flash) |
| `ubiquitous-language` install route | Copied to workspace `.agents/skills/ubiquitous-language` and global `~/.gemini/config/skills/` |
| `refactoring/` pack install route | Copied to workspace `.agents/skills/refactoring` and global `~/.gemini/config/skills/` |

Skills were installed into Antigravity IDE's local and workspace `.agents/skills` customization roots and verified before session execution.

---

## 2. What I changed in the glossary — [4]

The generated file is at `lab2_3/UBIQUITOUS_LANGUAGE.md`.

In the generated glossary, I corrected several agent assumptions to accurately reflect the game domain:
1. Replaced the generic data structure term `node` with the canonical domain entity **Segment** and mapped its aliases (`body part`, `tail block`).
2. Disambiguated coordinate conventions by distinguishing between snake head coordinates (`m1, m2`), food coordinates (`fi, fj`), and node coordinates (`mi, mj`), flagging the lack of a unified **Position** abstraction.
3. Removed speculative framework terminology (`GameController`, `InputHandler`) that the agent invented, keeping only concrete game domain concepts (**Snake**, **Food**, **Board**, **Obstacle**, **Tick**, **Collision**, **Score**, **Player**).
4. Added the missing domain concept **Player** with explicit ownership rules over snakes and controls.

---

## 3. Smell delta — [6]

Reports: `lab2_3/audits/main.md` (the code as you received it) and `lab2_3/audits/lab1-head.md` (after your Lab-1 PR).

| | count | representative site (`file:line`) |
|---|---|---|
| Smells my Lab-1 PR **introduced** | 4 | `game.cpp:272` |
| Smells my Lab-1 PR **left untouched** | 6 | `game.cpp:69` |
| Smells my Lab-1 PR **removed** | 0 | `game.cpp:30` |

The third row will probably be zero. Leave it in.

---

## 4. Rejected candidates — [6]

At least three things the agent reported that are *not* real findings on this codebase.

| smell reported | `file:line` | why it does not hold |
|---|---|---|
| Data Class | `game.cpp:58` | `struct node` is a standard low-level linked-list node container in C++ rather than an anemic domain model. |
| Large Class | `game.cpp:65` | `Snake` is ~110 lines and cohesively encapsulates snake movement and segment traversal without unnecessary bloat. |
| Lazy Class | `obstacle.cpp:147` | `Obstacle` cleanly encapsulates obstacle layout coordinates and separation of static hazards from board logic. |

---

## 5. Commit map — [7]

Run `lab2_3/check-lab2_3.sh` and paste the table it prints.

| # | sha | subject | what it is |
|---|---|---|---|
| 1 | f06e8a3 | Add ubiquitous language glossary | glossary |
| 2 | eeab3a2 | Add code smell audit for main branch | smell report |
| 3 | 7506550 | Refactor GameBoard to centralize snake entity management | the refactor alone |
| 4 | f7e9d03 | Add second player support | the feature alone |

---

## 6. Two-run measurement — [4]

Run 1 is your Lab-1 branch — the numbers you already reported. Run 2 is commit 4 alone.

| | Run 1 (Lab 1) | Run 2 (commit 4) |
|---|---|---|
| Smells introduced | 4 | 0 |
| Lines changed, `git diff --shortstat -w` | 124 | 93 |
| Lines changed, **raw** (no `-w`) | 124 | 93 |
| Functions reached | 5 | 3 |
| Prompts to working code | 4 | 2 |
| Wall-clock time | 45 min | 15 min |

Commit 3 (the refactor) on its own: 137 lines `-w`, 140 raw.

`check-lab2_3.sh` prints the four line-count numbers for run 2. Use them — they are measured the same way for every group, which is what makes the class comparison mean anything.

---

## 7. Analysis Q1–Q2 — [5]

**Q1. Which smell did commit 3 actually fix?** Commit 3 fixed **Shotgun Surgery** (`game.cpp:65`, `game.cpp:109`) and internal **Duplicate Code** by centralizing snake entity management. Previously, `GameBoard` hardcoded a single `Snake` member. Adding a second player would have required modifying 7 distinct sites: member declarations, constructor, `display()`, `createGrid()`, `handleInput()`, `run()`, and `Food::newFood()`. By refactoring `GameBoard` to own a `std::vector<Snake>` and iterating over it polymorphically, "how many snakes exist" is defined in exactly one place (the constructor initialization), reducing multi-snake extension cost from 7 scattered edits to 1.

**Q2. Compare commit 4 to your Lab-1 diff.** In Lab 1, adding Player 2 required duplicating 5 code blocks across `createGrid()`, `display()`, `handleInput()`, `run()`, and `Food::newFood()`, adding 124 lines of repetitive, error-prone logic. In contrast, commit 4 added only 46 net lines with zero logic duplication in game loops or food spawning, because `createGrid()` and `Food::newFood()` already operated over the `snakes` collection. The cost shifted from invasively rewriting core game loops to simply configuring Player 2's starting state and key bindings.

---

## 8. Analysis Q3–Q4 — [5]

**Q3. Go back through your Lab-1 `LLM-LOG.md`. Did the assistant ever suggest restructuring before adding the feature?** No. In Lab 1, the assistant never suggested restructuring or introducing a collection abstraction before adding Player 2. It responded directly to targeted debugging and compatibility prompts without considering codebase extensibility. To prompt restructuring, I would have had to explicitly ask: *"Before implementing multiplayer, how should GameBoard and Snake be refactored so that player count can be scaled without duplicating game loop logic?"*

**Q4. How do you know commit 3 did not change behaviour?** I compiled and ran the binary, testing single-player movement with arrow keys/WASD, food consumption, score updates, wall collisions, and restarting to verify identical gameplay. However, this manual sanity check lacks rigorous regression proof. To truly know behaviour did not change, I would have needed automated unit/regression tests for grid state, collision invariants, and deterministic movement steps before and after refactoring.

---

Sections 7 and 8 together: **340 words** (500 words maximum).

## If you did not finish

All parts completed successfully.
