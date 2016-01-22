include $(TOPDIR)/rules.mk

PKG_NAME:=glorytun
PKG_VERSION:=0.1.0
PKG_RELEASE:=1
PKG_REV:=154ee9a7d704105a73721b3d6ed721ff0abb505b

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/jedisct1/glorytun.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_MAINTAINER:=Frank Denis <github@pureftpd.org>

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/glorytun
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=VPN
	TITLE:=Glorytun
	DEPENDS:=+kmod-tun +libsodium
	URL:=https://github.com/angt/glorytun
endef

define Package/glorytun/description
	Glorytun
endef

TARGET_CFLAGS += -std=c99
TARGET_CFLAGS += -D_GNU_SOURCE

define Package/glorytun/conffiles
/etc/config/glorytun
endef

define Build/Configure
        (cd $(PKG_BUILD_DIR); ./autogen.sh --host=$(GNU_TARGET_NAME) --target=$(GNU_TARGET_NAME); cd -);
        $(call Build/Configure/Default,$CONFIGURE_ARGS)
endef

define Package/glorytun/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/glorytun $(1)/usr/sbin
endef

$(eval $(call BuildPackage,glorytun))
