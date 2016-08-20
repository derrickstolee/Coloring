This project contains a basic program for checking list-colorability of a graph.

## Compilation

Navigate to the `src/` directory and type `make`. The binaries will be loaded into the `bin/` directory.

## Execution

To test a set of graphs, we use the `lists` program and pass the graph information through `stdin` using graph6 or sparse6 strings.

To test _k_-choosability, run `lists --choosable -k [#colors]`, where `#colors` is the size of lists to use (_k_). The input is expected to be a list of grraph6 or sparse6 strings, separated by newlines.

To test _f_-choosability (choosability with lists of size _f_(_v_) on a vertex _v_), run `lists --fchoosable`. The input is expected to be a list of graph6 or sparse6 strings, followed (in a new line) by a list of integers representing _f_(_v_) for the vertices, in order.

To check reducibility with a given list of "external degrees" (or rather, computing _f_-choosability where _f_(_v_) = _k_ - ext(_v_)), run `lists --reducible -k [#colors]` and the input is a list of graph6 or sparse6 strings, followed (in a new line) by a list of integers representing ext(_v_) for the vertices, in order.

Finally, all runs can be modified to restrict to choosability with separation by specifying `-c [#]` where the number given is the maximum number of common colors in adjacent lists.

For example, one could navigate to the `bin` directory after building and type the following command:"

  `lists --reducible -k 4 -c 2 --print < ../examples/reducible-k4c2.txt`

This results in output that begins with the following for the first graph tested:

  `48 connected subgraphs`<br/>
`recursive calls: 619873`<br/>
`:Ea@_gMC is 4-reducible (with external degrees 0 2 1 2 1 2)`<br/>
`done in 0.31250 seconds.`

These examples were used during the collaboration leading to [(4,2)-choosability of planar graphs with forbidden structures](https://arxiv.org/abs/1512.03787).
