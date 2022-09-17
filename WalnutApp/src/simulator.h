#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"

// Enum for what level of booking a player has
enum CardState {
	None,
	// Warning? Intermediate stages could be useful for dictating how careful a player should be, but this could also be done with a simple float and leave the enum for concrete card values
	Yellow,
	// Final warning?
	Red
};

// Enum for player / team mentalities, dictating the manner in which they will react in certain situations
enum Mentality {
	UltraDefence, // Ultra defensive, will always look to defend even when an attacking opportunity arises (for endgame final defence)
	Defence, // Defensive, will look to remain back at most times
	Support, // Supportive, will look to aid transitions (stay back during advanced attacks, stay forward during sustained defence in order to carry ball)
	Attack, // Attacking, will look to attack at most times
	UltraAttack // Ultra attacking, will always look to attack even when conceding a chance (for endgame final push)
};

// Potential enum for player width to dictate where they stay horizontally in game (maybe not needed)
enum PlayerWidth {
	Central, // Will aim to stay central dependant on flow
	WideLeft, // Will aim to stay on the left
	CentreLeft, // Will aim to stay on the left of the centre
	CentreRight, // " right of centre
	WideRight, // Will aim to stay on the right
	PureCentre, // Will always stay central regardless of flow
	PureWideRight, // Will always stay right
	PureWideLeft, // " left
	InsideLeft, // Cuts in from the left
	InsideRight, // Cuts in from the right
	LeaningLeft, // Starts central but moves right when in posession
	LeaningRight, // " moves left "
	FreeRoam // Free roams wherever they wish
};

// Enum for player roles that dictate how they react to situations in terms of the actions they perform
enum PlayerRole {
	Goalkeeper, // Standard, defensive goalkeeper
	SweeperKeeper, // Proactive goalkeeper, looks to sweep and involve in transitions
	CentreBack, // Standard defender
	FullBack, // More defensive wide defender
	WingBack, // More progressive wide defender / attacking
	DefensiveMid, // Midfielder that prefers to defend
	BallWinningMid, // Looks to win the ball at all times
	CentreMid, // Standard central midefielder
	BoxToBox, // Looks to progress the ball vertically
	Playmaker, // Looks like play key passes from deeper
	AdvancedPlaymaker, // Further up the pitch playmaker
	WidePlaymaker, // Wider playmaker
	WideMid, // Wide standard midfielder
	DefensiveWideMid, // Defensive wide midfielder
	AttackingMid, // Standard attacking midfielder
	ShadowStriker, // Attacking midfielder that plays off the striker
	Winger, // Wide attacker, looks to run with the ball 
	InvertedWinger, // Looks to run with the ball from wide positions to inside and make passes 
	InsideForward, // Looks to cut inside and score
	WideForward, // Looks to score from wide positions
	Striker, // Looks to score primarily
	Poacher, // Looks only to get into positions to score, no build up
	DeepLyingForward, // Looks to get involved in build up and score
	TargetForward // Looks to win headers and be the target for long balls
};

// Potential enum of player traits to further dictate how a player reacts
// Some of the potential traits could also be attributes (e.g. shooting tendency)
enum PlayerTraits {
	ShootsFirstTime,
	PlacesShots,
	ShootsWithPower,
	SafeClearences,
	HardTackler,
	SoftTackler,
	RunsWithBall, // Could be attribute (passing tendency vs dribbling tendency)
	DirectPasser, // Could be attribute (passing directness / passing safety)
	SafePasser,
	HoldsUpPlay
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

// Potential abstract class for a simulatable object (e.g. player or ball)
class SimulationObject {
public:
	virtual void Step(float deltaT) = 0; // Pure virtual function for what to do per timestep
	void Nudge(glm::vec3 d) { // Function to nudge the position by a direction vector
		position += d;
	}
private:
	glm::vec3 position; // Current position in 3d space
	glm::vec3 velocity; // Current velocity as a 3d vector
};

// Example of inherited class
class BallInSim : SimulationObject {
public:
	//
	void Step(float deltaT) {}
private:
	//
};

// Class to store information about a player in a simulation
// TODO: Figure out if this itself stores player attribute informaiton, or just another player class and then uses that information to simulate activity
class PlayerInSimulation {
public:
	PlayerInSimulation(glm::vec2 _initialPosition, PlayerRole _role, PlayerWidth _width, Mentality _mentality, char* _name, std::vector<PlayerTraits> _traits = {}) {
		position = _initialPosition;
		neutralPosition = _initialPosition;
		targetPosition = _initialPosition;

		role = _role;
		width = _width;
		playerMentality = _mentality;

		name = _name;

		state = (_role == PlayerRole::Goalkeeper || _role == PlayerRole::SweeperKeeper) ? PlayerState::GKIdle : PlayerState::Idle;
		plannedState = state;

		hasBall = false;
		fitness = 1.0f;
		card = CardState::None;

		traits = _traits;
	}

	// Set the player state
	void setState(PlayerState s) {
		state = s;
	}

	void addState(PlayerState s) {} // Add a state to the queue // TODO

