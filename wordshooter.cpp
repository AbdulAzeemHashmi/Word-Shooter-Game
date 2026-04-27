/*
Abdul Azeem
24i-2013
AI-A
Programming Fundamental 
Project
*/
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */

alphabets randomBoard[30]; // 1D array for storing the random alphabets (15 per row)
alphabets shooterAlphabet; // Alphabet currently on the shooter
bool isBoardInitialized = false; // Ensure randomization happens only once
alphabets movingAlphabet; // Alphabet currently being moved
int movingAlphabetX = 0, movingAlphabetY = 0; // Current position of the moving alphabet
int targetX = -1, targetY = -1; // Target position (-1 means no movement)
bool isMoving = false; // Flag to track if an alphabet is moving
int timeLeft = 120; // 120 seconds (2 minutes)


void DisplayFunction() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glClear(GL_COLOR_BUFFER_BIT); // Clear the canvas

    // Check if the game is over
    static int timeCounter = 0; // Track elapsed time in seconds
    if (timeCounter >= 120) { // Game over condition
        // Display "Game Over" message
        DrawString(width / 2 - 70, height / 2, width, height, "Game Over!", colors[RED]);
        DrawString(width / 2 - 100, height / 2 - 50, width, height, "Final Score: " + Num2Str(score), colors[BLUE_VIOLET]);
        glutSwapBuffers();
        return; // Stop further rendering
    }

    // Randomly initialize the randomBoard and shooterAlphabet if not already done
    if (!isBoardInitialized) {
        alphabets alphaArray[26]; // Array of all possible alphabets
        for (int i = 0; i < 26; ++i) {
            alphaArray[i] = static_cast<alphabets>(i); // Fill with `AL_A`, `AL_B`, ..., `AL_Z`
        }

        srand(time(0)); // Seed for randomness

        // Fill the randomBoard with random alphabets (30 total, 15 per row)
        for (int i = 0; i < 30; ++i) {
            randomBoard[i] = alphaArray[rand() % 26]; // Assign random alphabets
        }

        // Assign a random alphabet to the shooter
        shooterAlphabet = alphaArray[rand() % 26];

        isBoardInitialized = true; // Mark as initialized
    }

    // Constants for layout
    int startX = 10;               // X position for the first alphabet
    int startY = height - 120;     // Y position for the first row
    int spacingX = 60;             // Horizontal spacing
    int rowSpacing = 70;           // Vertical spacing between rows

    // Draw the first two rows from the randomBoard array0
    for (int i = 0; i < 30; ++i) {
        int row = i / 15; // Calculate row (0 for first row, 1 for second row)
        int col = i % 15; // Calculate column within the row

        // Skip empty cells
        if (randomBoard[i] != static_cast<alphabets>(-1)) {
            DrawAlphabet(randomBoard[i], startX + (col * spacingX), startY - (row * rowSpacing), awidth, aheight);
        }
    }

    // Draw the moving alphabet
    if (isMoving) {
        DrawAlphabet(movingAlphabet, movingAlphabetX, movingAlphabetY, awidth, aheight);
    }

    // Draw shooter at the bottom center
    int shooterX = (width / 2) - 15;  // Horizontal center
    int shooterY = 40;                // Slightly above the bottom
    DrawShooter(shooterX - 15, shooterY, bwidth + 20, bheight + 10);
    DrawAlphabet(shooterAlphabet, shooterX, shooterY + 10, awidth, aheight); // Display shooterAlphabet

    // Draw Score at the top-left corner
    DrawString(40, height - 50, width, height + 5, "Score: " + Num2Str(score), colors[BLUE_VIOLET]);

    // Draw Time Left at the top-center
    int timeLeft = 120 - timeCounter;
    DrawString(width / 2 - 70, height - 55, width, height, "Time Left: " + Num2Str(timeLeft) + " secs", colors[RED]);

    // Increment time counter every second
    timeCounter++;

    // ------------------- DO NOT MODIFY THESE LINES ------------------- //
    DrawShooter((width / 2) - 35, 0, bwidth, bheight);
    glutSwapBuffers();
    // ------------------- DO NOT MODIFY THESE LINES ------------------- //
}

bool FindWordInDictionary(const string &word);
void RemoveWord(int startX, int startY, int dx, int dy, int length);

