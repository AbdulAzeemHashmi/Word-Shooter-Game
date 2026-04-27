/*
    Abdul Azeem
    24i-2013
    BAI-2A
*/
#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <cmath>

using namespace std;

class PlayerCar;

class Position 
{
    int x, y;
    
    public:
    
    Position(int x = 0, int y = 0) : x(x), y(y) 
    {
    
    }
    
    int getX() const 
    {
        return x; 
    }
    
    int getY() const 
    {
        return y; 
    }
    
    void setX(int newX) 
    { 
        x = newX; 
    }
    
    void setY(int newY) 
    { 
        y = newY; 
    }
    
    void moveUp() 
    {
        y++; 
    }
    
    void moveDown() 
    { 
        y--; 
    }
    
    void moveLeft() 
    { 
        x--; 
    }
    
    void moveRight() 
    { 
        x++; 
    }
    
    double calculateDistance(const Position& other) const 
    { 
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2)); 
    }
    
    bool isEqual(const Position& other) const 
    {
        return x == other.x && y == other.y; 
    }
    
    bool operator==(const Position& other) const 
    { 
        return x == other.x && y == other.y; 
    }
    
    void checkBounds(int maxWidth, int maxHeight) 
    {
        if (x < 0) 
            x = 0;
        if (x >= maxWidth) 
            x = maxWidth - 1;
        if (y < 0) 
            y = 0;
        if (y >= maxHeight) 
            y = maxHeight - 1;
    }
};

class Passenger 
{
    Position pos;
    bool picked;
    int pickupTime, dropOffTime, scoreOnPickup, scoreOnDropOff;
    
    public:
    
    Passenger() : pos(0, 0), picked(false), pickupTime(0), dropOffTime(0), scoreOnPickup(10), scoreOnDropOff(30) 
    {
    
    }
    
    Passenger(int x, int y) : pos(x, y), picked(false), pickupTime(0), dropOffTime(0), scoreOnPickup(10), scoreOnDropOff(30) 
    {
    
    }
    
    Position getPosition() const 
    {
        return pos; 
    }
    
    void setPosition(int x, int y);
    
    bool isPicked() const 
    { 
        return picked; 
    }
    void render(int gridSize) const;
    
    void pickup(int currentTime);
    
    void dropOff(int currentTime);
    
    int calculateBonusScore(int timeTaken) const;
    
    void interact(PlayerCar& player, int& dropX, int& dropY, int gridSize, int screenWidth, int screenHeight);
    
    int getScoreOnPickup() const 
    { 
        return scoreOnPickup; 
    }
    
    int getScoreOnDropOff() const 
    { 
        return scoreOnDropOff; 
    }
};

class DeliveryPoint 
{
    Position pos;
    bool delivered;
    int pickupTime, dropTime;
    
    public:
    
    DeliveryPoint(int x = 0, int y = 0) : pos(x, y), delivered(false), pickupTime(-1), dropTime(-1) 
    {
    
    }
    
    int getX() const 
    { 
        return pos.getX(); 
    }
    
    int getY() const 
    { 
        return pos.getY(); 
    }
    
    Position getPosition() const 
    {
        return pos; 
    }
    
    void setPosition(int x, int y);
    
    bool isDelivered() const 
    { 
        return delivered; 
    }
    
    void startDeliveryTimer(int currentTime);
    
    void deliver(int currentTime);
    
    int getDeliveryDuration() const;
    
    void render(int gridSize) const;
    
    void reset();
    
    void interact(PlayerCar& player, int& dropX, int& dropY);
};

class FuelStation 
{
    Position pos;
    bool available;
    int fuelAmount, cooldownStartTime, cooldownDuration;
    
    public:
    
    FuelStation(int x = 0, int y = 0) : pos(x, y), available(true), fuelAmount(50), cooldownStartTime(0), cooldownDuration(10) 
    {
    
    }
    
    Position getPosition() const 
    { 
        return pos; 
    }
    
    bool isAvailable() const 
    { 
        return available;
    }
    
    void setPosition(int x, int y);
    
    void update(int currentTime);
    
    void refuel(PlayerCar& player, int currentTime);
    
    void interact(PlayerCar& player, int& dropX, int& dropY);
    
    void render(int gridSize) const;
};

class Car 
{
    protected:
    
    Position pos;
    char symbol;
    char** map;
    Passenger* passengers;
    DeliveryPoint* deliveryPoints;
    Mix_Chunk* pickupSound;
    Mix_Chunk* deliverySound;
    int fuel, speed, direction, timer, score, money, xBoundary, yBoundary;
    
