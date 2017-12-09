add_executable(vehicle_detection
    vehicle_detection.cpp)

target_include_directories(vehicle_detection
    PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(vehicle_detection
    PRIVATE ${OpenCV_LIBS})
