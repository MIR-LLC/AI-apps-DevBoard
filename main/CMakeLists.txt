set(3RDPARTY_DIR "${PROJECT_DIR}/3rdparty")

idf_component_register(SRCS "main.cpp" INCLUDE_DIRS REQUIRES "grc_cmd" "io")
