#
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ngrokc
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_REV:=a340cc5

PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_REV=a340cc5
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_URL:=https://github.com/dosgo/ngrok-c.git

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/ngrokc
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=Web Servers/Proxies
	TITLE:=Secure tunnels to localhost(C++ port)
	URL:=https://github.com/dosgo/ngrok-c
	DEPENDS:=+libc +libpthread +libpolarssl +libstdcpp
endef


define Build/Configure
	echo "#include <string.h>" >> $(PKG_BUILD_DIR)/sendmsg.h
	sed -i 's/CC=g++//g'  $(PKG_BUILD_DIR)/build.sh
	sed -i 's/libpolarssl-linux.a/-lpolarssl/g'  $(PKG_BUILD_DIR)/build.sh
endef

define Build/Compile
	cd $(PKG_BUILD_DIR) && CC=$(TARGET_CROSS)g++ ./build.sh
endef

define Package/ngrokc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/ngrokc $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ngrokc))
