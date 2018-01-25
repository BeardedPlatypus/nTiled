<img src="https://github.com/BeardedPlatypus/nTiled/blob/master/nTiled_icon.png?raw=true" alt="nTiled" title="nTiled" align="middle" height=100px />

# Table of Contents
- [nTiled](#ntiled)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)

# `nTiled`

[![nTiled Banner](https://gifs.com/embed/ntiledbanner-0VE67L)]()

`nTiled` is an `openGL` viewer that implements tiled and clustered shading as
well as a novel technique named hierarchical shading. It is written to evaulate
the time and space complexity of said algorithms.  

`nTiled` is written as part of my thesis in order to obtain my masters in
computer science at KU Leuven.  

The thesis text can be found at [this repository](https://github.com/BeardedPlatypus/thesis-latex)  
The datasets and produced data can be found at [this repository](https://github.com/BeardedPlatypus/thesis-data-suite)  


# Installation

I am going to assume you have visual studio 2015 express installed and ready
to be used, if this is not the case you can obtain it free of charge
[here](https://www.visualstudio.com/downloads/)

1. Clone the `nTiled` repository  

        cd <desired parent directory>
        git clone https://github.com/BeardedPlatypus/nTiled.git
      
2. Download the dependencies and put them at their relative path
   1. openGL: [glad](https://github.com/Dav1dde/glad)  
   
      Follow the instructions in the readme of glad to generate the
      appropriate c code and headers.  
      Place these these files at `<solution_directory>/nTiled/lib/>`   
      
   2. [glfw](http://www.glfw.org)  
   
      Download the files from the homepage and put these under
      `<solution_directory>/nTiled/lib/glfw/>`
      If a different version then 3.1.2 is used this needs to be
      updated in the `nTiled` properties  
      
   3. [glm](https://github.com/g-truc/glm/releases)  
   
      Download the files and put these under
      `<solution_directory>/nTiled/lib/glm/>`  
      
   4. [assimp](http://www.assimp.org)  
   
      Download the files from the homepage and put these under
      `<solution_directory>/nTiled/lib/assimp/>`  
      
   5. [rapidjson](https://github.com/miloyip/rapidjson)  
   
      Follow the installation instructions on the README of rapidjson  

3. You should now be able to start the solution from inside visual studio
   and it should compile.  


# Usage

<img src="http://i.imgur.com/kYHsWii.png" alt="nTiled ui" title="nTiled ui" align="middle" height=600px />

`nTiled` can be run by running the produced executable followed by
the path to a run configuration file.

    nTiled <path_to_conf_file.json>

An example run configuration file can be found in the example folder.
This is the same file that will be run when no path is specified.

For other example files see [the data repository](https://github.com/BeardedPlatypus/thesis-data-suite)
For a complete overview of the configuration options see :ref: conf_spec


# Documentation

The documentation of `nTiled` is hosted at [readthedocs.io](www.readthedocs.io) and can be found at [nTiled.readthedocs.io](http://ntiled.readthedocs.io/en/latest/index.html).
