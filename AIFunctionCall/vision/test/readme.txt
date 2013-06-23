To get tests to work, you first need to have gtest downloaded on
your computer. You can download it using the following command 
(GTEST_DEST_DIR can be any path you choose).

  $ GTEST_DEST_DIR=/my/favorite/install/path/
  $ cd $GTEST_DEST_DIR
  $ wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
  $ unzip gtest-1.6.0.zip

Now, gtest is on your computer. Before you can build the vision unittests,
you need to set the GTEST_DIR environment variable. You can do this
using the following command (you might want to put a version of this in
your .bashrc).

  $ export GTEST_DIR=$GTEST_DEST_DIR/gtest-1.6.0

Now, you can build and run the vision tests as follows:

  $ cd /path/to/ai/AIFunctionCall/vision/test
  $ make
  $ ./vision_tests

