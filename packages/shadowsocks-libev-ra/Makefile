include $(TOPDIR)/rules.mk

PKG_NAME:=shadowsocks-libev-ra
PKG_VERSION:=2.4.5
PKG_RELEASE:=2
PKG_REV:=da063789157027d85bb2082e397fab7c070b0bd9

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/shadowsocks/shadowsocks-libev.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_MAINTAINER:=Max Lv <max.c.lv@gmail.com>

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1
PKG_USE_MIPS16:=0

PKG_CONFIG_DEPENDS:= \
	CONFIG_SHADOWSOCKS_LIBEV_POLARSSL \
	CONFIG_SHADOWSOCKS_LIBEV_MBEDTLS \
	CONFIG_SHADOWSOCKS_LIBEV_OPENSSL

include $(INCLUDE_DIR)/package.mk

define Package/shadowsocks-libev-ra/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Lightweight Secured Socks5 Proxy
  DEPENDS:=+libpthread +libsodium +libev +SHADOWSOCKS_LIBEV_POLARSSL:libpolarssl +SHADOWSOCKS_LIBEV_OPENSSL:libopenssl +CONFIG_SHADOWSOCKS_LIBEV_MBEDTLS:libmbedtls
  URL:=https://github.com/shadowsocks/shadowsocks-libev
endef

define Package/shadowsocks-libev-ra
  $(call Package/shadowsocks-libev-ra/Default)
  TITLE+= (CLIENT)
endef

define Package/shadowsocks-libev-ra-server
  $(call Package/shadowsocks-libev-ra/Default)
  TITLE+= (SERVER)
endef

define Package/shadowsocks-libev-ra/config
  source "$(SOURCE)/Config.in"
endef

define Package/shadowsocks-libev-ra-server/config
  source "$(SOURCE)/Config.in"
endef

CONFIGURE_ARGS += \
	--enable-system-shared-lib --disable-ssp \
	$(if $(CONFIG_SHADOWSOCKS_LIBEV_POLARSSL),--with-crypto-library=polarssl,) \
	$(if $(CONFIG_SHADOWSOCKS_LIBEV_MBEDTLS),--with-crypto-library=mbedtls,)

define Package/shadowsocks-libev-ra/description
ShadowSocks-Libev is a lightweight secured scoks5 proxy for embedded devices and low end boxes.
endef

Package/shadowsocks-libev-ra-server/description=$(Package/shadowsocks-libev-ra/description)

#define Package/shadowsocks-libev-ra/conffiles
#/etc/shadowsocks.json
#endef

define Package/shadowsocks-libev-ra/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-{local,redir,tunnel} $(1)/usr/bin
endef

define Package/shadowsocks-libev-ra-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-server $(1)/usr/bin
endef

$(eval $(call BuildPackage,shadowsocks-libev-ra))
$(eval $(call BuildPackage,shadowsocks-libev-ra-server))
