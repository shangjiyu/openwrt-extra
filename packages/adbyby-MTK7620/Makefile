#
# Copyright (C) 2014 shangjiyu <shangjiyu@gmail.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=adbyby-MTK7620
PKG_VERSION:=2.1.0
PKG_RELEASE=1

#PKG_SOURCE_URL:=http://info.adbyby.com/download
#PKG_SOURCE:=7620n.tar.gz
#PKG_MAINTAINER:=shangjiyu <shangjiyu@gmail.com>

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/adbyby-MTK7620/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=AD Bye Bye-MTK7620
  URL:=https://github.com/shangjiyu/openwrt-extra
endef

define Package/adbyby-MTK7620
  $(call Package/adbyby-MTK7620/Default)
endef

define Package/adbyby-MTK7620/description
	ADBYBY is a tools to remove AD from internet browsing.
endef

define Build/Compile
endef

define Package/adbyby-MTK7620/conffiles
/etc/adbyby.ini
endef

define Package/adbyby-MTK7620/install
	$(CP) ./files/* $(1)
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) .files/usr/bin/adbyby $(1)/usr/bin/adbyby
endef

$(eval $(call BuildPackage,adbyby-MTK7620))

