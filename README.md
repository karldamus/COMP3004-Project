# COMP3004-Project

## Project Overview
This is the final project for COMP3004 at Carleton University.

## Team #43
- Karl Damus
- Jong Sheen Alias Gabriel Chung
- Evan Allen

[GitHub](https://github.com/karldamus/COMP3004-Project)

## Project Deliverables
- [Use Cases](https://github.com/karldamus/COMP3004-Project/blob/main/project%20deliverables/Use%20Cases.pdf)
- [Sequence Diagrams](https://github.com/karldamus/COMP3004-Project/blob/main/project%20deliverables/Sequence%20Diagrams.pdf)
- [UML Class Diagram](https://github.com/karldamus/COMP3004-Project/blob/main/project%20deliverables/UML%20Class%20Diagram.pdf)
- [Traceability Matrix](https://github.com/karldamus/COMP3004-Project/blob/main/project%20deliverables/Traceability%20Matrix.pdf)
- [Use Case Diagram](https://github.com/karldamus/COMP3004-Project/blob/main/project%20deliverables/Use%20Case%20Diagram.pdf)

## Feature List
- Power: Turn On / Turn Off + Ending a Session
- Battery level
- Select a session (pre-determined or user designated)
- Connection test
- Adjust intensity
- Record / save a session
- Replay a session

## Getting Started
1. Clone this repository
2. Launch QTCreator
3. Select Open Project
4. Select `COMP3004-Project/src/src.pro`
5. Run the program

## Who Did What?
### Evan Allen
- UI:
	- Session Group
	- Session Type
	- tDCS Numbers
	- CES Mode
	- Session Display Timer
	- User Designated Time Input
- Implementation:
	- MainWindow: selecting session groups and displaying
	- MainWindow: selecting session types and displaying
	- MainWindow: starting sessions and updating display timer
	- MainWindow: adjust intensity levels and displaying
	- MainWindow: connection test 
- Use Cases
- Class Diagram
- Sequence Diagrams

### Karl Damus
- UI:
	- Power
	- Intensity Level Display
	- Increment Intensity
- Implementation:
	- User: read from json file
	- User: write to file
	- User: create new file
	- User: other helper methods for user file
	- Session: initialization and helper methods
	- MainWindow: save session
	- MainWindow: power button helpers
	- MainWindow: load user designated session
- Use Cases
- Sequence Diagrams

### Gabriel Chung
- UI:
	- User Session list
	- Switch user interface
	- Styled buttons
- Implementation:
	- MainWindow: power on and off methods
	- MainWindow: soft off method
	- MainWindow: switch between users and default user init
	- MainWindow: recording sessions
	- MainWindow: displaying user sessions
	- MainWindow: intensity light controls
	- MainWindow and User: saving user sessions intensity
	- User: refactored code
	- Session: refactored code
- Use Cases
- Sequence Diagrams
- Class Diagram

## Tests
**Start a User Designated Length Session**
1. Hold power button, wait for device to turn on
2. Press power button 3 times to cycle to User Designated session group
3. Set User Designated session length with the scroll box
4. Use intensity arrows to select a session type
5. Press start button
6. Wait for connection test to finish
7. Wait for session to finish
8. Wait for soft off to finish
9. Device will be turned off

**Save a Session**
1. Hold power button, wait for device to turn on
2. Select user by pressing the cycle button beside users list
3. Press record button
4. Change session time in the scroll box
5. Change session type with the intensity buttons
6. Press save button

**Start a Saved Session**
1. Hold power button, wait for device to turn on
2. Select user by pressing the cycle button beside users list
3. Select a saved session
4. Press start button

## File Structure
- [main.cpp](https://github.com/karldamus/COMP3004-Project/tree/main/src/main.cpp)
- [mainwindow.cpp](https://github.com/karldamus/COMP3004-Project/tree/main/src/mainwindow.cpp)
- [mainwindow.h](https://github.com/karldamus/COMP3004-Project/tree/main/src/mainwindow.h)
- [user.cpp](https://github.com/karldamus/COMP3004-Project/tree/main/src/user.cpp)
- [user.h](https://github.com/karldamus/COMP3004-Project/tree/main/src/user.h)
- [session.cpp](https://github.com/karldamus/COMP3004-Project/tree/main/src/session.cpp)
- [session.h](https://github.com/karldamus/COMP3004-Project/tree/main/src/session.h)
- [mainwindow.ui](https://github.com/karldamus/COMP3004-Project/tree/main/src/mainwindow.ui)
- [qtresources.qrc](https://github.com/karldamus/COMP3004-Project/tree/main/src/qtresources.qrc)
- [user.json](https://github.com/karldamus/COMP3004-Project/tree/main/src/user.json)
- [user.json.example](https://github.com/karldamus/COMP3004-Project/tree/main/src/user.json.example)
- [icons/](https://github.com/karldamus/COMP3004-Project/tree/main/src/icons)
- [src.pro](https://github.com/karldamus/COMP3004-Project/tree/main/src/src.pro)
- [src.pro.user](https://github.com/karldamus/COMP3004-Project/tree/main/src/src.pro.user)
