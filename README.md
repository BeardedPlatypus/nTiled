<p align="center"><img src="https://github.com/BeardedPlatypus/nTiled/blob/master/nTiled_icon.png?raw=true" alt="nTiled" title="nTiled" width=12.5% /></p>
<p align="center"><img src="https://github.com/BeardedPlatypus/nTiled/blob/master/nTiled.png?raw=true" alt="nTiled" title="nTiled" width=40% /></p>

## Table of Contents
- [Overview](#overview)
- [Installation](#installation)
- [Usage](#usage)
- [Example Videos](#example-videos)
- [Documentation](#documentation)

## Overview

`nTiled` is an `openGL` renderer that implements both a forward and deferred
pipeline. It supports three light assignment algorithms:

* [Tiled Shading](https://espace.library.uq.edu.au/data/UQ_385844/tiled_shading_preprint.pdf?Expires=1516958921&Signature=d4SLzYdshwdPGifXMOgHX7iI2dSwAWGalNtQ5EXSYGgOGuraLUIIwj2VrQCScC9a0Ae2PyV1TDiZY6Kin0blRxf7Pf0CsNgTX4E9g-jVedd1YyUwrmrxexhkgqliq5igMAZUowzfpLf5U7kkC0IOWsBSOYTCOq9ejLXeeaUfpu7Wh7F02pCB0flCIqhldc~VraWhwtmHrunmrhtQxDVRt-1MCIMwfAHcGTS5Yo7tSmjPOIv4NLN-umMP7GmlQes0O4~l-qwUS2zk25IupKx-CpGj0f8Q6DJ6StOJmeQf8GqD00R0aLnJetnXf0I-V3~5kvziVnrB2jnwWYuE8z9o0g__&Key-Pair-Id=APKAJKNBJ4MJBJNC6NLQ)
* [Clustered Shading](https://pdfs.semanticscholar.org/76e6/f44d050613371082764010a0731aa1eff8a3.pdf)
* A novel algorithm called Hashed Shading, based upon [Linkless Octrees](https://pdfs.semanticscholar.org/6fe4/2c3dfbfa347956e917e9f1861ebc04375720.pdf).

`nTiled` is implemented to test the time and space complexity of said 
algorithms. It is written as part of my thesis to obtain my masters degree in 
Computer Science at the KU Leuven.  

The thesis (in dutch) can be found at [this repository](https://github.com/BeardedPlatypus/thesis-latex)  
The accompanying paper (in english) can be found at [this repository](https://github.com/BeardedPlatypus/thesis-paper)  
The datasets and produced data can be found at [this repository](https://github.com/BeardedPlatypus/thesis-data)  


## Installation

I am going to assume you have visual studio 2015 express installed and ready
to be used, if this is not the case you can obtain it free of charge
[here](https://www.visualstudio.com/downloads/)

Clone the `nTiled` repository  

        cd <desired parent directory>
        git clone https://github.com/BeardedPlatypus/nTiled.git  
      
Download the following dependencies and put them in the `<solution_director>/lib/` folder:

* openGL: [glad](https://github.com/Dav1dde/glad)  
  
    Click [this link](http://glad.dav1d.de/#profile=compatibility&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c&loader=on)
    or follow the instructions in the readme of glad to generate the
    appropriate c code and headers.  
    Place the `include` and `src` folders in `<solution_directory>/lib/GL>` .
      
* [glfw](http://www.glfw.org)  
   
    Download the 32-bit binary files from the homepage and put these under
    `<solution_directory>/lib/glfw/>`
    If a different version then 3.2.1 is used, this needs to be
    updated in the `nTiled` properties  
      
* [glm](https://github.com/g-truc/glm/releases)  
   
    Download the files and put these under `<solution_directory>/lib/glm/>`  
      
* [assimp](http://www.assimp.org)  
   
    Download the latest source files and put these in `<solution_directory>/lib/assimp/>`  
    These source files need to be compiled with visual studio 2017. 
    After compiling, make sure to put the generated binaries into
    either the `PATH` variable, or in either the `Debug` or 
    `Release` folder of `nTiled`.
      
* [rapidjson](https://github.com/miloyip/rapidjson)  
   
    Follow the installation instructions specified in the README of rapidjson  
      
* [catch](https://github.com/catchorg/Catch2)
   
    Download the `catch.hpp` header and store in 
    `<solution_directory>/lib/catch/`.  
    If you are not interested in the test suite, than this 
    dependency can be ignored.  
       
* [dear imgui](https://github.com/ocornut/imgui)
   
     Download the repository and place in
     `<solution_directory>/lib/imgui/`  

Start the solution in Visual Studio 2017. Check if all dependencies are properly
configured. Once completed, the program should compile.

## Usage

<img src="http://i.imgur.com/kYHsWii.png" alt="nTiled ui" title="nTiled ui" align="middle" height=400px />

`nTiled` can be run by running the produced executable followed by
the path to a run configuration file.

    nTiled <path_to_conf_file.json>

An example run configuration file can be found in the example folder.
This is the same file that will be run when no path is specified.

For other example files see [the data repository](https://github.com/BeardedPlatypus/thesis-data-suite)

## Documentation

The documentation of `nTiled` is hosted at [readthedocs.io](www.readthedocs.io) and can be found at [nTiled.readthedocs.io](http://ntiled.readthedocs.io/en/latest/index.html).  

