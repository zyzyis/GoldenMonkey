Introduction
--------------------

GoldenMonkey is a ray tracer running on IBM Cell Processor using Map/Reduce programming model.

File Structure
-----------------------
- accelerator : accelerator data structure
- camera	    : camera model
- core	     : core data structure
- integrator  : color integration model
- jobber	     : queue framework for SPUs
- sampler     : view point sampling
- spu         : SPU kernel files
- util	     : utilities (e.g. threads)

How to use it
-----------------------
Use the makefile in the src folder to generate the binary file on a PlayStation 3 and run it! 
There are some examples under example folder.

More
----------------------
There is the thesis documentation under doc folder. It tells the detail of this ray tracing application.

