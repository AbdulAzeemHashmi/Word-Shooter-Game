<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:FF6B6B,100:6BCBFF&height=200&section=header&text=Word%20Shooter&fontSize=60&fontColor=ffffff&animation=fadeIn&fontAlignY=35&desc=GLUT%20Ultimate%20Edition&descAlignY=55&descSize=20" alt="Word Shooter Banner" />

<a href="https://github.com/AbdulAzeemHashmi/Word-Shooter-Game">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&pause=1000&color=6BCBFF&center=true&vCenter=true&width=600&lines=Shoot+Letters.+Form+Words.+Score+Big!;370%2C099+Word+Dictionary+Loaded;8-Directional+Word+Detection;Built+in+C%2B%2B17+with+OpenGL%2FGLUT" alt="Typing SVG" />
</a>

![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Graphics](https://img.shields.io/badge/Graphics-OpenGL%2FGLUT-5586A4?style=for-the-badge&logo=opengl&logoColor=white)
![Dictionary](https://img.shields.io/badge/Dictionary-370%2C099%20Words-orange?style=for-the-badge)
![Stars](https://img.shields.io/github/stars/AbdulAzeemHashmi/Word-Shooter-Game?style=for-the-badge&color=yellow)
![License](https://img.shields.io/badge/License-Educational-brightgreen?style=for-the-badge)

</div>

---

## 🎯 About

A fast paced **Word Shooter** game built entirely in **C++17** with **OpenGL/GLUT**. 🎮

Shoot letter tiles from a cannon at the bottom of the screen and match them with letters already on the board to form **valid English words**. Everything happens live inside one window: real time scoring 💯, a countdown timer ⏱️, and instant word validation 🔍.

The project features a **370,099 word dictionary** 📖, **8 directional word detection** (horizontal, vertical, and all 4 diagonals), and smooth **texture based rendering** 🎨 backed by a clean build system ⚙️.

<div align="center">
<img src="https://user-images.githubusercontent.com/74038190/216122041-518ac897-8d92-4c6b-9b3f-ca01dcaf38ee.gif" width="500" alt="coding animation placeholder" />
</div>

> 💡 Tip: Replace the animation above with an actual gameplay GIF once you record one. Tools like **ScreenToGif** or **Peek** work great for capturing OpenGL windows.

---

## ✨ Features

| Feature | Emoji | Status |
|---|---|---|
| Interactive shooter cannon | 🎯 | ✅ |
| 370,099 word dictionary (`words_alpha.txt`) | 📖 | ✅ |
| 2 minute timed rounds with live countdown | ⏱️ | ✅ |
| Letter tiles with texture rendering | 🔤 | ✅ |
| 8 directional word detection (H, V, 4 diagonals) | 🔍 | ✅ |
| Dynamic scoring based on word length | 💯 | ✅ |
| Binary texture data (`image-data.bin`) | 🎨 | ✅ |
| Mouse based targeting and shooting | 🖱️ | ✅ |
| Real time score display | 📊 | ✅ |
| Makefile build system | ⚙️ | ✅ |

---

## 🚀 Quick Start

### 1️⃣ Install FreeGLUT

<table>
<tr><th>OS</th><th>Command</th></tr>
<tr>
<td>🐧 Linux</td>
<td>

```bash
sudo apt install freeglut3-dev build-essential libfreeimage-dev
```

</td>
</tr>
<tr>
<td>🍎 macOS</td>
<td>

```bash
brew install freeglut freeimage
```

</td>
</tr>
<tr>
<td>🪟 Windows</td>
<td>MinGW w64 plus freeglut (copy <code>freeglut.dll</code> next to the exe)</td>
</tr>
</table>

### 2️⃣ Build

```bash
make
```

Or compile manually:

```bash
g++ wordshooter.cpp util.cpp -o word-shooter -lGL -lGLU -lglut -lfreeimage -std=c++17 -Wall -Wextra
```

### 3️⃣ Run

```bash
./word-shooter
```

<div align="center">
<img src="https://raw.githubusercontent.com/mayankpathak1/mayankpathak1/main/assets/rainbow-line.gif" width="100%" alt="divider" />
</div>

---

## 🕹️ Controls

| Input | Action |
|---|---|
| 🖱️ Mouse Move | Aim the shooter cannon |
| 🖱️ Left Click | Shoot the letter tile toward the target |
| ⌨️ ESC | Quit the game |

---

## 📁 File Tree

```
Word-Shooter-Game/
├── wordshooter.cpp      🎮 main game logic
├── util.cpp             🧰 utility functions
├── util.h                🧰 utility headers
├── Board.cpp             🧩 board management
├── Board.h                🧩 board declarations
├── CImg.h                 🖼️ image processing library
├── image-data.bin         🎨 precompiled texture data
├── words_alpha.txt        📖 370,099 English words
├── Makefile                ⚙️ build configuration
├── install-libraries.sh    🛠️ dependency installer
└── README.md               📄 this file
```

---

## 🎮 Gameplay

### 🎯 Objective

Form valid English words within **2 minutes** to score as many points as possible.

### 🔄 How It Works

```mermaid
flowchart LR
    A[🎲 Random Board Setup] --> B[🎯 Aim with Mouse]
    B --> C[🖱️ Click to Shoot Tile]
    C --> D[🔍 Scan 8 Directions]
    D --> E{Valid Word Found?}
    E -->|Yes ✅| F[💯 Add Score]
    F --> G[🧹 Remove Matched Tiles]
    E -->|No ❌| H[🔁 Wait for Next Shot]
    G --> H
    H --> I{⏱️ Time Up?}
    I -->|No| B
    I -->|Yes| J[🏁 Game Over Screen]
```

1. **Random Board Initialization** 🎲
   Two rows of randomly selected letter tiles (30 total, 15 per row), plus a random letter loaded into the shooter cannon at the bottom center.

2. **Shooting Mechanism** 🎯
   A mouse click targets a location on the board. The letter shoots from the cannon toward that position and lands on the board.

3. **Word Detection** 🔍
   After placement, the game scans all 8 directions (horizontal, vertical, diagonal) and finds the longest valid word containing the placed tile, checked against the 370k word dictionary.

4. **Scoring** 💯
   Points equal the length of the valid word found. Matched words are removed from the board, and the score updates in real time.

5. **Game End** 🏁
   When the 120 second timer hits zero, a game over screen shows the final score. Rerun the program to play again 🔁.

---

## 🛠️ Configuration Constants

Edit the **top of `wordshooter.cpp`** to tweak gameplay:

| Constant | Default | Meaning |
|---|---|---|
| `width` | 930 | canvas width in pixels 📐 |
| `height` | 660 | canvas height in pixels 📐 |
| `bradius` | 30 | ball/tile radius ⚪ |
| `timeLeft` | 120 | round duration in seconds ⏱️ |
| `nalphabets` | 26 | number of letters, A through Z 🔤 |
| `dictionarysize` | 370099 | dictionary entries 📖 |
| `FPS` | 10 | frames per second 🎞️ |

---

## 📊 Dictionary & Validation

The `words_alpha.txt` file contains **370,099 English words** 📖:

* One word per line, sorted alphabetically 🔤
* Loaded fully into memory at startup 🚀
* Searched during word validation 🔍

**Sample validation flow:**

```
User shoots 'O' at position (5, 1)
Scan all 8 directions from (5, 1):
  → Horizontal: "CAT" ✅ Found in dictionary!
  → Vertical: "O" (single letter, ignored)
  → Diagonals: checked...
Highest scoring word: "CAT" (length 3)
Score += 3 💯
Remove matched tiles 🧹
```

---

## 🎨 Graphics & Rendering

### 🖼️ Texture System

* 26 letter textures (`a.bmp` through `z.bmp`) 🔤
* Precompiled into a single binary `image-data.bin` for fast loading ⚡
* Loaded via `RegisterTextures()` at startup 🚀
* Drawn using OpenGL quads with texture mapping 🎨

### 📐 Coordinate System

* Origin: bottom left corner (0, 0)
* X axis: left to right ➡️
* Y axis: bottom to top ⬆️
* Board: top left quadrant with a 15x2 grid 🧩

---

## 🧪 Extending the Game

| Idea | How | Emoji |
|---|---|---|
| Difficulty Levels | Reduce `timeLeft` or increase board size | 🔥 |
| Multiplayer | Add split screen or alternating turns | 👥 |
| Power ups | Add bomb tiles or time extensions | 💣 |
| Combo Bonuses | Extra points for multiple words per placement | 🔗 |
| Leaderboard | Save top scores to a binary file | 🏆 |
| Sound Effects | Use system calls or an audio library | 🔊 |
| Themes | Toggle board colors or tile styles | 🎨 |

---

## 🐛 Known Limitations

* 🔍 **Linear dictionary search**, can be optimized with binary search
* 👤 **Single player only**, no network or local multiplayer
* 🧩 **Board mechanics**, no gravity or tile shifting
* 💣 **No power ups**, basic mechanics only
* 🎨 **Texture loading**, requires a precompiled `image-data.bin`

---

## 🔧 Troubleshooting

| Issue | Solution |
|---|---|
| ❌ "Couldn't Read the Image Data file" | Ensure `image-data.bin` is in the working directory |
| ❌ Missing dictionary words | Verify `words_alpha.txt` is in the working directory |
| ❌ Compilation errors | Install FreeGLUT with `sudo apt install freeglut3-dev` |
| ❌ No textures display | Ensure the binary texture file is valid |
| 🐢 Slow performance | Reduce the `FPS` constant or optimize dictionary search |

---

## 👨‍💻 Author

<div align="center">

<img src="https://github.com/AbdulAzeemHashmi.png" width="100" style="border-radius:50%" alt="Abdul Azeem Hashmi avatar" />

### Abdul Azeem Hashmi
Programming Fundamentals Project 📚 | AI-A Batch | 24i-2013

[![GitHub](https://img.shields.io/badge/GitHub-AbdulAzeemHashmi-181717?style=for-the-badge&logo=github)](https://github.com/AbdulAzeemHashmi)
[![Repo](https://img.shields.io/badge/Repo-Word--Shooter--Game-blue?style=for-the-badge&logo=github)](https://github.com/AbdulAzeemHashmi/Word-Shooter-Game)

</div>

---

## 📄 License

This project is provided as is for educational purposes 🎓.

<div align="center">

⭐ If you enjoyed this project, consider giving it a star on GitHub! ⭐

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:6BCBFF,100:FF6B6B&height=120&section=footer" width="100%" alt="footer wave" />

</div>