    public:
    
    Car(Position p, char s, int f, int sp, int dir, int t, int xB, int yB, char** m, Passenger* pass, DeliveryPoint* deliver)
        : pos(p), symbol(s), fuel(f), speed(sp), direction(dir), timer(t), score(0), money(0), xBoundary(xB), yBoundary(yB), 
        map(m), passengers(pass), deliveryPoints(deliver), pickupSound(NULL), deliverySound(NULL) 
        {
        
        }
        
    bool initAudio() 
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) 
            return false;
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) 
            return false;
        pickupSound = Mix_LoadWAV("/home/abdul-azeem-hashmi/Downloads/Ishq Hai (Official Music Video)  Mismatched Season 3  A Netflix Series  Anurag Saikia - Ishtar Music.mp3");
        
        if (!pickupSound) 
            return false;
        deliverySound = Mix_LoadWAV("/home/abdul-azeem-hashmi/Downloads/Ishq Hai (Official Music Video)  Mismatched Season 3  A Netflix Series  Anurag Saikia - Ishtar Music.mp3");
        
        if (!deliverySound) 
            return false;
        return true;
    }
    
    void playSound(Mix_Chunk* sound) 
    {
        if (sound) 
            Mix_PlayChannel(-1, sound, 0);
    }
    
    virtual void move() 
    {
        if (fuel <= 0 || timer <= 0) 
        {
            stop();
            return;
        }
        
        int dx = 0, dy = 0;
        
        switch (direction) 
        {
            case 0: 
                dx = speed; 
                break;
            case 90: 
                dy = speed; 
                break;
            case 180: 
                dx = -speed; 
                break;
            case 270: 
                dy = -speed; 
                break;
        }
        
        int newX = pos.getX() + dx;
        int newY = pos.getY() + dy;
        
        if (isInBounds(newX, newY)) 
        {
            pos.setX(newX);
            pos.setY(newY);
            fuel -= speed;
            timer--;
            checkTaskCompletion();
        }
    }
    
    bool isInBounds(int gridX, int gridY) 
    {
        return (gridX >= 0) && (gridX < xBoundary) && (gridY >= 0) && (gridY < yBoundary);
    }
    
    void checkTaskCompletion()
    {
        for (int i = 0; i < 3; i++) 
            if (passengers[i].getPosition().getX() != -1 && !passengers[i].isPicked() &&
                pos.getX() == passengers[i].getPosition().getX() && pos.getY() == passengers[i].getPosition().getY()) {
                passengers[i].pickup(timer);
                score += 10;
                money += 20;
                playSound(pickupSound);
                return;
            }
        
        for (int i = 0; i < 3; i++) 
            if (deliveryPoints[i].getX() != -1 && !deliveryPoints[i].isDelivered() && pos.getX() == 
                deliveryPoints[i].getPosition().getX() && pos.getY() == deliveryPoints[i].getPosition().getY()) 
                {
                    deliveryPoints[i].deliver(timer);
                    score += 30;
                    money += 50;
                    playSound(deliverySound);
                    return;
                }
    }
    
    void render() 
    {
        cout << "Rendering Car at (" << pos.getX() << ", " << pos.getY() << ") Symbol: " << symbol << endl;
        cout << "Fuel: " << fuel << " | Timer: " << timer << " | Score: " << score << " | Money: " << money << endl;
    }
    
    void accelerate() 
    { 
        if (speed < 5) 
            speed++; 
    }
    
    void decelerate() 
    {  
        if (speed > 1) 
            speed--; 
    }
    
    void stop() 
    { 
        speed = 0; 
    }
    
    void cleanupAudio() 
    {  
        if (pickupSound) 
            Mix_FreeChunk(pickupSound);
        if (deliverySound) 
            Mix_FreeChunk(deliverySound);
        Mix_CloseAudio();
        SDL_Quit();
    }
    
    Position getPosition() const 
    { 
        return pos; 
    }
    
    int getFuel() const 
    { 
        return fuel; 
    }
    
    int getTimer() const 
    { 
        return timer; 
    }
    
    int getScore() const 
    { 
        return score; 
    }
    
    int getMoney() const 
    { 
        return money; 
    }
    
    void setScore(int newScore) 
    { 
        score = newScore; 
    }
    
    void setMoney(int newMoney) 
    { 
        money = newMoney; 
    }
    
    void setFuel(int newFuel) 
    { 
        fuel = newFuel; 
    }
};

class PlayerCar : public Car 
{
    char role;
    
