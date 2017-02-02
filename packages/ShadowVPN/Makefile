include $(TOPDIR)/rules.mk

PKG_NAME:=ShadowVPN

PKG_VERSION:=79fc94e71
PKG_RELEASE=0.2.1
PKG_REV=79fc94e717a22d87aed67a1f01b03266e99a1681

PKG_SOURCE_URL:=https://github.com/rains31/ShadowVPN.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_MAINTAINER:=clowwindy <clowwindy42@gmail.com>

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ShadowVPN/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=ShadowVPN
  DEPENDS:=+kmod-tun +ip
  URL:=https://github.com/rains31/ShadowVPN
endef

define Package/ShadowVPN
  $(call Package/ShadowVPN/Default)
endef

define Package/ShadowVPN/description
A fast, safe VPN based on libsodium
endef

define Package/ShadowVPN/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/shadowvpn $(1)/usr/bin
endef

$(eval $(call BuildPackage,ShadowVPN))
