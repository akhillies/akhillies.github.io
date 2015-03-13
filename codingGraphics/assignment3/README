Akhil Batra

COMPILED ON:
	Runs on Mac OSX

WHERE EVERYTHING IS:
	Main function is in ass3.cpp
	Source code is in src/
	input files of individual objects are in inputFiles/
	scene files are in scenes/
	output files are in output/
	images created by program are in images/

FEATURES:
	Can read .bez and .obj files
	Has hidden line mode
	Can read in multiple files (each one holding one object) and give each a unique transformation
	Has basic phong shading
	Can activate wire frame as well as fill mode at same time
	Can output OBJ files and then read in those files (as well as others)

RESOURCES:
	OpenGL Documentation

IN PROGRAM COMMANDS:
	f = activates wire frame while keeping the filled frame
	h = activates hidden line mode
	s = switches between smooth and flat shading
	w = activates wire frame mode
	Arrow Keys = rotate the selected object (which will be highlighted) or the whole world if no objects selected
	Shift + Arrow Keys = translate selected object or world if none selected
	SPACE = quit program
	+/= = scales the selected object to be bigger (or world if no object selected)
	- = scales the selected object to be smaller (or world if no object selected)

INPUT REQUIREMENTS:
	Make sure to only have 1 object PER FILE - input can take either .bez or .obj without any extra input command, even in the combined file
	Basic command line always starts with “./as3 <INPUT_FILE>”, then follow the instructions below for each type of input file and what must be in the file
	FOR .SCENE FILES:
		For command line input, there cannot be anything else after the <INPUT_FILE>
		Inside the file, each line has to be defining its own object
		All lines will start with the file name (files must be placed in “inputFiles/“)
		Next will be the subdivision parameter (not used if .obj file)
		And finally the optional -a tag if the user wishes to have adaptive tessellation rather than uniform (only used for .bez files)
	FOR .BEZ FILES:
		Command line input must have the subdivision parameter after the <FILE_NAME> followed by an optional -a tag for adaptive tessellation versus the default uniform
		First line must have the number of patches in the objects as first set of characters
		From second line onwards, each line defines a Bezier curve, and every 4 lines defines a Bezier surface
		Curves are defined by 4 sets of vectors (with 3 values each) and must be written on the same line in order and with at least one space in between each float
		In between each Bezier surface, there must be a blank line
	FOR .OBJ FILES:
		Command line input must have the <FILE_NAME> followed by nothing else
		Commands within the file available:
			v x y z = for defining a vertex at point x, y, z
			vn x y z = for defining vertex normals with vector as x, y, z (does not have to be normalized). This normal will be assigned to the first vertex created that has no normal (vertex must be created before assigning normal)
			f v1 v2 v3 <v4> = for defining a face with the given vertices (can only take 3 vertices). v1, v2, and v3 correspond to the index of the vertex, it goes in order of vertices inputted starting from 1 (vertices must be created before assigning to a face) and the vertex indices must be written in clockwise order, and there can be 3 to 4 vertices per face
			ka r g b = ambient color of all the faces in this file, values must be between 0 and 1
			kd r g b = diffuse color for all the faces in this file, RGB values must be between 0 and 1
			ks r g b i = specular color and intensity for all the faces in this file, RGB values must be between 0 and 1