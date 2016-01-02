# an CERNET h3c client for SHNU 2013 protocl version
# Copyright (C) 2014 shangjiyu@gmail.com
#
#
# This is free software, licensed under the GNU General Public License v2.
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=h3clite
PKG_VERSION:=2013
PKG_RELEASE:=20160102

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/h3clite
  SECTION:=net
  CATEGORY:=Network
  TITLE:=an CERNET SHNU h3clite client
  URL:=https://github.com/gmsj0001/shnu-h3c
  SUBMENU:=CERNET
endef

define Package/h3clite/description
 SHNU h3clite client.
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

define Package/h3clite/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/h3clite $(1)/usr/bin
endef

$(eval $(call BuildPackage,h3clite))
