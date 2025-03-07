project(mainFile)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(mainFile PUBLIC ${SFML_INCLUDE_DIR}) 
link_directories(${SFML_LIB_DIR})
target_link_libraries(${PROJECT_NAME}
PUBLIC
    mylib
PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

add_custom_command(TARGET mainFile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SFML_BIN_DIR} $<TARGET_FILE_DIR:mainFile>
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Work")