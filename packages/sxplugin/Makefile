#
# Copyright (C) 2015 shangjiyu@gmail.com
#
# (XXX:请自行在下面修改ppp版本号(PPP_VERSION)以调整插件安装目录)
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sxplugin
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PPP_VERSION:=2.4.7

include $(INCLUDE_DIR)/package.mk

define Package/sxplugin/Default
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+ppp
  TITLE:=shanxun ppp plugin
endef

define Package/sxplugin
  $(call Package/sxplugin/Default)
endef

define Package/sxpluginSigned
  $(call Package/sxplugin/Default)
  DEPENDS+= +libpolarssl
  TITLE+= (RSA Signed)
endef

define Package/sxplugin/description
 shanxun ppp plugin.
endef

Package/sxpluginSigned/description = $(Package/sxplugin/description)

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

define Package/sxplugin/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PPP_VERSION)/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/sxplugin.so $(1)/usr/lib/pppd/$(PPP_VERSION)/sxplugin.so
endef

define Package/sxpluginSigned/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PPP_VERSION)/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/sxpluginSigned.so $(1)/usr/lib/pppd/$(PPP_VERSION)/sxpluginSigned.so
endef

$(eval $(call BuildPackage,sxplugin))
$(eval $(call BuildPackage,sxpluginSigned))
