RFID-Based Voting System using UART – ARM LPC2129

A secure and embedded voting system implemented using RFID, UART protocol, and ARM LPC2129 microcontroller. This project uses three voting switches and one display switch to count votes and show results through UART.

Project Overview
This project demonstrates an electronic voting machine using RFID authentication. Three switches are used to cast votes for three different candidates, and one switch is used to display the total vote count. UART is used to display authentication messages, voting confirmation, and results.

System Components

1. ARM LPC2129 Microcontroller
- Controls voting logic
- Reads switch inputs
- Handles UART communication

2. RFID Reader
- Reads RFID tag information
- Allows only authenticated users to vote

3. Switches (4 Total)
- Switch 1 → Vote for Candidate A
- Switch 2 → Vote for Candidate B
- Switch 3 → Vote for Candidate C
- Switch 4 → Display total vote count

4. UART Communication
Displays:
- RFID authentication status
- Vote confirmation
- Final voting results

Technologies Used
- ARM LPC2129 Microcontroller
- Embedded C
- RFID Module
- UART Serial Protocol
- Pushbutton Switches
- Keil IDE
- Flash Magic

Features
- RFID-based voter authentication
- Three-candidate voting system
- Secure vote counting
- Real-time UART display
- Simple and expandable design

How the System Works
1. Power on the system
2. Scan RFID card
3. If authenticated, voting is enabled
4. Press any of the 3 voting switches:
   - Switch 1 → Candidate A
   - Switch 2 → Candidate B
   - Switch 3 → Candidate C
5. Press Switch 4 → Display total vote count

Future Enhancements
- Add EEPROM/Flash for saving votes permanently
- Add touchscreen interface
- Add LED/Buzzer feedback
- Admin mode with password
- Multi-round voting

Author
Madhavan T
Embedded Systems Developer
