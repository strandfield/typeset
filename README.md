
# `typeset` - A TeX-inspired C++ typesetting library

`libtypeset` is a C++ library that aims at providing an abstract layouting engine 
suitable for typesetting complex documents.

The library is heavily inspired by [TeX](https://en.wikipedia.org/wiki/TeX), the 
typesetting system designed by [Donald Knuth](https://www-cs-faculty.stanford.edu/~knuth/).
It currently implements many of the primitives present in the language including boxes, 
glue, kerns and penalties.


## Typesetting text

The Qt framework is used to provide some examples of typesetting text.

Albeit being extremely simple, the concepts of boxes and glue turn out to 
be also incredibly powerful.
They make text justification an easy task while also allowing particular 
spacing rules out of the box.

The following picture shows how a single line of text can be rendered on 
pages with different width.
First the line is rendered on its ideal page width, then the page width is 
increased respectively by 15% and 25%.

![Image: single line text-typesetting](https://raw.githubusercontent.com/RugessNome/liblayout/master/doc/assets/spacing.png)

Notice how the spacing rules are different after a comma or a period. 
Here, the interword space increase 50% faster after a comma and 100% 
faster after a period, hence making the space after a period almost twice 
as large as a regular space.


`libtypeset` also provides a basic implementation of TeX linebreaking algorithm.
This algorithm is used to break a paragraph of text into a series of line. 

![Animation: linebreaking](https://raw.githubusercontent.com/RugessNome/liblayout/master/doc/assets/linebreaking.gif)

Unlike a naive algorithm which would consider each line independently, 
TeX linebreaking algorithm considers the paragraph as a whole to produce 
more aesthetic results. 
Indeed, the basic strategy that consists of filling a line with as much 
text as possible and then going to the next line may produce lines 
with too few words that will look bad.
TeX tries to avoid such situations by sometimes putting fewer words than 
possible on a line to make the next lines look better. 
The whole process is in fact an optimisation problem: the algorithm tries 
to minimise a cost (in the context of TeX, this cost is called the demerit).
