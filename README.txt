COMP2215_1920_Qing Pan
Project: Floppy Bird

Implementation of a game on the LaFortuna, including a demo.

The game shows the title frame first, if there is no interaction
for four seconds, the demo frame will start showing how to play
the game by using bot code.

Interaction is done by 4 directions on the encoder, mainly for
controlling the movement of the bird.

In the main frame, bird should be controlled by user, preventing from
dash into barriers. The bird also has 3 lifes in total, which also shows
in the main frame. Whenever it loses one life, the game will pause for
three seconds.

- Peter Danneger's code (adapted by Klaus-Peter Zauner) for the rotary encoder
- Steve Gunn's code (adapted by Klaus-Peter Zauner) for the lcd
- Nikola Bozhinov's code for the main controlling part

