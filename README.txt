####Uncube Readme#####
A PROJECT BY : codeNamuna
MUSIC BY : purple-planet.com

This is a simple c++ code for generating a 3D maze out of a 2D image map using GLUT for OpenGL.
You can find the .exe file inside the project folder.

1. The project is in code::blocks, so make sure it is setup for working with openGL. 
here is a link to the process:  http://wiki.codeblocks.org/index.php/Using_FreeGlut_with_Code::Blocks

Navigation in game:

w: move forward
s: move backward
a: move left
d: move right
f: flash
Esc: exit

The objective of the game is to find the exit(blue block) before your steps run out. The Powerups(yellow block) provide you +20 steps on stepping over them.

To create a custom maze:

1. Go to scripts folder and find the untitled2.jpg file.
2. using this as a reference, create your own map in a drawing tool like msPaint or Photoshop. 
	a. Determine the number of rows and columns you need in your map.
	b. Divide your whole image into rows *column number of blocks.
	c. A black box represents a wall.
	d. A white box is empty space.
	e. A yellow box is a power up.
	f. The red box is the starting point of the game.
	g. The blue box is the ending point in the game.
3. Run the generate_map.py python script in your computer. Make sure you have PIL installed. If not, use pip to fetch the library.
4. Now in your project folder, open the bruh.cbp file.
5. In the gameLogic.h file, set the variables ROWS and COLUMNS to the rows and columns you determined.
6. Set proper paths to your project in the:
	a. playSound() function in gameLogic.h
	b.fstream in movement_extractor.h and map_extractor.h
7. Run the code. 