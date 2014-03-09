#
# Copyright (C) 2012 tsl0922@sina.com
#
# (XXX:请自行在下面修改ppp版本号(PPP_VERSION)以调整插件安装目录)
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sanxun
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PPP_VERSION:=2.4.5

include $(INCLUDE_DIR)/package.mk

define Package/sanxun
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+ppp
  TITLE:=sanxun ppp plugin
endef

define Package/sanxun/description
 sanxun ppp plugin.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS += \
	-I$(PKG_BUILD_DIR) \
	$(TARGET_CPPFLAGS)

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		all
endef

define Package/sanxun/install
	$(INSTALL_DIR) $(1)/usr/lib/ppp/$(PPP_VERSION)/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/sxplugin.so $(1)/usr/lib/ppp/$(PPP_VERSION)/sxplugin.so
endef

$(eval $(call BuildPackage,sanxun))
