# FootballSImGui

A port of my C++ Football Simulator from https://github.com/stollaz/FootballSimCpp, made to use a GUI instead of a terminal, with the help of ImGUI (specifically Walnut, thanks to TheCherno).

### Notes
While I prefer to do my development within VSCode and building using a Makefile (due to simplicity and usually how easy it is), this project uses a build system in Visual Studio 2022 that is provided by TheCherno in his WalnutApp template (https://github.com/TheCherno/Walnut). Building should be possible by loading the whole project in VS22 and building (however I am unfamiliar with both VS22 and using git with it, so apologies if this does not work).

---

## Currently
- Mock menu layout
- Debug1 contains a test for a visual football pitch
  - Contains player markers
  - Arranged in a 4-3-3 formation, with the line heights and width customisable by the user in a Settings panel
  - Players appear as markers with their shirt number on the marker
  - Hovering over a player marker will display the player name
  - The pitch is rendered mathematically, and stretches to fill the viewport
- Debug2 contains test code for simulation
  - Simulator class exists with code for starting, pausing, unpausing and resetting the simulation
  - Steps and time elapsed are counted, along with a time muliplier and game time is counted
  - Step function exists but does no game logic
- Template player in simulation class exists
- Enum for player states in simulation exists
- Code exists for printing an event log to the screen

### TODO:
- Port all functionality from terminal-based simulator to GUI simulator, via the use of writing an event log to the screen
- Then create a new engine that incorporates player position movements, ball movements, and basic physics, all calculated at a given tickrate (maybe 20)
- Detect and animate highlights
