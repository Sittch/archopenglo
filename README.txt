Mitchell Allen
CSCI5229: Computer Graphics
Writeup for Final Project
2022-06-30

This is an OpenGL program that displays an archipelago scene. There are multiple desert islands
with palm trees and coconuts, and another island with a lighthouse that is drawn but not
yet fully functional. The ocean has a waving motion, reflects light, and is semi-transparent
to allow views of the objects underwater. Several items are floating in the ocean: beach
balls, a coconut, and a log. Several strands of seaweed can be seen swaying under the water.
The moving fish goal ran into issues when I was unable to import the obj file I had created.

The lighthouse contains a rotating light. There are two separate skyboxes drawn to more easily
display the environment above and below sea level. The scene also contains a fog effect,
although I was unable to split it so that the area underwater was foggier than the air. It
seems that, due to the nature of the glFog function, there is not a straightforward way to do
this. Due to implementation difficulties, the "flag" and "fish" files are unused within the
final product, but I felt that it would be beneficial to include them given the amount of
time spent on their development.

I referenced many of the class examples, especially example 24, and Paul Hoffman assisted
with the water surface.

Roughly 112 hours have been spent on the scene.

Key Bindings
  x/X	Toggle axes display
  p/P	Toggle display mode
  
  l/L	Toggle lighting
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