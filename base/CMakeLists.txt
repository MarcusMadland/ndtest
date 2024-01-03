# Sources
file(
	GLOB_RECURSE
	BASE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/**.h #
	${CMAKE_CURRENT_SOURCE_DIR}/include/**.inl #
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp #
)

# Combine the sources
set(ALL_SOURCES ${BASE_SOURCES} ${PLATFORM_SOURCES})

# Solution Filters
foreach(_source IN ITEMS ${ALL_SOURCES})
	if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

# Exclude files from the build but keep them in project
file(GLOB_RECURSE BASE_NOBUILD "${CMAKE_CURRENT_SOURCE_DIR}/src/amalgamated.*")
foreach(BASE_SRC ${BASE_NOBUILD})
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR} PROPERTIES HEADER_FILE_ONLY ON)
endforeach()

add_library(base STATIC ${ALL_SOURCES})

# Build system specific configurations
if(MINGW)
	set(BASE_COMPAT_PLATFORM mingw)
elseif(WIN32)
	set(BASE_COMPAT_PLATFORM msvc)
elseif(APPLE) # APPLE is technically UNIX... ORDERING MATTERS!
	set(BASE_COMPAT_PLATFORM osx)
elseif(UNIX)
	set(BASE_COMPAT_PLATFORM linux)
endif()

# Includes
target_include_directories(
	base
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include #
	${CMAKE_CURRENT_SOURCE_DIR}/include/compat/${BASE_COMPAT_PLATFORM} #
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty #
)

# All configurations
target_compile_definitions(base PUBLIC "BASE_CONFIG_DEBUG=$<IF:$<CONFIG:Debug>,1,$<BOOL:${BASE_CONFIG_DEBUG}>>")
target_compile_definitions(base PUBLIC "__STDC_LIMIT_MACROS")
target_compile_definitions(base PUBLIC "__STDC_FORMAT_MACROS")
target_compile_definitions(base PUBLIC "__STDC_CONSTANT_MACROS")

# Special Visual Studio Flags
if(MSVC)
	target_compile_definitions(base PUBLIC "_CRT_SECURE_NO_WARNINGS")
endif()

target_compile_features(base PUBLIC cxx_std_14)
target_compile_options(base PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)

# Link against psapi on Windows
if(WIN32)
	target_link_libraries(base PUBLIC psapi user32)
endif()

# Additional dependencies on Unix
if(ANDROID)
	# For __android_log_write
	find_library(LOG_LIBRARY log)
	mark_as_advanced(LOG_LIBRARY)
	target_link_libraries(base PUBLIC ${LOG_LIBRARY})
elseif(APPLE)
	find_library(FOUNDATION_LIBRARY Foundation)
	mark_as_advanced(FOUNDATION_LIBRARY)
	target_link_libraries(base PUBLIC ${FOUNDATION_LIBRARY})
elseif(UNIX)
	# Threads
	find_package(Threads)
	target_link_libraries(base ${CMAKE_THREAD_LIBS_INIT} dl)

	# Real time (for clock_gettime)
	target_link_libraries(base rt)
endif()