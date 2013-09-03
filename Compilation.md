## Dependencies

This code depends on OpenCV, Boost, and gtest. This is a summary of the procedure
to install each component.

1.  OpenCV

    1. Download a .tar.gz for OpenCV from [here](http://opencv.org/downloads.html).

    2. Extract it using `tar -xvf opencv*`

    3. Perform an out-of-source build.

            $ cd opencv*
            $ mkdir build
            $ cd build
            $ cmake ..
            $ make
            $ sudo make install

2.  Boost
    This can be installed via apt-get.

        sudo apt-get install libboost-all-dev

3. gtest

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

    It is convenient to place the `export GTEST_ROOT=...` line in your ~/.bashrc
    file.


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

    $ cd robosub
    robosub $ mkdir build
    robosub $ cd build
    robosub/build $ cmake ..
    robosub/build $ make
    robosub/build $ test/robosub_tests

This process will also build the robosub executable in the src
folder.

