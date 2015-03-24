#
# Makefile for sanxun ppp plugin
#
# Copyright (C) 2015 <shangjiyu@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version
# 2 of the License, or (at your option) any later version.
#

# CC=$(CROSS_COMPILE)gcc
# CFLAGS=-Os -Wall
LIBS = -lpolarssl

all:sxplugin.so sxpluginSigned.so

sxplugin.so:sxplugin.c
	$(CC) $(CFLAGS) sxplugin.c -fPIC -shared -o sxplugin.so
sxpluginSigned.so:sxpluginSigned.c
	$(CC) $(CFLAGS) sxpluginSigned.c -fPIC -shared -o sxpluginSigned.so $(LIBS)
