add_library(hawthorn_parser STATIC parser.c)
target_include_directories(hawthorn_parser PUBLIC "${DEFAULT_INCLUDE_DIR}")
target_link_libraries(hawthorn_parser PRIVATE hawthorn_value hawthorn_lexer
                                              hawthorn_chunk hawthorn_table)
