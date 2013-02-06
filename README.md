whitespace-interpreter
======================
An interpreter written in C++ for the esoteric programming language Whitespace.

Instructions
============
Compile with ``g++ -o whitespace main.cpp -std=c++11 -fpermissive``.

The `std=c++11` flag is required because of a single use of `auto` and
we need the `-fpermissive` flag to convert a long to an enum.

Authors
=======
In alphabetical order:
* Simon Klaver
* Tim van der Meij
* Willem Obbens

License
=======
Not specified as of yet.

Official website
================
[http://compsoc.dur.ac.uk/whitespace](http://compsoc.dur.ac.uk/whitespace)
