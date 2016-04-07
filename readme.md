A 2D unstructured Finite Element solver for the heat equation.

![](/result.png?raw=true)

fem.exe \<case\> \<output\>
Accepts a mesh in the Gmsh 2 ASCII format.

Supports only triangle mesh elements

Only Dirichlet boundries are currently implemented

The case file format is

\<mesh path\>
\<convergence tolerance\>
\<group name\> \<bc type (d)\> \<value\>
