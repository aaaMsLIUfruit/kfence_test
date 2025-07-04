# ##############################################################################
# apps/testing/mm/kasantest/CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

if(CONFIG_TESTING_KFENCE)
  set(CFLAGS
      -Wno-error
      -Wno-use-after-free
      -Wno-array-bounds
      -Wno-free-nonheap-object
      -Wno-stringop-overflow
      "-O0"
      "-g3")
  
  file(GLOB cwe416_srcs "cwe416/*.c")
  file(GLOB cwe415_srcs "cwe415/*.c")
  file(GLOB cwe122_srcs "cwe122/*.c")
  
  nuttx_add_application(NAME kfencetest COMPILE_FLAGS ${CFLAGS} SRCS kfencetest.c ${cwe416_srcs} ${cwe415_srcs} ${cwe122_srcs})
endif()
