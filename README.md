# Reverse Boggle Solving with Genetic Algorithms

Compute the best scoring Boggle board given a dictionary. A variation of the 0/1 knapsack problem. Attempting to come up with a decent heuristic solution using GAs.

Because of the restrictions we've placed on the problem, the genetic algorithm as implemented only relies on mutation and selection (not crossover). As such, it's technically an 'Evolutionary Algorithm'. The set of mutations should be more sophisticated for it to work well, though.

Sampling for mutation and selection is done using the [Alias Method](https://en.wikipedia.org/wiki/Alias_method). Constructing the Alias table requires O(k) time (for a multinomial distribution with k possible events), but sampling can be done in O(1) time. Since we have to sample k times to create a new population of size k, this reduces generation-creation complexity from O(k^2) (as sampling normally takes O(k) time) to O(k).

## Compilation

To compile the source, there's a standard Makefile, so you can just run:

```
  make
```

Note that this compiles __all__ versions of the reverse boggle solver (genetic, brute force, and greedy). You can make just the genetic version with:

```
  make genetic
```

## Usage

A sample usage that would run 1 iteration of the genetic algorithm with 100 members of the population, using the data in the `data/` directory, and no random seed:

```
  ./gnboggle 1 100 -1 data/usa2.txt
```

More generally, the usage is:

```
  ./gnboggle [iterations] [population size] [random seed, or -1 for none] [dictionary file]
```

Where the dictionary file is one word per line.

# Reverse Boggle Solving with Brute Force

Compute the best scoring Boggle board given a dictionary by brute force. Iterate through all possible permutations of boards and report the best one.

## Compilation

You can compile the brute force version the same way you compile the genetic version (both are in the `all` target):

```
  make
```

If you just want to make the brute force version, you can compile with:

```
  make bruteforce
```

## Usage

A sample usage that would evaluate 1000 permutations of the boards, using the data in the `data/` directory, and no random seed:

```
  ./bfboggle 1000 -1 data/usa2.txt
```

More generally, the usage is:

```
  ./bfboggle [maxIteration] [random seed, or -1 for none] [dictionary file]
```

Where the dictionary file is one word per line.

# Reverse Boggle Solving with a Greedy Algorithm

## Compilation

Same as above. If you just want to make the greedy version, you can compile with:

```
  make greedy
```

## Usage

```
  ./grboggle data/usa2.txt
```
