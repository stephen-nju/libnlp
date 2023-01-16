set(SOURCES strutils.c msgqueue.c thrdpool.c)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} ${INCLUDE_DIRECTORIES})

if(BUILD_SHARED_LIBS)
  # Building shared library
else()
  # Building static library
  target_compile_definitions(${PROJECT_NAME} PUBLIC "LIBNLP_STATIC")
  if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME
                                                     "libnlp_static")
  endif()
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE "LIBNLP_EXPORTS")

target_link_libraries(${PROJECT_NAME} PRIVATE project_options utf8proc)
