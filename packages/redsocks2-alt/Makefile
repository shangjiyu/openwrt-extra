#
# Copyright (C) 2014 OpenWrt-dist
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

_PKG_NAME:=redsocks2
PKG_NAME:=$(_PKG_NAME)-alt
PKG_VERSION:=0.60
PKG_RELEASE=2

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=https://github.com/aa65535/openwrt-redsocks2/releases/download/v$(PKG_VERSION)
#PKG_MD5SUM:=b88bf23bf75fa7642f8d230db21f73b9

PKG_SOURCE_PROTO:=git
PKG_REV=ffad0b5
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(_PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(_PKG_NAME)-$(PKG_REV).tar.bz2
PKG_SOURCE_URL:=https://github.com/semigodking/redsocks.git

PKG_LICENSE:=GPLv2
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Zhuofei Wang <SemigodKing@gmail.com>

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/redsocks2-alt
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Redirect any TCP connection to a SOCKS or HTTPS proxy server
	URL:=https://github.com/semigodking/redsocks
	DEPENDS:=+libevent2 +REDSOCKS2_POLARSSL:libpolarssl +REDSOCKS2_OPENSSL:libopenssl
endef

define Package/redsocks2-alt/description
This is a modified version of original redsocks. \
The name is changed to be REDSOCKS2 since this release to distinguish with original redsocks. \
This variant is useful for anti-GFW (Great Fire Wall).
endef

define Package/redsocks2-alt/config
	source "$(SOURCE)/Config.in"
endef

define Build/Configure
	$(CP) $(PKG_BUILD_DIR)/../$(_PKG_NAME)-$(PKG_VERSION)/* $(PKG_BUILD_DIR)
	$(call Build/Configure/Default)
endef

define Build/Compile
	$(call Build/Compile/Default,$(if $(CONFIG_REDSOCKS2_POLARSSL),USE_CRYPTO_POLARSSL=true))
endef

define Package/redsocks2-alt/conffiles
/etc/config/redsocks2
endef

define Package/redsocks2-alt/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/redsocks2 $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/redsocks2.config $(1)/etc/config/redsocks2
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/redsocks2.init $(1)/etc/init.d/redsocks2
	chmod +x $(1)/etc/init.d/redsocks2
	$(INSTALL_DIR) $(1)/etc/redsocks2
	$(INSTALL_DATA) ./files/redsocks2.template $(1)/etc/redsocks2/config.template
endef

$(eval $(call BuildPackage,redsocks2-alt))