    bool isValidPosition(const Position& newPos, int gridSize) 
    {
        int gridX = newPos.getX() / gridSize, gridY = newPos.getY() / gridSize;
        
        if (gridX < 0 || gridX >= (xBoundary / gridSize)) 
            return false;
        if (gridY < 0 || gridY >= (yBoundary / gridSize)) 
            return false;
        return isInBounds(gridX, gridY);
    }
    
    void boost() 
    {
        if (fuel >= 10) 
        {
            speed += 2;
            fuel -= 10;
            cout << "Boost activated! Speed: " << speed << ", Fuel: " << fuel << endl;
        }
        else 
            cout << "Not enough fuel for boost!" << endl;
    }
    
    public:
    
    PlayerCar(Position p, char s, int f = 100, int sp = 1, int dir = 0, int t = 180, int xB = 800, int yB = 600,
        char** m = nullptr, Passenger* pass = nullptr, DeliveryPoint* deliver = nullptr)
        : Car(p, s, f, sp, dir, t, xB, yB, m, pass, deliver), role('T') 
        {
        
        }
        
    void pickUpPassenger() 
    {
        score += 10;
        money += 5;
        cout << "Picked up passenger! 🧍 Score: " << score << ", Money: " << money << endl;
    }
    
    void completeDelivery() 
    {
        score += 15;
        money += 10;
        cout << "Completed delivery! 📦 Score: " << score << ", Money: " << money << endl;
    }
    
    void refuel() 
    {
        fuel = 100;
        cout << "Refueled to full. ⛽ Fuel: " << fuel << endl;
    }
    
    int getX() const 
    { 
        return pos.getX(); 
    }
    
    int getY() const 
    { 
        return pos.getY(); 
    }
    
    void handleInput(char input, int gridSize) 
    {
        switch (input) 
        {
            case 'a': 
                direction = 180; 
                break;
            case 'd': 
                direction = 0; 
                break;
            case 'w': 
                direction = 90; 
                break;
            case 's': 
                direction = 270; 
                break;
            case ' ': 
                speed = 0; 
                break;
            case '+': 
                if (speed < 5) 
                    speed++; 
                    break;
            case '-': 
                if (speed > 1) 
                    speed--; 
                    break;
            case 'b': 
                boost(); 
                break;
        }
    }
    void move(int gridSize) 
    {
        if (fuel <= 0 || speed == 0) 
            return;
        int dx = 0, dy = 0;
        
        switch (direction) 
        {
            case 0: 
                dx = speed * gridSize; 
                break;
            case 90: 
                dy = speed * gridSize; 
                break;
            case 180: 
                dx = -speed * gridSize; 
                break;
            case 270: 
                dy = -speed * gridSize; 
                break;
        }
        
        Position newPos = pos;
        newPos.setX(pos.getX() + dx);
        newPos.setY(pos.getY() + dy);
        
        if (isValidPosition(newPos, gridSize)) 
        {
            pos = newPos;
            fuel -= speed;
            checkTaskCompletion();
        }
    }
    
    char getRole() const 
    { 
        return role;
    }
    
    void setRole(char newRole) 
    { 
        role = newRole; 
    }
    
    void draw(int gridSize) 
    {
        int x = pos.getX(), y = pos.getY();
        
        if (role == 'T') 
        {
            DrawRectangle(x, y, 40, 20, colors[YELLOW]);
            DrawCircle(x + 5, y - 5, 5, colors[BLACK]);
            DrawCircle(x + 35, y - 5, 5, colors[BLACK]);
            DrawString(x + 10, y + 25, "T", colors[BLACK]);
        } 
        else if (role == 'K') 
        {
            DrawRectangle(x, y, 60, 30, colors[BLUE]);
            DrawRectangle(x + 45, y + 10, 20, 10, colors[GRAY]);
            DrawCircle(x + 10, y - 5, 5, colors[BLACK]);
            DrawCircle(x + 50, y - 5, 5, colors[BLACK]);
            DrawString(x + 20, y + 35, "K", colors[WHITE]);
        }
    }
    
    void handleCollision()
    {
        const int fuelLoss = 5;
        fuel -= fuelLoss;
        
        if (fuel <= 0) 
            fuel = 0;
        cout << "Collision! Fuel reduced by " << fuelLoss << ". Current fuel: " << fuel << endl;
    }
    
    void checkFuel() 
    {
        if (fuel <= 0) 
        {
            stop();
            cout << "Out of fuel! You must refuel!" << endl;
        }
    }
    
    void checkTimer() 
    {
        if (timer <= 0) 
        {
            stop();
            cout << "Time's up! You failed the mission!" << endl;
        }
    }
    
