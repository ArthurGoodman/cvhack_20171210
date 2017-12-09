add_executable(video_player
    video_player.cpp)

target_include_directories(video_player
    PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(video_player
    PRIVATE ${OpenCV_LIBS})
