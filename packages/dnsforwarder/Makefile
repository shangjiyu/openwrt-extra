#
# Copyright (C) 2014-2015 MATTHEW728960 
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=dnsforwarder
PKG_VERSION:=5-01-15
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).zip
PKG_SOURCE_URL:=https://codeload.github.com/holmium/dnsforwarder/zip/5
PKG_INSTALL:=1

PKG_CONFIG_DEPENDS := \
  ENABLE_DOWNLOADER_LIBCURL \
  ENABLE_DOWNLOADER_WGET \
  ENABLE_BASE64_OPENSSL \
  ENABLE_BASE64_UUDECODE \
  ENABLE_STATIC

include $(INCLUDE_DIR)/package.mk

define Package/dnsforwarder-5/config
  source "$(SOURCE)/Config.in"
endef

define Package/dnsforwarder-5
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=DNS
  TITLE:=Dnsforwarder-5
  URL:=https://github.com/holmium/dnsforwarder/
  DEPENDS:=+libpthread +ENABLE_DOWNLOADER_LIBCURL:libcurl  +ENABLE_BASE64_OPENSSL:libopenssl  +ENABLE_BASE64_UUDECODE:uudecod +ENABLE_DOWNLOADER_WGET:wget 
endef

define Package/dnsforwarder-5/description
TCP+UDP DNSCache. GfwList can be included.
endef

CONFIGURE_ARGS += \
	--enable-downloader=$(if $(CONFIG_ENABLE_DOWNLOADER_LIBCURL),libcurl,wget)  \
	--enable-base64decoder=$(if $(ENABLE_BASE64_OPENSSL),openssl,uudecode) \
	$(if $(CONFIG_ENABLE_STATIC),--enable-static, ) 

define Package/dnsforwarder-5/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/controller
	$(INSTALL_DIR) $(1)/usr/bin

	$(INSTALL_CONF) ./files/dnsforwarder.config $(1)/etc
	$(INSTALL_CONF) ./files/luci/dnsforwarder.config $(1)/etc/config/dnsforwarder
	$(INSTALL_BIN) ./files/dnsforwarder.init $(1)/etc/init.d/dnsforwarder
	$(INSTALL_DATA) ./files/luci/cbi-dnsforwarder.lua $(1)/usr/lib/lua/luci/model/cbi/dnsforwarder.lua
	$(INSTALL_DATA) ./files/luci/controller-dnsforwarder.lua $(1)/usr/lib/lua/luci/controller/dnsforwarder.lua
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dnsforwarder $(1)/usr/bin
endef

$(eval $(call BuildPackage,dnsforwarder-5))
