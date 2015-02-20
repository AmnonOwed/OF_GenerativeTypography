Generative Typography
=====================

OpenFrameworks Repository location: [https://github.com/AmnonOwed/OF_GenerativeTypography](https://github.com/AmnonOwed/OF_GenerativeTypography)

Processing Repository location: [https://github.com/AmnonOwed/CAN_GenerativeTypography](https://github.com/AmnonOwed/CAN_GenerativeTypography)

This is an openFrameworks port of the CAN_GenerativeTypography examples, originally written in Processing.
I am doing this port mainly for learning purposes, but of course I hope it may be of use to others as well.
The list of examples shows the recommended order and also shows which examples have been ported so far.

In some cases, there is no openFrameworks equivalent (~~strikethrough~~).
In some cases, there is a comparable alternative (~~strikethrough~~ NewName).
In some cases, an example is added specifically for openFrameworks **(OF)**.

Steps to run the examples:

1. Download or clone the complete repository into the **of_release\apps\** folder.
2. Use the openFrameworks projectGenerator to create a runnable project for your chosen IDE from the provided src files. Note: has to be repeated for each separate example.

The original blog post can be found [here](http://www.creativeapplications.net/processing/generative-typography-processing-tutorial).

All code examples ported by [Amnon Owed](http://vimeo.com/amnon) / [@AmnonOwed](https://twitter.com/AmnonOwed) with [OpenFrameworks 0.8.4.](http://www.openframeworks.cc/)

Special thanks to the helpful folks (drakko, jvcleave, hubris, arturo) at the [OpenFrameworks forum](http://forum.openframeworks.cc/)! :)

Recommended order:

1. Getting Started
  - [x] DisplayText
  - [ ] ~~AvailableFonts~~
  - [x] CustomFont
  - [x] ~~UsingPGraphics~~ UsingFbo
  - [x] MultiSampling **(OF)**

2. Image-Based
  - [x] InsideOutsideText
  - [x] GradientCutout
  - [x] Waves
  - [x] AggregateDrawing
  - [x] AggregateParticles
  - [x] FlowField
  - [x] FlowFieldVariant
  - [x] ReactionDiffusion

3. PDF Export
  - [x] WavesPDF
  - [x] CirclePacking
  - [x] ~~UsingGeomerative~~ StringAsPoints
  - [x] LinesCircles
  - [x] Ocean
  - [ ] VoronoiType
  - [ ] VoronoiVariant

4. 3D
  - [ ] Basic3DType
  - [ ] HemeshToPShape
  - [ ] MeshManipulation
  - [ ] Deconstructed
  - [ ] SlicedUp
  - [ ] FlowField3D
  - [ ] SpherePacking
 