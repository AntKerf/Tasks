cmake_minimum_required(VERSION 3.15)
project(Tasks)

add_subdirectory(Clusterizer/)
add_subdirectory(Vectorizer/)

add_custom_target(Tasks)
add_dependencies(Tasks Clusterizer Vectorizer)