    void refuel(int amount) 
    {
        fuel += amount;
        if (fuel > 100) 
            fuel = 100;
        cout << "Refueled. Fuel level: " << fuel << endl;
    }
    
    void render() 
    {
        cout << "🚗 Player Car @ (" << pos.getX() << ", " << pos.getY() << ") | Symbol: " << symbol
             << "\n⛽ Fuel: " << fuel << " | 💨 Speed: " << speed << " | ⏱️ Timer: " << timer
             << " | 🏁 Score: " << score << " | 💰 Money: " << money << endl;
    }
};

void Passenger::setPosition(int x, int y) 
{
    pos.setX(x);
    pos.setY(y);
}

void Passenger::render(int gridSize) const 
{
    if (!picked) 
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        int x = pos.getX(), y = pos.getY();
        glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x + gridSize, y);
        glVertex2i(x + gridSize, y + gridSize);
        glVertex2i(x, y + gridSize);
        glEnd();
    }
}

void Passenger::pickup(int currentTime) 
{
    if (!picked) 
    {
        picked = true;
        pickupTime = currentTime;
        cout << "Passenger picked up at time " << currentTime << "!" << endl;
    }
}

void Passenger::dropOff(int currentTime) 
{
    if (picked) 
    {
        picked = false;
        dropOffTime = currentTime;
        int timeTaken = dropOffTime - pickupTime, bonusScore = calculateBonusScore(timeTaken), totalScore = scoreOnDropOff + bonusScore;
        
        cout << "Passenger dropped off at time " << currentTime << ". Time taken: " << timeTaken << " seconds." << endl;
        cout << "Score for delivery: " << totalScore << " (Base score: " << scoreOnDropOff << ", Bonus score: " << bonusScore << ")" << endl;
    }
}

int Passenger::calculateBonusScore(int timeTaken) const 
{
    if (timeTaken <= 30) 
        return 20;
    else if (timeTaken <= 60) 
        return 10;
    return 0;
}

void Passenger::interact(PlayerCar& player, int& dropX, int& dropY, int gridSize, int screenWidth, int screenHeight) 
{
    if (!picked && player.getX() == pos.getX() && player.getY() == pos.getY()) 
    {
        picked = true;
        player.pickUpPassenger();
        dropX = (rand() % (screenWidth / gridSize)) * gridSize;
        dropY = (rand() % (screenHeight / gridSize)) * gridSize;
    }
}

void DeliveryPoint::setPosition(int x, int y) 
{
    pos.setX(x);
    pos.setY(y);
}

void DeliveryPoint::startDeliveryTimer(int currentTime) 
{
    if (pickupTime == -1) 
        pickupTime = currentTime;
}

void DeliveryPoint::deliver(int currentTime) 
{
    if (!delivered) 
    {
        delivered = true;
        dropTime = currentTime;
    }
}

int DeliveryPoint::getDeliveryDuration() const 
{
    if (pickupTime != -1 && dropTime != -1) 
        return dropTime - pickupTime;
    return -1;
}

void DeliveryPoint::render(int gridSize) const 
{
    glColor3f(delivered ? 0.5f : 0.2f, delivered ? 0.5f : 0.7f, delivered ? 0.5f : 0.2f);
    int x = pos.getX(), y = pos.getY();
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + gridSize, y);
    glVertex2i(x + gridSize, y + gridSize);
    glVertex2i(x, y + gridSize);
    glEnd();
}

void DeliveryPoint::reset() 
{
    delivered = false;
    pickupTime = -1;
    dropTime = -1;
}

void DeliveryPoint::interact(PlayerCar& player, int& dropX, int& dropY) 
{
    if (!delivered && player.getX() == pos.getX() && player.getY() == pos.getY()) 
    {
        delivered = true;
        player.completeDelivery();
        dropX = -1;
        dropY = -1;
    }
}

void FuelStation::setPosition(int x, int y) 
{
    pos.setX(x);
    pos.setY(y);
}

void FuelStation::update(int currentTime) 
{
    if (!available && currentTime - cooldownStartTime >= cooldownDuration) 
        available = true;
}

void FuelStation::refuel(PlayerCar& player, int currentTime) 
{
    if (available) 
    {
        int currentFuel = player.getFuel();
        
        if (currentFuel < 100) 
        {
            int fuelToAdd = fuelAmount;
            
            if (currentFuel + fuelToAdd > 100) 
                fuelToAdd = 100 - currentFuel;
                
            player.setFuel(currentFuel + fuelToAdd);
            available = false;
            cooldownStartTime = currentTime;
        }
    }
}

