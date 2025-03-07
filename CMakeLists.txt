cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(AccelerateGS VERSION 0.1.0 LANGUAGES CXX C)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)

find_package (Eigen3 3.4 REQUIRED NO_MODULE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(OpenGL_GL_PREFERENCE GLVND)

set(IMGUI_PATH ../libs/imgui)

set(STB_PATH ../libs/stb)

add_subdirectory(../libs/dlib-19.24.7/dlib dlib_build)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(
	${GLFW_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${GLM_INCLUDE_PATH}
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
	../libs/nlohmann/
	${STB_PATH}
)

link_directories(${OPENGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})

add_compile_definitions("RELEASE=$<CONFIG:Release>")

add_executable(VariableFrustum 
		src/main.cpp
        src/glad/glad.h
		src/glad/glad.c
        src/Shader.h
        src/Shader.cpp
        src/Scene.h
        src/Scene.cpp
        src/ShaderProgram.h
        src/ShaderProgram.cpp
        src/TriangleMesh.h
        src/TriangleMesh.cpp
        src/Renderable.h
        src/Renderable.cpp
		src/FrustumTracker.h
		src/FrustumTracker.cpp
		src/PLYReader.h
		src/PLYReader.cpp
		${IMGUI_PATH}/imgui.h
		${IMGUI_PATH}/imgui.cpp
		${IMGUI_PATH}/imconfig.h
		${IMGUI_PATH}/imgui_demo.cpp
		${IMGUI_PATH}/imgui_draw.cpp
		${IMGUI_PATH}/imgui_internal.h
		${IMGUI_PATH}/imstb_rectpack.h
		${IMGUI_PATH}/imstb_textedit.h
		${IMGUI_PATH}/imstb_truetype.h
		${IMGUI_PATH}/imgui_tables.cpp
		${IMGUI_PATH}/imgui_widgets.cpp
		${IMGUI_PATH}/backends/imgui_impl_glfw.h
		${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
		${IMGUI_PATH}/backends/imgui_impl_opengl3.h
		${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
		../../libs/nlohmann/json.hpp
		)

target_link_libraries(VariableFrustum
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARIES} 
    ${GLEW_LIBRARIES}
	glfw
	${GLEW_LIBRARY}
	Eigen3::Eigen
	${OpenCV_LIBS}
	dlib::dlib
)

set_property(TARGET VariableFrustum PROPERTY CXX_STANDARD 17)