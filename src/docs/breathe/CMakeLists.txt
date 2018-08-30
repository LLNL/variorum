include(ExternalProject)

message(STATUS " [+] Adding external project: breathe")
ExternalProject_Add(breathe
  GIT_REPOSITORY https://github.com/michaeljones/breathe.git
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  SOURCE_DIR ${PROJECT_BINARY_DIR}/thirdparty_builtin/breathe
  PREFIX ${PROJECT_BINARY_DIR}/thirdparty_builtin/breathe-prefix
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make SPHINXBUILD=${SPHINX_EXECUTABLE}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1
)
