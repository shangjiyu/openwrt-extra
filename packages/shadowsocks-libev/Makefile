include $(TOPDIR)/rules.mk

PKG_NAME:=shadowsocks-libev
PKG_VERSION:=2.3.1
PKG_RELEASE=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=hatps://github.com/shadowsocks/shadowsocks-libev.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=f45f7f28150d19dd4f28466525cec5fea50f1aaf
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_MAINTAINER:=Max Lv <max.c.lv@gmail.com>

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1
PKG_USE_MIPS16:=0

PKG_CONFIG_DEPENDS:= \
	CONFIG_SHADOWSOCKS_LIBEV_POLARSSL \
	CONFIG_SHADOWSOCKS_LIBEV_OPENSSL

include $(INCLUDE_DIR)/package.mk

define Package/shadowsocks-libev/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Lightweight Secured Socks5 Proxy
  DEPENDS:=+libpthread +SHADOWSOCKS_LIBEV_POLARSSL:libpolarssl +SHADOWSOCKS_LIBEV_OPENSSL:libopenssl
  URL:=https://github.com/shadowsocks/shadowsocks-libev
endef

define Package/shadowsocks-libev
  $(call Package/shadowsocks-libev/Default)
  TITLE+= (CLIENT)
endef

define Package/shadowsocks-libev-server
  $(call Package/shadowsocks-libev/Default)
  TITLE+= (SERVER)
endef

define Package/shadowsocks-libev/config
  source "$(SOURCE)/Config.in"
endef

define Package/shadowsocks-libev-server/config
  source "$(SOURCE)/Config.in"
endef

CONFIGURE_ARGS += \
	$(if $(CONFIG_SHADOWSOCKS_LIBEV_POLARSSL),--with-crypto-library=polarssl,)

define Package/shadowsocks-libev/description
Shadowsocks-libev is a lightweight secured scoks5 proxy for embedded devices and low end boxes.
endef

Package/shadowsocks-libev-server/description=$(Package/shadowsocks-libev/description)

#define Package/shadowsocks-libev/conffiles
#/etc/shadowsocks.json
#endef

define Package/shadowsocks-libev/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-{local,redir,tunnel} $(1)/usr/bin
endef

define Package/shadowsocks-libev-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-server $(1)/usr/bin
endef

$(eval $(call BuildPackage,shadowsocks-libev))
$(eval $(call BuildPackage,shadowsocks-libev-server))
