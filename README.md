# FloppyBirdAvr

#### **Overview**
This project referenced mobile-based FloppyBird game and was developed on Lafortuna board which is powered
by AT90USB1286(an AVR microcontroller). The player uses 4-directional encoder
to control the character's movement to dodge incoming random barriers. 

#### **Game Mechanics**
This game contains several frames: title frame, main frame, restart frame, 
win frame and a demo frame. Each of those frames were bound to a fixed number
which can be used to recognize a frame or switch to another frame.

The game shows the title frame first. From there you will be able to
press on the central button to play the game. If no click occurs in four seconds, 
the title frame will switch to demo frame automatically which used bot code to show
the user how to play the game.

Interaction was done by 4 directions on the encoder, mainly for
controlling the movement of the bird and making a choice.

In the main frame, bird should be controlled by the user, preventing from
dash into barriers. Each click of the north button moves the bird 5 pixels up. 
The bird also has 3 HP in total, which also shows in the main frame. Whenever it loses one HP, the game will pause for
three seconds. Once the bird is getting closed to a barrier, a flower will pop up from the top of barrier in certain possibility.

#### **Future Improvements**
- Find a better format to arrange all elements in the main frame.
- Possibly include different levels of the game and add more types of barrier.
- An algorithm for the flower to show itself gradually rather than suddenly.

#### **Screenshots**
![image](https://github.com/qp1u18/floppyBird/blob/master/screenshot/1-start.jpg)
![image](https://github.com/qp1u18/floppyBird/blob/master/screenshot/2-intermediate%20_stage.jpg)
![image](https://github.com/qp1u18/floppyBird/blob/master/screenshot/3-restart.jpg)

#### **Reference**
- Peter Danneger's code (adapted by Klaus-Peter Zauner) for the rotary encoder
- Steve Gunn's code (adapted by Klaus-Peter Zauner) for the lcd
- Nikola Bozhinov's code for the main controlling part

