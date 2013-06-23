To get tests to work, you first need to have gtest downloaded on
your computer. You can download it using the following command 
(GTEST_DEST_DIR can be any path you choose).

  $ GTEST_DEST_DIR=/my/favorite/install/path/
  $ cd $GTEST_DEST_DIR
  $ wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
  $ unzip gtest-1.6.0.zip
  $ export GTEST_ROOT=GTEST_DEST_DIR/gtest-1.6.0
  $ cd $GTEST_ROOT
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ cd ~/Programming/robosub/ai/test/
  $ mkdir tests_build
  $ cd tests_build
  $ cmake ..
  $ make
  $ ./robosub_tests 

