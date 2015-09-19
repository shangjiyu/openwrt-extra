#
# Copyright (C) 2009-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=yixun
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/yixun/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Yixun Crack
  SUBMENU:=CERNET
  URL:=https://github.com/lanhao34/yixun
  MAINTAINER:=maz-1
  DEPENDS:=+libc
endef

define Package/yixun
	$(call Package/yixun/Default)
endef

define Build/Install
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	cd $(PKG_BUILD_DIR) && $(TARGET_CROSS)gcc yixun.c -o yixun
endef

define Package/yixun/description
  Yixun Crack
endef

define Package/yixun/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/usr/share/yixun
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/yixun $(1)/usr/bin/yixun.bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/yixun.sh $(1)/usr/bin/yixun
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/yixun.cfg $(1)/etc/yixun.cfg
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/yixun.sed $(1)/usr/share/yixun/yixun.sed
endef

$(eval $(call BuildPackage,yixun))
