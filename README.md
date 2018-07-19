# ScratchWork

Welcome to my little repository of one-off code. Whenever I get an idea and want to test it, that code winds up here. I realized I don't really work continuously on big projects outside of work, but rather enjoy the little experiments. If programming is art, these are doodles. I want to keep track of these, rather than waiting for the next hard drive failure to take them away. That means some of this code might not function correctly, might be nonsensical, or ideas that I'd never do in production, but just for fun.

## C++/NumericalLimits

Just a simple project that prints out C++ numerical limits for the primitive types

## C++/GenTrees

Based on my own [StackOverflow question](https://stackoverflow.com/q/50261225/1248889) (that never got answered), how would one generate all trees (non-binary) given a number of nodes. Inspiration came a lot from  Peter Luschny's [Permutation Trees](http://www.luschny.de/math/factorial/combi/PermutationTrees.html). I struggled with this one for a while until I derived that the number of trees is equal to the seqence 1, 2, 5, 14, 42... From that I was able to discover [Catalan numbers](https://en.wikipedia.org/wiki/Catalan_number), which greatly aided my understanding of this combinational problem. This led me to [Permutations of Parenthesis - StackOverflow](https://stackoverflow.com/a/3172190/1248889), which formed a basis of my code. After transforming the recursive solution into a stack based one, then changing parenthesis to an encoding of children in a tree of nodes, I was able to finally generate all trees of a given number of nodes.
