configure_file("${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
               "${CMAKE_CURRENT_BINARY_DIR}/conf.py")

add_sphinx_target(variorum_sphinx)
