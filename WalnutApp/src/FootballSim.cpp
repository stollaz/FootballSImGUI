#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include <stdio.h>
#include <execution>
#include <queue>
#include <wtypes.h>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/xchar.h"
#include "fmt/color.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <locale>
#include <time.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <windows.h>
#include <chrono>

using namespace Walnut;

class TextItem {
public:
	std::string content;
	ImVec4 colour;
	bool newline;

	TextItem(std::string _content, ImVec4 _colour, bool _newline) : content(_content), colour(_colour), newline(_newline) {}
	TextItem(std::string _content, bool _newline) : content(_content), colour(ImVec4(255,255,255,255)), newline(_newline) {}
};

class TextLine {
public:
	std::vector<TextItem> parts;

	void AddItem(TextItem t) {
		parts.push_back(t);
	}

	TextLine(){};
};

class TextTest : public Walnut::Layer {
public:
	void InitialiseBuffers() {
		writeBuffer.clear();
		screenBuffer.clear();

		for (int i = 0; i < maxIndex; i++) {
			writeBuffer.push_back(fmt::format("{}", i));
		}

		for (int i = 0; i < bufferSize; i++) {
			screenBuffer.push_back(writeBuffer.at(i));
		}

		maxitemindex = bufferSize - 1;
	}

	void InitialiseEventBuffers() {
		eventBuffer.clear();
		eventScreenBuffer.clear();

		TextLine t = TextLine();
		t.AddItem(TextItem("RED CARD! ", GetColour(255, 0, 0), false));
		t.AddItem(TextItem("Player has been sent off.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t);

		TextLine t1 = TextLine();
		t1.AddItem(TextItem("YELLOW CARD! ", GetColour(255, 255, 0), false));
		t1.AddItem(TextItem("Player has been booked.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t1);

		TextLine t2 = TextLine();
		t2.AddItem(TextItem("GOAL! Player scored a goal.", GetColour(255, 255, 255), true));
		eventBuffer.push_back(t2);

		TextLine t3 = TextLine();
		t3.AddItem(TextItem("RED! ", GetColour(255, 0, 0), false));
		t3.AddItem(TextItem("GREEN! ", GetColour(0, 255, 0), false));
		t3.AddItem(TextItem("BLUE! ", GetColour(0, 0, 255), false));
		t3.AddItem(TextItem("DEFAULT. ", true));
		eventBuffer.push_back(t3);

		for (int i = 0; i < bufferSize; i++) {
			eventScreenBuffer.push_back(eventBuffer.at(i));
		}
	}

	virtual void OnAttach() override {
		InitialiseEventBuffers();
	}

	ImVec4 GetColour(int r, int g, int b) {
		return ImVec4(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255), 255);
	}

	void RenderText(TextItem t) {
		ImGui::TextColored(t.colour, t.content.c_str());
		if (!t.newline) ImGui::SameLine(0, 0);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		//if (maxitemindex != maxIndex-1) {
		//	if (ImGui::Button("Next")) {
		//		maxitemindex++;
		//		screenBuffer.erase(screenBuffer.begin());
		//		//screenBuffer.pop_back();
		//		screenBuffer.push_back(writeBuffer.at(maxitemindex));
		//	}
		//}
		//else if (ImGui::Button("Reset")) {
		//	InitialiseEventBuffers();
		//}

		for (int i = 0; i < bufferSize; i++) {
			//ImGui::Text(screenBuffer.at(i).c_str());
			TextLine t = eventScreenBuffer.at(i);
			for (TextItem ti : t.parts) RenderText(ti);
		}

		ImGui::End();
	}
private:
	const int maxIndex = 4;
	const int bufferSize = 4;
	int maxitemindex = 0;
	std::vector<std::string> writeBuffer;
	std::vector<std::string> screenBuffer;

	std::vector<TextLine> eventBuffer;
	std::vector<TextLine> eventScreenBuffer;
};

enum WindowState
{
	MainMenu,
	Debug1,
	Debug2,
	Debug3,
	Options
};

class Player {
public:
	ImVec2 position;
	int number;
	char* name;

	Player(ImVec2 _p, int _num, char* _name) : position(_p), number(_num), name(_name) {};
};

class FootballUITest : public Walnut::Layer
{
public:
	ImVec2 GetDesktopResolution()
	{
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(0, 0);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		int w = desktop.right;
		int h = desktop.bottom;
		return ImVec2((float)w, (float)h);
	}

	virtual void OnAttach() override {
		// Demonstrate the various window flags. Typically you would just use the default!
		/*static bool no_titlebar = true;
		static bool no_scrollbar = false;
		static bool no_menu = true;
		static bool no_move = true;
		static bool no_resize = false;
		static bool no_collapse = true;
		static bool no_close = false;
		static bool no_nav = true;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool no_docking = true;
		static bool unsaved_document = false;*/

		static bool no_titlebar = false;
		static bool no_scrollbar = false;
		static bool no_menu = false;
		static bool no_move = false;
		static bool no_resize = false;
		static bool no_collapse = true;
		static bool no_close = true;
		static bool no_nav = false;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool no_docking = false;
		static bool unsaved_document = false;

		if (no_titlebar)        _windowFlags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       _windowFlags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           _windowFlags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            _windowFlags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          _windowFlags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        _windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             _windowFlags |= ImGuiWindowFlags_NoNav;
		if (no_background)      _windowFlags |= ImGuiWindowFlags_NoBackground;
		if (no_bring_to_front)  _windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (no_docking)         _windowFlags |= ImGuiWindowFlags_NoDocking;
		if (unsaved_document)   _windowFlags |= ImGuiWindowFlags_UnsavedDocument;
		if (no_close)           _pOpen = NULL; // Don't pass our bool* to Begin

		/*const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);*/

		//ImGui::SetWindowSize(ImVec2(1920, 1080));
	}

	// TODO: Fix options so window can be resized
	virtual void OnUIRender() override
	{
		char* name = "_badFootballSimulator (C++ Edition)";
		//ImGui::Begin("Hello");

		ImVec2 ws = ImGui::GetWindowSize();

		if (_windowState == WindowState::MainMenu) {
			ImGui::Begin("Main Window", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x/2-textSize.x/2, ThisRegionSize.y/3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("MAIN MENU");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)ws.x, (int)ws.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;
			/*ImGui::SliderFloat("Spacing 1", &spacing1, -10, 10);
			ImGui::SliderFloat("Spacing 2", &spacing2, -10, 10);*/

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x/4 -50,2.0f*ThisRegionSize.y/3));
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(2*ThisRegionSize.x / 4 -50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100,30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::SetCursorPos(ImVec2(3*ThisRegionSize.x / 4 -50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.5f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Options", ImVec2(100, 30))) {
				_windowState = WindowState::Options;
			}
			/*ImGui::SameLine(spacing1, spacing2);
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}*/
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug1) {
			ImGui::Begin("Simulate Single Game", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			static float GKLineHeight = 7; // ?? to ??
			static float DefensiveLineHeight = 20; // ?? to ??
			static float MidfieldLineHeight = 37; // ?? to ??
			static float AttackingLineHeight = 50; // ?? to ??

			static float WingerWidth = 35; // 0 to 45
			static float FBWidth = 35; // 0 to 45
			static float CMWidth = 10; // 0 to 45
			static float CBWidth = 10; // 0 to 45

			static float FullBackHeightOffset = 0;
			static float DMHeightOffset = -5;
			static float WingerHeightOffset = -3;

			static float MarkerSize = 10;

			static bool showNumbers = true;

			ImGui::Begin("Settings");

			ImGui::SliderFloat("Goalkeeper Height", &GKLineHeight, 0, 20);
			ImGui::SliderFloat("Defensive Line Height", &DefensiveLineHeight, 10, 30);
			ImGui::SliderFloat("Midfield Line Height", &MidfieldLineHeight, 20, 40);
			ImGui::SliderFloat("Attacking Line Height", &AttackingLineHeight, 40, 60);
			ImGui::NewLine();

			ImGui::SliderFloat("Full Back Width", &FBWidth, 0, 45);
			ImGui::SliderFloat("Winger Width", &MidfieldLineHeight, 0, 45);
			ImGui::SliderFloat("Centre Back Width", &CBWidth, 0, 45);
			ImGui::SliderFloat("Centre Mid Width", &CMWidth, 0, 45);
			ImGui::NewLine();

			ImGui::SliderFloat("Full Back Height Offset", &FullBackHeightOffset, -10, 10);
			ImGui::SliderFloat("Defensive Mid Height Offset", &DMHeightOffset, -10, 10);
			ImGui::SliderFloat("Winger Height Offset", &WingerHeightOffset, -10, 10);
			ImGui::NewLine();

			ImGui::SliderFloat("Marker Size", &MarkerSize, 5, 15);

			ImGui::Checkbox("Show Numbers", &showNumbers);

			if (ImGui::Button("Reset")) {
				GKLineHeight = 7; // ?? to ??
				DefensiveLineHeight = 20; // ?? to ??
				MidfieldLineHeight = 37; // ?? to ??
				AttackingLineHeight = 50; // ?? to ??

				WingerWidth = 35; // 0 to 45
				FBWidth = 35; // 0 to 45
				CMWidth = 10; // 0 to 45
				CBWidth = 10; // 0 to 45

				FullBackHeightOffset = 0;
				DMHeightOffset = -5;
				WingerHeightOffset = -3;
			}

			ImGui::End();

			ImGui::Begin("Pitch");

			ImVec2 WindowPos = ImGui::GetWindowPos();

			ImVec2 WindowSize = ImGui::GetWindowSize();

			int ShortestDim = std::min(WindowSize.x, WindowSize.y);

			// Dimensions:
			// Outline: 120x90 (metres, WxH)
			// Centre Circle: Radius 9.15m
			// Penalty Area: 16.5x40.3m
			// 6yd Box: 5.5x18.32m
			// Goal Width: 7.32m
			// Penalty Spot: 11m from byline
			// Corner segment: Radius 1m

			ImVec2 Pitch = ImVec2(120, 90);
			ImVec2 PitchToWindow = ImVec2(WindowSize.x / Pitch.x, WindowSize.y / Pitch.y);

			//int markerSize = ShortestDim/50;
			float markerSize = MarkerSize;
			auto draw = ImGui::GetForegroundDrawList();
			ImU32 team1Col = IM_COL32(255, 0, 0, 255);
			ImU32 team2Col = IM_COL32(3, 202, 252, 255);
			ImU32 lineCol = IM_COL32(255, 255, 255, 255);
			ImU32 grassCol = IM_COL32(23, 141, 23, 255);
			ImVec2 Offset = ImVec2(WindowPos.x + 5, WindowPos.y + 30);

			int xOffset = (int)Offset.x;
			int yOffset = (int)Offset.y;

			/*ImVec2 TopLeft = ImVec2(Offset.x, Offset.y);
			ImVec2 TopRight = ImVec2(Offset.x + WindowSize.x - 10, Offset.y);
			ImVec2 BottomLeft = ImVec2(Offset.x, Offset.y + WindowSize.y - 35);
			ImVec2 BottomRight = ImVec2(Offset.x + WindowSize.x - 10, Offset.y + WindowSize.y - 35);*/

			int TopEdge = Offset.y;
			int BottomEdge = Offset.y + WindowSize.y - 35;
			int LeftEdge = Offset.x;
			int RightEdge = Offset.x + WindowSize.x - 10;

			ImVec2 TopLeft = ImVec2(LeftEdge, TopEdge);
			ImVec2 TopRight = ImVec2(RightEdge, TopEdge);
			ImVec2 BottomLeft = ImVec2(LeftEdge, BottomEdge);
			ImVec2 BottomRight = ImVec2(RightEdge, BottomEdge);

			ImVec2 PitchSize = ImVec2(RightEdge - LeftEdge, BottomEdge - TopEdge);

			int localHalfway = (LeftEdge + RightEdge) / 2;

			ImVec2 Centre = ImVec2((LeftEdge + RightEdge) / 2, (TopEdge + BottomEdge) / 2);

			/*ImVec2 TopLeft = ImVec2(xOffset - 15, yOffset - 15);
			ImVec2 TopRight = ImVec2(xOffset + 2 * localHalfway + 15, yOffset - 15);
			ImVec2 BottomLeft = ImVec2(xOffset - 15, yOffset + 105);
			ImVec2 BottomRight = ImVec2(xOffset + 2 * localHalfway + 15, yOffset + 105);*/

			draw->AddRectFilled(TopLeft, BottomRight, grassCol); // Grass
			draw->AddRect(TopLeft, BottomRight, lineCol); // Outline
			draw->AddLine(ImVec2(localHalfway, TopEdge), ImVec2(localHalfway, BottomEdge), lineCol); // Halfway Line
			draw->AddCircle(Centre, (9.15/120)*PitchSize.x, lineCol); // Centre Circle
			draw->AddCircleFilled(Centre, 3, lineCol); // Centre Dot

			draw->AddRect(ImVec2(LeftEdge, Centre.y - (20.15/90) * PitchSize.y), ImVec2(LeftEdge + (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Left Penalty Area
			draw->AddRect(ImVec2(RightEdge-1, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(RightEdge - (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Right Penalty Area

			draw->AddRect(ImVec2(LeftEdge, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(LeftEdge + (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Left 6yd Area
			draw->AddRect(ImVec2(RightEdge-1, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(RightEdge - (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Right 6yd Area

			draw->AddCircleFilled(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Left Penalty Spot
			draw->AddCircleFilled(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Right Penalty Spot

			// Left D
			draw->PathArcTo(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120)* PitchSize.x, -0.94, 0.94);
			draw->PathStroke(lineCol);

			// Right D
			draw->PathArcTo(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120)* PitchSize.x, 3.14159 - 0.94, 3.14159 + 0.94);
			draw->PathStroke(lineCol);

			// Top Left Corner
			draw->PathArcTo(TopLeft, (1.0 / 120)* PitchSize.x, 0, 3.14159/2);
			draw->PathStroke(lineCol);

			// Top Right Corner
			draw->PathArcTo(TopRight, (1.0 / 120)* PitchSize.x, 3.14159/2, 3.14159);
			draw->PathStroke(lineCol);

			// Bottom Left Corner
			draw->PathArcTo(BottomLeft, (1.0 / 120)* PitchSize.x, 0, -3.14159 / 2);
			draw->PathStroke(lineCol);

			// Bottom Right Corner
			draw->PathArcTo(BottomRight, (1.0 / 120)* PitchSize.x, 3.14159, 3*3.14159 / 2);
			draw->PathStroke(lineCol);

			// Players

			// TODO: LIsts of player locations
			// Have a player object, as well as a location (relative to the 120x90 pitch, which will be translated to screen space)
			// Could also contain some information on momentum / inertia for movement
			// For testing now though just use ImVec2 for pure positions and testing

			// Add players to Team 1
			// Players have a position, number and name
			std::vector<Player> team1Players;

			team1Players.push_back(Player(ImVec2(GKLineHeight, Pitch.y/2), 1, "David De Gea")); // GK

			team1Players.push_back(Player(ImVec2(DefensiveLineHeight+FullBackHeightOffset, (Pitch.y / 2) - FBWidth), 23, "Luke Shaw")); // LB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 5, "Harry Maguire")); // LCB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 19, "Raphael Varane")); // RCB
			team1Players.push_back(Player(ImVec2(DefensiveLineHeight + FullBackHeightOffset, (Pitch.y / 2) + FBWidth), 20, "Diogo Dalot")); // RB

			team1Players.push_back(Player(ImVec2(MidfieldLineHeight, (Pitch.y / 2)-CMWidth), 8, "Bruno Fernandes")); // LCM
			team1Players.push_back(Player(ImVec2(MidfieldLineHeight+DMHeightOffset, (Pitch.y / 2)), 6, "Frenkie de Jong")); // CDM
			team1Players.push_back(Player(ImVec2(MidfieldLineHeight, (Pitch.y / 2)+CMWidth), 17, "Fred")); // RCM

			team1Players.push_back(Player(ImVec2(AttackingLineHeight+WingerHeightOffset, (Pitch.y / 2)-WingerWidth), 25, "Jadon Sancho")); // LW
			team1Players.push_back(Player(ImVec2(AttackingLineHeight, (Pitch.y / 2)), 7, "Cristiano Ronaldo")); // ST
			team1Players.push_back(Player(ImVec2(AttackingLineHeight + WingerHeightOffset, (Pitch.y / 2)+WingerWidth), 11, "Antony")); // RW

			//for (Player p : team1Players) draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x/Pitch.x)*PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team1Col);

			ImFont *font = ImGui::GetFont(); // Get Font

			// Iterate over players in Team 1
			for (Player &p : team1Players) {
				ImVec2 pos = p.position; // Get position part
				draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team1Col); // Draw marker for player

				// Draw player number on marker, centering on marker
				float fontSize = 2 * MarkerSize;
				auto TextSize = ImGui::CalcTextSize(std::to_string(p.number).c_str());
				TextSize.x *= (fontSize / 20);
				TextSize.y *= (fontSize / 20);
				if (showNumbers) draw->AddText(font, fontSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x * 0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y * 0.5f), lineCol, std::to_string(p.number).c_str());
			}

			std::vector<Player> team2Players;

			team2Players.push_back(Player(ImVec2(Pitch.x - GKLineHeight, Pitch.y / 2), 31, "Ederson")); // GK

			team2Players.push_back(Player(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) - FBWidth), 27, "Joao Cancelo")); // LB
			team2Players.push_back(Player(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 14, "Aymeric Laporte")); // LCB
			team2Players.push_back(Player(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 5, "Ruben Dias")); // RCB
			team2Players.push_back(Player(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) + FBWidth), 2, "Kyle Walker")); // RB

			team2Players.push_back(Player(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) - CMWidth), 17, "Kevin De Bruyne")); // LCM
			team2Players.push_back(Player(ImVec2(Pitch.x - (MidfieldLineHeight + DMHeightOffset), (Pitch.y / 2)), 16, "Rodri")); // CDM
			team2Players.push_back(Player(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) + CMWidth), 20, "Bernardo Silva")); // RCM

			team2Players.push_back(Player(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) - WingerWidth), 47, "Phil Foden")); // LW
			team2Players.push_back(Player(ImVec2(Pitch.x - AttackingLineHeight, (Pitch.y / 2)), 9, "Erling Haaland")); // ST
			team2Players.push_back(Player(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) + WingerWidth), 26, "Riyad Mahrez")); // RW

			// Iterate over players in Team 2
			for (Player& p : team2Players) {
				ImVec2 pos = p.position; // Get position part
				draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, team2Col); // Draw marker for player

				// Draw player number on marker, centering on marker
				float fontSize = 2 * MarkerSize;
				auto TextSize = ImGui::CalcTextSize(std::to_string(p.number).c_str());
				TextSize.x *= (fontSize / 20);
				TextSize.y *= (fontSize / 20);
				if (showNumbers) draw->AddText(font, 2*MarkerSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x*0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y*0.5f), lineCol, std::to_string(p.number).c_str());
			}

			// Do name hover text at the very end so that markers are never rendered on top of the text
			// If the user hovers over the player on the screen, display their name
			for (Player& p : team1Players) {
				if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x - markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y - markerSize),
					ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x + markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y + markerSize))) { // Check if mouse is in bounding box around player marker
					draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineCol, p.name); // Draw text above the cursor
				}
			}
			for (Player& p : team2Players) {
				if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x - markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y - markerSize),
					ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x + markerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y + markerSize))) { // Check if mouse is in bounding box around player marker
					draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineCol, p.name); // Draw text above the cursor
				}
			}

			ImGui::End();

			// Dimensions:
			// Outline: 120x90 (metres, WxH)
			// Centre Circle: Radius 9.15m
			// Penalty Area: 16.5x40.3m
			// 6yd Box: 5.5x18.32m
			// Goal Width: 7.32m
			// Penalty Spot: 11m from byline
			// Corner segment: Radius 1m

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Simulate Single Game");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug2) {
			ImGui::Begin("Simulate Full Season", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Simulate Full Season");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
				_windowState = WindowState::Debug3;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Debug3){
			ImGui::Begin("Debug", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Debug");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}
			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
				_windowState = WindowState::Debug2;
			}
			ImGui::End();
		}
		else if (_windowState == WindowState::Options) {
			ImGui::Begin("Options", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);

			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Options");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Text("Window size is %dx%d", (int)_windowSize.x, (int)_windowSize.y);
			ImGui::Text("This content region size is %dx%d", (int)ThisRegionSize.x, (int)ThisRegionSize.y);

			static float spacing1 = 0.0f;
			static float spacing2 = 15.0f;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 50, 2.5f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Back", ImVec2(100, 30))) {
				_windowState = WindowState::MainMenu;
			}

			static int x = (int)_windowSize.x;
			static int y = (int)_windowSize.y;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 500, 2.0f * ThisRegionSize.y / 3));

			//ImVec2 ScreenResolution = GetDesktopResolution();
			ImGui::SliderInt("Window Width", &x, 160, 2560);
			if (ImGui::IsItemEdited()) _windowSize.x = (float)x;

			ImGui::SetCursorPos(ImVec2(2 * ThisRegionSize.x / 4 - 500, 2.2f * ThisRegionSize.y / 3));
			ImGui::SliderInt("Window Height", &y, 90, 1440);
			if (ImGui::IsItemEdited()) _windowSize.y = (float)y;

			ImGui::End();
		}

		//else if (_windowState == WindowState::Debug3) {
		//	ImGui::Begin("Debug 2", p_open, window_flags);

		//	ImGui::SetWindowFontScale(2.0f);
		//	ImGui::Text("DEBUG 3");
		//	ImGui::SetWindowFontScale(1.0f);

		//	static float spacing1 = 0.0f;
		//	static float spacing2 = 15.0f;
		//	/*ImGui::SliderFloat("Spacing 1", &spacing1, -10, 10);
		//	ImGui::SliderFloat("Spacing 2", &spacing2, -10, 10);*/

		//	if (ImGui::Button("Main Menu", ImVec2(100, 30))) {
		//		_windowState = WindowState::MainMenu;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 1", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug1;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 2", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug2;
		//	}
		//	ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("Debug 3", ImVec2(100, 30))) {
		//		_windowState = WindowState::Debug3;
		//	}
		//	/*ImGui::SameLine(spacing1, spacing2);
		//	if (ImGui::Button("New Window")) {
		//		ImGui::BeginPopupContextWindow("Testing");
		//		ImGui::Begin("New Window");
		//		ImGui::End();
		//	}*/
		//	ImGui::End();
		//}

		//ImGui::ShowDemoWindow();
	}
private:
	WindowState _windowState = WindowState::MainMenu;
	ImGuiWindowFlags _windowFlags = 0;
	ImVec2 _windowSize = ImVec2(1600, 900);
	bool* _pOpen = (bool*)true;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	//app->PushLayer<TextTest>();
	app->PushLayer<FootballUITest>();

	/*app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});*/
	return app;
}