void FuelStation::interact(PlayerCar& player, int& dropX, int& dropY) 
{
    if (available && player.getX() == pos.getX() && player.getY() == pos.getY()) 
    {
        player.refuel();
        available = false;
    }
}

void FuelStation::render(int gridSize) const 
{
    glColor3f(available ? 0.0f : 0.5f, available ? 1.0f : 0.5f, 0.0f);
    int x = pos.getX(), y = pos.getY();
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + gridSize, y);
    glVertex2i(x + gridSize, y + gridSize);
    glVertex2i(x, y + gridSize);
    glEnd();
}

class Leaderboard 
{
    char name[20];
    int score;
    char role[10];
    
    public:
    
    Leaderboard() 
    {
        name[0] = '\0';
        score = 0;
        role[0] = '\0';
    }
    
    void set(const char* playerName, int playerScore, const char* playerRole) 
    {
        int i = 0;
        
        while (playerName[i] != '\0' && i < 19) 
        {
            name[i] = playerName[i];
            i++;
        }
        
        name[i] = '\0';
        score = playerScore;
        i = 0;
        
        while (playerRole[i] != '\0' && i < 9) 
        {
            role[i] = playerRole[i];
            i++;
        }
        role[i] = '\0';
    }
    
    void saveToFile() 
    {
        ofstream fout("leaderboard.dat", ios::binary | ios::app);
        
        if (fout.is_open())
        {
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
    }
    
    void displayTop5(const char* currentName) 
    {
        Leaderboard entries[100];
        int count = 0;
        ifstream fin("leaderboard.dat", ios::binary);
        
        if (fin.is_open()) 
        {
            while (fin.read((char*)&entries[count], sizeof(Leaderboard))) 
            {
                count++;
                if (count >= 100) 
                    break;
            }
            fin.close();
        }
        
        for (int i = 0; i < count - 1; i++) 
            for (int j = 0; j < count - i - 1; j++) 
                if (entries[j].score < entries[j + 1].score) 
                {
                    Leaderboard temp = entries[j];
                    entries[j] = entries[j + 1];
                    entries[j + 1] = temp;
                }
          
        cout << "\n==== TOP 5 SCORES ====\n";
        
        int top = (count < 5) ? count : 5;
        
        for (int i = 0; i < top; i++) 
        {
            bool highlight = (strcmp(entries[i].name, currentName) == 0);
            
            if (highlight)
                cout << i + 1 << ". *" << entries[i].name << " (" << entries[i].role << ")* - " << entries[i].score << "\n";
            else
                cout << i + 1 << ". " << entries[i].name << " (" << entries[i].role << ") - " << entries[i].score << "\n";
        }
        
        cout << "======================\n";
    }
    
    void clearLeaderboard() 
    {
        ofstream fout("leaderboard.dat", ios::binary | ios::trunc);
        fout.close();
    }
    
    int getHighestScore() 
    {
        Leaderboard entry;
        int highest = 0;
        
        ifstream fin("leaderboard.dat", ios::binary);
        
        if (fin.is_open()) 
        {
            while (fin.read((char*)&entry, sizeof(Leaderboard))) 
                if (entry.score > highest) 
                    highest = entry.score; 
            fin.close();
        }
        return highest;
    }
};

class Button 
{
    int x, y, width, height;
    bool hovered;
    char label[30];
    void (*action)();
    
    public:
    
    Button() : x(0), y(0), width(0), height(0), hovered(false), action(nullptr) 
    { 
        strcpy(label, ""); 
    }
    
    Button(int _x, int _y, int _w, int _h, const char* _label, void (*_action)()) : x(_x), y(_y), width(_w), height(_h), hovered(false), action(_action) 
    {
        strncpy(label, _label, 29);
        label[29] = '\0';
    }
    
    void checkHover(int mouseX, int mouseY) 
    {
        hovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
    }
    
    void onClick() 
    {
        if (action) 
            action();
    }
    
    bool isHovered() const 
    { 
        return hovered; 
    }
    
    void render() const 
    {
        glColor3f(hovered ? 1.0f : 0.8f, hovered ? 1.0f : 0.8f, hovered ? 0.0f : 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
        
        int textX = x + width / 4;
        int textY = y + height / 3;
        
        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(textX, textY);
        
        for (int i = 0; label[i] != '\0'; ++i)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[i]);
    }
    
    int getX() const 
    { 
        return x; 
    }
    
    int getY() const 
    {
        return y; 
    }
    
