add_library(hawthorn_chunk STATIC chunk.c)

target_include_directories(hawthorn_chunk PUBLIC "${DEFAULT_INCLUDE_DIR}")

target_link_libraries(hawthorn_chunk PRIVATE megonil_array)