	void setTargetPos(glm::vec2 p) {} // Set the target position to be acted on with the state
	void changeMentality(Mentality m) {} // Change the player mentality considering the flow of the game
	void addTrait(PlayerTraits t) {} // Give the player a trait (maybe this eventually comes from the base player class though)
	void modifyFitness(float f) {} // Modify the player's fitness by an amount (e.g. applying an injury or incremental fitness decline over the course of a match)

	void Step(float deltaT) {}
private:
	PlayerState state; // State of the player
	PlayerState plannedState; // Planned next state of player (e.g. currently dribbling, planning to shoot)
	// states could also be done in a form of queue, so multiple could be submitted at once (e.g. moving to a location to pass and then overlap, or move then shoot etc.)
	// Maybe also a tuple of possible states and locations? (e.g. dribble to (x0,y0) and then pass to (x1,y1)

	//std::queue<PlayerState> q;

	PlayerRole role;
	PlayerWidth width;
	Mentality playerMentality;
	std::vector<PlayerTraits> traits; // Potential storage of player traits

	// TODO: Attributes for how much deviation from role / width / mentality is allowed ?

	glm::vec2 position; // Current position of the player on the pitch
	glm::vec2 neutralPosition; // Neutral / starting position of the player
	glm::vec2 targetPosition; // Target position (e.g. for dribbling, marking)
	bool hasBall; // Whether the player is in control of the ball or not
	float fitness; // Fitness level on scale from 0.0 (dead) to 1.0 (perfectly fit)
	CardState card; // The state of the card for the player

	char* name; // Player name? Or link to existing player class
	// Team team; // Team information (e.g. formation, other players, team mentality)
	// TODO: Attributes (e.g. speed, height, finishing etc.)
	//		Or link to existing player object with this information, and keep this class for simulation information

	// Stats (maybe these want to be public just to make modification easier, and to avoid having dozens of setters)
	/*float distanceRun;
	int touches;
	int goals;
	int assists;
	int passesAttempted;
	int passesCompleted;
	int crossesAttempted;
	int crossesCompleted;
	int tacklesAttempted;
	int tacklesCompleted;
	int interceptionsAttempted;
	int interceptionsCompleted;
	int blocks;
	int clearances;
	int saves;
	int dribblesAttempted;
	int dribblesCompleted;
	int shots;
	int shotsOnTarget;
	int shotsOffTarget;
	int shotsBlocked;
	int hitWoodwork;
	int xGAccumulated;*/
};

class SimulatedTeam {
public:
	void AddPlayer(PlayerInSimulation p) { players.push_back(p); }

	SimulatedTeam(char* name, Mentality mentality, ImVec4 col) : teamName(name), teamMentality(mentality), colour(col) {}
private:
	std::vector<PlayerInSimulation> players;
	//Formation f;
	Mentality teamMentality;
	char* teamName;
	ImVec4 colour;
};

// Class to interact with the ball
class Ball {
public:
	// Apply a kick to the ball with a given strength in a given direction
	void Kick(float strength, glm::vec3 direction, float _spin) {
		// TODO
		// Start using physics equations here, so strenght is force rather than direct velocity? But maybe do apply some instantaneous velocity
		// F = ma, s = ut + 0.5at^2
		//velocity = strength; // Clamp this? Or at least have some max value? Or non-linear relationship?
		movementDirection = glm::normalize(direction + movementDirection); //Blend these with previous values too?
		acceleration = strength * movementDirection;
		velocity = (strength / 2) * movementDirection;
	}

	// Nudge the ball by a given vector (useful for debugging)
	void Nudge(glm::vec3 d) {
		position += d;
	}

	// Maybe this is useful? https://physics.ucf.edu/~roldan/classes/phy2048-ch4_sp12.pdf
	// Perform a step of the ball's movement, e.g. to simulate gravity or to continue a pre-existing movement momentum
	// Takes in a delta T, how much simulation time has elapsed since last step
	// This might be useful for friction https://gamedev.stackexchange.com/questions/114983/how-to-apply-friction-vector-to-acceleration-in-top-down-2d-game
	void Step(float deltaT) {
		// TODO
		// Ball should fall due to gravity on each tick
		// As well as continue along its current direction of movement with its current acceleration and velocity, losing some extra velocity due to friction and air resistance, and moving in some extra direction due to spin
		// Ds = uDt + 0.5aDt^2
		// Force = Force Applied - Frictional Force
		glm::vec3 s = position + (velocity * deltaT + (0.5f * acceleration) * deltaT * deltaT);
		position = s;
		glm::vec3 v = velocity + acceleration * deltaT;
		velocity = v;
		acceleration = acceleration - (airResistance * glm::normalize(velocity)) - (friction * glm::normalize(velocity)); // This is wrong but less wrong
	}

	void Reset() {
		position = defaultPosition;
		velocity = glm::vec3(0, 0, 0);
		acceleration = glm::vec3(0, 0, 0);
		movementDirection = glm::vec3(0, 0, 0);
		spin = 0.0f;
	}

	glm::vec3 getPosition() { return position; } // Retrieve the ball's position on the pitch
	void setPosition(glm::vec3 p) { position = p; } // Set the ball's position (should only ever be used when resetting play, and should not be done directly from a player)