    int getWidth() const 
    { 
        return width; 
    }
    
    int getHeight() const 
    { 
        return height; 
    }
    
    const char* getLabel() const 
    { 
        return label; 
    }
};

class Game 
{
    Button playButton, quitButton;
    bool isRunning;
    int dropX, dropY, timer;
    
    PlayerCar player;
    Passenger passengers[3];
    FuelStation fuelStations[2];
    DeliveryPoint deliveries[3];
    Leaderboard leaderboard;
    
    const int GRID_SIZE = 20;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int MAX_PASSENGERS = 3;
    const int MAX_FUEL_STATIONS = 2;
    const int MAX_DELIVERIES = 3;
    
    Position getRandomGridPosition() 
    {
        return Position((rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE, (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE);
    }
    
    public:
    
    Game() : playButton(100, 400, 150, 50, "Play", startGame), quitButton(100, 330, 150, 50, "Quit", quitGame), isRunning(false), dropX(-1), dropY(-1), timer(0),
             player(Position(0, 0), 'C', 100, 1, 0, 180, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, passengers, deliveries) 
    {
        srand(time(0));
        player.initAudio();
        resetGame();
    }
    
    ~Game() 
    {
        player.cleanupAudio();
    }
    
    Button& getPlayButton() 
    {
        return playButton; 
    }
    
    Button& getQuitButton() 
    { 
        return quitButton; 
    }
    
    static void startGame() 
    {
        cout << "Game Started!" << endl;
    }
    
    static void quitGame() 
    {
        cout << "Game Quit!" << endl;
        exit(0);
    }
    
    int getTime() const 
    { 
        return timer; 
    }
    
    int& getDropX() 
    { 
        return dropX; 
    }
    
    int& getDropY() 
    { 
        return dropY; 
    }
    
    PlayerCar& getPlayer() 
    { 
        return player; 
    }
    
    int getGridSize() const 
    {
        return GRID_SIZE; 
    }
    
    int getScreenWidth() const 
    {   
        return SCREEN_WIDTH; 
    }
    
    int getScreenHeight() const 
    { 
        return SCREEN_HEIGHT; 
    }
    
    FuelStation* getFuelStations() 
    { 
        return fuelStations; 
    }
    
    int getMaxFuelStations() const 
    { 
        return MAX_FUEL_STATIONS;
    }
    
    DeliveryPoint* getDeliveries() 
    { 
        return deliveries; 
    }
    
    int getMaxDeliveries() const 
    {
        return MAX_DELIVERIES; 
    }
    
    Passenger* getPassengers() 
    {
        return passengers; 
    }
    
    int getMaxPassengers() const 
    { 
        return MAX_PASSENGERS; 
    }
    
    bool isGameRunning() const 
    { 
        return isRunning; 
    }
    
    void incrementTime() 
    { 
        timer++; 
    }
    
    void endGame() 
    { 
        isRunning = false; 
    }
    
    void resetGame() 
    {
        isRunning = true;
        timer = 180;
        dropX = dropY = -1;
        player = PlayerCar(getRandomGridPosition(), 'C', 100, 1, 0, 180, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, passengers, deliveries);
        player.setRole('T');
        player.setFuel(100);
        player.setScore(0);
        player.setMoney(0);
        
        for (int i = 0; i < MAX_PASSENGERS; i++) 
        {
            Position p = getRandomGridPosition();
            passengers[i].setPosition(p.getX(), p.getY());
        }
        
        for (int i = 0; i < MAX_FUEL_STATIONS; i++) 
        {
            Position p = getRandomGridPosition();
            fuelStations[i].setPosition(p.getX(), p.getY());
        }
        
        for (int i = 0; i < MAX_DELIVERIES; i++) 
        {
            Position p = getRandomGridPosition();
            deliveries[i].setPosition(p.getX(), p.getY());
        }
    }
    void updateGame() 
    {
        if (!isRunning) 
            return;
        
        timer--;
        player.setFuel(player.getFuel() - 1);
        
        for (int i = 0; i < MAX_FUEL_STATIONS; i++) 
            fuelStations[i].update(timer);
        
        if (timer <= 0 || player.getFuel() <= 0 || player.getScore() >= 100) 
        {
            isRunning = false;
            leaderboard.set("Player", player.getScore(), (player.getRole() == 'T' ? "Taxi" : "Delivery"));
            leaderboard.saveToFile();
        }
        player.checkFuel();
        player.checkTimer();
        handleCollisions();
    }
    
    void handleCollisions() 
    {
        Position playerPos = player.getPosition();
        
        if (player.getRole() == 'T') 
            for (int i = 0; i < MAX_PASSENGERS; i++) 
            {
                if (!passengers[i].isPicked() && passengers[i].getPosition().isEqual(playerPos)) 
                {
                    passengers[i].pickup(timer);
                    Position dropPos = getRandomGridPosition();
                    dropX = dropPos.getX();
                    dropY = dropPos.getY();
                    player.setScore(player.getScore() + passengers[i].getScoreOnPickup());
                    player.setMoney(player.getMoney() + 20);
                }
                if (passengers[i].isPicked() && Position(dropX, dropY).isEqual(playerPos)) {
                    passengers[i].dropOff(timer);
                    player.setScore(player.getScore() + passengers[i].getScoreOnDropOff());
                    player.setMoney(player.getMoney() + 50);
                    dropX = -1;
                    dropY = -1;
                }
            }
         
        else if (player.getRole() == 'K') 
            for (int i = 0; i < MAX_DELIVERIES; i++) 
                if (!deliveries[i].isDelivered() && deliveries[i].getPosition().isEqual(playerPos)) 
                {
                    deliveries[i].deliver(timer);
                    player.setScore(player.getScore() + 15);
                    player.setMoney(player.getMoney() + 75);
                }
          
        for (int i = 0; i < MAX_FUEL_STATIONS; i++) 
            if (fuelStations[i].isAvailable() && fuelStations[i].getPosition().isEqual(playerPos)) 
                fuelStations[i].refuel(player, timer);
           
    }
    
    void renderStat(const char* label, int value, int x) 
    {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", value);
        DrawString(x, SCREEN_HEIGHT - 20, label, colors[MISTY_ROSE]);
        DrawString(x + 50, SCREEN_HEIGHT - 20, buffer, colors[WHITE]);
    }
    
    void displayHUD() 
    {
        string status = "Fuel: " + to_string(player.getFuel()) + " | Score: " + to_string(player.getScore()) + " | Money: " + to_string(player.getMoney()) +
                       " | Time: " + to_string(timer) + " | Role: " + (player.getRole() == 'T' ? "Taxi" : "Delivery");
        glColor3f(1, 1, 1);
        glRasterPos2i(10, SCREEN_HEIGHT - 20);
        for (char c : status)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    void handleMouseClick(int button, int state, int x, int y) 
    {
        int flippedY = SCREEN_HEIGHT - y;
        
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
        {
            playButton.checkHover(x, flippedY);
            quitButton.checkHover(x, flippedY);
            
            if (playButton.isHovered()) 
            {
                playButton.onClick();
                isRunning = true;
            }
            
            if (quitButton.isHovered()) 
                quitButton.onClick();
        }
    }
    
    void handleSpecialKey(int key) 
    {
        if (!isRunning) 
            return;
        switch (key) 
        {
            case GLUT_KEY_LEFT: 
                player.handleInput('a', GRID_SIZE); 
                player.move(GRID_SIZE); 
                break;
            case GLUT_KEY_RIGHT: 
                player.handleInput('d', GRID_SIZE); 
                player.move(GRID_SIZE); 
                break;
            case GLUT_KEY_UP: 
                player.handleInput('w', GRID_SIZE); 
                player.move(GRID_SIZE); 
                break;
            case GLUT_KEY_DOWN: 
                player.handleInput('s', GRID_SIZE); 
                player.move(GRID_SIZE); 
                break;
        }
    }
    
    void handleKeyPress(unsigned char key) 
    {
        if (key == 'r' || key == 'R') 
            resetGame();
        else if (key == 't' || key == 'T') 
            player.setRole('T');
        else if (key == 'd' || key == 'D') 
            player.setRole('K');
        else 
            player.handleInput(key, GRID_SIZE);
    }
    
    void renderUI() 
    {
        playButton.render();
        quitButton.render();
    }
    
    void drawDropOff(int x, int y) 
    {
        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x + GRID_SIZE, y);
        glVertex2i(x + GRID_SIZE, y + GRID_SIZE);
        glVertex2i(x, y + GRID_SIZE);
        glEnd();
    }
    
    void renderHUD() 
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2i(0, SCREEN_HEIGHT - 30);
        glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT - 30);
        glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT);
        glVertex2i(0, SCREEN_HEIGHT);
        glEnd();
        renderStat("Fuel: ", player.getFuel(), 10);
        renderStat("Score: ", player.getScore(), 200);
        renderStat("Time: ", timer, 400);
        string role = (player.getRole() == 'T') ? "Taxi" : "Delivery";
        DrawString(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 20, ("Mode: " + role).c_str(), colors[GOLD]);
    }
    
