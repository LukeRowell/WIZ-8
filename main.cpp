#include "CHIP8.h"

std::string load_new_rom(CHIP8* system, sf::RenderWindow* window)
{
	#ifdef _WIN32

		std::string newWindowTitle = "WIZ-8 - ";
		char rom_path[260];
		char rom_title[260];
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));				//Zero out the needed memory for the OPENFILENAME structure, starting at the address of the structure

		ofn.lStructSize = sizeof(ofn);				//Size of the OPENFILENAME structure
		ofn.hwndOwner = NULL;						//Handle to the window which owns the dialog box, NULL if none
		ofn.lpstrFile = rom_path;					//Variable to store the path and filename that is returned from GetOpenFileName
		ofn.lpstrFile[0] = '\0';					//First character of the buffer must be the null terminator
		ofn.nMaxFile = sizeof(rom_path);			//Size of the buffer which holds the ROM file path
		ofn.lpstrFilter = "CHIP-8 Roms\0*.*\0";		//Sets the filter name, indicating to the user which type of files we want
		ofn.nFilterIndex = 1;						//Index is set to 1, so that we use the above file filter	
		ofn.lpstrFileTitle = rom_title;				//Name and extension of the file without path info, can be NULL
		ofn.lpstrFileTitle[0] = '\0';
		ofn.nMaxFileTitle = sizeof(rom_title);						//Size of file title, 0 if field has been ignored
		ofn.lpstrInitialDir = NULL;					//Initial directory displayed in the dialog box (differs depending on Windows version)
		ofn.lpstrTitle = "Select ROM";				//Title text for the dialog box
		ofn.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;		//OFN_NOCHANGEDIR restores the directory value to the original if the user moved directories
																					//OFN_PATHMUSTEXIST ensures that the path supplied is a valid one
		if (GetOpenFileName(&ofn))													//OFN_FILEMUSTEXIST ensures that the file supplied exists
		{
			system->initialize();						//Clear the memory, registers, stack, etc.
			system->loadGame(rom_path);					//Load the new game
			newWindowTitle += rom_title;				//Add the ROM title to the new window title
			window->setTitle(newWindowTitle.c_str());	//Update window title
		}

	#endif

	return rom_path;
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	srand(time(NULL));
	bool load_rom_flag = false;
	bool quit_flag = false;
	bool reset_flag = false;
	bool pause_flag = false;
	bool default_clock_rate_flag = false;
	bool save_state_1_flag = false;
	bool save_state_2_flag = false;
	bool save_state_3_flag = false;
	bool save_state_4_flag = false;
	bool load_state_1_flag = false;
	bool load_state_2_flag = false;
	bool load_state_3_flag = false;
	bool load_state_4_flag = false;
	bool default_window_size_flag = false;
	bool large_window_size_flag = false;
	bool change_bg_color_flag = false;
	bool change_fg_color_flag = false;
	bool enable_vsync_flag = false;
	bool vsync_on = false;
	bool mute_sound_flag = false;
	bool mute_on = false;
	bool show_controls_window_flag = false;
	bool show_license_window_flag = false;
	bool show_about_window_flag = false;
	sf::Color bgColor, fgColor;
	float bg_color[3] = { 0.0f, 0.0f, 0.0f };
	float fg_color[3] = { 255.f, 255.f, 255.f };
	int clock_rate = 500;
	int window_scale = 1;
	int window_x = 640;
	int window_y = 340;
	std::string windowTitle = "WIZ-8 - ";
	std::string DEFAULT_ROM = "chip8pic.ch8";
	std::string CURRENT_ROM;
	sf::RenderWindow window(sf::VideoMode(window_x * window_scale, window_y * window_scale), "");
	CHIP8 chip8;

	window.setFramerateLimit(clock_rate);
	ImGui::SFML::Init(window);
	window.resetGLStates();

	chip8.initialize();
	chip8.loadGame(DEFAULT_ROM);
	
	CURRENT_ROM = DEFAULT_ROM;
	windowTitle += CURRENT_ROM;
	window.setTitle(windowTitle.c_str());

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		window.setFramerateLimit(clock_rate);
		sf::Event event;

		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Num1:
						chip8.setKeyPadVal(0, 0x01);
						break;

					case sf::Keyboard::Num2:
						chip8.setKeyPadVal(1, 0x01);
						break;

					case sf::Keyboard::Num3:
						chip8.setKeyPadVal(2, 0x01);
						break;

					case sf::Keyboard::Num4:
						chip8.setKeyPadVal(3, 0x01);
						break;

					case sf::Keyboard::Q:
						chip8.setKeyPadVal(4, 0x01);
						break;

					case sf::Keyboard::W:
						chip8.setKeyPadVal(5, 0x01);
						break;

					case sf::Keyboard::E:
						chip8.setKeyPadVal(6, 0x01);
						break;

					case sf::Keyboard::R:
						chip8.setKeyPadVal(7, 0x01);
						break;

					case sf::Keyboard::A:
						chip8.setKeyPadVal(8, 0x01);
						break;

					case sf::Keyboard::S:
						chip8.setKeyPadVal(9, 0x01);
						break;

					case sf::Keyboard::D:
						chip8.setKeyPadVal(10, 0x01);
						break;

					case sf::Keyboard::F:
						chip8.setKeyPadVal(11, 0x01);
						break;
						
					case sf::Keyboard::Z:
						chip8.setKeyPadVal(12, 0x01);
						break;

					case sf::Keyboard::X:
						chip8.setKeyPadVal(13, 0x01);
						break;

					case sf::Keyboard::C:
						chip8.setKeyPadVal(14, 0x01);
						break;

					case sf::Keyboard::V:
						chip8.setKeyPadVal(15, 0x01);
						break;

					case sf::Keyboard::P:
						pause_flag ? pause_flag = false : pause_flag = true;
						break;

					case sf::Keyboard::J:
						reset_flag ? reset_flag = false : reset_flag = true;
						break;

					case sf::Keyboard::F1:
						save_state_1_flag = true;
						break;

					case sf::Keyboard::F2:
						save_state_2_flag = true;
						break;

					case sf::Keyboard::F3:
						save_state_3_flag = true;
						break;

					case sf::Keyboard::F4:
						save_state_4_flag = true;
						break;

					case sf::Keyboard::F5:
						load_state_1_flag = true;
						break;
					
					case sf::Keyboard::F6:
						load_state_2_flag = true;
						break;
					
					case sf::Keyboard::F7:
						load_state_3_flag = true;
						break;
					
					case sf::Keyboard::F8:
						load_state_4_flag = true;
						break;

					case sf::Keyboard::M:
						mute_sound_flag ? mute_sound_flag = false : mute_sound_flag = true;
						break;

					default:
						break;
				};
			}

			if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Num1:
						chip8.setKeyPadVal(0, 0x00);
						break;

					case sf::Keyboard::Num2:
						chip8.setKeyPadVal(1, 0x00);
						break;

					case sf::Keyboard::Num3:
						chip8.setKeyPadVal(2, 0x00);
						break;

					case sf::Keyboard::Num4:
						chip8.setKeyPadVal(3, 0x00);
						break;

					case sf::Keyboard::Q:
						chip8.setKeyPadVal(4, 0x00);
						break;

					case sf::Keyboard::W:
						chip8.setKeyPadVal(5, 0x00);
						break;

					case sf::Keyboard::E:
						chip8.setKeyPadVal(6, 0x00);
						break;

					case sf::Keyboard::R:
						chip8.setKeyPadVal(7, 0x00);
						break;

					case sf::Keyboard::A:
						chip8.setKeyPadVal(8, 0x00);
						break;

					case sf::Keyboard::S:
						chip8.setKeyPadVal(9, 0x00);
						break;

					case sf::Keyboard::D:
						chip8.setKeyPadVal(10, 0x00);
						break;

					case sf::Keyboard::F:
						chip8.setKeyPadVal(11, 0x00);
						break;

					case sf::Keyboard::Z:
						chip8.setKeyPadVal(12, 0x00);
						break;

					case sf::Keyboard::X:
						chip8.setKeyPadVal(13, 0x00);
						break;

					case sf::Keyboard::C:
						chip8.setKeyPadVal(14, 0x00);
						break;

					case sf::Keyboard::V:
						chip8.setKeyPadVal(15, 0x00);
						break;

					default:
						break;
				};
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Load ROM", NULL, &load_rom_flag);
				ImGui::MenuItem("Exit", NULL, &quit_flag);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Emulation"))
			{
				ImGui::MenuItem("Reset (J)", NULL, &reset_flag);
				ImGui::MenuItem("Pause (P)", NULL, &pause_flag);

				if (ImGui::BeginMenu("Clock Rate"))
				{
					ImGui::SliderInt("Hz", &clock_rate, 50, 5000, "%.0f");
					ImGui::MenuItem("Default (500Hz)", NULL, &default_clock_rate_flag);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Save State"))
				{
					ImGui::MenuItem("Save State 1 (F1)", NULL, &save_state_1_flag);
					ImGui::MenuItem("Save State 2 (F2)", NULL, &save_state_2_flag);
					ImGui::MenuItem("Save State 3 (F3)", NULL, &save_state_3_flag);
					ImGui::MenuItem("Save State 4 (F4)", NULL, &save_state_4_flag);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Load State"))
				{
					ImGui::MenuItem("Load State 1 (F5)", NULL, &load_state_1_flag);
					ImGui::MenuItem("Load State 2 (F6)", NULL, &load_state_2_flag);
					ImGui::MenuItem("Load State 3 (F7)", NULL, &load_state_3_flag);
					ImGui::MenuItem("Load State 4 (F8)", NULL, &load_state_4_flag);
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::BeginMenu("Window Size"))
				{
					ImGui::MenuItem("Default", NULL, &default_window_size_flag);
					ImGui::MenuItem("Large", NULL, &large_window_size_flag);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Color Palette"))
				{
					if (ImGui::ColorEdit3("Background", bg_color)) 
					{
						bgColor.r = static_cast<sf::Uint8>(bg_color[0] * 255.f);
						bgColor.g = static_cast<sf::Uint8>(bg_color[1] * 255.f);
						bgColor.b = static_cast<sf::Uint8>(bg_color[2] * 255.f);
						change_bg_color_flag = true;
					}

					if (ImGui::ColorEdit3("Foreground", fg_color))
					{
						fgColor.r = static_cast<sf::Uint8>(fg_color[0] * 255.f);
						fgColor.g = static_cast<sf::Uint8>(fg_color[1] * 255.f);
						fgColor.b = static_cast<sf::Uint8>(fg_color[2] * 255.f);
						change_fg_color_flag = true;
					}

					ImGui::EndMenu();
				}

				ImGui::MenuItem("Enable V-sync", NULL, &enable_vsync_flag);
				ImGui::MenuItem("Mute Sound (M)", NULL, &mute_sound_flag);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Controls...", NULL, &show_controls_window_flag);
				ImGui::MenuItem("License...", NULL, &show_license_window_flag);
				ImGui::MenuItem("About...", NULL, &show_about_window_flag);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (load_rom_flag)
		{
			load_rom_flag = false;
			pause_flag ? pause_flag = false : pause_flag = false;
			CURRENT_ROM = load_new_rom(&chip8, &window);
		}

		if (quit_flag)
		{
			window.close();
		}

		if (reset_flag)
		{
			reset_flag = false;
			pause_flag = false;
			chip8.initialize();
			chip8.loadGame(CURRENT_ROM);
		}

		if (default_clock_rate_flag)
		{
			default_clock_rate_flag = false;
			clock_rate = 500;
		}

		if (save_state_1_flag)
		{
			save_state_1_flag = false;
			chip8.saveState(0, CURRENT_ROM);
		}

		if (save_state_2_flag)
		{
			save_state_2_flag = false;
			chip8.saveState(1, CURRENT_ROM);
		}

		if (save_state_3_flag)
		{
			save_state_3_flag = false;
			chip8.saveState(2, CURRENT_ROM);
		}

		if (save_state_4_flag)
		{
			save_state_4_flag = false;
			chip8.saveState(3, CURRENT_ROM);
		}

		if (load_state_1_flag && CURRENT_ROM == chip8.getSaveStateRomTitle(0))
		{
			load_state_1_flag = false;
			pause_flag ? pause_flag = false : pause_flag = false;
			chip8.loadState(0);
		}
		
		if (load_state_2_flag && CURRENT_ROM == chip8.getSaveStateRomTitle(1))
		{
			load_state_2_flag = false;
			pause_flag ? pause_flag = false : pause_flag = false;
			chip8.loadState(1);
		}
		
		if (load_state_3_flag && CURRENT_ROM == chip8.getSaveStateRomTitle(2))
		{
			load_state_3_flag = false;
			pause_flag ? pause_flag = false : pause_flag = false;
			chip8.loadState(2);
		}
		
		if (load_state_4_flag && CURRENT_ROM == chip8.getSaveStateRomTitle(3))
		{
			load_state_4_flag = false;
			pause_flag ? pause_flag = false : pause_flag = false;
			chip8.loadState(3);
		}

		if (default_window_size_flag)
		{
			default_window_size_flag = false;
			window.setSize(sf::Vector2u(window_x, window_y));
			chip8.scaleScreen(sf::Vector2f(10, 10));
			chip8.adjustDisplay(sf::Vector2f(0, 20));
		}

		if (large_window_size_flag)
		{
			large_window_size_flag = false;
			window.setSize(sf::Vector2u(window_x * 2, window_y * 2));
			chip8.scaleScreen(sf::Vector2f(10, 10.35));
			chip8.adjustDisplay(sf::Vector2f(0, 10));
		}

		if (change_bg_color_flag)
		{
			change_bg_color_flag = false;
			chip8.updateGraphics();
			chip8.setBackgroundColor(bgColor);
		}

		if (change_fg_color_flag)
		{
			change_fg_color_flag = false;
			chip8.updateGraphics();
			chip8.setForegroundColor(fgColor);
		}

		if (enable_vsync_flag)
		{
			enable_vsync_flag = false;

			if (vsync_on)
			{
				window.setVerticalSyncEnabled(false);
				vsync_on = false;
			}

			else
			{
				window.setVerticalSyncEnabled(true);
				vsync_on = true;
			}
		}

		if (mute_sound_flag)
		{
			mute_sound_flag = false;

			if (mute_on)
			{
				chip8.toggleSound(true);
				mute_on = false;
			}

			else
			{
				chip8.toggleSound(false);
				mute_on = true;
			}
		}

		if (show_controls_window_flag)
		{
			ImGui::SetNextWindowSize(ImVec2(450, 230), ImGuiSetCond_Appearing);
			ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
			ImGui::Begin("Controls", &show_controls_window_flag);
			ImGui::TextWrapped("CHIP-8 uses a 16-key hexadecimal keypad with the following layout:\n\n"
				"1 2 3 C\n4 5 6 D\n7 8 9 E\nA 0 B F\n\n"
				"This layout is mapped to your keyboard this way:\n\n"
				"1 2 3 4\nQ W E R\nA S D F\nZ X C V\n\n");
			ImGui::TextWrapped("Reset: J\n");
			ImGui::TextWrapped("Pause: P\n");
			ImGui::TextWrapped("Mute: M\n");
			ImGui::TextWrapped("Save State (1-4): F1 - F4\n");
			ImGui::TextWrapped("Load State (1-4): F5 - F8");
			ImGui::End();
		}

		if (show_license_window_flag)
		{
			ImGui::SetNextWindowSize(ImVec2(450, 230), ImGuiSetCond_Appearing);
			ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
			ImGui::Begin("License", &show_license_window_flag);
			ImGui::TextWrapped("The MIT License (MIT)\n"
				"Copyright (c) 2017 Luke Rowell\n\n"
				"Permission is hereby granted, free of charge, to any person "
				"obtaining a copy of this software and associated documentation "
				"files (the \"Software\"), to deal in the Software without "
				"restriction, including without limitation the rights to use, "
				"copy, modify, merge, publish, distribute, sublicense, and/or "
				"sell copies of the Software, and to permit persons to whom "
				"the Software is furnished to do so, subject to the following "
				"conditions:\n\n"
				"The above copyright notice and this permission notice shall be "
				"included in all copies or substantial portions of the Software.\n\n"
				"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY "
				"KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE "
				"WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE "
				"AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT "
				"HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, "
				"WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING "
				"FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR "
				"OTHER DEALINGS IN THE SOFTWARE.");
			ImGui::End();
		}

		if (show_about_window_flag)
		{
			ImGui::SetNextWindowSize(ImVec2(500, 230), ImGuiSetCond_Appearing);
			ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
			ImGui::Begin("About", &show_about_window_flag);
			ImGui::TextWrapped("WIZ-8 v1.0\n"
				"by Luke Rowell\n\n"
				"CHIP-8 emulator for Windows.\n\n"
				"More details: https://github.com/LukeRowell/WIZ-8\n\n"
				"Libraries used:\n\n"
				"SFML - https://www.sfml-dev.org/\n"
				"imgui - https://github.com/ocornut/imgui\n\n"
				"Resources:\n\n"
				"Big thanks to AlfonsoJLuna and his CHIP-8 emulator chip8swemu.\n"
				"Much of the GUI in this emulator was borrowed from and inspired\n"
				"by his work using ImGui. A more detailed list of resources can be\n"
				"found on the github for this project linked above.");
			ImGui::End();
		}

		if (!pause_flag)
		{
			chip8.emulateCycle();
		}

		if (chip8.getDrawFlag() && !pause_flag)
		{
			chip8.updateGraphics();
			chip8.setDrawFlag(false);
		}

		window.clear();

		chip8.draw(&window);
		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}