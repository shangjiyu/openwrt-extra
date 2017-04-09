#
# Copyright (C) 2016 wongsyrone 
#
# This is free software, licensed under the GNU General Public License v3.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pdnsd
PKG_VERSION:=1.2.9a
PKG_RELEASE:=3

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_NAME)-$(PKG_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/wongsyrone/pdnsd.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=ad22e203b810ae144aef734e335587d1c4b5f83d
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/pdnsd
	SECTION:=net
	CATEGORY:=Network
	SUBMENU:=Web Servers/Proxies
	DEPENDS:=+libpthread
	TITLE:=Proxy DNS Server
endef

define Package/pdnsd/config
	config PACKAGE_pdnsd_miniconf
		bool "Ship mini config file"
		default n
endef

define Package/pdnsd/conffiles
/etc/pdnsd.conf
endef

define Package/pdnsd/description
  pdnsd, is an IPv6 capable proxy DNS server with permanent caching (the cache
  contents are written to hard disk on exit) that is designed to cope with
  unreachable or down DNS servers (for example in dial-in networking).

  pdnsd can be used with applications that do dns lookups, eg on startup, and
  can't be configured to change that behaviour, to prevent the often
  minute-long hangs (or even crashes) that result from stalled dns queries.
endef

CONFIGURE_ARGS += \
	--with-cachedir=/var/pdnsd \
	--with-query-method=tcpudp \
	--enable-ipv6 \
	--with-tcp-max-query=3

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

# Use Link time optimization
TARGET_CFLAGS += -flto
TARGET_LDFLAGS += -Wl,-flto

define Package/pdnsd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/pdnsd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/pdnsd-ctl/pdnsd-ctl $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/pdnsd.init $(1)/etc/init.d/pdnsd
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/pdnsd.config $(1)/etc/config/pdnsd
	$(INSTALL_DIR) $(1)/etc
	$(if $(CONFIG_PACKAGE_pdnsd_miniconf),\
		$(INSTALL_CONF) ./files/pdnsd-mini.conf $(1)/etc/pdnsd.conf,\
		$(INSTALL_CONF) $(PKG_BUILD_DIR)/doc/pdnsd.conf $(1)/etc/)
endef

$(eval $(call BuildPackage,pdnsd))
