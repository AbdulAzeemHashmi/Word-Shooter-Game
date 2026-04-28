# 🎯 Word Shooter – GLUT Ultimate Edition  
*A high-octane word-matching game with 370k dictionary words, 2-minute timed rounds, 8-directional word detection, and full on-screen scoring.*

---

## 🎮 About

This is a **fast-paced Word Shooter** game written in **C++17** using **OpenGL/GLUT**.  
Shoot letter tiles from a cannon at the bottom to match them with board letters and form **valid English words**. All gameplay including **real-time scoring**, **live countdown timer**, and **word validation** happens **inside one window**.  
The project features a **370,099-word dictionary**, **8-directional word detection** (horizontal, vertical, diagonal), and **texture-based rendering** with a robust build system.

---

## ✨ Features

| Feature | Included |
|---------|----------|
| 🎯 **Interactive shooter cannon** | ✅ |
| 📖 **370,099-word dictionary** (`words_alpha.txt`) | ��� |
| ⏱️ **2-minute timed rounds** with live countdown | ✅ |
| 🔤 **Letter tiles with texture rendering** | ✅ |
| 🔍 **8-directional word detection** (H, V, 4 diagonals) | ✅ |
| 💯 **Dynamic scoring** based on word length | ✅ |
| 🎨 **Binary texture data** (`image-data.bin`) | ✅ |
| 🖱️ **Mouse-based targeting and shooting** | ✅ |
| 📊 **Real-time score display** | ✅ |
| ⚙️ **Makefile build system** | ✅ |

---

## 🚀 Quick Start

### 1. Install FreeGLUT

| OS | Command |
|--|--|
| **Linux** | `sudo apt install freeglut3-dev build-essential libfreeimage-dev` |
| **macOS** | `brew install freeglut freeimage` |
| **Windows** | MinGW-w64 + freeglut (copy `freeglut.dll` next to exe) |

### 2. Build
```bash
make
```

Or manually compile:
```bash
g++ wordshooter.cpp util.cpp -o word-shooter -lGL -lGLU -lglut -lfreeimage -std=c++17 -Wall -Wextra
```

### 3. Run
```bash
./word-shooter
```

---

## 🕹️ Controls

| Input | Action |
|--|--|
| **Mouse Move** | Aim the shooter cannon |
| **Left Click** | Shoot the letter tile toward target |
| **ESC** | Quit the game |

---

## 📁 File Tree

```
Word-Shooter-Game/
├── wordshooter.cpp      ← main game logic
├── util.cpp             ← utility functions
├── util.h               ← utility headers
├── Board.cpp            ← board management
├── Board.h              ← board declarations
├── CImg.h               ← image processing library
├── image-data.bin       ← precompiled texture data
├── words_alpha.txt      ← 370,099 English words
├── Makefile             ← build configuration
├── install-libraries.sh ← dependency installer
└── README.md            ← this file
```

---

## 🎮 Gameplay

### Objective
Form valid English words within **2 minutes** to score points.

### How It Works

1. **Random Board Initialization**  
   - Two rows of randomly selected letter tiles (30 total, 15 per row)
   - A random letter in the shooter cannon at bottom-center

2. **Shooting Mechanism**  
   - Mouse click targets a location on the board
   - Letter shoots from cannon toward target position
   - Tile placed on the board at target location

3. **Word Detection**  
   - After placement, game scans all 8 directions (horizontal, vertical, diagonal)
   - Finds longest valid word containing the placed tile
   - Uses dictionary lookup in 370k-word list

4. **Scoring**  
   - Points = length of valid word found
   - Matched words removed from board
   - Score updates in real-time

5. **Game End**  
   - **Time's up**: After 120 seconds, game over screen shows final score
   - **Restart**: Rerun the program to play again

---

## 🛠️ Configuration Constants

Edit **top of `wordshooter.cpp`** to tweak:

| Constant | Default | Meaning |
|--|--|--|
| `width` | 930 | canvas width (pixels) |
| `height` | 660 | canvas height (pixels) |
| `bradius` | 30 | ball/tile radius |
| `timeLeft` | 120 | round duration (seconds) |
| `nalphabets` | 26 | number of letters (A-Z) |
| `dictionarysize` | 370099 | dictionary entries |
| `FPS` | 10 | frames per second |

---

## 📊 Dictionary & Validation

The `words_alpha.txt` file contains **370,099 English words**:
- One word per line, alphabetically sorted
- Loaded into memory at startup
- Searched linearly during word validation

**Sample validation flow:**
```
User shoots 'O' at position (5, 1)
Scan all 8 directions from (5, 1):
  → Horizontal: "CAT" ✓ Found in dictionary!
  → Vertical: "O" (single letter, ignored)
  → Diagonals: ... (checked)
Highest scoring word: "CAT" (length 3)
Score += 3
Remove matched tiles
```

---

## 🎨 Graphics & Rendering

### Texture System
- **26 letter textures** (a.bmp – z.bmp)
- Precompiled into binary `image-data.bin` for fast loading
- Loaded via `RegisterTextures()` at startup
- Drawn with OpenGL quads and texture mapping

### Coordinate System
- **Origin**: Bottom-left (0, 0)
- **X-axis**: Left to right
- **Y-axis**: Bottom to top
- **Board**: Top-left quadrant with 15×2 grid

---

## 🧪 Extending the Game

| Idea | How |
|--|--|
| **Difficulty Levels** | Reduce `timeLeft` or increase board size |
| **Multiplayer** | Split-screen or alternating turns |
| **Power-ups** | Add bomb tiles or time extensions |
| **Combo Bonuses** | Extra points for multiple words per placement |
| **Leaderboard** | Save top scores to binary file |
| **Sound Effects** | Use system calls or audio library |
| **Themes** | Toggle board colors or tile styles |

---

## 🐛 Known Limitations

- **Linear dictionary search** → can be optimized with binary search  
- **Single player only** → no network/local multiplayer  
- **Board mechanics** → no gravity or tile shifting  
- **No power-ups** → basic mechanics only  
- **Texture loading** → requires precompiled `image-data.bin`

---

## 🔧 Troubleshooting

| Issue | Solution |
|--|--|
| **"Couldn't Read the Image Data file"** | Ensure `image-data.bin` is in working directory |
| **Missing dictionary words** | Verify `words_alpha.txt` is in working directory |
| **Compilation errors** | Install FreeGLUT: `sudo apt install freeglut3-dev` |
| **No textures display** | Ensure binary texture file is valid |
| **Slow performance** | Reduce FPS constant or optimize dictionary search |

---

## 📝 Author

**Abdul Azeem Hashmi**  
Programming Fundamentals Project | AI-A Batch | 24i-2013

---

## 📄 License

This project is provided as-is for educational purposes.
