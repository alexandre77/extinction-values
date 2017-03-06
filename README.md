# extinction-values
Extinction values for image processing

A gray-level image can be interpreted as a topographical surface, and represented by a component tree, based on the inclusion relation of connected components obtained by threshold decomposition. Relations between plateaus, valleys or mountains of this relief are useful in computer vision systems. An important definition to characterize the topographical surface is the dynamics, introduced by Grimaud (1992), associated to each regional minimum. This concept has been extended, by Vachier and Meyer (1995), by the definition of extinction values associated to each extremum of the image. This paper proposes four new extinction values -- two based on the topology of the component tree: (i) number of descendants and (ii) sub-tree height; and two geometric: (iii) height and (iv) width of a level component bounding box. This paper describes efficient computation of these extinction values based on the incremental determination of attributes from the component tree construction in quasi-linear time, compares the computation time of the method and illustrates the usefulness of these new extinction values from real examples.

Install on:
    Linux-based systems:
        $ make
    Windows:
        $ cl -DWINDOWS -EHsc -o extinction.exe main.cpp extinctions.cpp gettime.cpp lista.cpp maxtree.cpp mm.cpp

Usage:
    extinction option image_in.pgm image_out.pgm [threshold]

Option:
    height
    area
    volume
    desc
    htop
    hbbox
    wbbox

Example:
    $ ./extinction desc tomatoes.pgm result.pgm 300
