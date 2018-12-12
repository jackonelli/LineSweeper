# TENNIS LINE SWEEPER

The tennis court is all brushed off to perfection and only the clearing of the now dusty lines is left. You fetch your tool of choice and as you reach the first line you return to the age old question:
What is the optimal way of sweeping the lines?

No more! Unleash this ant colony and let them find the optimal path!

## DEPENDENCIES

### Sigma js
Graphs drawn with [sigmajs](http://sigmajs.org/), from their installation guide:
To use it, clone the repository:

```
git clone git@github.com:jacomyal/sigma.js.git
```

To build the code:

 - Install [Node.js](http://nodejs.org/).
 - Install [gjslint](https://developers.google.com/closure/utilities/docs/linter_howto?hl=en).
 - Use `npm install` to install sigma development dependencies.
 - Use `npm run build` to minify the code with [Uglify](https://github.com/mishoo/UglifyJS). The minified file `sigma.min.js` will then be accessible in the `build/` folder.

### nlohmann/json
JSON for C++. Header only at: https://github.com/nlohmann/json

## TODO
 - ~~Common tools in sep file~~
 - ~~Better datastructure for paths~~
 - ~~txt to json for data~~
 - ~~Cppcheck~~
 - ~~Unit tests~~
 - ~~Cmake~~
 - ~~javascript graph~~
 - Shared lib .so or no lib at all
 - Wasm check
 - Const shit up
 - Better logging
 - Exceptions
 - Standalone C++ main function, with command line options
 - ~~Original problem verify~~
 - ~~Unsigned~~

## RESOURCES
 - Boost Graph
 - Clp https://projects.coin-or.org/Clp

## THEORY
https://pubsonline.informs.org/doi/pdf/10.1287/ited.2015.0150

## NON-THEORY
https://www.quora.com/What-is-the-quickest-way-to-sweep-the-lines-of-a-clay-tennis-court-after-a-game
