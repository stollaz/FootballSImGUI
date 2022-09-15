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
#include <algorithm>

#include "player.cpp"
#include "playeringame.cpp"
#include "team.cpp"
#include "rotatableteam.cpp"
#include "goal.cpp"
#include "teamseasonstats.cpp"
#include "teamgamestats.cpp"
#include "logitem.cpp"
#include "game.cpp"
#include "gameweek.cpp"
#include "playerseasonstats.cpp"
#include "data.h"
#include "simulate.h"

using namespace Walnut;

// Old event logging text stuff is here, use this once conflicts are solved

//class TextItem {
//public:
//	std::string content;
//	ImVec4 colour;
//	bool newline;
//
//	TextItem(std::string _content, ImVec4 _colour, bool _newline) : content(_content), colour(_colour), newline(_newline) {}
//	TextItem(std::string _content, bool _newline) : content(_content), colour(ImVec4(255,255,255,255)), newline(_newline) {}
//};
//
//class TextLine {
//public:
//	std::vector<TextItem> parts;
//
//	void AddItem(TextItem t) {
//		parts.push_back(t);
//	}
//
//	TextLine(){};
//};
//
//class TextTest : public Walnut::Layer {
//public:
//	void InitialiseBuffers() {
//		writeBuffer.clear();
//		screenBuffer.clear();
//
//		for (int i = 0; i < maxIndex; i++) {
//			writeBuffer.push_back(fmt::format("{}", i));
//		}
//
//		for (int i = 0; i < bufferSize; i++) {
//			screenBuffer.push_back(writeBuffer.at(i));
//		}
//
//		maxitemindex = bufferSize - 1;
//	}
//
//	void InitialiseEventBuffers() {
//		eventBuffer.clear();
//		eventScreenBuffer.clear();
//
//		TextLine t = TextLine();
//		t.AddItem(TextItem("RED CARD! ", GetColour(255, 0, 0), false));
//		t.AddItem(TextItem("Player has been sent off.", GetColour(255, 255, 255), true));
//		eventBuffer.push_back(t);
//
//		TextLine t1 = TextLine();
//		t1.AddItem(TextItem("YELLOW CARD! ", GetColour(255, 255, 0), false));
//		t1.AddItem(TextItem("Player has been booked.", GetColour(255, 255, 255), true));
//		eventBuffer.push_back(t1);
//
//		TextLine t2 = TextLine();
//		t2.AddItem(TextItem("GOAL! Player scored a goal.", GetColour(255, 255, 255), true));
//		eventBuffer.push_back(t2);
//
//		TextLine t3 = TextLine();
//		t3.AddItem(TextItem("RED! ", GetColour(255, 0, 0), false));
//		t3.AddItem(TextItem("GREEN! ", GetColour(0, 255, 0), false));
//		t3.AddItem(TextItem("BLUE! ", GetColour(0, 0, 255), false));
//		t3.AddItem(TextItem("DEFAULT. ", true));
//		eventBuffer.push_back(t3);
//
//		for (int i = 0; i < bufferSize; i++) {
//			eventScreenBuffer.push_back(eventBuffer.at(i));
//		}
//	}
//
//	virtual void OnAttach() override {
//		InitialiseEventBuffers();
//	}
//
//	ImVec4 GetColour(int r, int g, int b) {
//		return ImVec4(std::clamp(r, 0, 255), std::clamp(g, 0, 255), std::clamp(b, 0, 255), 255);
//	}
//
//	void RenderText(TextItem t) {
//		ImGui::TextColored(t.colour, t.content.c_str());
//		if (!t.newline) ImGui::SameLine(0, 0);
//	}
//
//	virtual void OnUIRender() override
//	{
//		ImGui::Begin("Hello");
//		//if (maxitemindex != maxIndex-1) {
//		//	if (ImGui::Button("Next")) {
//		//		maxitemindex++;
//		//		screenBuffer.erase(screenBuffer.begin());
//		//		//screenBuffer.pop_back();
//		//		screenBuffer.push_back(writeBuffer.at(maxitemindex));
//		//	}
//		//}
//		//else if (ImGui::Button("Reset")) {
//		//	InitialiseEventBuffers();
//		//}
//
//		for (int i = 0; i < bufferSize; i++) {
//			//ImGui::Text(screenBuffer.at(i).c_str());
//			TextLine t = eventScreenBuffer.at(i);
//			for (TextItem ti : t.parts) RenderText(ti);
//		}
//
//		ImGui::End();
//	}
//private:
//	const int maxIndex = 4;
//	const int bufferSize = 4;
//	int maxitemindex = 0;
//	std::vector<std::string> writeBuffer;
//	std::vector<std::string> screenBuffer;
//
//	std::vector<TextLine> eventBuffer;
//	std::vector<TextLine> eventScreenBuffer;
//};

