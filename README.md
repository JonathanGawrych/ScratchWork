# ScratchWork

Welcome to my little repository of one-off code. Whenever I get an idea and want to test it, that code winds up here. I realized I don't really work continuously on big projects outside of work, but rather enjoy the little experiments. If programming is art, these are doodles. I want to keep track of these, rather than waiting for the next hard drive failure to take them away. That means some of this code might not function correctly, might be nonsensical, or ideas that I'd never do in production, but just for fun.

## C++

### NumericalLimits

Just a simple project that prints out C++ numerical limits for the primitive types.

### GenTrees

Based on my own [StackOverflow question](https://stackoverflow.com/q/50261225/1248889) (that never got answered), how would one generate all trees (non-binary) given a number of nodes. Inspiration came a lot from  Peter Luschny's [Permutation Trees](http://www.luschny.de/math/factorial/combi/PermutationTrees.html). I struggled with this one for a while until I derived that the number of trees is equal to the seqence 1, 2, 5, 14, 42... From that I was able to discover [Catalan numbers](https://en.wikipedia.org/wiki/Catalan_number), which greatly aided my understanding of this combinational problem. This led me to [Permutations of Parenthesis - StackOverflow](https://stackoverflow.com/a/3172190/1248889), which formed a basis of my code. After transforming the recursive solution into a stack based one, then changing parenthesis to an encoding of children in a tree of nodes, I was able to finally generate all trees of a given number of nodes.

### ArmDebugCompilerCrash

Minimial repro of a compiler crash that was found in a build loop. This is during multiple calls in a catch statement with default parameters of a certain size. We spent a few hours and reduced the code down to this. Why this code crashes was later discover by a guy on the MSVC compilter team:

> This is a compiler issue with branch shortening: we shorten a branch to use the 2-byte form initially but later discover that it in fact needs a longer encoding. However, the branch wasn't lengthened properly causing the compiler crash when it noticed this inconsistency.
> Since this occurs in debug mode, working around it involves restructuring the source code so that branches don't need to go past the 2-byte form (1020 bytes apart) which in this case would involve splitting the function into 2. With how the release schedule currently lines up this fix would be in a preview release some time in December and an official release early next year.
-- <cite>[Modi Mo](https://developercommunity.visualstudio.com/solutions/291627/view.html)</cite>

### CoincidentalLifetimeManagement

A small demo of what I call Coincidental Lifetime Management&trade;. This is when a developer sets up a system that has some smart pointers and some raw pointers. By mixing the two, or worse - introducing race conditions, you can build a fragile framework where slight changes to code might cause everything to crumble. This demonstrates some of those fragile issues.

### CompileTimeArraySplit

Arrays that haven't decade to their pointer type carry their length during compile time. If we wanted to modify that array length, we need to interesting casts in order to do so. This shows off a couple of compile time operations on array, such as shrinking the array by one, or spliting into two parts (with even/odd handling).

### CombinationGrowingNumber

This is slightly related to GenTrees. I wanted all combinations of three different trees, each up to N number of nodes. This is a pretty simple problem (just use 3 nested for loops). However, I wanted to start with all trees of 2 nodes or less, then 3 nodes or less, then 4 nodes or less, but this would redo work. Thus I needed a way to generate combinations of numbers of size N that excludes all combinations of N - 1. A conditional check could do it `if (a != n || b != n || c != n) continue;` could suffice, but then it would be neededlessly counting.

While the overhead might be worse than the continue in some cases, I did want to write an algorithm that could generate the combinations. The number of calls will equal M^N - (M-1)^N, as opposed to M^N. Interstingly some of these are named sequences:

* N = 2 - [A005408](https://oeis.org/A005408): [Gnomic numbers](http://mathworld.wolfram.com/GnomonicNumber.html) (1d): 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, ...
* N = 3 - [A003215](https://oeis.org/A003215): [Centered hexagonal numbers](http://mathworld.wolfram.com/HexNumber.html) (2d): 1, 7, 19, 37, 61, 91, 127, 169, 217, 271, ...
* N = 4 - [A005917](https://oeis.org/A005917): [Rhombic dodecahedral numbers](http://mathworld.wolfram.com/RhombicDodecahedralNumber.html) (3d): 1, 15, 65, 175, 369, 671, 1105, 1695, 2465, 3439, ...

All of these are [Nexus Numbers](http://mathworld.wolfram.com/NexusNumber.html) which is a subclass of [Figurate Numbers](http://mathworld.wolfram.com/FigurateNumber.html).
