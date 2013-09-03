set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost COMPONENTS date_time thread REQUIRED )
find_package( Threads )
include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )

add_library(decision_libs
    base_dlm.cpp driving_dlm.cpp mock_dlm.cpp
    parking_dlm.cpp path_dlm.cpp pizza_dlm.cpp speed_trap_dlm.cpp
    start_dlm.cpp tollbooth_dlm.cpp traffic_light_dlm.cpp
)

target_link_libraries(decision_libs
    movement_libs
    utility_libs 
    vision_libs 
    boost_thread
    boost_date_time 
    ${CMAKE_THREAD_LIBS_INIT}
    ${BOOST_LIBRARIES}
)

