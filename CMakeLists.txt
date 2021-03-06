CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Boing)

ADD_COMPILE_DEFINITIONS(CE_SDL2_WINDOW_TITLE="Boing :: by Modus Create")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-implicit-exception-spec-mismatch")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-implicit-exception-spec-mismatch")
include(ProcessorCount)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/creative-engine/cmake.files)


#ADD_COMPILE_DEFINITIONS(__DINGUX__=true)
ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_SDL2_AUDIO__=true)
ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_DESKTOP_CONTROLS__=true)
ADD_COMPILE_DEFINITIONS(__MODUS_TARGET_DESKTOP_DISPLAY__=true)

# Include Creative Engine
# Exports:
# ${CREATIVE_ENGINE_INCLUDE_DIRS} - used in INCLUDE_DIRECTORIES(...)
# ${CREATIVE_ENGINE_SOURCE_FILES} - used in ADD_EXECUTABLE(...)
# ${CREATIVE_ENGINE_LINK_LIBRARIES} - used in TARGET_LINK_LIBRARIES(...)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/creative-engine/CreativeEngine.cmake)




### LDK END   ####

SET(BOING_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/resources
    ${CMAKE_SOURCE_DIR}/src/GameState
    ${CMAKE_SOURCE_DIR}/src/GameState/Playfields
    ${CMAKE_SOURCE_DIR}/src/GameState/Powerups
    ${CMAKE_SOURCE_DIR}/src/GameOverState
    ${CMAKE_SOURCE_DIR}/src/HighScoresState
    ${CMAKE_SOURCE_DIR}/src/TitleState
    ${CMAKE_SOURCE_DIR}/src/MainOptionsState
    ${CMAKE_SOURCE_DIR}/src/SplashState
    ${CMAKE_SOURCE_DIR}/src/RulesState
    ${CMAKE_SOURCE_DIR}/src/Resources.bin
    ${CMAKE_BINARY_DIR}/usr/local/include
    )

INCLUDE_DIRECTORIES(
    ${BOING_INCLUDE_DIRS}
    ${CREATIVE_ENGINE_INCLUDE_DIRS}
)

# gather Boing sources
FILE(GLOB_RECURSE BOING_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${CREATIVE_ENGINE_SOURCE_FILES}
    ${BOING_SRC}
)

ProcessorCount(N)
if (NOT N EQUAL 0)
  set(${PROJECT_NAME}_FLAGS -j${N})
  set(${PROJECT_NAME}_BUILD_FLAGS -j${N})
  set(${PROJECT_NAME}_args ${${PROJECT_NAME}_args} PARALLEL_LEVEL ${N})
endif ()

BUILD_COMMAND(
    $(MAKE) --silent
)

# make Boing.app
if (APPLE)
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
  XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES
  CODE_SIGN_INJECT_BASE_ENTITLEMENTS NO)
  INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ".")
endif (APPLE)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CREATIVE_ENGINE_LINK_LIBRARIES})
