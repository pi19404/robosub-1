Note: The prompt

    robosub $

represents the root of the repository.

## Building the source code

Overview: Perform an out-of-tree cmake build, then use make to build
the code.

    robosub $ mkdir build
    robosub $ cd build
    robosub/build $ cmake -DCMAKE_BUILD_TYPE=Debug ../src
    robosub/build $ make
    robosub/build $ ./robosub --version
    robosub/build $ ./robosub

If you do NOT want to have debug logging enabled, the cmake line should be

    robosub/build $ cmake ../src -DUSE_DEBUG_LOGGING=OFF -DCMAKE_BUILD_TYPE=Debug

The build type `Debug` will produce slower code, but it will be suitable for tools
like valgrind and gdb. The `Release` build type will use -O2 optimization.


## Building the source code and the tests

Overview: This is trickier. First, we need to make sure
that gtest is correctly built. Then we need to build the testing code
using the CMakeLists.txt in the parent directory of the tests directory.
This is organized in this way so that the build is able to see both the
testing code and the project source code.

First download and build gtest.

    $ DEST_DIR=/my/favorite/install/path/
    $ cd $DEST_DIR
    $DEST_DIR $ wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
    $DEST_DIR $ unzip gtest-1.6.0.zip
    $DEST_DIR $ export GTEST_ROOT=GTEST_DEST_DIR/gtest-1.6.0
    $DEST_DIR $ cd $GTEST_ROOT
    $GTEST_ROOT $ mkdir build
    $GTEST_ROOT $ cd build
    $GTEST_ROOT/build $ cmake ..
    $GTEST_ROOT $ make

Note: At this point, GTEST_ROOT must be declared, there must be a folder named
$GTEST_ROOT/build, and you must have down an out-of-tree build from inside of
$GTEST_ROOT/build! It is convenient to place the `export GTEST_ROOT=...`
line in your ~/.bashrc file.

    $ cd $AI
    robosub $ mkdir build
    robosub $ cd build
    robosub/build $ cmake ..
    robosub/build $ make
    robosub/build $ test/robosub_tests

This process will also build the robosub executable in the src
folder.

