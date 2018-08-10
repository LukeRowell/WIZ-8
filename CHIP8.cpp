#include "CHIP8.h"

CHIP8::CHIP8() : fontset{ 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
							0x20, 0x60, 0x20, 0x20, 0x70, // 1
							0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
							0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
							0x90, 0x90, 0xF0, 0x10, 0x10, // 4
							0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
							0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
							0xF0, 0x10, 0x20, 0x40, 0x40, // 7
							0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
							0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
							0xF0, 0x90, 0xF0, 0x90, 0x90, // A
							0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
							0xF0, 0x80, 0x80, 0x80, 0xF0, // C
							0xE0, 0x90, 0x90, 0x90, 0xE0, // D
							0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
							0xF0, 0x80, 0xF0, 0x80, 0x80 }  // F
{
	background_color = sf::Color::Black;
	foreground_color = sf::Color::White;

	grid.create(64, 32, background_color);
	gridTexture.loadFromImage(grid);
	gridSprite.setTexture(gridTexture);
	gridSprite.setPosition(sf::Vector2f(0, 20));
	gridSprite.scale(sf::Vector2f(10, 10));

	if (!beep.openFromFile("beep.wav"))
	{

	}

	else
	{
		beep.setVolume(25);
		beep.setPitch(2);
	}

	delayTimer = 0x00;
	soundTimer = 0x00;
	drawFlag = false;
	awaitingKeyPress = false;
}

CHIP8::~CHIP8()
{

}

void CHIP8::setDrawFlag(bool newStatus)
{
	drawFlag = newStatus;
}

bool CHIP8::getDrawFlag()
{
	return drawFlag;
}

void CHIP8::setAwaitingKeyPress(bool newStatus)
{
	awaitingKeyPress = newStatus;
}

bool CHIP8::getAwaitingKeyPress()
{
	return awaitingKeyPress;
}

void CHIP8::setKeyPadVal(int index, unsigned char val)
{
	keypad[index] = val;
}

unsigned char CHIP8::getKeyPadVal(int index)
{
	return keypad[index];
}

void CHIP8::setBackgroundColor(sf::Color newBackgroundColor)
{
	background_color = newBackgroundColor;
}

void CHIP8::setForegroundColor(sf::Color newForegroundColor)
{
	foreground_color = newForegroundColor;
}

std::string CHIP8::getSaveStateRomTitle(int index)
{
	return save_states[index].rom_title;
}

void CHIP8::initialize()
{
	PC = 0x0200;
	opcode = 0x0000;
	I = 0x0000;
	SP = 0x0000;

	for (int i = 0; i < MEM_SIZE; i++)
	{
		memory[i] = 0x00;
	}

	for (int i = 0; i < 80; i++)
	{
		memory[i + 0x0050] = fontset[i];
	}

	for (int i = 0; i < 16; i++)
	{
		keypad[i] = 0x00;
		stack[i] = 0x00;
	}

	for (int i = 0; i < 2048; i++)
	{
		graphics[i] = 0x00;
	}

	delayTimer = 0x00;
	soundTimer = 0x00;
	drawFlag = false;
	awaitingKeyPress = false;
}

bool CHIP8::loadGame(std::string fileName)
{
	std::ifstream ROM_File;
	int memoryOffset = 0x0000;
	char currentVal;
	bool status;

	ROM_File.open(fileName, std::ios::binary);

	if (!ROM_File.is_open())
	{
		std::cout << "Could not open ROM: " << fileName << std::endl;
		status = false;
	}

	else
	{
		while (ROM_File.get(currentVal))
		{
			memory[0x0200 + memoryOffset] = (unsigned char)currentVal;
			memoryOffset++;
		}

		status = true;
	}

	ROM_File.close();

	return status;
}

void CHIP8::scaleScreen(sf::Vector2f scaleFactor)
{
	gridSprite.setScale(sf::Vector2f(1, 1));
	gridSprite.scale(scaleFactor);
}

