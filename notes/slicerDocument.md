# Information about Slicer
## Useful for making presentation

### Background
3D printing is essentially numerous precise repititions of 2 dimensional printing - each iteration adds to the overall thickness of the object. Most objects are designed in 3 dimensions using CAD software such as Solidworks, ProE, CATIA, etc. To be printed using a 3D printer, they need to be converted to a pile of 2D representations that can then be printed one at a time. The software that performs this conversion is called a slicer. In both, FDM and Laser SLA printers, slicer output are lines and arcs. These are stored  as special instructions known as GCodes and M Codes. The tool tip - nozzle in case of FDM and laser pointer in case of Laser SLA, then retraces those geometries to create each 2D layer of the object. For this function, ample paid and free, proprietary and open source software are available. The reason is that G Codes and M Codes have long been used in CNC machines as tooling instructions. Therefore, over time, G Code slicers have matured. Another contributing factor is that both, 3D cad designs, and GCodes are essentially representation of geometries - so it is arguably easier to inter-convert between them as opposed to conversion between fundamentally different formats.

The other variety of SLA printers, often better and faster, use an ultraviolet projector (TI DLP chip). They display the 2D cross section of a 3D object on a photoresin, thereby solidifying an entire layer in one go.  For this, the 3D CAD model needs to be converted into a 2D image in a format thats supported by projection systems - such as jpeg or png.

BOLT initially worked on developing a DLP SLA printer that beat the current available solutions in areas of cost, customer support, and degree of automation. Three of the major areas of product development were -

1. Mechanical setup and controller electronics - to control movement of motors, build platform, etc.
2. Driver electronics for TI DLP UV chips - these chips form the core of UV light projection technologies.
3. A software that converts 3D CAD designs to 2D images, suitable for display on a projector, i.e. a slicer.

Mechanical setup is similar to FDM. We got through it in a breeze. Software for controlling the motors was same as FDM, and a number of free, open source ones were quickly identified and implemented. Development of driver electronics was deferred until later. Partly because it was within accessible domain knowledge and partly because reference implementations were already available from Texas Instruments. Developing own driver circuit would have only driven costs down - while crucial for manufacturing, unimportant during R&D. This was skipped for the benefit of time.
 
Software that converts 3D CAD designs into 2D images were a whole different story. There were three-four major solutions in the marketplace that could be used without a working internet connection. However, all of them were proprietary, and paid. Moreover, it is likely that industrial SLA printers use custom made expensive software. AutoCAD was the only solution available free legally. Their slicer is hosted on the cloud, meaning one has to upload an STL file, and download the sliced 2D images in a zipped folder. They are offerrring this service is because they released an open source SLA 3D printer.  We found it particularly surprising, since they open-sourced everything else about their printer - electronics and mechanical design. But the slicer wasn't released, and can only be accessed through their website. This indicated to us that a good slicer requires respectable degree of engineering, and has commercial potential since even AutoCAD chose to not release it. It is likely they are also using software on their server side that is not distributable due to licensing restrictions. It is noteworthy, that a few years prior to this release, they even bought out a company that was working on slicing software. Therefore, we considered useful to develop a DLP SLA slicing software. Since the existing free implementations are clumsy, and the only other ones are proprietary, we had to build one from scratch.

### Work done so far

Some sections of the slicer development were straightforward. For instance, the reading of 3D CAD file. 3D desgins are stored in a number of formats by the popular CAD software. Mostly these are proprietary. However, there are some formats that all of them support, and are open source. One of them is the standard template library or STL format. Its industry standard, particularly in 3D printing, and it has been around for a long time. While it has its limiations, the simplicity of it has been the reason the entire 3D printing industry prefers to use it. Hobbyists also are well versed with it. Sites like thingieverse support only this format. On the output side of things, projector compatible image formats that the slicer is expected to produce are also well understood. While aspect ratio and size of output images presented some challenges, they were fixed by including functions to scale the output in our code.  Therefore, display of true-to-life cross-sections of the object was achieved.

The next challenge, that of the conversion algorithm from 3D STL to 2D png was not so straightforward. The domain knowledge was either unavailable or inaccessible. While we found enough examples of people or companies that had done it, the 'method' was not to be found despite a lot of searching. Also, there was no easily accessible affordable domain expert to be recruited. This is largely due to the fact that  very few SLA 3D printers have been made in India. Virtually no one has the domain knowledge necessary. We had to rebuild each aspect of the algorithm from white papers, blog posts, lecture notes of professors working on computer graphics, etc. Since all of the slicer from scratch, it required significant resources. 

Initially, a sequential, simple algorithm was used. The algorithm - 

1. Reconstructs a 3D object from file
2. 'Slices' or intersects the object with numerous planes - one for each layer.
3. Creates a 2 dimensional geometric outline from the intersection.
	- At this step, G Codes and M Codes can readily be generated. However, our slicer needs to do the other steps below as well.
4. Fills the geometry with lines, using filling algorithms. These include the most challenges, since a lot of edge cases need to be handled.
5. Convert the 2D geometric filled shape into a rasterized, pixellated representation.
6. Store the pixels in a format known to projectors, such as png or jpeg.

### Result
We have a working slicer, comparable to proprietary online and offline slicing solutions in speed of operation. Additionally, and more importantly, we have a solution that we can tweak and modify to suit the needs of any printer we build. We also now understand the workings of the slicer, and can develop it into a standalone product if need be.

### Future work
Key challenges - optimization, display - The slicer does not produce very aesthetically pleasing output. This can be improved. 
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
