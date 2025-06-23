############################################################################
# apps/testing/mm/kasantest/Makefile
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
############################################################################

include $(APPDIR)/Make.defs

# KASan validity test

MAINSRC   = kfencetest.c
PROGNAME  = kfencetest
PRIORITY  = $(CONFIG_TESTING_KFENCE_PRIORITY)
STACKSIZE = $(CONFIG_TESTING_KFENCE_STACKSIZE)

# CWE Test Sources
SRCS = kfencetest.c
SRCS += $(wildcard cwe416/*.c)
SRCS += $(wildcard cwe415/*.c)
SRCS += $(wildcard cwe122/*.c)

# Disable memory-related warnings to allow test behavior
CFLAGS += -Wno-error
CFLAGS += -Wno-use-after-free       # 允许故意使用释放后的内存（测试检测逻辑）
CFLAGS += -Wno-stringop-overflow     # 允许字符串操作越界（触发缓冲区溢出）
CFLAGS += -Wno-array-bounds          # 允许数组越界访问（测试越界检测）
CFLAGS += -Wno-free-nonheap-object   # 允许释放非堆内存（测试非法释放检测）

# 关闭优化以保留调试信息
CFLAGS += -O0
CFLAGS += -g3                        # 生成详细调试符号

include $(APPDIR)/Application.mk
