set(RAPIDASSIST_HEADER_FILES ""
  ${CMAKE_SOURCE_DIR}/include/rapidassist/cli.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/console.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/cppencoder.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/environment.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/filesystem.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/generics.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/logger.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/random.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/strings.h
  ${CMAKE_SOURCE_DIR}/include/rapidassist/time_.h
)

if(RAPIDASSIST_BUILD_GTESTHELP)
  # add another header file
  set(RAPIDASSIST_HEADER_FILES ${RAPIDASSIST_HEADER_FILES} ${CMAKE_SOURCE_DIR}/include/rapidassist/gtesthelp.h)

  find_package(GTest)

  set(RAPIDASSIST_GTESTHELP_SOURCE_FILES gtesthelp.cpp)
endif()

add_library(rapidassist STATIC
  ${RAPIDASSIST_HEADER_FILES}
  ${RAPIDASSIST_EXPORT_HEADER}
  ${RAPIDASSIST_VERSION_HEADER}
  ${RAPIDASSIST_CONFIG_HEADER}
  ${RAPIDASSIST_GTESTHELP_SOURCE_FILES}
  console.cpp
  cli.cpp
  cppencoder.cpp
  environment.cpp
  filesystem.cpp
  logger.cpp
  random.cpp
  time_.cpp
  strings.cpp
)

# Unit test projects requires to link with pthread if also linking with gtest
if(RAPIDASSIST_BUILD_GTESTHELP)
  if(NOT WIN32)
    set(PTHREAD_LIBRARIES -pthread)
  endif()
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(rapidassist PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(rapidassist
  PUBLIC
    $<INSTALL_INTERFACE:${RAPIDASSIST_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
target_link_libraries(rapidassist PRIVATE ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES})

install(TARGETS rapidassist
        EXPORT rapidassist-targets
        ARCHIVE DESTINATION ${RAPIDASSIST_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${RAPIDASSIST_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${RAPIDASSIST_INSTALL_BIN_DIR}
)
