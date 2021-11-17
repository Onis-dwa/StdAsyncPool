cmake_minimum_required (VERSION 3.8)

project ("StdAsyncPool")
add_executable (StdAsyncPool "StdAsyncPool.cpp")
set_property(TARGET "StdAsyncPool" PROPERTY CXX_STANDARD 17)
