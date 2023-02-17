Archipelago in OpenGL<br/>
<br/>
![demo](https://github.com/Sittch/archopenglo/blob/main/demo.gif)<br/>
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
<br/>
Key Bindings<br/>
<br/>
  x/X	Toggle axes display<br/>
  p/P	Toggle display mode<br/>
  f/F Toggle fog<br/>
  <br/>
  l/L	Toggle lighting<br/>
  h/H Toggle lighthouse<br/>
  m/M	Toggle light movement<br/>
  5/6	Move light manually<br/>
  ]/[	Adjust sun elevation<br/>
  <br/>
  b/B	Adjust ambient level<br/>
  v/V	Adjust diffuse level<br/>
  n/N	Adjust specular level<br/>
  y/Y	Adjust emission level<br/>
  <br/>
  F1	Toggle smooth color model<br/>
  F2	Local viewer<br/>
  F3	Toggle light orbit distance<br/>
  <br/>
  PgUp   Zoom out<br/>
  PgDn   Zoom in<br/>
<br/>
  arrows Change overhead view angle<br/>
  0      Reset camera<br/>
  <br/>
  ESC    Exit<br/>
  <br/>