enum WindowState
{
	MainMenu,
	Debug1,
	Debug2,
	Debug3,
	Options
};

class PitchInfo {
public:
	int LeftEdge;
	int RightEdge;
	int TopEdge;
	int BottomEdge;

	ImVec2 TopLeft;
	ImVec2 TopRight;
	ImVec2 BottomLeft;
	ImVec2 BottomRight;

	ImVec2 Pitch;
	ImVec2 PitchSize;

	PitchInfo(ImVec2 topLeft, ImVec2 bottomRight, ImVec2 pitch, ImVec2 pitchSize) {
		TopLeft = topLeft;
		BottomRight = bottomRight;

		LeftEdge = topLeft.x;
		RightEdge = bottomRight.x;
		TopEdge = topLeft.y;
		BottomEdge = bottomRight.y;

		TopRight = ImVec2(RightEdge, TopEdge);
		BottomLeft = ImVec2(LeftEdge, BottomEdge);

		Pitch = pitch;
		PitchSize = pitchSize;
	}
};

// TODO:
// Move render marker function to player class so that each player can render their own marker
// For debugging this means that individual players can easily display their status
// E.G. for match engine testing, can highlight who is making a pass to who, etc.
// Need to figure out how to deal with accessing pitch info (e.g. edges and corners of render area), as well as positional lines and offsets
class PlayerOnPitch {
public:
	ImVec2 position;
	int number;
	char* name;
	bool isSelected = false;
	int markerSize;

	PitchInfo pitchInfo;

	ImU32 colour;
	ImU32 outlineColour;
	ImU32 textColour;

	void DrawMarker(bool showNumber, bool drawOutline, ImDrawList* draw) {
		if (drawOutline) 
			draw->AddCircle(ImVec2(pitchInfo.LeftEdge + (position.x / pitchInfo.Pitch.x) * pitchInfo.PitchSize.x, pitchInfo.TopEdge + (position.y / pitchInfo.Pitch.y) * pitchInfo.PitchSize.y), markerSize + 1, outlineColour); // Draw outline
		draw->AddCircleFilled(ImVec2(pitchInfo.LeftEdge + (position.x / pitchInfo.Pitch.x) * pitchInfo.PitchSize.x, pitchInfo.TopEdge + (position.y / pitchInfo.Pitch.y) * pitchInfo.PitchSize.y), markerSize, colour); // Draw marker for player

		// Draw player number on marker, centering on marker
		float fontSize = 2 * markerSize;
		auto TextSize = ImGui::CalcTextSize(std::to_string(number).c_str());
		TextSize.x *= (fontSize / 20);
		TextSize.y *= (fontSize / 20);
		if (showNumber) 
			draw->AddText(ImGui::GetFont(), fontSize, ImVec2(pitchInfo.LeftEdge + (position.x / pitchInfo.Pitch.x) * pitchInfo.PitchSize.x - TextSize.x * 0.5f, pitchInfo.TopEdge + (position.y / pitchInfo.Pitch.y) * pitchInfo.PitchSize.y - TextSize.y * 0.5f), textColour, std::to_string(number).c_str());
	}

	PlayerOnPitch(ImVec2 _p, int _num, char* _name, PitchInfo _pitchInfo) : position(_p), number(_num), name(_name), pitchInfo(_pitchInfo) {};
};

class SimplePlayer {
public:
	ImVec2 position;
	int number;
	char* name;

	SimplePlayer(ImVec2 _p, int _num, char* _name) : position(_p), number(_num), name(_name) {};
};

// TODO
// Determines a line of a formation (maybe not needed)
class FormationLine {
public:
	bool isWide; // Determines whether the line is wide or not (e.g. front 3 with wingers vs front 3 with all strikers
};

// TODO
// Defines a custom formation defined by number of players in each line, offsets of players in lines, etc.
class Formation {
public:
	bool hasDM;
	bool hasAM;
	bool hasWingers;
	bool hasFullbacks;

	std::vector<int> lineCounts;

	Formation(std::vector<int> _lineCounts) {
		bool valid = true;

		int sum = 0;
		for (int x : _lineCounts) sum += x;
		if (sum != 10) valid = false;
	}
};

enum CardState {
	None,
	Yellow,
	Red
};

