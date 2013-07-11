This folder contains the Golden Monkey Ray tracer developed by Si Yin <zyzyis@gmail.com>. It is the work of Si Yin's Master thesis in Vrije Universiteit Amsterdam and is the world first map/reduce based ray tracing algorithm developed on the IBM Cell Processor. 

Want to know the files?
-----------------------
/accelerator : accelerator data structure
/camera	     : camera model
/core	     : core data structure
/integrator  : color integration model
/jobber	     : queue framework for SPUs
/sampler     : view point sampling
/spu         : SPU kernel files
/util	     : utilities (e.g. threads)
/*	     : make files and main file

Want to have a try?
-----------------------
Use the makefile in the src folder to generate the binary file on a PlayStation 3 and run it! There are examples under example folder.

Want to know more?
----------------------
There is the thesis documentation under doc folder. It tells the detail of this ray tracing application.

Want to know the author?
----------------------
contact <zyzyis@gmail.com>