void CheckWords(int cellX, int cellY) {
    // Directions: {dx, dy} pairs for horizontal, vertical, diagonal
    int directions[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, // Horizontal, Vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonals
    };

    string longestWord = ""; // To store the longest word found
    int maxLength = 0; // Length of the longest word
    int startX = cellX, startY = cellY, endX = cellX, endY = cellY; // Coordinates of the word to remove
    int dirX = 0, dirY = 0; // Direction of the longest word

    // Traverse in all directions
    for (int d = 0; d < 8; ++d) {
        string currentWord = ""; // Word being formed in the current direction
        int x = cellX, y = cellY;

        while (x >= 0 && x < 15 && y >= 0 && y < 2) { // Ensure within bounds
            int index = y * 15 + x; // Convert 2D to 1D index
            if (randomBoard[index] == static_cast<alphabets>(-1)) break; // Stop at empty cells

            currentWord += static_cast<char>('a' + randomBoard[index]); // Build the word

            // Check if the current word exists in the dictionary
            if (FindWordInDictionary(currentWord)) {
                if (currentWord.length() > maxLength) {
                    maxLength = currentWord.length();
                    longestWord = currentWord;
                    startX = cellX;
                    startY = cellY;
                    endX = x;
                    endY = y;
                    dirX = directions[d][0];
                    dirY = directions[d][1];
                }
            }

            // Move in the current direction
            x += directions[d][0];
            y += directions[d][1];
        }
    }

    // If a word was found, remove it and update the score
    if (maxLength > 0) {
        cout << "Found word: " << longestWord << endl;
        RemoveWord(startX, startY, dirX, dirY, maxLength);
    }
}


bool FindWordInDictionary(const string &word) {
    for (int i = 0; i < dictionarysize; ++i) {
        if (dictionary[i] == word) {
            return true; // Word found
        }
    }
    return false; // Word not found
}


void RemoveWord(int startX, int startY, int dx, int dy, int length) {
    for (int i = 0; i < length; ++i) {
        int x = startX + i * dx;
        int y = startY + i * dy;

        if (x >= 0 && x < 15 && y >= 0 && y < 2) {
            int index = y * 15 + x; // Convert 2D to 1D index
            randomBoard[index] = static_cast<alphabets>(-1); // Set to empty
        }
    }
    score += length; // Update score based on word length
    cout << "Score Updated: " << score << endl;
}


/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
	

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) { // Left-click released
        int clickX = x;
        int clickY = height - y; // Invert Y-axis as OpenGL uses bottom-left as origin

        // Ensure the click is inside the board
        if (clickX >= 10 && clickX <= (10 + 15 * 60) && clickY >= (height - 120 - 70) && clickY <= (height - 120)) {
            targetX = clickX;
            targetY = clickY;
            movingAlphabet = shooterAlphabet; // Set the moving alphabet
            movingAlphabetX = (width / 2) - 15; // Start at shooter position
            movingAlphabetY = 50; // Slightly above the shooter
            isMoving = true; // Set the moving flag
        }
    }
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {
    static int gameTimerCounter = 0; // Tracks elapsed time in seconds

    if (gameTimerCounter >= 120) { // Check if 2 minutes (120 seconds) have passed
        // End the game
        isMoving = false; // Stop movement
        for (int i = 0; i < 30; ++i) {
            randomBoard[i] = static_cast<alphabets>(-1); // Clear the board
        }
        glutPostRedisplay(); // Redraw the screen to display "Game Over" message
        return; // Stop further timer processing
    }

    // Update alphabet movement
    if (isMoving) {
        int stepX = (targetX > movingAlphabetX) ? 15 : -15;
        int stepY = (targetY > movingAlphabetY) ? 15 : -15;

        movingAlphabetX += stepX;
        movingAlphabetY += stepY;

        if (ABS(movingAlphabetX - targetX) <= ABS(stepX) && ABS(movingAlphabetY - targetY) <= ABS(stepY)) {
            isMoving = false;
            targetX = -1;
            targetY = -1;

            int cellX = (movingAlphabetX - 10) / 60;
            int cellY = (movingAlphabetY - (height - 120)) / 70;

            int boardIndex = cellY * 15 + cellX;
            randomBoard[boardIndex] = movingAlphabet;

            CheckWords(cellX, cellY);
        }
    }

    // Increment the game timer every second
    gameTimerCounter++;

    // Update display and schedule the next timer event after 1 second
    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(1000, Timer, 0); // Call Timer every 1000 ms (1 second)
}





/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets



	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
