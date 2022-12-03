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
5. Control system executes the power off sequence

![[Use Case Diagram End Session.png]]
