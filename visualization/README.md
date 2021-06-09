These tools provide basic visualization methods for the the vector output of the genetic algorithm code. They both use the graph
visualization package Graphviz [https://graphviz.gitlab.io](https://graphviz.gitlab.io).

Python version
==============

The example python script requires a working python installation of Graphviz. For conda users a working environment can be 

```
conda env create -f environment.yml
conda activate acse-4-p3
```

or you can (conda) install the packages yourself
```
conda install graphviz python-graphviz
```

C++ version
===========

A C++ stub is also provided, linking against the Graphviz C library. A Dockerfile provides an example ubuntu setup, a similar strategy should work on the DUG system.

On a Windows build, it will be necessary to download and build graphviz, and then modify the included `plot.vcxproj` appropriately.

Brief graphviz build instructions are

 1. Download the source code with  ` git clone --recurse-submodules https://gitlab.com/graphviz/graphviz.git`
 2. Add `%HomePath%\graphviz\windows\dependencies\graphviz-build-utilities\` to your path
 3. Start Visual Studio, open `graphviz\graphviz.sln` and build the package.

