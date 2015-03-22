Akhil Batra

COMPILED ON:
	Runs on Mac OSX

WHERE EVERYTHING IS:
	Main function is in ass2.cpp
	Source code is in src/
	obj files are in objFiles/
	final image outputs are in output/images/


RESOURCES:
	got FreeImage from http://freeimage.sourceforge.net/
	updated the OSX MakeFile from https://github.com/sol-prog/FreeImage-OSX


INPUT:
	Takes in obj files with a couple catches (look at rules):

w x = sets the width of the image to be rendered
h y = sets the height of the image to be rendered

eye x y z = specifies the camera position for the screen
ll x y z = specifies the lower left point of image plane
lr x y z = specifies the lower right point of image plane
ul x y z = specifies the upper left point of image plane
ur x y z = specifies the upper right point of image plane
d n = specifies the depth for the rays - will be same for all rays in this image

v x y z = vertex at (x, y, z) -> assigned numbers 1, 2,… based on input order

f v1 v2 v3 = triangular face with vertices at index v1, v2, v3 -> counter-clockwise order and also numbered from 1 onwards based on when inputted (these indices are continued with spheres)
s x y z r = sphere with center at (x, y, z) and radius r -> indices numbered from 1 onwards (indices for spheres are continued from the indices for faces)

kd n r g b = diffuse color for the n-th object -> RGB values must be between 0 and 1
ks n r g b i = specular color and intensity for the n-th object -> rgb values must be between 0 and 1
ka n r g b = ambient color of the n-th object
kr n r g b = reflectance percentage of each rgb value

pl x y z r g b = point light -> xyz and rgb of the light
dl x y z r g b = directional light -> xyz and rgb of the light

sca n x y z = scales the n-th object by the give x y z along the respective axises
rot n x y z = specifies the DEGREES the n-th object needs to be rotated in each xyz direction