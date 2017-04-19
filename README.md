# Reverse Boggle Solving with Genetic Algorithms

Compute the best scoring Boggle board given a dictionary. A variation of the 0/1 knapsack problem. Attempting to come up with a decent heuristic solution using GAs.

Because of the restrictions we've placed on the problem, the genetic algorithm as implemented only relies on mutation and selection (not crossover). As such, it's technically an 'Evolutionary Algorithm'. The set of mutations should be more sophisticated for it to work well, though.

Sampling for mutation and selection is done using the [Alias Method](https://en.wikipedia.org/wiki/Alias_method). Constructing the Alias table requires O(k) time (for a multinomial distribution with k possible events), but sampling can be done in O(1) time. Since we have to sample k times to create a new population of size k, this reduces generation-creation complexity from O(k^2) (as sampling normally takes O(k) time) to O(k).

## Compilation

To compile the source, there's a standard Makefile, so you can just run:

```
  make
```

## Usage

A sample usage that would run 1 iteration of the genetic algorithm with 100 members of the population, using the data in the `data/` directory, and no random seed:

```
  ./gboggle 1 100 -1 data/usa2.txt
```

More generally, the usage is:

```
  ./gboggle [iterations] [population size] [random seed, or -1 for none] [dictionary file]
```

Where the dictionary file is one word per line.
