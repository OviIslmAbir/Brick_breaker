# 🧱 Neon Brick Breaker

A futuristic ASCII console Brick Breaker game built in pure **C** for Windows. Break every brick, survive with limited lives, and climb through increasingly faster levels — all rendered with flicker-free, colorized console graphics.

## 🎮 Features

- **Paddle** — Move left/right to keep the ball in play
- **Ball Physics** — Realistic bounce off walls, paddle, and bricks, with angle changing based on where the ball hits the paddle
- **Bricks** — Color-coded brick rows you break for points
- **Levels** — Clear all bricks to advance; each level adds more brick rows and increases ball speed
- **Lives** — Start with 3 lives; lose one each time the ball falls past the paddle

## 🕹️ Controls

| Key | Action |
|-----|--------|
| ← / → | Move paddle |
| SPACE | Launch the ball |
| ESC | Quit |
| R | Retry (on Game Over screen) |

## 🛠️ Tech Stack

- **Language:** C
- **Libraries:** `windows.h`, `conio.h` (Windows console API)
- **Platform:** Windows only (uses native console cursor/color functions)

## ⚙️ Build & Run

### Using gcc / MinGW
```bash
gcc brick_breaker.c -o brick_breaker.exe
brick_breaker.exe
```

### Using Code::Blocks
1. Create a new **C** project (not C++)
2. Add `brick_breaker.c` as the source file
3. Build (F9) and Run

> **Note:** This game relies on `windows.h` and `conio.h`, so it only compiles and runs on Windows.

## 📁 Project Structure

```
├── brick_breaker.c    # Main game source code
└── README.md
```

## 📸 Preview

```
========= NEON BRICK BREAKER =========
SCORE: 40    LIVES: 3    LEVEL: 1
+========================================+
|[==][==][==][==][==][==][==][==][==][==]|
|[==][==][==][==][==][==][==][==][==][==]|
|[==][==][==][==][==][==][==][==][==][==]|
|                                        |
|                  O                     |
|                                        |
|              ========                  |
+========================================+
```

## 👤 Author

Built by **Abhi Islam Abir**