    void renderGameOver() 
    {
        string message;
        if (player.getFuel() <= 0) 
            message = "GAME OVER - OUT OF FUEL!";
        else if (timer <= 0) 
            message = "GAME OVER - TIME'S UP!";
        else if (player.getScore() >= 100)
            message = "MISSION COMPLETED!";
        else 
            message = "GAME OVER";
        glColor3f(1.0f, 0.0f, 0.0f);
        
        glRasterPos2i(SCREEN_WIDTH / 2 - message.length() * 9, SCREEN_HEIGHT / 2);
        
        for (char c : message)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
            
        leaderboard.displayTop5("Player");
    }
};

Game game;

void display() 
{
    glClearColor(0, 148.0f / 255.0f, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game.getPlayer().draw(game.getGridSize());
    
    if (game.getPlayer().getRole() == 'T') 
        for (int i = 0; i < game.getMaxPassengers(); i++)
            if (!game.getPassengers()[i].isPicked())
                game.getPassengers()[i].render(game.getGridSize());
                
    else if (game.getPlayer().getRole() == 'K') 
        for (int i = 0; i < game.getMaxDeliveries(); i++)
            if (!game.getDeliveries()[i].isDelivered())
                game.getDeliveries()[i].render(game.getGridSize());
    
    for (int i = 0; i < game.getMaxFuelStations(); i++)
        if (game.getFuelStations()[i].isAvailable())
            game.getFuelStations()[i].render(game.getGridSize());
            
    if (game.getDropX() != -1 && game.getDropY() != -1 && game.getPlayer().getRole() == 'T')
        game.drawDropOff(game.getDropX(), game.getDropY());
        
    game.renderHUD();
    
    if (!game.isGameRunning()) 
    {
        game.renderUI();
        game.renderGameOver();
    }
    
    glutSwapBuffers();
}

void SetCanvasSize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (width <= height)
        gluOrtho2D(0.0, 800, 0.0, 600 * (height / (float)width));
    else
        gluOrtho2D(0.0, 800 * (width / (float)height), 0.0, 600);
        
    glMatrixMode(GL_MODELVIEW);
}

