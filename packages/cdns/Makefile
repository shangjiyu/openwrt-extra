#
# Copyright (C) 2015 Gmod
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=cdns
PKG_VERSION:=1.00
PKG_RELEASE:=7f064d05516f511d80adc99ce831e187e2d0b35a

PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_RELEASE).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/semigodking/cdns.git
PKG_SOURCE_VERSION:=$(PKG_RELEASE)

PKG_LICENSE:=Apache v2.0
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Zhuofei Wang <SemigodKing@gmail.com>

PKG_BUILD_DEPENDS:=argp-standalone
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/cdns
	SECTION:=net
	CATEGORY:=Network
	TITLE:=CureDNS is an experimental tool to cure your poisoned DNS.
	URL:=https://github.com/semigodking/cdns
	DEPENDS:=+libevent2
endef

define Package/cdns/description
cdns is an experimental tool to cure your poisoned DNS. The benifits are:
1,No VPN required
2,You can use many foreign DNS servers as upstream server.
3,You can get really good experience if you use fast foreign DNS server as up stream server.
endef

define Package/cdns/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cdns $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/cdns.config $(1)/etc/config/cdns
	$(INSTALL_DIR) $(1)/etc/cdns
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/cdns.init $(1)/etc/init.d/cdns
endef

$(eval $(call BuildPackage,cdns))