// Enum for state of player in simulation
// TODO: Trim and refine these
enum PlayerState {
	// Neutral States
	Null, // Null state, for absent players or to indicate no future plan
	Idle, // Idle, not moving (e.g. waiting in position)
	Positioning, // Moving into / remaining in default position
	MovingToBall, // Moving directly to the ball
	// Defensive States
	Clearing, // In the process of clearing the ball in any direction 
	MovingToBlock, // Moving into a position to block the ball, between ball and goal
	Chasing, // Chasing after a player in posession of the ball (out of posession)
	Intercepting, // Looking to intercept / collect a loose ball
	Covering, // (Rename) Looking to get between ball and goal (defensive)
	Tackling, // Making a tackle
	TacklingHard, // Making a hard (desperate) tackle
	TacklingSoft, // Making a soft (safe, on booking) tackle
	Heading, // Looking to win a header (clearance / defensive)
	Marking, // Marking a given location / player
	// Ball Carrying actions
	Dribbling, // Dribbling with the ball to a given area (moving)
	Shielding, // Shielding the ball from an opponent (stationary)
	// Ball Progressing Actions
	Scanning, // In the process of looking for a teammate to pass to
	Passing, // In the process of passing the ball (ground)
	CrossingAir, // In the process of crossing the ball (high for header)
	CrossingLow, // In the process of crossing the bacll low
	ThroughBall, // Making a through ball
	// Attacking Actions
	LookingForSpace, // Looking for and moving into open space to recieve the ball
	CallingForBall, // Calling for the ball from a teammate
	LiningUpShot, // In the process of lining up a shot
	Shooting, // In the process of shooting the ball
	ShootingFinesse, // In the process of shooting the ball with placement over power
	ShootingPower, // In the process of shooting the ball with power over placement
	Attacking, // Moving into an attacking position
	HeadingAttacking, // Looking to win a header (to shoot / attacking)
	Overlapping, // Overlapping in a wide area (player out of posession)
	Underlapping, // Underlapping from a wide area (player out of posession)
	Penalty, // Taking a penalty
	FreeKickDirect, // Taking a direct free kick, looking to shoot
	FreeKickIndirect, // Taking an indirect free kick, looking to pass
	// Goalkeeping Actions
	GKIdle, // Goalkeeping idle, neutral positioning
	Sweeping, // Goalkeeper sweeping to collect a loose ball
	GKAlert, // Goalkeeper alert to danger, moving between ball and goal
	Saving, // In the process of making a save
	Kicking, // In the process of making a goal kick / free kick
	GKShortPass, // In the process of making a short pass to defender
	GKLongPass, // In the process of making a long pass upfield
};

// Class to store information about a player in a simulation
class PlayerInSimulation {
public:
	PlayerInSimulation() {}
private:
	PlayerState state; // State of the player
	PlayerState plannedState; // Planned next state of player (e.g. currently dribbling, planning to shoot)

	ImVec2 position; // Current osition of the player
	ImVec2 neutralPosition; // Neutral / starting position of the player
	ImVec2 targetPosition; // Target position (e.g. for dribbling, marking)
	bool hasBall; // Whether the player is in control of the ball or not
	float fitness; // Fitness level on scale from 0.0 (dead) to 1.0 (perfectly fit)
	CardState card; // The state of the card for the player

	char* name;
	// TODO: Stats
	//		Or link to existing player object with this information, and keep this class for simulation information
};

// Enum for state of the simulator
enum SimulationState {
	Off,
	Running,
	Paused,
	Stopped,
};

class Simulator {
public:
	// Initialise the simulator class
	Simulator(int _tps) {
		TPS = _tps;
		tickTime = 1000 / (float)TPS;
		lastTickTime = 0;
		timeMultiplier = 1.0f;
		state = SimulationState::Off;
		tickNumber = 0;
		startTime = NULL;
		timeElapsed = NULL;
		matchTimeElapsed = NULL;
		matchTimeMins = 0;
		matchTimeSecs = 0;
	}

	// Public setter for the TPS 
	void setTPS(int t) {
		if (t < 1) ImGui::BeginPopup("Tick rate must be positive");
		if (t > 1000) ImGui::BeginPopup("Tick rate must be less than 1000");
		else TPS = t;
		setTickTime();
	}
	int getTPS() { return TPS; } // Public getter for the TPS

	float getTickTime() { return tickTime; } // Public getter for the time per tick

	// Public setter for the match time multiplier
	void setTimeMultiplier(float t) {
		if (t <= 0.0f) ImGui::BeginPopup("Time multiplier must be greater than zero");
		else if (t >= 10.0f) ImGui::BeginPopup("Time multiplier must be less than 10");
		else timeMultiplier = t;
	}
	float getTimeMultiplier() { return timeMultiplier; } // Public getter for the match time multiplier

	float getLastTickTime() { return lastTickTime; } // Public getter for current tick number
	float getStartTime() { return startTime; } // Public getter for start time of simulation
	float getTimeElapsed() { return timeElapsed; } // Public getter for the time elapsed in simulation

	int getMatchTimeMins() { return matchTimeMins; } // Public getter for match time in minutes
	int getMatchTimeSecs() { return matchTimeSecs; } // Public getter for match time in seconds

	// Public function to get a string explaining the state of the simulator
	char* printState() {
		switch (state) {
			case SimulationState::Off:
				return "Simulation is off.";
			case SimulationState::Paused:
				return "Simulation is paused.";
			case SimulationState::Running:
				return "Simulation is running.";
			case SimulationState::Stopped:
				return "Simulation is stopped.";
			default:
				return "Simulation is undefined. Error.";
		}
	}

