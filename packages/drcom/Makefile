# an CERNET drcom client for ZJUT
# Copyright (C) 2014 shangjiyu@gmail.com
#
#
# This is free software, licensed under the GNU General Public License v2.
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=drcom
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/drcom
  SECTION:=net
  CATEGORY:=Network
  TITLE:=an CERNET ZJUT drcom client
  URL:=https://github.com/shangjiyu/drcom/
  SUBMENU:=CERNET
endef

define Package/drcom/description
 ZJUT drcom client.
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

define Package/drcom/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drcom $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drmonitor.sh $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/drcom.conf $(1)/etc/
endef

$(eval $(call BuildPackage,drcom))
