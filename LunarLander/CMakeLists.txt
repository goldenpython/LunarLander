cmake_minimum_required(VERSION 2.6)
PROJECT(LunarLander)

SET(IRRLICHT_DIR "c:/irrlicht-1.8.1" CACHE PATH "Irrlicht's location")

INCLUDE_DIRECTORIES(
   "${IRRLICHT_DIR}/include"
   ${PROJECT_SOURCE_DIR}/include
)

FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS src/*.h)


ADD_EXECUTABLE(LunarLander
   ${SRCS}
   ${HDRS}
)

if (WIN32)
	if (MSVC)
		set(COMPILER "VisualStudio")
	endif (MSVC)

	if (("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32") OR (${CMAKE_SIZEOF_VOID_P} EQUAL "4"))
		set(PLATFORM "Win32")
	elseif (("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64") OR (${CMAKE_SIZEOF_VOID_P} EQUAL "8"))
		set(PLATFORM "Win64")
	else ()
		message("???")
	endif ()

	set (IRRLICHT_BINDIR_NAME "${PLATFORM}-${COMPILER}")

	# message(${CMAKE_SIZEOF_VOID_P} "=" ${CMAKE_GENERATOR_TOOLSET}  "="      ${CMAKE_GENERATOR_PLATFORM}  "="   ${CMAKE_VS_PLATFORM_NAME}   "="        ${PLATFORM}   "=" ${IRRLICHT_BINDIR_NAME})

	TARGET_LINK_LIBRARIES(LunarLander
		"${IRRLICHT_DIR}/lib/${IRRLICHT_BINDIR_NAME}/Irrlicht.lib"
	)

	set (IRRLICHT_DLL_FULL_PATH "${IRRLICHT_DIR}/bin/${IRRLICHT_BINDIR_NAME}/irrlicht.dll")


	STRING(REGEX REPLACE "/" "\\\\" srcFile "${IRRLICHT_DLL_FULL_PATH}")
	STRING(REGEX REPLACE "/" "\\\\" dstFile "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")

	ADD_CUSTOM_COMMAND(
		TARGET LunarLander POST_BUILD 
		COMMAND copy ${srcFile} ${dstFile}
		COMMENT "copying irrlicht dll. " 
	)
endif (WIN32)
