<img src="https://github.com/BeardedPlatypus/nTiled/blob/master/nTiled_icon.png?raw=true" alt="nTiled" title="nTiled" align="middle" height=100px />

# Table of Contents
- [nTiled](#ntiled)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)

# nTiled

TODO ADD GIF HERE

`nTiled` is an `openGL` renderer that implements both a forward and deferred
pipeline. Furthermore, it supports three light assignment algorithms:

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

1. Clone the `nTiled` repository  

        cd <desired parent directory>
        git clone https://github.com/BeardedPlatypus/nTiled.git
      
2. Download the dependencies and put them at their relative path
   1. openGL: [glad](https://github.com/Dav1dde/glad)  
  
      Click [this link](http://glad.dav1d.de/#profile=compatibility&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c&loader=on)
      or follow the instructions in the readme of glad to generate the
      appropriate c code and headers.  
      Place these these files at `<solution_directory>/nTiled/lib/>` 
      such that the `include` and `src` folders are in a `GL` folder.
      
   2. [glfw](http://www.glfw.org)  
   
      Download the 32-bit binary files from the homepage and put these under
      `<solution_directory>/nTiled/lib/glfw/>`
      If a different version then 3.1.2 is used, this needs to be
      updated in the `nTiled` properties  
      
   3. [glm](https://github.com/g-truc/glm/releases)  
   
      Download the files and put these under
      `<solution_directory>/nTiled/lib/glm/>`  
      
   4. [assimp](http://www.assimp.org)  
   
      Download the files from the homepage and put these under
      `<solution_directory>/nTiled/lib/assimp/>`  
      After compiling, make sure to put the generated binaries into
      either the `PATH` variable, or in either the `Debug` or 
      `Release` folder of `nTiled`.
      
   5. [rapidjson](https://github.com/miloyip/rapidjson)  
   
      Follow the installation instructions on the README of rapidjson  
      
   6. [catch](https://github.com/catchorg/Catch2)
   
       Download the `catch.hpp` header and store in 
       `<solution_directory>/nTiled/lib/catch/`
   7. [dear imgui](https://github.com/ocornut/imgui)
   
       Download the repository and place in
       `<solution_directory>/nTiled/lib/imgui/`

3. You should now be able to start the solution from inside visual studio
   and it should compile.  


## Usage

<img src="http://i.imgur.com/kYHsWii.png" alt="nTiled ui" title="nTiled ui" align="middle" height=600px />

`nTiled` can be run by running the produced executable followed by
the path to a run configuration file.

    nTiled <path_to_conf_file.json>

An example run configuration file can be found in the example folder.
This is the same file that will be run when no path is specified.

For other example files see [the data repository](https://github.com/BeardedPlatypus/thesis-data-suite)
For a complete overview of the configuration options see :ref: conf_spec


## Documentation

The documentation of `nTiled` is hosted at [readthedocs.io](www.readthedocs.io) and can be found at [nTiled.readthedocs.io](http://ntiled.readthedocs.io/en/latest/index.html).
