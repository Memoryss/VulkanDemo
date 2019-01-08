file(GLOB FRAMEWORK_SCRS "*.cpp")
file(GLOB FRAMEWORD_HEADERS "*.h")

include_directories(${EXTERNAL_PATH})
add_library(Framework STATIC ${FRAMEWORK_SCRS} ${FRAMEWORD_HEADERS})