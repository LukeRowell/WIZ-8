#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <ShlObj.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

struct State
{
	std::string rom_title;
	const static int MEM_SIZE = 4096;	//Total memory size
	unsigned short opcode;				//current opcode
	unsigned char memory[MEM_SIZE];		//total memory
	unsigned char V[16];				//16 "V" registers
	unsigned short stack[16];			//Stack
	unsigned short SP;					//Stack pointer
	unsigned short I;					//I register
	unsigned short PC;					//Program counter
	unsigned char graphics[2048];		//Visual state
	unsigned char delayTimer;			//Delay timer
	unsigned char soundTimer;			//Sound timer
	unsigned char keypad[16];			//Hex keypad
	bool drawFlag;						//Becomes true if the screen needs to be updated
	bool awaitingKeyPress;				//Waiting for any key to be pressed
};

class CHIP8
{
	public:

		CHIP8();
		~CHIP8();

		void setDrawFlag(bool newStatus);
		bool getDrawFlag();

		void setAwaitingKeyPress(bool newStatus);
		bool getAwaitingKeyPress();

		void setKeyPadVal(int index, unsigned char val);
		unsigned char getKeyPadVal(int index);

		void setBackgroundColor(sf::Color newBackgroundColor);
		
		void setForegroundColor(sf::Color newForegroundColor);
		
		std::string getSaveStateRomTitle(int index);

		void initialize();

		bool loadGame(std::string fileName);
	
		void scaleScreen(sf::Vector2f scaleFactor);

		void adjustDisplay(sf::Vector2f adjustments);

		void toggleSound(bool newState);

		void saveState(int index, std::string rom_name);
		
		void loadState(int index);

		void emulateCycle();

		void updateGraphics();

		void draw(sf::RenderWindow* window);

	private:

		const static int MEM_SIZE = 4096;	//Total memory size
		unsigned short opcode;				//current opcode
		unsigned char memory[MEM_SIZE];		//total memory
		unsigned char V[16];				//16 "V" registers
		unsigned short stack[16];			//Stack
		unsigned short SP;					//Stack pointer
		unsigned short I;					//I register
		unsigned short PC;					//Program counter
		unsigned char graphics[2048];		//Visual state
		unsigned char fontset[80];			//Font set
		unsigned char delayTimer;			//Delay timer
		unsigned char soundTimer;			//Sound timer
		unsigned char keypad[16];			//Hex keypad
		bool drawFlag;						//Becomes true if the screen needs to be updated
		bool awaitingKeyPress;				//Waiting for any key to be pressed
		State save_states[4];				//Save states

		sf::Image grid;
		sf::Texture gridTexture;
		sf::Sprite gridSprite;
		sf::Color background_color;
		sf::Color foreground_color;
		sf::Music beep;
};

#endif