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