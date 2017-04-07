include $(TOPDIR)/rules.mk

PKG_NAME:=cdns
PKG_VERSION:=ab40a7
PKG_RELEASE:=1
PKG_REV:=ab40a799f0d17481ffe90d244c98122ab607489f

PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/semigodking/cdns.git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/cdns
	SECTION:=net
	CATEGORY:=Network
	TITLE:=CureDNS
	MAINTAINER:=semigodking
	DEPENDS:=+libevent2
endef

define Package/cdns/description
	cdns is an experimental tool to cure your poisoned DNS.
endef

define Package/cdns/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/cdns $(1)/usr/bin/cdns
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/cdns.conf $(1)/etc/config/cdns
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/cdns.init $(1)/etc/init.d/cdns
endef

$(eval $(call BuildPackage,cdns))