	SimulationState getState() { return state; } // Public getter for simulator state as a SimulationState enum

	// TODO
	// Initialise the simulation (might be the same as Begin)
	void Initialise() {

	}

	// TODO
	// Begin the simulation
	void Begin() {
		if (state != SimulationState::Off); // Do some popup as this is illegal
		else {
			state = SimulationState::Running; // Set the state
			startTime = ImGui::GetTime(); // Initialise start time to current time
			lastTickTime = ImGui::GetTime(); // Set last tick time to current time
			calculateMatchTime(); 
		}
	}

	// TODO
	// Step through the simulation - to be run on each tick
	void Step() {
		tickNumber++; // Increment tick number
		timeElapsed += ImGui::GetTime() - lastTickTime; // Increment time elapsed (to avoid elapsing time while paused / stopped)
		matchTimeElapsed += (ImGui::GetTime() - lastTickTime) * timeMultiplier; // Increment match time elapsed (considering time multiplier)
		lastTickTime = ImGui::GetTime(); // Set last tick time to current time
		calculateMatchTime(); // Recalculate match time

		// DO CALCULATIONS PER TICK
		// e.g. move players, move ball, etc.
	}

	// TODO
	// Pause the simulation
	void Pause() {
		if (state != SimulationState::Running); // Do some popup as this is illegal
		else {
			state = SimulationState::Paused;
		}
	}

	// TODO
	// Unpause the simulation
	void Unpause() {
		if (state != SimulationState::Paused); // Do some popup as this is illegal
		else {
			state = SimulationState::Running;
			lastTickTime = ImGui::GetTime(); // When unpausing, set last tick time to now to avoid counting time while paused
		}
	}
	
	// TODO
	// Stop the simulation
	void Stop() {
		if (state == SimulationState::Stopped || state == SimulationState::Off); // Do some popup as this is illegal
		else {
			state = SimulationState::Stopped;
		}
	}

	// TODO
	// Reset the simulation
	void Reset() {
		state = SimulationState::Off;
		tickNumber = 0;
		lastTickTime = 0;
		startTime = NULL;
	}

private:
	int TPS; // Ticks per second
	float tickTime; // Length of a tick in ms
	float timeMultiplier; // Multiplier to passage of time (e.g. 1.0 is normal speed, 0.5 is half speed, 2.0 is double speed)
	ImVec2 ballPosition; // Position of the ball
	SimulationState state; // State of the system as an enum

	int tickNumber; // Current tick number
	float lastTickTime; // Time of last tick
	float startTime; // Time simulation was started
	float timeElapsed; // Time elapsed during simulation (not counting time paused / stopped)
	float matchTimeElapsed; // Match time elapsed (including time multiplier)

	int matchTimeMins; // Minute of current match for scoreboard 
	int matchTimeSecs; // Second *

	void setTickTime() { tickTime = 1000.0f / (float)TPS; } // Private method to set tick time

	// Private method to calculate the match time
	void calculateMatchTime() {
		matchTimeMins = (int)floor(matchTimeElapsed) / 60;
		matchTimeSecs = (int)floor(matchTimeElapsed) % 60;
	}

	// Player vectors
	// Probably want to use something more complex than SimplePlayer
	// Decide whether to store position and action in input player type or within the class
	std::vector<SimplePlayer> team1;
	std::vector<SimplePlayer> team2;
};

class FootballUITest : public Walnut::Layer
{
public:
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

