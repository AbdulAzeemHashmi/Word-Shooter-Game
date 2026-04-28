# Word Shooter Game 🎮

A fun and interactive word-matching game built with C++ and OpenGL. Shoot letters to form valid English words and score points before time runs out!

## 🎯 Overview

**Word Shooter Game** is an educational game that combines spelling and puzzle-solving with arcade-style gameplay. Players shoot alphabetic characters from a shooter at the bottom of the screen to match them with letters on the board. When valid words are formed, they are removed and points are awarded.

- **Language:** C++
- **Graphics Library:** OpenGL (GLUT)
- **Game Duration:** 2 minutes (120 seconds)
- **Dictionary:** 370,099 English words

## 🎮 Game Features

- **Dynamic Board:** 30 randomly selected letters displayed in a 15×2 grid
- **Shooter Mechanism:** Launch letters from the bottom center of the screen toward the board
- **Word Validation:** Automatically detects valid English words in 8 directions:
  - Horizontal (left-right)
  - Vertical (up-down)
  - Diagonal (4 directions)
- **Scoring System:** Earn points based on word length
- **Time Limit:** Complete challenge within 120 seconds
- **Real-time Display:** 
  - Current score
  - Time remaining
  - Active shooter letter

## 🛠️ Installation & Setup

### Prerequisites

You need the following libraries installed:

- **GLUT** (OpenGL Utility Toolkit)
- **GLU** (OpenGL Utility Library)
- **OpenGL (GL)**
- **X11** (X Window System)
- **FreeImage** (for image handling)

### Quick Setup (Linux/Mac)

Run the installation script:

```bash
bash install-libraries.sh