void CHIP8::adjustDisplay(sf::Vector2f adjustments)
{
	gridSprite.setPosition(sf::Vector2f(0, 0));
	gridSprite.move(adjustments);
}

void CHIP8::toggleSound(bool newState)
{
	if (newState)
	{
		beep.setVolume(25);
	}

	else
	{
		beep.setVolume(0);
	}
}

void CHIP8::saveState(int index, std::string rom_name)
{
	save_states[index].rom_title = rom_name;
	save_states[index].opcode = opcode;
	
	for (int i = 0; i < MEM_SIZE; i++)
	{
		save_states[index].memory[i] = memory[i];
	}

	for (int i = 0; i < 16; i++)
	{
		save_states[index].V[i] = V[i];
		save_states[index].stack[i] = stack[i];
	}

	save_states[index].SP = SP;
	save_states[index].I = I;
	save_states[index].PC = PC;

	for (int i = 0; i < 2048; i++)
	{
		save_states[index].graphics[i] = graphics[i];
	}

	save_states[index].delayTimer = delayTimer;
	save_states[index].soundTimer = soundTimer;
	save_states[index].drawFlag = drawFlag;
	save_states[index].awaitingKeyPress = awaitingKeyPress;
}

void CHIP8::loadState(int index)
{
	initialize();

	opcode = save_states[index].opcode;

	for (int i = 0; i < MEM_SIZE; i++)
	{
		memory[i] = save_states[index].memory[i];
	}

	for (int i = 0; i < 16; i++)
	{
		V[i] = save_states[index].V[i];
		stack[i] = save_states[index].stack[i];
	}

	SP = save_states[index].SP;
	I = save_states[index].I;
	PC = save_states[index].PC;

	for (int i = 0; i < 2048; i++)
	{
		graphics[i] = save_states[index].graphics[i];
	}

	delayTimer = save_states[index].delayTimer;
	soundTimer = save_states[index].soundTimer;
	drawFlag = save_states[index].drawFlag;
	awaitingKeyPress = save_states[index].awaitingKeyPress;
}