	glm::vec3 getVelocity() { return velocity; } // Retrieve the ball's velocity - maybe make this noisy so it is hard to guess exact velocity? Or deal with this on player side?
	glm::vec3 getMovementDirection() { return movementDirection; } // Retrieve ball's movement direction - again maybe make this noisy? Include crude uncertaintly, e.g. direction is more uncertain at high speeds, vice versa
	glm::vec3 getAcceleration() { return acceleration; }
	glm::vec3 getFriction() { return friction; }
	glm::vec3 getAirResistance() { return airResistance; }

	Ball(glm::vec3 p, float w = 1.0f, float f = 0.0f, float a = 0.0f) {
		defaultPosition = p;

		Reset();

		weight = w;
		friction = glm::vec3(f, f, 0);
		airResistance = glm::vec3(0, 0, a);
	}
private:
	glm::vec3 defaultPosition; // Default position of the ball

	glm::vec3 position; // Current position of the ball (x,y,z), z = height
	glm::vec3 velocity; // Current velocity of the ball
	glm::vec3 acceleration; // Current acceleration of the ball - maybe split into horizonal and vertical? or even as a vec3 for each direction?
	glm::vec3 movementDirection; // Direction of movement of the ball (unit vector)
	float spin = 0.f; // Spin of the ball - -1 is max anti-clockwise (left) spin, 1 is max clockwise (right) spin

	float weight; // Weight of the ball (should only deviate from 1.0 for debugging probably)
	glm::vec3 friction; // Friction the ball feels on the grass
	glm::vec3 airResistance; // Air resistance the ball feels in the air
	const float g = 9.81;
};

// Enum for state of the simulator
enum SimulationState {
	Off, // Simulation is disabled
	Running, // Simulation is running 
	Paused, // Simulation is temporarily paused with current state saved
	Stopped, // Simulation is stopped (likely unused and unecessary)
};

class Simulator {
public:
	// Initialise the simulator class
	Simulator(int _tps, Ball b) : ball(b) {
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

	// Render the simulation contents
	// TODO
	void Render() {

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

	int getTickNumber() { return tickNumber; }
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

	Ball getBall() { return ball; }
	glm::vec3 getBallPosition() { return ball.getPosition(); }

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

			team1.push_back(PlayerInSimulation(glm::vec2(45, 45), PlayerRole::CentreMid, PlayerWidth::FreeRoam, Mentality::Support, "Test 1"));
			team2.push_back(PlayerInSimulation(glm::vec2(135, 45), PlayerRole::CentreMid, PlayerWidth::FreeRoam, Mentality::Support, "Test 2"));

			ball.Kick(10, glm::vec3(1, 0, 0), 0);
		}
	}

	void TestMovement1() {
		ball.setPosition(glm::vec3(
			PitchDim.x / 2 + 10 * cos(matchTimeElapsed),
			PitchDim.y / 2 + 10 * sin(matchTimeElapsed),
			0
		));
	}

	void TestMovement2(float matchTimeElapsedInLastTick) {
		ball.Nudge(glm::vec3(
			0.1, 0.1,
			0
		));
	}

	void TestMovement3(float matchTimeElapsedInLastTick) {
		ball.Step(matchTimeElapsedInLastTick);
	}

	// TODO
	// Step through the simulation - to be run on each tick
	void Step() {
		tickNumber++; // Increment tick number
		timeElapsed += ImGui::GetTime() - lastTickTime; // Increment time elapsed (to avoid elapsing time while paused / stopped)
		float matchTimeElapsedInLastTick = (ImGui::GetTime() - lastTickTime) * timeMultiplier;
		matchTimeElapsed += matchTimeElapsedInLastTick; // Increment match time elapsed (considering time multiplier)
		lastTickTime = ImGui::GetTime(); // Set last tick time to current time
		calculateMatchTime(); // Recalculate match time

		// TEST MOVEMENT
		// Move the ball in a circle, making sure movement is dependant on actual (match) time elapsed rather than just pure tick number
		TestMovement1();
		//TestMovement2(matchTimeElapsedInLastTick);
		//TestMovement3(matchTimeElapsedInLastTick);

		// TODO
		// DO CALCULATIONS PER TICK
		// e.g. move players, move ball, etc.
		// How does this interact with the pitch rendering?
		//		Does it return data to then be rendered? Or render it directly?
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
		timeElapsed = 0;
		matchTimeElapsed = 0;
		matchTimeMins = 0;
		matchTimeSecs = 0;

		//ball.setPosition(glm::vec3(PitchDim.x / 2, PitchDim.y / 2, 0));
		ball.Reset();
	}

private:
	int TPS; // Ticks per second
	float tickTime; // Length of a tick in ms
	float timeMultiplier; // Multiplier to passage of time (e.g. 1.0 is normal speed, 0.5 is half speed, 2.0 is double speed)
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
	std::vector<PlayerInSimulation> team1;
	std::vector<PlayerInSimulation> team2;
	ImVec2 ballPosition; // Position of the ball on the pitch
	Ball ball; // Ball data

	const glm::vec2 PitchDim = glm::vec2(120, 90);
};