# whack_a_mole
Arduino code for a physical whack-a-mole game I constructed

The code in the arduino folder controls the whack-a-mole game I created, which consisted of solenoid valves connected to pneumatic pistons to make the moles go up and down, and manual buttons on top of the pistons to sense when the moles had been hit. It is structured to run 30 second rounds when I press a button at the back of the booth, and be able to stop if needed when I press that button again during the round.

The code in the testing folder is largely the same as the arduino folder, but written for C++, as I needed a quick way to test my code, and preferred the Visual Studio IDE, so I made a quick simulation of the game in the console and used that to debug.