void CHIP8::emulateCycle()
{
	unsigned short x, y;						//Temp variables for addressing the V registers
	unsigned char tempNN;						//Temp variable for holding the value of "NN" when called for by an opcode
	unsigned char randomNum = rand() % 256;		//Random number generated for opcode CXNN
	unsigned short x_loc, y_loc, height, pixel;	//X location, Y location, sprite height, and pixel variables used for opcode DXYN

	opcode = memory[PC] << 8 | memory[PC + 1];		//Fetch opcode
	x = (opcode & 0x0F00) >> 8;
	y = (opcode & 0x00F0) >> 4;
	tempNN = opcode & 0x00FF;
	x_loc = V[x];
	y_loc = V[y];
	height = opcode & 0x000F;

	if (awaitingKeyPress)		//Check and see if we're awaiting a key press, if we got one then move on to the next instruction
	{
		for (int i = 0; i < 16; i++)
		{
			if (keypad[i] == 0x01)
			{
				V[x] = i;
				PC += 2;
				awaitingKeyPress = false;
				break;
			}
		}
	}

	else
	{
		switch (opcode & 0xF000)		//Decode and execute opcode
		{
			case 0x0000:
				switch (opcode & 0x000F)
				{
					case 0x0000:		//(00E0) Clears the screen
						for (int i = 0; i < 2048; i++)
						{
							graphics[i] = 0x00;
						}

						for (int i = 0; i < 64; i++)
						{
							for (int j = 0; j < 32; j++)
							{
								grid.setPixel(i, j, background_color);
							}
						}

						PC += 2;
						break;

					case 0x000E:		//(00EE) Returns from a subroutine
						SP--;
						PC = stack[SP];
						PC += 2;
						break;

					default:
						std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
						break;
				};
				break;

			case 0x1000:		//(1NNN) Jumps to address NNN
				PC = opcode & 0x0FFF;
				break;

			case 0x2000:	//(2NNN) Calls subroutine at NNN
				stack[SP] = PC;
				SP++;
				PC = opcode & 0x0FFF;
				break;

			case 0x3000:		//(3XNN) Skips the next instruction if VX equals NN
				if (V[x] == tempNN)
				{
					PC += 4;
				}

				else
				{
					PC += 2;
				}
				break;

			case 0x4000:		//(4XNN) Skips the next instruction if VX doesn't equal NN
				if (V[x] != tempNN)
				{
					PC += 4;
				}

				else
				{
					PC += 2;
				}
				break;

			case 0x5000:		//(5XY0) Skips the next instruction if VX equals VY
				if (V[x] == V[y])
				{
					PC += 4;
				}

				else
				{
					PC += 2;
				}
				break;

			case 0x6000:		//(6XNN) Sets VX to NN
				V[x] = tempNN;
				PC += 2;
				break;

			case 0x7000:		//(7XNN) Adds NN to VX
				V[x] += tempNN;
				PC += 2;
				break;

			case 0x8000:
				switch (opcode & 0x000F)
				{
					case 0x0000:		//(8XY0) Sets VX to the value of VY
						V[x] = V[y];
						PC += 2;
						break;

					case 0x0001:		//(8XY1) Sets VX to VX or VY
						V[x] = V[x] | V[y];
						PC += 2;
						break;

					case 0x0002:		//(8XY2) Sets VX to VX and VY
						V[x] = V[x] & V[y];
						PC += 2;
						break;

					case 0x0003:		//(8XY3) Sets VX to VX xor VY
						V[x] = V[x] ^ V[y];
						PC += 2;
						break;

					case 0x0004:		//(8XY4) Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
						if (V[y] > (0xFF - V[x]))
						{
							V[0xF] = 1;
						}

						else
						{
							V[0xF] = 0;
						}

						V[x] += V[y];
						PC += 2;
						break;

					case 0x0005:		//(8XY5) VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
						if (V[x] > V[y])
						{
							V[0xF] = 1;
						}

						else
						{
							V[0xF] = 0;
						}

						V[x] -= V[y];
						PC += 2;
						break;

					case 0x0006:		//(8XY6) Shifts VY right by one and stores the result to VX (VY remains unchanged). VF is set to the value of the least significant bit of VY before the shift
						V[0xF] = V[x] & 0x01;
						V[x] >>= 1;
						PC += 2;
						break;

					case 0x0007:		//(8XY7) Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
						if (V[y] > V[x])
						{
							V[0xF] = 1;
						}

						else
						{
							V[0xF] = 0;
						}

						V[x] = V[y] - V[x];
						PC += 2;
						break;

					case 0x000E:		//(8XYE) Shifts VY left by one and copies the result to VX. VF is set to the value of the most significant bit of VY before the shift
						V[0xF] = V[x] >> 7;
						V[x] <<= 1;
						PC += 2;
						break;

					default:
						std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
						break;
				};
				break;

			case 0x9000:		//(9XY0) Skips the next instruction if VX doesn't equal VY
				if (V[x] != V[y])
				{
					PC += 4;
				}

				else
				{
					PC += 2;
				}
				break;

			case 0xA000:		//(ANNN) Sets the I register to the address NNN
				I = opcode & 0x0FFF;
				PC += 2;
				break;

			case 0xB000:		//(BNNN) Jumps to the address NNN plus V0
				PC = (opcode & 0x0FFF) + V[0];
				break;

			case 0xC000:		//(CXNN) Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
				V[x] = tempNN & randomNum;
				PC += 2;
				break;

			case 0xD000:		//(DXYN) Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
				V[0xF] = 0;

				for (int yline = 0; yline < height; yline++)		//For each line of the sprite
				{
					pixel = memory[I + yline];		//Grab the line from memory

					for (int xline = 0; xline < 8; xline++)		//For each bit in the line
					{
						if ((pixel & (0x80 >> xline)) != 0)		//If that bit is flipped on
						{
							if (graphics[(x_loc + xline + ((y_loc + yline) * 64))] == 1 || graphics[(x_loc + xline + ((y_loc + yline) * 64))] == 0xCD)		//If the bit in that position of the screen is flipped on
							{
								V[0xF] = 1;		//Set the collision flag to true
							}

							graphics[x_loc + xline + ((y_loc + yline) * 64)] ^= 1;		//XOR the bit in that position to turn it on/off accordingly
						}
					}
				}

				drawFlag = true;		//Set the draw flag to true to indicate the screen needs to be updated
				PC += 2;
				break;

			case 0xE000:
				switch (opcode & 0x000F)
				{
					case 0x000E:		//(EX9E) Skips the next instruction if the key stored in VX is pressed
						if (keypad[V[x]] == 1)
						{
							PC += 4;
						}

						else
						{
							PC += 2;
						}
						break;

					case 0x0001:		//(EXA1) Skips the next instruction if the key stored in VX isn't pressed
						if (keypad[V[x]] != 1)
						{
							PC += 4;
						}

						else
						{
							PC += 2;
						}
						break;

					default:
						std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
						break;
				};
				break;

			case 0xF000:
				switch (opcode & 0x000F)
				{
					case 0x0007:		//(FX07) Sets VX to the value of the delay timer
						V[x] = delayTimer;
						PC += 2;
						break;

					case 0x000A:		//(FX0A) A key press is awaited, and then stored in VX
						awaitingKeyPress = true;
						break;

					case 0x0008:		//(FX08) Sets the sound timer to VX
						soundTimer = V[x];
						PC += 2;
						break;

					case 0x000E:		//(FX1E) Adds VX to I
						I += V[x];
						PC += 2;
						break;

					case 0x0009:		//(FX29) Sets I to the location of the sprite for the character in VX
						I = 0x50 + (V[x] * 0x5);
						PC += 2;
						break;

					case 0x0003:		//(FX33) Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
						memory[I] = V[x] / 100;
						memory[I + 1] = (V[x] / 10) % 10;
						memory[I + 2] = V[x] % 10;
						PC += 2;
						break;

					case 0x0005:
						switch (opcode & 0x00F0)
						{
							case 0x0010:		//(FX15) Sets the delay timer to VX
								delayTimer = V[x];
								PC += 2;
								break;

							case 0x0050:		//(FX55) Stores V0 to VX (including VX) in memory starting at address I
								for (int i = 0; i <= x; i++)
								{
									memory[I + i] = V[i];
								}
								PC += 2;
								break;

							case 0x0060:		//(FX65) Fills V0 to VX (including VX) with values from memory starting at address I
								for (int i = 0; i <= x; i++)
								{
									V[i] = memory[I + i];
								}
								PC += 2;
								break;

							default:
								std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
								break;
						};
						break;

					default:
						std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
						break;
				};
				break;

				default:
					std::cout << "ERROR: Unrecognized opcode: " << std::hex << opcode << std::endl;
					break;
		};

		//update timers
		if (delayTimer > 0)
		{
			delayTimer--;
		}

		if (soundTimer > 0)
		{
			if (soundTimer == 1)
			{
				beep.play();
			}

			soundTimer--;
		}
	}
}

void CHIP8::updateGraphics()
{
	int graphicsIndex = 0;

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (graphics[graphicsIndex] == 1 || graphics[graphicsIndex] == 0xCD)
			{
				grid.setPixel(j, i, foreground_color);
			}

			else
			{
				grid.setPixel(j, i, background_color);
			}

			graphicsIndex++;
		}
	}

	gridTexture.loadFromImage(grid);
}

void CHIP8::draw(sf::RenderWindow* window)
{
	window->draw(gridSprite);
}