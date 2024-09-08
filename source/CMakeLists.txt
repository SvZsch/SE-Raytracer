add_executable(example example.cpp)

target_link_libraries(example ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(example glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS example DESTINATION .)

add_executable(raytracer raytracer.cpp)

target_link_libraries(raytracer ${FRAMEWORK_NAME} ${LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
  )
add_dependencies(raytracer glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS raytracer DESTINATION .)

# Build tests
add_executable(tests
  tests.cpp)
target_link_libraries(tests ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(tests glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

add_executable(generate_sdf_frames
  generate_sdf_frames.cpp)
target_link_libraries(tests ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(tests glfw ${FRAMEWORK_NAME} ${COPY_BINARY})
 

