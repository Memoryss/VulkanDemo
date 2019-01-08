function(build_demo demo_name)
	set(demo_dir ${CMAKE_CURRENT_SOURCE_DIR}/${demo_name})
	message(${CMAKE_CURRENT_SOURCE_DIR}/${demo_name})
	file(GLOB SOURCE *.cpp *.h ${demo_dir}/*)
	message(${FRAMEWORK_PATH})
	message(${CMAKE_SOURCE_DIR}/external)
	include_directories(${FRAMEWORK_PATH} ${EXTERNAL_PATH})
	add_executable(${demo_name} WIN32 ${SOURCE})
	target_link_libraries(${demo_name} Framework ${VULKAN_LIBRARY})
endfunction(build_demo)

function(build_demos)
	foreach(dir ${demo_dirs})
		build_demo(${dir})
	endforeach(dir)
endfunction(build_demos)

set(demo_dirs
	Point
)

build_demos()