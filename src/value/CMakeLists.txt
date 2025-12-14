add_library(hawthorn_obj STATIC obj.c)
add_library(hawthorn_value STATIC value.c)

target_include_directories(hawthorn_obj PUBLIC "${DEFAULT_INCLUDE_DIR}")
target_include_directories(hawthorn_value PUBLIC "${DEFAULT_INCLUDE_DIR}")

target_link_libraries(hawthorn_value PRIVATE hawthorn_obj)
