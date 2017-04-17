# my_kilo

This is my version of the `kilo` editor, as [detailed at
viewsourcecode.org/snaptoken/kilo](http://viewsourcecode.org/snaptoken/kilo/index.html), originally from [antirez](http://antirez.com/news/108).

I'm creating my own version partly as an exercise of my C skills, but
also as a refactoring exercise. I don't necessarily intend this as a
criticizm of Snaptoken's or antirez's code. They each had their own goals in
mind in creating their project, and they can say for themselves how well they
achieved their desired end result.

The original codebase was based on a couple of restrictions that I plan to
stick to as well:

* Implement the code using the C99 language (and specifically, the gcc variant of it).
* Do not use `ncurses` or any other third party libraries

I feel like it probably would be good to convert the code to use `ncurses`,
but I'm not familiar with the library, and I feel like that would bog me down
too much from what I really want to do, which is refactor/reimplement `kilo`.

The original codebase also has at least one restriction I'm not going to
adhere to:

* Snaptoken's `kilo` was implemented in a single source file, I'm going to use more. Partly this is because of how I plan to deal with string constants, and partly just because even for a small project like this, splitting the code up into separate files makes it easier to separate the code into logically related groups.

* I'm going to use my own stylistic preferences for a lot of things. For instance, using typedef names for structs, and using underscores to separate names instead of camel case.