	std::vector<SimplePlayer> CreateTeam1() {
		std::vector<SimplePlayer> team1Players;

		team1Players.push_back(SimplePlayer(ImVec2(GKLineHeight, Pitch.y / 2), 1, "David De Gea")); // GK

		team1Players.push_back(SimplePlayer(ImVec2(DefensiveLineHeight + FullBackHeightOffset, (Pitch.y / 2) - FBWidth), 23, "Luke Shaw")); // LB
		team1Players.push_back(SimplePlayer(ImVec2(DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 5, "Harry Maguire")); // LCB
		team1Players.push_back(SimplePlayer(ImVec2(DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 19, "Raphael Varane")); // RCB
		team1Players.push_back(SimplePlayer(ImVec2(DefensiveLineHeight + FullBackHeightOffset, (Pitch.y / 2) + FBWidth), 20, "Diogo Dalot")); // RB

		team1Players.push_back(SimplePlayer(ImVec2(MidfieldLineHeight, (Pitch.y / 2) - CMWidth), 8, "Bruno Fernandes")); // LCM
		team1Players.push_back(SimplePlayer(ImVec2(MidfieldLineHeight + DMHeightOffset, (Pitch.y / 2)), 6, "Frenkie de Jong")); // CDM
		team1Players.push_back(SimplePlayer(ImVec2(MidfieldLineHeight, (Pitch.y / 2) + CMWidth), 17, "Fred")); // RCM

		team1Players.push_back(SimplePlayer(ImVec2(AttackingLineHeight + WingerHeightOffset, (Pitch.y / 2) - WingerWidth), 25, "Jadon Sancho")); // LW
		team1Players.push_back(SimplePlayer(ImVec2(AttackingLineHeight, (Pitch.y / 2)), 7, "Cristiano Ronaldo")); // ST
		team1Players.push_back(SimplePlayer(ImVec2(AttackingLineHeight + WingerHeightOffset, (Pitch.y / 2) + WingerWidth), 11, "Antony")); // RW

		return team1Players;
	}

	std::vector<SimplePlayer> CreateTeam2() {
		std::vector<SimplePlayer> team2Players;

		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - GKLineHeight, Pitch.y / 2), 31, "Ederson")); // GK

		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) - FBWidth), 27, "Joao Cancelo")); // LB
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) - CBWidth), 14, "Aymeric Laporte")); // LCB
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - DefensiveLineHeight, (Pitch.y / 2) + CBWidth), 5, "Ruben Dias")); // RCB
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - (DefensiveLineHeight + FullBackHeightOffset), (Pitch.y / 2) + FBWidth), 2, "Kyle Walker")); // RB

		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) - CMWidth), 17, "Kevin De Bruyne")); // LCM
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - (MidfieldLineHeight + DMHeightOffset), (Pitch.y / 2)), 16, "Rodri")); // CDM
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - MidfieldLineHeight, (Pitch.y / 2) + CMWidth), 20, "Bernardo Silva")); // RCM

		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) - WingerWidth), 47, "Phil Foden")); // LW
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - AttackingLineHeight, (Pitch.y / 2)), 9, "Erling Haaland")); // ST
		team2Players.push_back(SimplePlayer(ImVec2(Pitch.x - (AttackingLineHeight + WingerHeightOffset), (Pitch.y / 2) + WingerWidth), 26, "Riyad Mahrez")); // RW

		return team2Players;
	}

	// Dimensions:
	// Outline: 120x90 (metres, WxH)
	// Centre Circle: Radius 9.15m
	// Penalty Area: 16.5x40.3m
	// 6yd Box: 5.5x18.32m
	// Goal Width: 7.32m
	// Penalty Spot: 11m from byline
	// Corner segment: Radius 1m

	// Render the background pitch textures
	// TODO: Store pitch lines somehwere for local size on pitch, so they can be used to detect when e.g. ball / player is in penalty area 
	void RenderPitch(ImDrawList *draw) {
		draw->AddRectFilled(TopLeft, BottomRight, grassCol); // Grass
		draw->AddRect(TopLeft, BottomRight, lineCol); // Outline
		draw->AddLine(ImVec2(localHalfway, TopEdge), ImVec2(localHalfway, BottomEdge), lineCol); // Halfway Line
		draw->AddCircle(Centre, (9.15 / 120) * PitchSize.x, lineCol); // Centre Circle
		draw->AddCircleFilled(Centre, 3, lineCol); // Centre Dot

		draw->AddRect(ImVec2(LeftEdge, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(LeftEdge + (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Left Penalty Area
		draw->AddRect(ImVec2(RightEdge - 1, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(RightEdge - (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineCol); // Right Penalty Area

		draw->AddRect(ImVec2(LeftEdge, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(LeftEdge + (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Left 6yd Area
		draw->AddRect(ImVec2(RightEdge - 1, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(RightEdge - (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineCol); // Right 6yd Area

		draw->AddRectFilled(ImVec2(LeftEdge, Centre.y - (3.66 / 90) * PitchSize.y), ImVec2(LeftEdge - (0.5 / 120) * PitchSize.x, Centre.y + (3.66 / 90) * PitchSize.y), lineCol); // Left Goal
		draw->AddRectFilled(ImVec2(RightEdge - 1, Centre.y - (3.66 / 90) * PitchSize.y), ImVec2(RightEdge + (0.5 / 120) * PitchSize.x, Centre.y + (3.66 / 90) * PitchSize.y), lineCol); // Right Goal

		draw->AddCircleFilled(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Left Penalty Spot
		draw->AddCircleFilled(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), 3, lineCol); // Right Penalty Spot

		// Left D
		draw->PathArcTo(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120) * PitchSize.x, -0.94, 0.94);
		draw->PathStroke(lineCol);

		// Right D
		draw->PathArcTo(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120) * PitchSize.x, 3.14159 - 0.94, 3.14159 + 0.94);
		draw->PathStroke(lineCol);

		// Top Left Corner
		draw->PathArcTo(TopLeft, (1.0 / 120) * PitchSize.x, 0, 3.14159 / 2);
		draw->PathStroke(lineCol);

		// Top Right Corner
		draw->PathArcTo(TopRight, (1.0 / 120) * PitchSize.x, 3.14159 / 2, 3.14159);
		draw->PathStroke(lineCol);

		// Bottom Left Corner
		draw->PathArcTo(BottomLeft, (1.0 / 120) * PitchSize.x, 0, -3.14159 / 2);
		draw->PathStroke(lineCol);

		// Bottom Right Corner
		draw->PathArcTo(BottomRight, (1.0 / 120) * PitchSize.x, 3.14159, 3 * 3.14159 / 2);
		draw->PathStroke(lineCol);
	}

	void RenderPlayerMarker(SimplePlayer p, ImU32 colour, ImDrawList* draw) {
		ImVec2 pos = p.position; // Get position part
		if (drawOutline) draw->AddCircle(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), MarkerSize + 1, IM_COL32(0, 0, 0, 255)); // Draw outline
		draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), MarkerSize, colour); // Draw marker for player

		// Draw player number on marker, centering on marker
		float fontSize = 2 * MarkerSize;
		auto TextSize = ImGui::CalcTextSize(std::to_string(p.number).c_str());
		TextSize.x *= (fontSize / 20);
		TextSize.y *= (fontSize / 20);
		if (showNumbers) draw->AddText(ImGui::GetFont(), fontSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x * 0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y * 0.5f), lineCol, std::to_string(p.number).c_str());
	}

	void RenderHoverText(SimplePlayer p, ImDrawList* draw) {
		if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x - MarkerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y - MarkerSize),
			ImVec2(LeftEdge + (p.position.x / Pitch.x) * PitchSize.x + MarkerSize, TopEdge + (p.position.y / Pitch.y) * PitchSize.y + MarkerSize))) { // Check if mouse is in bounding box around player marker
			auto TextSize = ImGui::CalcTextSize(p.name);
			draw->AddRectFilled(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), ImVec2(ImGui::GetMousePos().x + TextSize.x, ImGui::GetMousePos().y - 20 + TextSize.y), IM_COL32(0, 0, 0, 100), 1.0f); // Draw transluscent background for text
			draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineCol, p.name); // Draw text above the cursor
		}
	}

	// Render the game / players on top of the pitch
	void RenderPlayers(std::vector<SimplePlayer> team1Players, std::vector<SimplePlayer> team2Players, ImDrawList* draw) {
		ImFont* font = ImGui::GetFont(); // Get Font

		// Iterate over players in teams
		for (SimplePlayer& p : team1Players) RenderPlayerMarker(p, team1Col, draw);
		for (SimplePlayer& p : team2Players) RenderPlayerMarker(p, team2Col, draw);

		// Do name hover text at the very end so that markers are never rendered on top of the text
		// If the user hovers over the player on the screen, display their name
		for (SimplePlayer& p : team1Players) RenderHoverText(p, draw);
		for (SimplePlayer& p : team2Players) RenderHoverText(p, draw);
	}

	// Master render function for the pitch view
	void RenderGame() {
		ImVec2 WindowPos = ImGui::GetWindowPos();

		ImVec2 WindowSize = ImGui::GetWindowSize();

		int ShortestDim = std::min(WindowSize.x, WindowSize.y);

		// Conversions for pitch size to window size
		ImVec2 PitchToWindow = ImVec2(WindowSize.x / Pitch.x, WindowSize.y / Pitch.y);

		float markerSize = MarkerSize;
		ImDrawList *draw = ImGui::GetForegroundDrawList(); // Initialise draw list to be able to make draw calls

		// Offset vector from edge of window to provide slight padding
		// Also since (0,0) is in the top left where the window title bar is, not where the viewable portion is
		ImVec2 Offset = ImVec2(WindowPos.x + 5, WindowPos.y + 30);

		int xOffset = (int)Offset.x;
		int yOffset = (int)Offset.y;

		// Define edges of the renderable pitch area
		TopEdge = Offset.y;
		BottomEdge = Offset.y + WindowSize.y - 35;
		LeftEdge = Offset.x;
		RightEdge = Offset.x + WindowSize.x - 10;

		// Define corners of the renderable pitch area
		TopLeft = ImVec2(LeftEdge, TopEdge);
		TopRight = ImVec2(RightEdge, TopEdge);
		BottomLeft = ImVec2(LeftEdge, BottomEdge);
		BottomRight = ImVec2(RightEdge, BottomEdge);

		PitchSize = ImVec2(RightEdge - LeftEdge, BottomEdge - TopEdge); // Define the pitch size on the screen

		localHalfway = (LeftEdge + RightEdge) / 2; // Define the local halfway point on the screen

		Centre = ImVec2((LeftEdge + RightEdge) / 2, (TopEdge + BottomEdge) / 2); // Define the local centre of the pitch on the screen

		// Only modify object if there are changes
		if (pitchInfo.TopEdge != TopEdge || pitchInfo.BottomEdge != BottomEdge || pitchInfo.RightEdge != RightEdge || pitchInfo.LeftEdge != LeftEdge)
			pitchInfo = PitchInfo(TopLeft, BottomRight, Pitch, PitchSize);

		// Draw pitch
		RenderPitch(draw);

		// Players

		// TODO: Lists of player locations
		// Have a player object, as well as a location (relative to the 120x90 pitch, which will be translated to screen space)
		// Could also contain some information on momentum / inertia for movement
		// For testing now though just use ImVec2 for pure positions and testing

		// Add players to Teams
		// Players have a position, number and name
		std::vector<SimplePlayer> team1Players = CreateTeam1();
		std::vector<SimplePlayer> team2Players = CreateTeam2();

		RenderPlayers(team1Players, team2Players, draw);
	}

	// Show the settings
	void RenderSettings() {
		ImGui::SliderFloat("Goalkeeper Height", &GKLineHeight, 0, 20);
		ImGui::SliderFloat("Defensive Line Height", &DefensiveLineHeight, 10, 30);
		ImGui::SliderFloat("Midfield Line Height", &MidfieldLineHeight, 20, 40);
		ImGui::SliderFloat("Attacking Line Height", &AttackingLineHeight, 40, 60);
		ImGui::NewLine();

		ImGui::SliderFloat("Full Back Width", &FBWidth, 0, 45);
		ImGui::SliderFloat("Winger Width", &WingerWidth, 0, 45);
		ImGui::SliderFloat("Centre Back Width", &CBWidth, 0, 45);
		ImGui::SliderFloat("Centre Mid Width", &CMWidth, 0, 45);
		ImGui::NewLine();

		ImGui::SliderFloat("Full Back Height Offset", &FullBackHeightOffset, -10, 10);
		ImGui::SliderFloat("Defensive Mid Height Offset", &DMHeightOffset, -10, 10);
		ImGui::SliderFloat("Winger Height Offset", &WingerHeightOffset, -10, 10);
		ImGui::NewLine();

		ImGui::SliderFloat("Marker Size", &MarkerSize, 5, 15);

		ImGui::Checkbox("Show Numbers", &showNumbers);
		ImGui::Checkbox("Draw Outline", &drawOutline);

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

		ImGui::NewLine();
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	}

	// Show the settings
	void RenderSimSettings() {
		ImGui::SliderInt("Tick Rate (TPS)", &TPS, 1, 64);
		if (ImGui::IsItemEdited()) simulator.setTPS(TPS);

		ImGui::SliderFloat("Time Multiplier", &timeMultiplier, 0.5, 5.0);
		if (ImGui::IsItemEdited()) simulator.setTimeMultiplier(timeMultiplier);
		if (ImGui::Button("Reset")) {
			TPS = 20;
			simulator.setTPS(20);
			timeMultiplier = 1.0f;
			simulator.setTimeMultiplier(1.0f);
		}
			;
		ImGui::NewLine();
		ImGui::Text("%d TPS (%.1f ms per tick)", simulator.getTPS(), simulator.getTickTime());
		//ImGui::Text("Current time: %f", simulator.getStartTime());
		ImGui::NewLine();

		ImVec2 pos = ImGui::GetCursorPos();
		if (simulator.getState() == SimulationState::Off) {
			if (ImGui::Button("Start Simulation", ImVec2(150,30))) {
				// TODO
				simulator.Begin();
			}
		}
		else {
			if (ImGui::Button("Reset Simulation", ImVec2(150, 30))) {
				// TODO
				simulator.Reset();
			}
		}
		
		ImGui::SetCursorPos(ImVec2(pos.x + 160, pos.y));
		if (simulator.getState() == SimulationState::Paused) {
			if (ImGui::Button("Unpause Simulation", ImVec2(150, 30))) {
				// TODO
				simulator.Unpause();
			}
		}
		else {
			if (ImGui::Button("Pause Simulation", ImVec2(150, 30))) {
				// TODO
				simulator.Pause();
			}
		}

		ImGui::Text(simulator.printState());
		ImGui::NewLine();

		if (simulator.getState() == SimulationState::Running && ((ImGui::GetTime() - simulator.getLastTickTime()) * simulator.getTimeMultiplier() >= simulator.getTickTime() / 1000))  simulator.Step();
		ImGui::Text("%d ticks have elapsed in simulation.", simulator.getTickNumber());
		ImGui::Text("%.2f s have elapsed in simulation.", simulator.getTimeElapsed()); // This doesn't work as it doesnt account for time paused or stopped
		//ImGui::Text("Match Time: %02d:%02d", (int)floor(simulator.getTimeElapsed())/60, (int)floor(simulator.getTimeElapsed()) % 60); // This doesn't work as it doesnt account for time paused or stopped
		ImGui::Text("Match Time: %02d:%02d", simulator.getMatchTimeMins(), simulator.getMatchTimeSecs());
		ImGui::NewLine();

		ImGui::Separator();
		// -----------

		ImGui::SliderFloat("Marker Size", &MarkerSize, 5, 15);

		ImGui::Checkbox("Show Numbers", &showNumbers);
		ImGui::Checkbox("Draw Outline", &drawOutline);

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

		ImGui::NewLine();
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
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
			if (ImGui::Button("Render Test", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(2*ThisRegionSize.x / 4 -50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Sim Test", ImVec2(100,30))) {
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
			ImGui::Begin("Pitch Render Test", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImGui::Begin("Settings");

			RenderSettings();

			ImGui::End();

			ImGui::Begin("Pitch");

			RenderGame();

			ImGui::End();

			// Rest of generic debug window settings for navigation

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Pitch Render Test");
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
			if (ImGui::Button("Sim Test", ImVec2(100, 30))) {
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
			ImGui::Begin("Simulation Test", _pOpen, _windowFlags);
			ImGui::SetWindowSize(_windowSize);
			ImVec2 ThisRegionSize = ImGui::GetContentRegionAvail();
			ImVec2 textSize = ImGui::CalcTextSize(name);

			ImGui::Begin("Settings");

			RenderSimSettings();

			ImGui::End();

			ImGui::Begin("Pitch");

			RenderGame();

			ImGui::End();

			// Rest of generic debug window settings for navigation

			ImVec2 initialPos = ImGui::GetCursorPos();
			ImVec2 newPos = ImVec2(ThisRegionSize.x / 2 - textSize.x / 2, ThisRegionSize.y / 3);

			ImGui::SetCursorPos(newPos);

			ImGui::Text(name);

			ImGui::SetCursorPos(initialPos);

			ImGui::SetWindowFontScale(2.0f);
			ImGui::Text("Simulation Test");
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
			if (ImGui::Button("Render Test", ImVec2(100, 30))) {
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
			if (ImGui::Button("Render Test", ImVec2(100, 30))) {
				_windowState = WindowState::Debug1;
			}
			ImGui::SetCursorPos(ImVec2(3 * ThisRegionSize.x / 4 - 50, 2.0f * ThisRegionSize.y / 3));
			//ImGui::SameLine(spacing1, spacing2); 
			if (ImGui::Button("Sim Test", ImVec2(100, 30))) {
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
	}
private:
	WindowState _windowState = WindowState::MainMenu;
	ImGuiWindowFlags _windowFlags = 0;
	ImVec2 _windowSize = ImVec2(1600, 900);
	bool* _pOpen = (bool*)true;

	// Modifiable line heights
	float GKLineHeight = 7; // ?? to ??
	float DefensiveLineHeight = 20; // ?? to ??
	float MidfieldLineHeight = 37; // ?? to ??
	float AttackingLineHeight = 50; // ?? to ??

	// Modifiable player widths
	float WingerWidth = 35; // 0 to 45
	float FBWidth = 35; // 0 to 45
	float CMWidth = 10; // 0 to 45
	float CBWidth = 10; // 0 to 45

	// Modifiable height offsets for wide players and central players
	float FullBackHeightOffset = 0;
	float DMHeightOffset = -5;
	float WingerHeightOffset = -3;

	float MarkerSize = 5; // Size of the marker to represent players

	bool showNumbers = false; // To show shirt numbers or not
	bool drawOutline = true; // To draw an outline around player markers or not

	// Initialise colours
	// TODO: Make these global with color picker for team colors
	ImU32 team1Col = IM_COL32(255, 0, 0, 255);
	ImU32 team2Col = IM_COL32(3, 202, 252, 255);
	ImU32 lineCol = IM_COL32(255, 255, 255, 255);
	ImU32 grassCol = IM_COL32(23, 141, 23, 255);

	// Dimensions of pitch in metres for use in internal calculations
	ImVec2 Pitch = ImVec2(120, 90);

	// Define edges of the renderable pitch area
	int TopEdge;
	int BottomEdge;
	int LeftEdge;
	int RightEdge;

	// Define corners of the renderable pitch area
	ImVec2 TopLeft;
	ImVec2 TopRight;
	ImVec2 BottomLeft;
	ImVec2 BottomRight;

	ImVec2 PitchSize; // Define the pitch size on the screen

	int localHalfway; // Define the local halfway point on the screen

	ImVec2 Centre; // Define the local centre of the pitch on the screen

	PitchInfo pitchInfo = PitchInfo(TopLeft, BottomRight, Pitch, PitchSize);

	int TPS = 20; // Tick rate in ticks per second
	float tickTime = 1000 / TPS; // Tick time in ms
	float timeMultiplier = 1.0f; // Time multiplier

	Simulator simulator = Simulator(20);
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