void Timer(int) 
{
    game.updateGame();
    
    if (game.getTime() <= 0) 
        game.endGame();
    
    glutPostRedisplay();
    glutTimerFunc(1000, Timer, 0);
}

void NonPrintableKeys(int key, int, int) 
{
    game.handleSpecialKey(key);
}

void PrintableKeys(unsigned char key, int, int) 
{
    game.handleKeyPress(key);
}

void MousePressedAndMoved(int x, int y) 
{
    int gridSize = game.getGridSize(), screenHeight = game.getScreenHeight(), gridX = (x / gridSize) * gridSize, gridY = ((screenHeight - y) / gridSize) * gridSize;
    
    PlayerCar& player = game.getPlayer();
    
    if (player.getRole() == 'T') 
        for (int i = 0; i < game.getMaxPassengers(); i++)
            game.getPassengers()[i].interact(player, game.getDropX(), game.getDropY(), gridSize, game.getScreenWidth(), game.getScreenHeight());
     else if (player.getRole() == 'K') 
        for (int i = 0; i < game.getMaxDeliveries(); i++)
            game.getDeliveries()[i].interact(player, game.getDropX(), game.getDropY());
    
    for (int i = 0; i < game.getMaxFuelStations(); i++)
        game.getFuelStations()[i].interact(player, game.getDropX(), game.getDropY());
}

void MouseMoved(int x, int y) 
{
    int flippedY = game.getScreenHeight() - y;
    game.getPlayButton().checkHover(x, flippedY);
    game.getQuitButton().checkHover(x, flippedY);
    glutPostRedisplay();
}

void MouseClicked(int button, int state, int x, int y) 
{
    game.handleMouseClick(button, state, x, y);
    glutPostRedisplay();
}

int main(int argc, char **argv) 
{
    InitRandomizer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(1200, 1200);
    glutCreateWindow("OOP Project");
    SetCanvasSize(1200, 1200);
    glutDisplayFunc(display);
    glutSpecialFunc(NonPrintableKeys);
    glutKeyboardFunc(PrintableKeys);
    glutTimerFunc(1000, Timer, 0);
    glutMouseFunc(MouseClicked);
    glutPassiveMotionFunc(MouseMoved);
    glutMotionFunc(MousePressedAndMoved);
    glutMainLoop();
    return 1;
}
#endif
