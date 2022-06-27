Mitchell Allen
CSCI5229: Computer Graphics
Writeup for Project Checkpoint
2022-06-27

Thi is an OpenGL program that displays an archipelago scene. There are two desert islands
with palm trees and coconuts, and a third island with a lighthouse that is drawn but not
yet fully functional. The ocean has a waving motion, reflects light, and is semi-transparent
to allow views of the objects underwater. Several items are floating in the ocean: two beach
balls, a coconut, and a log. A few strands of seaweed can be seen swaying under the water,
and I plan to add more once I am able to implement flag-waving movement on them. I would also
like to add moving fish under the water before the final presentation. There are two separate
skyboxes drawn to more easily display the environment above and below sea level. If possible,
I would like to try to split the fog effect so that the area underwater is foggier than the
air, but I have not yet figured out a way to do this. Lastly, I would like to adjust my camera
controls to permit first-person movement, or at least allow freer movement for the viewer.

I referenced many of the class examples, especially example 24, and Paul Hoffman assisted
with the water surface.

Roughly 60 hours have been spent on the scene so far.

Key Bindings
  x/X	Toggle axes display
  p/P	Toggle display mode
  
  l/L	Toggle lighting
  m/M	Toggle light movement
  5/6	Move light manually
  ]/[	Adjust light elevation
  
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