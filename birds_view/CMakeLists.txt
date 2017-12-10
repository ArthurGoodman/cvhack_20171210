add_executable(birds_view
    birds_view.cpp)

target_include_directories(birds_view
    PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(birds_view
    PRIVATE ${OpenCV_LIBS})
