include $(TOPDIR)/rules.mk

PKG_NAME:=ChinaDNS

PKG_VERSION:=9e4261c78e
PKG_RELEASE:=1.3.3
PKG_REV:=9e4261c78e911851f76a9b9113d48fcdc44ce32e

PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/shadowsocks/ChinaDNS.git
PKG_SOURCE_VERSION:=$(PKG_REV)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ChinaDNS/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=ChinaDNS
  URL:=https://github.com/shadowsocks/ChinaDNS
endef

define Package/ChinaDNS
  $(call Package/ChinaDNS/Default)
endef

define Package/ChinaDNS/description
A DNS forwarder that ignores incorrect(you knew it) responses.
endef

define Package/ChinaDNS/conffiles
/etc/ipset
endef

define Package/ChinaDNS/install
	$(INSTALL_DIR) $(1)/etc/ipset
	$(INSTALL_DATA) ./files/whitelist $(1)/etc/ipset/whitelist
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/chinadns $(1)/usr/bin
endef

$(eval $(call BuildPackage,ChinaDNS))
