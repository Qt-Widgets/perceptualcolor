# SPDX-License-Identifier: MIT
# 
# Copyright (c) 2020 Lukas Sommer somerluk@gmail.com
#  
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#  
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#  
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE. 

cmake_minimum_required(VERSION 3.0)
project(qperceptualcolorpicker)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set(qperceptualcolorpicker_SRC
  src/main.cpp
  src/qperceptualcolorpicker.cpp
  src/qlchchromalightnessdiagramm.cpp
  src/qlchhuewheel.cpp
  src/qlchcolorwheel.cpp
  src/qpolardegreepointf.cpp
  src/helper.cpp
  src/ColorSpace.cpp
  src/Comparison.cpp
  src/Conversion.cpp
)

# Tell CMake to create the helloworld executable
add_executable(qperceptualcolorpicker ${qperceptualcolorpicker_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(qperceptualcolorpicker Qt5::Widgets)

# Install the executable
install(TARGETS qperceptualcolorpicker DESTINATION bin)
