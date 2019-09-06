# Copyright 2017, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# project
project ( wilton_systemd CXX )

set ( ${PROJECT_NAME}_DEPS
        staticlib_json
        staticlib_utils
        systemd )
        
staticlib_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# executable
add_executable ( ${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/src/wiltoncall_systemd.cpp )
        
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${WILTON_DIR}/core/include
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} )
        
target_link_libraries ( ${PROJECT_NAME} PRIVATE
        wilton_core
        wilton_logging
        ${${PROJECT_NAME}_DEPS_PC_LIBRARIES} )
