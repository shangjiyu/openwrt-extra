include $(TOPDIR)/rules.mk

PKG_NAME:=glorytun
PKG_VERSION:=0.0.34
PKG_RELEASE:=4
PKG_REV:=13f9f4c896bdd2d81f81c5cda860906c7f455273

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/angt/glorytun.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_MAINTAINER:=Adrien Gallouët <adrien@gallouet.fr>

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/glorytun
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=VPN
	TITLE:=Glorytun
	DEPENDS:=+kmod-tun +libsodium +librt +ip +iptables +@BUSYBOX_DEFAULT_PKILL
	URL:=https://github.com/angt/glorytun
endef

define Package/glorytun/description
Glorytun - Small, Simple and Stupid TCP VPN.
endef

TARGET_CFLAGS += -std=c99 -D_GNU_SOURCE

define Package/glorytun/conffiles
/etc/config/glorytun
endef

#define Build/Configure
#        (cd $(PKG_BUILD_DIR); ./autogen.sh --host=$(GNU_TARGET_NAME) --target=$(GNU_TARGET_NAME); cd -);
#        $(call Build/Configure/Default,$CONFIGURE_ARGS)
#endef

define Package/glorytun/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/glorytun.sh $(1)/usr/sbin/glorytun.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/glorytun $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/glorytun $(1)/etc/init.d/glorytun
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/glorytun.defaults $(1)/etc/uci-defaults/glorytun.defaults
endef

define Package/glorytun/postinst
#!/bin/sh
if [ -z $${IPKG_INSTROOT} ] ; then
	( . /etc/uci-defaults/glorytun.defaults ) && rm -f /etc/uci-defaults/glorytun.defaults
	[ -x /etc/init.d/glorytun ] && /etc/init.d/glorytun restart;
fi
endef

$(eval $(call BuildPackage,glorytun))
