#
# Makefile for sanxun ppp plugin
#
# Copyright (C) 2012 <tsl0922@sina.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version
# 2 of the License, or (at your option) any later version.
#

# CC=$(CROSS_COMPILE)gcc
# CFLAGS=-Os -Wall

all:sxplugin.so

sxplugin.so:
	$(CC) $(CFLAGS) sxplugin.c -fPIC -shared -o sxplugin.so
