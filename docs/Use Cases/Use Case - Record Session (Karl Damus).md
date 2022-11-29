**Use Case: Record Session**

*Primary Actor:* Device User / Device Control
*Scope:* Oasis Pro System
*Level:* Subfunction
*Stakeholders and interests:*
- User of the device - to record their therapy session and have it saved to their user profile treatment history
- Oasis Pro Device - to gather the session type, duration, and intensity level and save it to the active users treatment history

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

![[Sequence Diagram Record Session.png]]