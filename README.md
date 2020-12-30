# DTM-Guerledan
Digital Terrain Model (Modèle Númerique de Terrain) of the lake Guerlédan.

[![Build Status](https://travis-ci.com/birromer/dtm-guerledan.svg?token=AsJ1AsSzkK7yk9VnTknt&branch=main)](https://travis-ci.com/github/birromer/dtm-guerledan)

## Folders:
`build/` is the temporary folder for building  
`include/` has the header files  
`src/` has the source files  
`data/` has the data necessary for the terrain model  
`doc/` has the generated html documentation  

## Building:
In order to build you must have the Proj library installed at `/usr/local/` path, it is used to project the coordinates in an XY system. Instructions can be found [here](https://proj.org/install.html#compilation-and-installation-from-source-code).

With the installed library, one can build the raster program with the available build script:

    ./build.sh
    
## Execution:
The `create_raster` binary will be put inside the `build/src/` folder.

The data file must be inside the `data/` folder.

It accepts the following parameters:

    # required
    -d or --data: Used to specify the path for data to be used in the terrain generation
    -s or --size: USed to specify the desired width of the generated image

    # optional
    --help: Displays a help message
    -v or --verbose: Activates debugging prints inside the program
    -h or --hillshade: Activates the hillshade algorithm in order to produce shadows in the terrain with a fake sun; WARNING: it is not working properly
    
Example with specified file, verbisity on and 800 pixels wide image:

    build/src/create_raster -v -d ./data/Guerledan_Feb19_50cm_wgs84.txt -s 800
