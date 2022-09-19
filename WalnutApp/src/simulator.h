#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"

#include <Windows.h>

#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
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
	MovingToPosition, // Generic moving to state where player moves to a given position
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
	Collecting, // Moving to collect and pick up the ball
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

// Class to interact with the ball
class Ball {
public:
	// Apply a kick to the ball with a given strength in a given direction
	void Kick(float strength, glm::vec3 direction, float _spin = 0.0f) {
		// TODO
		// Start using physics equations here, so strenght is force rather than direct velocity? But maybe do apply some instantaneous velocity
		// F = ma, s = ut + 0.5at^2
		//velocity = strength; // Clamp this? Or at least have some max value? Or non-linear relationship?
		movementDirection = glm::normalize(direction + movementDirection); //Blend these with previous values too?
		acceleration = strength * movementDirection;
		velocity = (strength) * movementDirection;

		PlaySound(TEXT("src/ball_kick.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	// Nudge the ball by a given vector (useful for debugging)
	void Nudge(glm::vec3 d) {
		position += d;
	}

	void Bounce() {
		velocity.z = -velocity.z * 0.5f;
		PlaySound(TEXT("src/ball_bounce.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
		//acceleration = -(airResistance * glm::normalize(velocity)) - (friction * glm::normalize(velocity)); // This is wrong but less wrong

		// Apply either friction or air resitance to ball components depending on whether it is on the ground or not
		if (position.z <= 0.1f) acceleration.x = -(friction * glm::normalize(velocity)).x;
		else acceleration.x = -(airResistance * glm::normalize(velocity)).x;

		if (position.z <= 0.1f) acceleration.y = -(friction * glm::normalize(velocity)).y;
		else acceleration.y = -(airResistance * glm::normalize(velocity)).y;

		acceleration.z = -g - (airResistance * glm::normalize(velocity)).y;
		//if (glm::length(acceleration) < 0.3f) acceleration = glm::vec3(0, 0, 0);
		// When velocity gets too low, set velocity to zero to avoid oscillation or infinite rolling
		// Also set acceleration to 0 at this point too to avoid backwards movement
		//if (glm::length(velocity) < 0.25f) { velocity = glm::vec3(0, 0, 0); acceleration = glm::vec3(0, 0, 0); }

		// Early-stop terms to stop the ball quicker when at low velocities
		// Done per component to allow for better treating of verticality
		if (glm::abs(velocity.x) < 0.25f) { velocity.x = 0; acceleration.x = 0; }
		if (glm::abs(velocity.y) < 0.25f) { velocity.y = 0; acceleration.y = 0; }
		if (glm::abs(velocity.z) < 0.25f && position.z <= 0) { velocity.z = 0; acceleration.z = 0; } // Vertical stopping only occurs on the ground
		
		if (velocity == glm::vec3(0, 0, 0)) Stop();

		// Dampening term for bouncing, reversing velocity directiona and reducing velocity magnitude slowly over time (constant to be tweaked)
		if (position.z <= 0 && velocity.z < 0) { Bounce(); }
		//if (velocity.z < 0) { velocity.z = -velocity.z * 0.5f; }

		// TODO: Spin? Need to add velocity some proportion of the way around a arc towards a perpendicular vector

		// Somehow, when friction counteracts velocity / acceleration perfectly (when ball stops / changes direction), ball needs to stay stopped
	}

	void Reset() {
		position = defaultPosition;
		Stop();
	}

	void Stop() {
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
	const float g = 9.81f;
};


// Class to store information about a player in a simulation
// TODO: Figure out if this itself stores player attribute informaiton, or just another player class and then uses that information to simulate activity
class PlayerInSimulation {
public:
	PlayerInSimulation(glm::vec2 _initialPosition, int _number, char* _name, PlayerRole _role, PlayerWidth _width, Mentality _mentality, std::vector<PlayerTraits> _traits = {}) {
		position = _initialPosition;
		neutralPosition = _initialPosition;
		targetPosition = _initialPosition;

		role = _role;
		width = _width;
		playerMentality = _mentality;

		name = _name;
		number = _number;

		state = (_role == PlayerRole::Goalkeeper || _role == PlayerRole::SweeperKeeper) ? PlayerState::GKIdle : PlayerState::Idle;
		plannedState = state;

		hasBall = false;
		fitness = 1.0f;
		card = CardState::None;

		traits = _traits;

		q.push(state);
	}

	void addBall(Ball *b) { ball = b; }

	// Set the player state
	void setState(PlayerState s) {
		state = s;
	}

	// Add a state to the queue
	void addState(PlayerState s) {
		q.push(s);
	} 

	void popState() {
		q.pop();
	}

	void setTargetPos(glm::vec2 p) {} // Set the target position to be acted on with the state
	void changeMentality(Mentality m) {} // Change the player mentality considering the flow of the game
	void addTrait(PlayerTraits t) {} // Give the player a trait (maybe this eventually comes from the base player class though)
	void modifyFitness(float f) {} // Modify the player's fitness by an amount (e.g. applying an injury or incremental fitness decline over the course of a match)
	void resetPosition() { position = neutralPosition; }

	glm::vec2 getPosition() { return position; }
	char* getName() { return name; }
	int getNumber() { return number; }
	PlayerState getState() { return state; }

	bool checkNearBall(Ball b, float epsilon = 0.25f) {
		glm::vec2 ballPos = glm::vec2(b.getPosition().x, b.getPosition().y);
		float dist = glm::length(ballPos - position);
		return dist <= epsilon;
	}

	template<typename T>
	void myFunction(float deltaT, T&& lambda) {
		lambda();
	}

	// A test step function that just jiggles the player randomly
	void TestStep(float deltaT) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		r1 = (r1 - 0.5) * 2;
		r2 = (r2 - 0.5) * 2;

		glm::vec2 dir = glm::normalize(glm::vec2(r1, r2));

		dir.x *= deltaT * 5;
		dir.y *= deltaT * 5;

		//std::cout << "moved in direction " << dir.x << "," << dir.y << std::endl;
		SetPosition(position + dir);
		//std::cout << "New position: " << position.x << "," << position.y << std::endl;
	}

	// Basic step function to make players move to random locations repetaedly
	void RandomMovement(float deltaT) {
		// If the player is idle
		if (state == PlayerState::Idle || state == PlayerState::Null) {
			// If the queue isn't empty, get the next state from the queue
			if (q.front() != PlayerState::Idle) {
				state = q.front();
				q.pop();
			}
			// If the queue is empty, set to moving to position
			else {
				q.push(PlayerState::MovingToPosition);
				state = PlayerState::MovingToPosition;
				targetPosition = glm::vec2((int)(rand() % 120), (int)(rand() % 90));
				std::cout << " [" << name << "] Target: " << targetPosition.x << ", " << targetPosition.y << std::endl;
			}

		}

		// If the player is moving to a position, slowly move in that direction
		if (state == PlayerState::MovingToPosition) {
			glm::vec2 direction = glm::normalize(targetPosition - position);
			direction.x *= deltaT * 5;
			direction.y *= deltaT * 5;

			position += direction;

			// Stop moving when within range, to avoid stuttering
			if (glm::length(position - targetPosition) < 0.1f) {
				targetPosition = position;
				state = PlayerState::Idle; // Reset state
				q.push(PlayerState::MovingToPosition); // Add new movement state
			}
		}

		// Crude check for if the ball is nearby to kick the ball
		// Physics still broken AF
		if (checkNearBall(*ball, 1.0f) && (ImGui::GetTime() - lastKickedBall) > 1) {
			std::cout << "[" << name << "] tried to kick the ball" << std::endl;
			glm::vec3 pos3d = glm::vec3(position.x, position.y, 0);
			glm::vec3 dir = ball->getPosition() - pos3d;
			ball->Kick(10, glm::normalize(dir), 0);
			lastKickedBall = ImGui::GetTime();
		}
	}

	// Do this on every tick, should contain all the logic to simulate the player
	void Step(float deltaT) {
		//TestStep(deltaT);
		//RandomMovement(deltaT);
	}
private:
	PlayerState state; // State of the player
	PlayerState plannedState; // Planned next state of player (e.g. currently dribbling, planning to shoot)
	// states could also be done in a form of queue, so multiple could be submitted at once (e.g. moving to a location to pass and then overlap, or move then shoot etc.)
	// Maybe also a tuple of possible states and locations? (e.g. dribble to (x0,y0) and then pass to (x1,y1)

	std::queue<PlayerState> q;

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
	Ball *ball = nullptr;

	int lastKickedBall = 0;

	char* name; // Player name? Or link to existing player class
	int number;
	// Team team; // Team information (e.g. formation, other players, team mentality)
	// TODO: Attributes (e.g. speed, height, finishing etc.)
	//		Or link to existing player object with this information, and keep this class for simulation information

	void SetPosition(glm::vec2 p) { position = p; }

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

// Enum for state of the simulator
enum SimulationState {
	Off, // Simulation is disabled
	Running, // Simulation is running 
	Paused, // Simulation is temporarily paused with current state saved
	Stopped, // Simulation is stopped (likely unused and unecessary)
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

		LeftEdge = (int)topLeft.x;
		RightEdge = (int)bottomRight.x;
		TopEdge = (int)topLeft.y;
		BottomEdge = (int)bottomRight.y;

		TopRight = ImVec2(RightEdge, TopEdge);
		BottomLeft = ImVec2(LeftEdge, BottomEdge);

		Pitch = pitch;
		PitchSize = pitchSize;
	}
};

// Maybe separate class to render everything
class Renderer {
public:
	// Render the background pitch textures
	// TODO: Store pitch lines somehwere for local size on pitch, so they can be used to detect when e.g. ball / player is in penalty area 
	void RenderPitch(ImDrawList* draw) {
		Initialise(Pitch); // ???

		if (pitchInfo.TopEdge != TopEdge || pitchInfo.BottomEdge != BottomEdge || pitchInfo.RightEdge != RightEdge || pitchInfo.LeftEdge != LeftEdge)
			pitchInfo = PitchInfo(TopLeft, BottomRight, Pitch, PitchSize);
		
		draw->AddRectFilled(TopLeft, BottomRight, grassColour); // Grass
		draw->AddRect(TopLeft, BottomRight, lineColour); // Outline
		draw->AddLine(ImVec2(localHalfway, TopEdge), ImVec2(localHalfway, BottomEdge), lineColour); // Halfway Line
		draw->AddCircle(Centre, (9.15 / 120) * PitchSize.x, lineColour); // Centre Circle
		draw->AddCircleFilled(Centre, 3, lineColour); // Centre Dot

		draw->AddRect(ImVec2(LeftEdge, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(LeftEdge + (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineColour); // Left Penalty Area
		draw->AddRect(ImVec2(RightEdge - 1, Centre.y - (20.15 / 90) * PitchSize.y), ImVec2(RightEdge - (16.5 / 120) * PitchSize.x, Centre.y + (20.15 / 90) * PitchSize.y), lineColour); // Right Penalty Area

		draw->AddRect(ImVec2(LeftEdge, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(LeftEdge + (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineColour); // Left 6yd Area
		draw->AddRect(ImVec2(RightEdge - 1, Centre.y - (9.16 / 90) * PitchSize.y), ImVec2(RightEdge - (5.5 / 120) * PitchSize.x, Centre.y + (9.16 / 90) * PitchSize.y), lineColour); // Right 6yd Area

		draw->AddRectFilled(ImVec2(LeftEdge, Centre.y - (3.66 / 90) * PitchSize.y), ImVec2(LeftEdge - (0.5 / 120) * PitchSize.x, Centre.y + (3.66 / 90) * PitchSize.y), lineColour); // Left Goal
		draw->AddRectFilled(ImVec2(RightEdge - 1, Centre.y - (3.66 / 90) * PitchSize.y), ImVec2(RightEdge + (0.5 / 120) * PitchSize.x, Centre.y + (3.66 / 90) * PitchSize.y), lineColour); // Right Goal

		draw->AddCircleFilled(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), 3, lineColour); // Left Penalty Spot
		draw->AddCircleFilled(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), 3, lineColour); // Right Penalty Spot

		// Left D
		draw->PathArcTo(ImVec2(LeftEdge + (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120) * PitchSize.x, -0.94, 0.94);
		draw->PathStroke(lineColour);

		// Right D
		draw->PathArcTo(ImVec2(RightEdge - (11.0 / 120) * PitchSize.x, Centre.y), (9.15 / 120) * PitchSize.x, 3.14159 - 0.94, 3.14159 + 0.94);
		draw->PathStroke(lineColour);

		// Top Left Corner
		draw->PathArcTo(TopLeft, (1.0 / 120) * PitchSize.x, 0, 3.14159 / 2);
		draw->PathStroke(lineColour);

		// Top Right Corner
		draw->PathArcTo(TopRight, (1.0 / 120) * PitchSize.x, 3.14159 / 2, 3.14159);
		draw->PathStroke(lineColour);

		// Bottom Left Corner
		draw->PathArcTo(BottomLeft, (1.0 / 120) * PitchSize.x, 0, -3.14159 / 2);
		draw->PathStroke(lineColour);

		// Bottom Right Corner
		draw->PathArcTo(BottomRight, (1.0 / 120) * PitchSize.x, 3.14159, 3 * 3.14159 / 2);
		draw->PathStroke(lineColour);
	}

	// Render the player marker
	void RenderPlayerMarker(PlayerInSimulation p, ImU32 colour) {
		ImVec2 pos = ImVec2(p.getPosition().x, p.getPosition().y); // Get position part
		if (drawOutline) draw->AddCircle(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize + 1, IM_COL32(0, 0, 0, 255)); // Draw outline
		draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), markerSize, colour); // Draw marker for player

		// Draw player number on marker, centering on marker
		float fontSize = 2 * markerSize;
		auto TextSize = ImGui::CalcTextSize(std::to_string(p.getNumber()).c_str());
		TextSize.x *= (fontSize / 20);
		TextSize.y *= (fontSize / 20);
		if (showNumbers) draw->AddText(ImGui::GetFont(), fontSize, ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x - TextSize.x * 0.5f, TopEdge + (pos.y / Pitch.y) * PitchSize.y - TextSize.y * 0.5f), lineColour, std::to_string(p.getNumber()).c_str());
	
		RenderHoverText(p);
	}

	// Render the ball
	void RenderBall(Ball b) {
		ImVec2 pos = ImVec2(b.getPosition().x, b.getPosition().y);
		float ballSize = markerSize + 0.25*(b.getPosition().z)+1;
		if (drawOutline) draw->AddCircle(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), ballSize + 1, IM_COL32(0, 0, 0, 255)); // Draw outline
		draw->AddCircleFilled(ImVec2(LeftEdge + (pos.x / Pitch.x) * PitchSize.x, TopEdge + (pos.y / Pitch.y) * PitchSize.y), ballSize, IM_COL32(255, 255, 255, 255)); // Draw marker for player
	}

	void Initialise(ImVec2 pitch) {
		ImVec2 WindowPos = ImGui::GetWindowPos();

		ImVec2 WindowSize = ImGui::GetWindowSize();

		// Conversions for pitch size to window size
		ImVec2 PitchToWindow = ImVec2(WindowSize.x / Pitch.x, WindowSize.y / Pitch.y);

		draw = ImGui::GetForegroundDrawList(); // Initialise draw list to be able to make draw calls

		// Offset vector from edge of window to provide slight padding
		// Also since (0,0) is in the top left where the window title bar is, not where the viewable portion is
		ImVec2 Offset = ImVec2(WindowPos.x + 5, WindowPos.y + 30);

		int xOffset = (int)Offset.x;
		int yOffset = (int)Offset.y;

		// Define edges of the renderable pitch area
		TopEdge = (int)Offset.y;
		BottomEdge = (int)Offset.y + WindowSize.y - 35;
		LeftEdge = (int)Offset.x;
		RightEdge = (int)Offset.x + WindowSize.x - 10;

		// Define corners of the renderable pitch area
		TopLeft = ImVec2(LeftEdge, TopEdge);
		TopRight = ImVec2(RightEdge, TopEdge);
		BottomLeft = ImVec2(LeftEdge, BottomEdge);
		BottomRight = ImVec2(RightEdge, BottomEdge);

		PitchSize = ImVec2(RightEdge - LeftEdge, BottomEdge - TopEdge); // Define the pitch size on the screen

		localHalfway = (LeftEdge + RightEdge) / 2; // Define the local halfway point on the screen

		Centre = ImVec2((LeftEdge + RightEdge) / 2, (TopEdge + BottomEdge) / 2); // Define the local centre of the pitch on the screen
	}

	void ChangeSettings(float _markerSize, bool _drawOutlines, bool _showNumbers) {
		markerSize = _markerSize;
		drawOutline = _drawOutlines;
		showNumbers = _showNumbers;
	}

	void SetDrawOutlines(bool b) { drawOutline = b; }
	void SetShowNumbers(bool b) { showNumbers = b; }
	ImDrawList* GetDrawList() { return draw; }

	/*Renderer(ImVec2 pitch = ImVec2(120, 90), float marker = 5.0f) : Pitch(pitch) {
		markerSize = marker;
	}*/

	void SetDraw(ImDrawList* d) { draw = d; }

	Renderer(glm::vec2 pitch = glm::vec2(120, 90), float marker = 5.0f) : Pitch(pitch.x, pitch.y) {
		markerSize = marker;

		ChangeSettings(marker, true, true);

		//Initialise(ImVec2(pitch.x, pitch.y));
	}
private:
	ImVec2 windowSize;
	ImVec2 Pitch = ImVec2(120, 90); // Dimensions of pitch in metres for use in internal calculations

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

	ImDrawList* draw;

	int localHalfway; // Define the local halfway point on the screen

	ImVec2 Centre; // Define the local centre of the pitch on the screen

	PitchInfo pitchInfo = PitchInfo(TopLeft, BottomRight, Pitch, PitchSize);

	float markerSize;
	ImU32 grassColour = IM_COL32(23, 141, 23, 255);
	ImU32 lineColour = IM_COL32(255, 255, 255, 255);
	bool drawOutline;
	bool showNumbers;

	// Render the hover text for a player that shows their name
	void RenderHoverText(PlayerInSimulation p) {
		if (ImGui::IsMouseHoveringRect(ImVec2(LeftEdge + (p.getPosition().x / Pitch.x) * PitchSize.x - markerSize, TopEdge + (p.getPosition().y / Pitch.y) * PitchSize.y - markerSize),
			ImVec2(LeftEdge + (p.getPosition().x / Pitch.x) * PitchSize.x + markerSize, TopEdge + (p.getPosition().y / Pitch.y) * PitchSize.y + markerSize))) { // Check if mouse is in bounding box around player marker
			auto TextSize = ImGui::CalcTextSize(p.getName());
			draw->AddRectFilled(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), ImVec2(ImGui::GetMousePos().x + TextSize.x, ImGui::GetMousePos().y - 20 + TextSize.y), IM_COL32(0, 0, 0, 100), 1.0f); // Draw transluscent background for text
			draw->AddText(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y - 20), lineColour, p.getName()); // Draw text above the cursor
		}
	}
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
		score1 = 0;
		score2 = 0;

		SimulatorWindow = NULL; // THIS HAS TO BE SET FROM THE UI SIDE

		Initialise();
	}

	// Render the simulation contents
	// TODO
	void Render() {
		for (PlayerInSimulation p : team1) { renderer.RenderPlayerMarker(p, IM_COL32(255, 0, 0, 255)); }

		for (PlayerInSimulation p : team2) { renderer.RenderPlayerMarker(p, IM_COL32(3, 202, 252, 255)); }

		renderer.RenderBall(ball);
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
	int getTeam1Score() { return score1; } // Public getters for team scores
	int getTeam2Score() { return score2; }

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

	Renderer getRenderer() { return renderer; }
	void setRenderer(Renderer r) { renderer = r; }

	void SetWindow(ImDrawList* draw) { SimulatorWindow = draw; renderer.SetDraw(SimulatorWindow); }

	void ChangeRenderSettings(float markerSize, bool drawOutline, bool showNumbers) {
		renderer.ChangeSettings(markerSize, drawOutline, showNumbers);
	}

	// TODO
	// Initialise the simulation (might be the same as Begin)
	void Initialise() {
		SetupTeams();
		ball.Reset();
	}

	void SetupTeams() {
		team1.clear();
		team2.clear();
		//team1.push_back(PlayerInSimulation(glm::vec2(40, 45), 8, "Test 1", PlayerRole::CentreMid, PlayerWidth::FreeRoam, Mentality::Support));
		//team2.push_back(PlayerInSimulation(glm::vec2(80, 45), 8, "Test 2", PlayerRole::CentreMid, PlayerWidth::FreeRoam, Mentality::Support));

		//team1.push_back(PlayerInSimulation(glm::vec2(X,Y), NUM, NAME, PlayerRole::ROLE, PlayerWidth::WIDTH, Mentality::MENTALITY);

		team1.push_back(PlayerInSimulation(glm::vec2(7, 45), 1, "David de Gea", PlayerRole::Goalkeeper, PlayerWidth::Central, Mentality::Defence));
		team1.push_back(PlayerInSimulation(glm::vec2(20, 10), 12, "Tyrell Malacia", PlayerRole::FullBack, PlayerWidth::WideLeft, Mentality::Support));
		team1.push_back(PlayerInSimulation(glm::vec2(20, 35), 6, "Lisandro Martinez", PlayerRole::CentreBack, PlayerWidth::CentreLeft, Mentality::Defence));
		team1.push_back(PlayerInSimulation(glm::vec2(20, 55), 19, "Raphael Varane", PlayerRole::CentreBack, PlayerWidth::CentreRight, Mentality::Defence));
		team1.push_back(PlayerInSimulation(glm::vec2(20, 80), 20, "Diogo Dalot", PlayerRole::FullBack, PlayerWidth::WideRight, Mentality::Support));
		
		team1.push_back(PlayerInSimulation(glm::vec2(37, 35), 14, "Christian Eriksen", PlayerRole::CentreMid, PlayerWidth::CentreLeft, Mentality::Support));
		team1.push_back(PlayerInSimulation(glm::vec2(32, 45), 18, "Casemiro", PlayerRole::DefensiveMid, PlayerWidth::Central, Mentality::Defence));
		team1.push_back(PlayerInSimulation(glm::vec2(37, 55), 8, "Bruno Fernandes", PlayerRole::AdvancedPlaymaker, PlayerWidth::CentreRight, Mentality::Support));
		
		team1.push_back(PlayerInSimulation(glm::vec2(47, 10), 25, "Jadon Sancho", PlayerRole::InsideForward, PlayerWidth::InsideLeft, Mentality::Attack));
		team1.push_back(PlayerInSimulation(glm::vec2(50, 45), 7, "Cristiano Ronaldo", PlayerRole::Striker, PlayerWidth::Central, Mentality::Attack));
		team1.push_back(PlayerInSimulation(glm::vec2(47, 80), 21, "Antony", PlayerRole::InsideForward, PlayerWidth::InsideRight, Mentality::Attack));

		//----

		team2.push_back(PlayerInSimulation(glm::vec2(120 - 7, 45), 1, "Ederson", PlayerRole::Goalkeeper, PlayerWidth::Central, Mentality::Defence));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 20, 10), 7, "Joao Cancelo", PlayerRole::FullBack, PlayerWidth::WideLeft, Mentality::Support));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 20, 35), 14, "Aymeric Laporte", PlayerRole::CentreBack, PlayerWidth::CentreLeft, Mentality::Defence));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 20, 55), 5, "Ruben Dias", PlayerRole::CentreBack, PlayerWidth::CentreRight, Mentality::Defence));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 20, 80), 2, "Kyle Walker", PlayerRole::FullBack, PlayerWidth::WideRight, Mentality::Support));
		
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 37, 35), 20, "Bernardo Silva", PlayerRole::BoxToBox, PlayerWidth::CentreLeft, Mentality::Support));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 32, 45), 16, "Rodri", PlayerRole::DefensiveMid, PlayerWidth::Central, Mentality::Defence));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 37, 55), 17, "Kevin De Bruyne", PlayerRole::Playmaker, PlayerWidth::CentreRight, Mentality::Support));
		
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 47, 10), 10, "Jack Grealish", PlayerRole::InsideForward, PlayerWidth::InsideLeft, Mentality::Attack));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 50, 45), 9, "Erling Haaland", PlayerRole::Striker, PlayerWidth::Central, Mentality::Attack));
		team2.push_back(PlayerInSimulation(glm::vec2(120 - 47, 80), 26, "Riyad Mahrez", PlayerRole::InsideForward, PlayerWidth::InsideRight, Mentality::Attack));
	
		for (auto &p : team1) p.addBall(&ball);
		for (auto &p : team2) p.addBall(&ball);
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
			//ball.Kick(15, glm::vec3(1, 1, 0), 0);
			//ball.Kick(20, glm::vec3(-1, 0, 1), 0);
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

	void Team1Scored() {
		score1++;
		ball.Reset();
	}

	void Team2Scored() {
		score2++;
		ball.Reset();
	}

	void KickBall(float strength, glm::vec3 dir, float spin = 0.0f) { ball.Kick(strength, dir, spin); }

	// TODO
	// Step through the simulation - to be run on each tick
	void Step() {
		tickNumber++; // Increment tick number
		timeElapsed += ImGui::GetTime() - lastTickTime; // Increment time elapsed (to avoid elapsing time while paused / stopped)
		float matchTimeElapsedInLastTick = (ImGui::GetTime() - lastTickTime) * timeMultiplier;
		matchTimeElapsed += matchTimeElapsedInLastTick; // Increment match time elapsed (considering time multiplier)
		lastTickTime = ImGui::GetTime(); // Set last tick time to current time
		calculateMatchTime(); // Recalculate match time

		for (auto& p : team1) p.addBall(&ball);
		for (auto& p : team2) p.addBall(&ball);

		// TEST MOVEMENT
		// Move the ball in a circle, making sure movement is dependant on actual (match) time elapsed rather than just pure tick number
		//TestMovement1();
		//TestMovement2(matchTimeElapsedInLastTick);
		TestMovement3(matchTimeElapsedInLastTick);

		for (PlayerInSimulation &p : team1) { p.Step(matchTimeElapsedInLastTick); }

		for (PlayerInSimulation &p : team2) { p.Step(matchTimeElapsedInLastTick); }

		//ball.Step(matchTimeElapsedInLastTick);

		// TODO
		// DO CALCULATIONS PER TICK
		// e.g. move players, move ball, etc.
		// How does this interact with the pitch rendering?
		//		Does it return data to then be rendered? Or render it directly?

		// Check for goals
		if (ball.getPosition().x >= 120 && (ball.getPosition().y > 45 - 3.66 && ball.getPosition().y < 45 + 3.66)) { Team1Scored(); }

		if (ball.getPosition().x <= 0 && (ball.getPosition().y > 45 - 3.66 && ball.getPosition().y < 45 + 3.66)) { Team2Scored(); }

		if (ball.getPosition().x < 0) { ball.setPosition(glm::vec3(0, ball.getPosition().y, ball.getPosition().z)); ball.Stop(); }
		if (ball.getPosition().x > 120) { ball.setPosition(glm::vec3(120, ball.getPosition().y, ball.getPosition().z)); ball.Stop(); }
		if (ball.getPosition().y < 0) { ball.setPosition(glm::vec3(ball.getPosition().x, 0, ball.getPosition().z)); ball.Stop(); }
		if (ball.getPosition().y > 90) { ball.setPosition(glm::vec3(ball.getPosition().x, 90, ball.getPosition().z)); ball.Stop(); }
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
		score1 = 0;
		score2 = 0;

		ball.Reset();
		for (PlayerInSimulation &p : team1) p.resetPosition();
		for (PlayerInSimulation &p : team2) p.resetPosition();
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

	int score1 = 0;
	int score2 = 0;

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

	Renderer renderer = Renderer(PitchDim);

	ImDrawList* SimulatorWindow;
};