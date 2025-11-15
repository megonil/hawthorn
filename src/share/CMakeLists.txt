add_library(megonil_array STATIC array.c)
add_library(megonil_error STATIC error.c)
add_library(megonil_string STATIC string.c)

target_include_directories(megonil_array PUBLIC "${DEFAULT_INCLUDE_DIR}")
target_link_libraries(megonil_array PRIVATE megonil_error)
target_include_directories(megonil_error PUBLIC "${DEFAULT_INCLUDE_DIR}")
target_include_directories(megonil_string PUBLIC "${DEFAULT_INCLUDE_DIR}")
