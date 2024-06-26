# CMakeList.txt for HAL
#   Build a library (`hal`) which exposes the header files as "hal/*.h"
#   Use header as: #include "hal/button.h"

include_directories(hal/include)
file(GLOB MY_SOURCES "src/*.c")

set(HOME $ENV{HOME})

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

add_library(hal STATIC ${MY_SOURCES})

target_include_directories(hal PUBLIC include)

target_link_libraries(hal LINK_PRIVATE app)


set(CMAKE_LIBRARY_PATH ${HOME}/cmpt433/public/pkgs)

target_link_libraries(hal PRIVATE
    ${HOME}/cmpt433/public/pkgs/libv4l2.so
    ${HOME}/cmpt433/public/pkgs/libv4lconvert.so
    ${HOME}/cmpt433/public/pkgs/libjpeg.so
)

# #add_executable(ultrasonic src/ultrasonic_sensor.c)

# target_link_libraries(camera2 LINK_PRIVATE hal)
# target_link_libraries(camera2 LINK_PRIVATE app)
# target_link_libraries(ultrasonic LINK_PRIVATE hal)
# target_link_libraries(ultrasonic LINK_PRIVATE app)




# Copy executable to final location (change `hello_world` to project name as needed)
# add_custom_command(TARGET ultrasonic POST_BUILD 
# COMMAND "${CMAKE_COMMAND}" -E copy 
#    "$<TARGET_FILE:ultrasonic>"
#    "~/cmpt433/public/myApps/final_project/ultrasonic" 
# COMMENT "Copying ARM executable to public NFS directory")