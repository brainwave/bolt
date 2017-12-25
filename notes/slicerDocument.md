# Information about Slicer
## Useful for making presentation

### Background
3D printing is essentially numerous precise repititions of 2 dimensional printing - each iteration adding a small amoun to the overall thickness of the object being printed. While most objects are modelled in 3 dimensions using CAD software. Fabricating these designs via most additive manufacturing technologies (including SLA printing) requires the 3 dimensional model to be converted to a 2 dimensional representation.

<Rewrite>
The process of 3D printing an object is essentially performing numerous precise reptitions of printing in 2 dimensions. Each iteration adds a small amount of thickness to the object. However, since most objects are designed in 3 dimensions via CAD software, they need to be converted to a stack of 2 dimensional representations. The printer then prints one 2d section at a time, additively building up the entire object.

The software that converts 3D CAD designs into stacks of 2D representations is called a slicer. In an FDM printer, the CAD designs are converted to lines and arcs that the tool tip (nozzle) can trace. These are stored as G codes and M codes - special instructions that make the nozzle move in a certain manner to rebuild each 2D layer of the full object. Certain SLA printers that use UV lasers also use a similar method to create the 3D component, therefore they also require G Codes. The other variety of SLA printers, that are often better and faster, use a ultraviolet projector (TI DLP chip) to display the 2D cross section of a 3D object on the photoresin. Therefore, the entire layer is solidified at one go. Both types of SLA printers, however, print the object by intermittently printing layer by layer.

Initially, Bolt 3D was developing a DLP based SLA 3D Printer. For this, we needed -

1. A mechanical setup, consisting of motors and controllers to move a build platform in various ways.
2. Driver electronics for a texas instruments DLP chip.
3. A software that converts 3D CAD designs to 2D images, suitable for display on a projector.

Mechanical setup is similar to FDM. We got through it in a breeze. Software for controlling the motors was same as FDM, and a number of free, open source ones were quickly identified and used.

The driver electronics was not developed. Partly because it was within accessible domain knowledge and partly because reference implementations were already available from Texas Instruments. Developing own driver circuit would have only driven costs down - while crucial for manufacturing, unimportant during R&D. Skipped in favour of time.
 
Software that converts 3D CAD designs into 2D images - three-four major solutions found online. However, all of them were proprietary. 3D systems, <other name>, AutoCAD, etc. AutoCAD was particularly surprising because they released everything else about the printer open source. Electronics, mechanical design, everything. Only the slicer they didn't release, and can only be accessed through their website. They even bought out a slicing company. It was considered useful to develop 3D SLA slicing software since the existing free implementations are clumsy, and the only other ones are proprietary.

### Work done so far

Some sections of the slicer development were straightforward. For instance, the reading of 3D CAD file. 3D desgins are in a number of formats, mostly proprietary. However, there are some formats that most CAD software use. STL is one of them. Its industry standard, particularly in printing, and its been around for a long time. While it has its limiations, the simplicity of it has been the reason the entire 3D printing industry uses it. Hobbyists also are well versed with it. Sites like thingieverse support only this format.

Image formats or output of the slicer was also well understood. The aspect ratio and size of images was still a hassle, but these were easily fixed by including functions that scaled the output, and thereby displaying true-to-life size of the printed object wrt to design/intent.

The rest of it was not understood. There were ample reference 'results' but the 'method' was not available despite a lot of searching. Also, there was no easily accessible affordable domain talent to be recruited - very few 3D printer software have been written in India. Virtually no one has the domain knowledge necessary.

Thereby, we had to invest resources in writing all of the slicer from scratch. Initially, a sequential, simply algorithm was used. The algorithm - 

reconstructs a 3D object from file
Divides the 3D objects into 2D sections by noting intersections of planes
Uses geometry, and math.
Key challenges - optimization, display
Key challenges - Edge cases
Key challenges - Monitoring of correctness
Key challenges - User errors
Key challenges  - portability. Written in C++

Except user errors, most were handled to satisfactory standards.

### Work to be done

UI improvement. CLI improvement. Difficulty - Easy, Priority - High
Algorithm improvement - not necessary ATM, can be done after complete product. Difficulty - Hard, Priority - Low
Correctness/Edge Cases - Difficulty - Hard. Priority - Low but volatile. Can be deferred, burden on user up until that point to ensure model is correct.
Itegration - Difficulty - Easy. Priority - Low, since rest of it is not developed.

### Examples
