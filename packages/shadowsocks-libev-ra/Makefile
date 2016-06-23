include $(TOPDIR)/rules.mk

PKG_NAME:=shadowsocks-libev-ra
PKG_VERSION:=2.4.7
PKG_RELEASE:=1
PKG_REV:=e69b0a999021a4694af1add82c904c817c10ba99

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
  DEPENDS:=+libpthread +libev +libsodium +SHADOWSOCKS_LIBEV_POLARSSL:libpolarssl +SHADOWSOCKS_LIBEV_OPENSSL:libopenssl +CONFIG_SHADOWSOCKS_LIBEV_MBEDTLS:libmbedtls
  URL:=https://github.com/shadowsocks/shadowsocks-libev
endef

define Package/shadowsocks-libev-ra
  $(call Package/shadowsocks-libev-ra/Default)
  TITLE+= (CLIENT)
endef

define Package/shadowsocks-libev-ra-spec
  $(call Package/shadowsocks-libev-ra/Default)
  TITLE+= (SPEC)
  DEPENDS+=+ipset +ip +iptables-mod-tproxy
endef

define Package/shadowsocks-libev-ra-server
  $(call Package/shadowsocks-libev-ra/Default)
  TITLE+= (SERVER)
endef

define Package/shadowsocks-libev-ra/config
  source "$(SOURCE)/Config.in"
endef

define Package/shadowsocks-libev-ra-spec/config
  source "$(SOURCE)/Config.in"
endef

define Package/shadowsocks-libev-ra-server/config
  source "$(SOURCE)/Config.in"
endef

CONFIGURE_ARGS += \
	--enable-system-shared-lib --disable-ssp \
	$(if $(CONFIG_SHADOWSOCKS_LIBEV_POLARSSL),--with-crypto-library=polarssl,) \
	$(if $(CONFIG_SHADOWSOCKS_LIBEV_MBEDTLS),--with-crypto-library=mbedtls,)
# Check whether compiler follows our inline suggestion
TARGET_CFLAGS += -Wno-error=inline -Winline

# Use Link time optimization
TARGET_CFLAGS += -flto
TARGET_LDFLAGS += -Wl,-flto

define Package/shadowsocks-libev-ra/description
ShadowSocks-Libev is a lightweight secured scoks5 proxy for embedded devices and low end boxes.
endef

Package/shadowsocks-libev-ra-spec/description=$(Package/shadowsocks-libev-ra/description)
Package/shadowsocks-libev-ra-server/description=$(Package/shadowsocks-libev-ra/description)

define Package/shadowsocks-libev-ra/conffiles
/etc/shadowsocks.json
endef

define Package/shadowsocks-libev-ra-spec/conffiles
/etc/config/shadowsocks
endef

define Package/shadowsocks-libev-ra-spec/postinst
#!/bin/sh
if [ -z "$${IPKG_INSTROOT}" ]; then
	uci -q batch <<-EOF >/dev/null
		delete firewall.shadowsocks
		set firewall.shadowsocks=include
		set firewall.shadowsocks.type=script
		set firewall.shadowsocks.path=/var/etc/shadowsocks.include
		set firewall.shadowsocks.reload=1
		commit firewall
EOF
/etc/init.d/shadowsocks enable >/dev/null 2>&1
fi
exit 0
endef

define Package/shadowsocks-libev-ra/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-{local,redir,tunnel} $(1)/usr/bin
endef

define Package/shadowsocks-libev-ra-spec/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-{local,redir,tunnel} $(1)/usr/bin
	$(INSTALL_BIN) ./files/shadowsocks.rule $(1)/usr/bin/ss-rules
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/shadowsocks.config $(1)/etc/config/shadowsocks
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/shadowsocks.spec $(1)/etc/init.d/shadowsocks
endef

define Package/shadowsocks-libev-ra-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ss-server $(1)/usr/bin
endef

$(eval $(call BuildPackage,shadowsocks-libev-ra))
$(eval $(call BuildPackage,shadowsocks-libev-ra-spec))
$(eval $(call BuildPackage,shadowsocks-libev-ra-server))
