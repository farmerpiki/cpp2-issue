cmake_minimum_required(VERSION 3.26)

project(pof)

# CMake 3.25
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
# CMake 3.26
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
# CMake 3.27
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
# CMake 3.28
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "bf70d4b0-9fb7-465c-9803-34014e70d112")

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
  "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -E -x c++ <SOURCE>"
  " -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
  " -fmodules-ts -fdeps-file=<DYNDEP_FILE> -fdeps-target=<OBJECT> -fdeps-format=p1689r5"
  " -o <PREPROCESSED_SOURCE>")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "gcc")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FLAG "-fmodules-ts -fmodule-mapper=<MODULE_MAP_FILE> -fdeps-format=p1689r5 -x c++")
else()
set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "clang")
set(CMAKE_CXX_EXTENSIONS OFF)
endif()

function(cpp2_to_cpp INPUT_FILE FLAGS OUTPUT_FILE IS_MIXED)
	if(IS_MIXED)
		add_custom_command(
			OUTPUT ${OUTPUT_FILE}
			COMMAND cppfront -p ${INPUT_FILE} -o ${OUTPUT_FILE} && (echo "${INPUT_FILE} might now be a pure file!") || (
				cppfront ${FLAGS} ${INPUT_FILE} -o ${OUTPUT_FILE}
			)
			DEPENDS ${INPUT_FILE}
			COMMENT "Converting ${INPUT_FILE} to ${OUTPUT_FILE} and checking purity"
		)
	else()
		add_custom_command(
			OUTPUT ${OUTPUT_FILE}
			COMMAND cppfront ${FLAGS} ${INPUT_FILE} -o ${OUTPUT_FILE}
			DEPENDS ${INPUT_FILE}
			COMMENT "Converting ${INPUT_FILE} to ${OUTPUT_FILE}"
		)
	endif()
endfunction()

file(GLOB CPP2_FILES "*.pure.cpp2" "*.mixed.cpp2")
file(GLOB H2_FILES "*.h2")

set(GENERATED_FILES)

foreach(file ${CPP2_FILES} ${H2_FILES})
	set(FLAGS "-a" "-fo")
	set(IS_MIXED FALSE)
	if(file MATCHES ".pure.cpp2$")
		set(input_ext ".pure.cpp2")
		set(output_ext ".pure.cpp")
		list(APPEND FLAGS "-p")
	elseif(file MATCHES ".mixed.cpp2$")
		set(input_ext ".mixed.cpp2")
		set(output_ext ".mixed.cpp")
		set(IS_MIXED TRUE)
	else()
		set(input_ext ".h2")
		set(output_ext ".h")
	endif()

	string(REPLACE ${input_ext} ${output_ext} output_file ${file})
	cpp2_to_cpp(${file} "${FLAGS}" ${output_file} ${IS_MIXED})
	list(APPEND GENERATED_FILES ${output_file})
endforeach()

set(GENERATED_H_FILES "")
foreach(file ${GENERATED_FILES})
    if(file MATCHES "\\.h$")
        list(APPEND GENERATED_H_FILES ${file})
    endif()
endforeach()

foreach(file ${GENERATED_H_FILES})
    list(REMOVE_ITEM GENERATED_FILES ${file})
endforeach()

list(REMOVE_ITEM GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.pure.cpp)

find_package(Boost 1.82.0 COMPONENTS system json REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		${GENERATED_H_FILES}
	PRIVATE
		FILE_SET CXX_MODULES
		BASE_DIRS
			"${CMAKE_CURRENT_SOURCE_DIR}"
		FILES
			${GENERATED_FILES}
)

# use specific gcc version headers to have access to newer stuff in ranges
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE "-fmodules-ts")
else()
	# target_compile_options(${PROJECT_NAME} PRIVATE "--gcc-install-dir=/usr/lib/gcc/x86_64-pc-linux-gnu/13/")
	# target_link_options(${PROJECT_NAME} PRIVATE  "-L/home/g-zu/tools/gcc/lib64/" "-L/home/g-zu/tools/gcc/lib/gcc/x86_64-pc-linux-gnu/14.0.0/")
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE ~/projects/cppfront/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} fmt::fmt OpenSSL::SSL)
