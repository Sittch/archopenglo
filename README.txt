Mitchell Allen
CSCI5229: Computer Graphics
Writeup for Final Project
2022-06-30

Roughly 112 hours have been spent on the scene. Simply entering "make all" and then "./final" will
run the program.

This is an OpenGL program that displays an archipelago scene. There are multiple desert islands
with palm trees and coconuts, and another island with a lighthouse that contains a second light
source, turned off by default. The ocean has a waving motion, reflects light, and is semi-
transparent to allow views of the objects underwater. Several items are floating in the ocean:
beach balls, a coconut, and a log. A forest of seaweed can be seen swaying under the water.
The moving fish goal ran into issues when I was unable to import the obj file I had created, but
I am very happy with the mesmerizing effect produced by the waving seaweed, as this consumed the
largest portion of my time of any feature that was successfully integrated into the final project.
There are two separate skyboxes drawn around the scene in order to more easily display the
environment above and below sea level. The scene also contains a fog effect that can be toggled,
although I was unable to split it so that the area underwater was foggier than the air. It seems
that, due to the nature of the glFog function, there is not a straightforward way to do this.
Due to implementation difficulties, the "flag" and "fish" files are unused within the final
product, but I felt that it would be beneficial to include them given the amount of time spent
on their development.

I referenced many of the class examples, especially example 24, and Paul Hoffman assisted
with the water surface.

Key Bindings
  x/X	Toggle axes display
  p/P	Toggle display mode
  f/F Toggle fog
  
  l/L	Toggle lighting
  h/H Toggle lighthouse
  m/M	Toggle light movement
  5/6	Move light manually
  ]/[	Adjust sun elevation
  
  b/B	Adjust ambient level
  v/V	Adjust diffuse level
  n/N	Adjust specular level
  y/Y	Adjust emission level
  
  F1	Toggle smooth color model
  F2	Local viewer
  F3	Toggle light orbit distance
  
  PgUp   Zoom out
  PgDn   Zoom in

  arrows Change overhead view angle
  0      Reset camera
  
  ESC    Exit