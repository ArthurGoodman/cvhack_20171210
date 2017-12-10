add_executable(hope
    main.cpp
    settings.cpp
    video_player.cpp
    )

target_include_directories(hope
    PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(hope
    PRIVATE ${OpenCV_LIBS})
