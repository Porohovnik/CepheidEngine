cmake_minimum_required(VERSION 3.13)

project(CepheidEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

#=======================================================================================================================
# Output file-shaders
#=======================================================================================================================
set(SHADERS_PATHS "shaders" "shaders_noBindless")

foreach(X IN LISTS SHADERS_PATHS)
    file(GLOB SHADERS "${X}/*.vert" "${X}/*.frag")
    foreach(SHADER ${SHADERS})
        get_filename_component(NAME_SHADER ${SHADER} NAME)
        configure_file(${SHADER} ${CMAKE_BINARY_DIR}/${X}/${NAME_SHADER} COPIONLY)
    endforeach(SHADER)
endforeach()


#=======================================================================================================================
# Add build options
#=======================================================================================================================
option(CEPHEID_ENGINE_BUILD_OBJECT_LIB "Build object library (not available in Xcode)" OFF)
option(CEPHEID_ENGINE_BUILD_STATIC_LIB "Build static library                         " ON)
option(CEPHEID_ENGINE_BUILD_SHARED_LIB "Build shared library                         " OFF)

if(DEFINED BUILD_SHARED_LIBS)
    set(MYLIB_SHARED_LIBS ${BUILD_SHARED_LIBS})
endif()

#=======================================================================================================================
# PROJECT FILES
#   List of project source files
#=======================================================================================================================
file(GLOB_RECURSE PUBLIC_HEADERS_FILES  RELATIVE ${PROJECT_SOURCE_DIR} "include/*.h")
file(GLOB_RECURSE SOURCE_LIB            RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")

#=======================================================================================================================
# LIBS CHOOSING TYPES
#=======================================================================================================================
if (${CEPHEID_ENGINE_BUILD_OBJECT_LIB} )
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-object)
    add_library(${PROJECT_NAME}-object OBJECT)
endif()

if (${CEPHEID_ENGINE_BUILD_STATIC_LIB})
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-static)
    add_library(${PROJECT_NAME}-static STATIC)
endif()

if (${CEPHEID_ENGINE_BUILD_SHARED_LIB})
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-shared)
    add_library(${PROJECT_NAME}-shared SHARED)
endif()
#=======================================================================================================================
# DEFINE LIBS
#=======================================================================================================================
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/GL_layer )
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/WIN_layer)

add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/glm )
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/freetype)
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/json)
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/pugixml)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -fPIC -std=c++20 -lXinerama -lXrandr -lXi -lXcursor -lpthread -lGLU  -lrt -lXrandr  -lXi -lXinerama -lX11  ")
endif (UNIX)

#=======================================================================================================================
# DEFINE FILE
#=======================================================================================================================
foreach(NAME IN LISTS LIBRARY_TYPE_NAME)
    target_sources(${NAME} PRIVATE ${PUBLIC_HEADERS_FILES} ${SOURCE_LIB})
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/GL_layer/include)
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/WIN_layer/include)

    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/Tuple-utilities-template-library)

    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/glm)
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/freetype/include)
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/stb_image)
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/json/include)
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/pugixml/srs)
    
    target_link_libraries(${NAME} PUBLIC
    gl_layer-static
    win_layer-static
    glm
    freetype
    nlohmann_json
    pugixml
    )
endforeach()

