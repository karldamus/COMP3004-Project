**Use Case: Selecting a Session** (Gabriel Chung)

*Primary Actor:* User
*Scope:* Oasis Pro System
*Stakeholders and interests:*
- User of the device - wants to select a session and start it
- Oasis Pro Device - wants to execute the selected session for the user

*Precondition:* the device is turned on and has sufficient battery to start a session
*Success guarantee:* user successfully selects and starts a session

*Main success scenario:*
1. User presses and releases power button to switch between the four standard session groups
2. Group icon changes
3. User presses the INT up or down buttons to highlight a session number
4. Frequency and mode icons associated with the session light up to indicate the frequency range and CES pulse type
5. User presses the select button ($\checkmark$) to start the highlighted session
6. Session number flashes and session begins after a five second delay
7. Connection test begins at the start of each session (see Use Case: Connection Test)

*Extensions:*
3.1. No sessions are available in the selected group
3.1.1. No numbers will be lit
6.1. User wants to adjust the intensity of the session (see Use Case: Adjusting Intensity)

- - -

**Use Case: Connection Test** (Gabriel Chung)

*Primary Actor:* Oasis Pro Device
*Stakeholders and interests:*
- Oasis Pro Device - wants to secure connection status before starting a session

*Precondition:* the device is turned on and a session has been selected and started
*Success guarantee:* connection test completes, confirming connection status

*Main success scenario:*
1. Device enters testing mode
2. CES Mode light starts blinking
3. Display the status of the connection on the graph
4. Wait for connection to be confirmed
5. Check if resuming or restoring a saved preference, if yes then display a Soft On animation, otherwise display a blank screen
6. Connection test ends
7. Intensity can be adjusted by the user (see Use Case: Adjusting Intensity)

*Extensions:*
3.1. The ear clips are disconnected during the session
3.1.1. Pause the session
3.1.2. Wait for the ear clips to be reconnected
3.1.3. Once reconnected, no connection will display for a couple of seconds
3.1.4. Graph scrolls up and down indicating that the unit is returning the voltage to a safe testing level, this may take up to 20 seconds

- - -

**Use Case: Adjusting Intensity** (Gabriel Chung)

*Primary Actor:* User
*Stakeholders and interests:*
- User - wants to adjust the intensity of the stimulus

*Precondition:* the device is turned on and a session has been selected
*Success guarantee:* the intensity of the session changes to what the user desires

*Main success scenario:*
1. Default intensity of a session is zero
2. User presses the up/down INT button to increase or decrease the intensity of the session
3. The corresponding light (1 to 8) lights up to show an approximate intensity level
4. The topmost lit number links to indicate the current intensity while adjusting intensity
5. Intensity of the session gets changed

*Extensions:*
3.1. Use tries to decrease below the minimum or increase above the maximum intensity level
3.1.1. Intensity does not change
3.1.2. Light displayed does not change

- - -

**Use Case: Replaying a Session** (Gabriel Chung)

*Primary Actor:* User
*Scope:* Oasis Pro System
*Stakeholders and interests:*
- User of the device - wants to replay selected treatments from history

*Precondition:* the device is powered and turned on
*Success guarantee:* user successfully replays a treatment from history

*Main success scenario:*
1. User presses the history button
2. The device displays a list of treatments from the treatment history
3. User presses the power button to cycle through the list
4. User presses the select button ($\checkmark$) to start the highlighted session
5. Device starts the selected session
6. Connection test begins (see Use Case: Connection Test)

*Extensions:*
2.1 There are no treatments in the treatment history
2.1.1 No sessions will be displayed
2.1.2 User cannot replay a session since there are no sessions
5.1 User wants to adjust intensity (see Use Case: Adjusting Intensity)

- - -

**Use Case: End Session** (Karl Damus)

*Primary Actor:* Device User
*Scope:* Oasis Pro System
*Level*: Main function
*Stakeholders and interests:*
- User of the device - to power off the Oasis Pro Device
- Oasis Pro Device - to gradually reduce the CES stimulus - soft power off the unit
- Intensity Control - reduce the level of intensity

*Success guarantee:* device powers off by gradually reducing the intensity of the CES stimulus

*Main success scenario:*
1. Control system receives a signal from device power button to power off the device
2. Control system continually and gradually tells the device to reduce the output intensity
3. Intensity control updates the intensity graph each time the intensity decreases
4. Intensity level reaches zero
5. Control system executes the power off sequence (see Use Case: Turn Off)

![[Use Case Diagram End Session.png]]
- - -

**Use Case: Record Session** (Karl Damus)

*Primary Actor:* Device User / Device Control
*Scope:* Oasis Pro System
*Level:* Subfunction
*Stakeholders and interests:*
- User of the device - to record their therapy session and have it saved to their user profile treatment history
- Oasis Pro Device - to gather the session type, duration, and intensity level and save it to the active users treatment history

*Success guarantee:* therapy session is successfully recorded and either saved to the users profile or dismissed

*Main success scenario:*
1. Control system receives signal from device button to record the therapy session
2. User initializes the therapy session
3. Control system carries out the therapy session
4. Control system prompts the user if they wish to save their recorded session upon termination of the session
5. Control system receives signal from user prompt that the user wishes to save their recorded session
6. Control system saves the session information (session type, duration, and intensity level) to the active users treatment history
7. Control system returns to the main session screen

*Variations:*
5'. Control system receives signal from user prompt that the user wishes to delete their recorded session - the control system will dismiss the recorded session and return to the main session screen  

![[Use Case Diagram - Record Session.png]]
- - -

**Use Case: Turn on the CES Device** (Evan Allen)

*Primary Actor:* User
*Scope:* Oasis Pro System
*Stakeholders and interests:* 
- User of the device - wants to put the device in a usable state

*Precondition:* battery is installed and charged and device is off
*Success guarantee:* device has been successfully turned on

*Main success scenario:*
1. User presses and holds the power button
2. Batter level is displayed on the device
3. Device is now in an operable state, allowing the user to perform other actions

- - -

**Use Case: Turn Off** (Evan Allen)

*Context of use:* Turn off the CES device
*Scope:* Oasis Pro System
*Level:* Subfunction
*Primary Actor:* User
*Stakeholders and interests:*
- User - to put the device in a non-operating state

*Precondition:* device is turned on

*Main success scenario:*
1. User presses and holds the power button for 1 second
2. Any session currently running is abruptly ended
3. Lights on device shut off

- - -

**Use Case: Create User** (Evan Allen)

*Context of use:* Create a new user to be stored on the device
*Scope:* Oasis Pro System
*Level:* Subfunction
*Primary Actor:* User
*Stakeholders and interests:*
- User - to create a new user profile for themselves
- Oasis Pro Device - to create a new user profile that allows the user to save sessions

*Precondition:* device is turned on

*Main success scenario:*
1. User enters a name for the profile in the create user text box
2. User presses create user button
3. Users profile is added to the list of user profiles
4. Device changes the current User Profile to the newly created profile

- - -

**Use Case: Change User** (Evan Allen)

*Context of use:* Change between User Profiles on the device
*Scope:* Oasis Pro System
*Level:* Subfunction
*Primary Actor:* User
*Stakeholders and interests:*
- User - change between different User Profiles
- Oasis Pro Device - change the current User Profile to a different one and changing the session history to reflect the new current User Profile

*Precondition:* device is turned on

*Main success scenario:*
1. User presses the Select User button next to a User Profile listed in the user profile section on the device
2. Device updates the Session History to reflect that of the selected user
3. Device updates the Saved Sessions to reflect that of the selected user

