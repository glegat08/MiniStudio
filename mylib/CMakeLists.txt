project(mylib)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    ${SOURCE_DIR}/BehaviorTree.cpp
    ${SOURCE_DIR}/Enemy.cpp
    ${SOURCE_DIR}/EnemyBT.cpp
    ${SOURCE_DIR}/Engine.cpp
    ${SOURCE_DIR}/Game.cpp
    ${SOURCE_DIR}/GameObject.cpp
    ${SOURCE_DIR}/Hero.cpp
    ${SOURCE_DIR}/HeroState.cpp
    ${SOURCE_DIR}/ObjectBT.cpp
    ${SOURCE_DIR}/PathManager.cpp
    ${SOURCE_DIR}/SceneBase.cpp
    ${SOURCE_DIR}/SceneManager.cpp
    ${SOURCE_DIR}/StateManager.cpp
    ${SOURCE_DIR}/TextureManager.cpp
)

set(HEADERS
    ${HEADER_DIR}/BehaviorTree.h
    ${HEADER_DIR}/Enemy.h
    ${HEADER_DIR}/EnemyBT.h
    ${HEADER_DIR}/Engine.h
    ${HEADER_DIR}/Game.h
    ${HEADER_DIR}/GameObject.h
    ${HEADER_DIR}/Hero.h
    ${HEADER_DIR}/HeroState.h
    ${HEADER_DIR}/ObjectBT.h
    ${HEADER_DIR}/PathManager.h
    ${HEADER_DIR}/SceneBase.h
    ${HEADER_DIR}/SceneManager.h
    ${HEADER_DIR}/StateManager.h
    ${HEADER_DIR}/TextureManager.h
)

add_library(${PROJECT_NAME}
STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(mylib PUBLIC ${SFML_INCLUDE_DIR})
link_directories(${SFML_LIB_DIR})
target_link_libraries(${PROJECT_NAME}
PRIVATE
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-system
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${HEADER_DIR}>
)

add_custom_command(TARGET mylib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SFML_BIN_DIR} $<TARGET_FILE_DIR:mylib>
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")