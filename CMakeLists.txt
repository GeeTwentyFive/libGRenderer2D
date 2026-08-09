cmake_minimum_required(VERSION 3.30...4.4)
project(GRenderer2D)
set(CMAKE_CXX_EXTENSIONS OFF)

# option(GRENDERER2D_BUILD_EXAMPLE "Build example program" ON)

add_library(${PROJECT_NAME}
	"GRenderer2D.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_library(GRENDERER2D_glad
        "libs/glad/glad.c"
)
target_link_libraries(${PROJECT_NAME} PRIVATE GRENDERER2D_glad)

# if (GRENDERER2D_BUILD_EXAMPLE)
#         add_subdirectory(_EXAMPLE)
# endif()