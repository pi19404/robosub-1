The makefiles for this project are designed to use a cascading structure
where, if the Makefile in src is called, the Makefiles for subdirectories
are all called in turn. As long as the src/Makefile is the one that is
executed, all #include directives should find other files in the
robosub project.

A few conventions:
 1)
  The One-Makefile-to-Rule-Them-All (OMTRTA) is able to set the optimization
  level for all compiled objects. To do this, the preferred method is to
  export a shell variable name ROBOSUB_CXXFLAGS. E.g.
    $ export ROBOSUB_CXXFLAGS=-O3
  will set high optimization for all compiled files.

 2)
  If the OMTRTA needs to handle an additional file in a subfolder, (e.g.
  you have created src/decision/bad_datenight_decision.cpp), then
  you should have an accompanying src/decision/bad_datenight_decision.h,
  although this probably isn't necessary. Also, you must add
  bad_datenight_decision.o to the list of OBJS in src/decision/Makefile. No
  other additions are necessary.

 3)
  If the OMTRTA needs to handle an additional file in the src folder,
  then add a single entry to the OBJS list in src/Makefile.

 4)
  If the OMTRTA needs to handle an additional directory, then the
  new directory needs to have a Makefile (please copy the existing
  Makefile in src/decision or src/state_machine). 

