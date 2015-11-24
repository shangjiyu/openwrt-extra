#
# Copyright (C) 2015 wongsyrone 
#
# This is free software, licensed under the GNU General Public License v3.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sans
# PKG_VERSION comes from configure.ac
PKG_VERSION:=0.1.0
PKG_RELEASE:=2
PKG_REV:=e391e9fdeb77cb5b7614f355d21856b959c8995e

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_SOURCE_URL:=https://github.com/XiaoxiaoPu/sans.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_MAINTAINER:=XiaoxiaoPu <i@xiaoxiao.im>
PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0

include $(INCLUDE_DIR)/package.mk

define Package/sans
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Simple anti-spoofing name server
	URL:=https://github.com/XiaoxiaoPu/sans
#	DEPENDS:=
endef

define Package/sans/description
  Simple Anti-spoofing Name Server, designed to defend against DNS spoofing, suitable for embedded devices and low end boxes.
  It serves as a backup of pcap-dnsproxy, dnscrypt-proxy or self private dns server.
endef

CONFIGURE_ARGS += \
	--prefix=/usr \
	--sysconfdir=/etc

define Package/sans/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/sans.init $(1)/etc/init.d/sans
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/sans.config $(1)/etc/config/sans
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) ./files/sans.conf $(1)/etc/sans.conf
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/sans $(1)/usr/bin
endef

$(eval $(call BuildPackage,sans))
