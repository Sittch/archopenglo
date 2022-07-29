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
  
This is an OpenGL program that displays an archipelago scene. There are multiple desert islands
with palm trees and coconuts, and another island with a lighthouse that contains a second light
source, turned off by default. The ocean has a waving motion, reflects light, and is semi-
transparent to allow views of the objects underwater. Several items are floating in the ocean:
beach balls, a coconut, and a log. A forest of seaweed can be seen swaying under the water.
There are two separate skyboxes drawn around the scene in order to more easily display the
environment above and below sea level. The scene also contains a fog effect that can be toggled,
although I was unable to split it so that the area underwater was foggier than the air. It seems
that, due to the nature of the glFog function, there is not a straightforward way to do this.
Due to implementation difficulties, the "flag" and "fish" files are unused within the final
product.
