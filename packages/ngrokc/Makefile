#
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ngrokc
PKG_VERSION:=1.0
PKG_RELEASE:=2
PKG_REV=b123ae7

PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
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
	DEPENDS:=+NGROKC_POLARSSL:libpolarssl +NGROKC_OPENSSL:libopenssl +libpthread +libstdcpp
endef

define Package/ngrokc/config
	source "$(SOURCE)/Config.in"
endef

define Build/Compile
	$(call Build/Compile/Default,-f $(if $(CONFIG_NGROKC_POLARSSL),Makefile.polarssl,Makefile.openssl))
endef

define Package/ngrokc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ngrokc $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ngrokc))
