include $(TOPDIR)/rules.mk

LUCI_TOPDIR=..

PKG_NAME:=luci-extra
PKG_RELEASE:=1

PKG_BUILD_PARALLEL:=0

PKG_VERSION:=2014-12-12

PKG_BUILD_DEPENDS:=$(if $(STAGING_DIR_ROOT),lua/host)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

LUA_TARGET:=source
LUCI_CFLAGS:=
LUCI_BUILD_PACKAGES:=
LUCI_SELECTED_MODULES:=

ifeq ($(BOARD),brcm-2.4)
  MAKE_FLAGS += CRAP="1"
endif


### Templates ###
define Package/luci/install/template
	$(CP) -a $(PKG_BUILD_DIR)/$(2)/dist/* $(1)/ -R
	$(CP) -a $(PKG_BUILD_DIR)/$(2)/ipkg/* $(1)/CONTROL/ 2>/dev/null || true
endef

### Protocols ###
define protocol
  define Package/luci-proto-$(1)
    SECTION:=luci
    CATEGORY:=LuCI
    TITLE:=LuCI - Lua Configuration Interface
    URL:=https://github.com/ravageralpha/my_openwrt_mod/
    MAINTAINER:=RA <ravageralpha@gmail.com>
    SUBMENU:=6. Protocols
    TITLE:=$(if $(2),$(2),LuCI $(1) protocol support)
    DEPENDS:=$(3)
  endef

  define Package/luci-proto-$(1)/install
	$(call Package/luci/install/template,$$(1),protocols/$(1))
  endef

  ifneq ($(CONFIG_PACKAGE_luci-proto-$(1)),)
    LUCI_SELECTED_MODULES+=protocols/$(1)
  endif

  LUCI_BUILD_PACKAGES += luci-proto-$(1)
endef

$(eval $(call protocol,openconnect-ra,Support for OpenConnect VPN,+openconnect))
$(eval $(call protocol,shadowvpn,Support for ShadowVPN,+ShadowVPN))

### Applications ###
define application
  define Package/luci-app-$(1)
    SECTION:=luci
    CATEGORY:=LuCI
    TITLE:=LuCI - Lua Configuration Interface
    URL:=https://github.com/nanpuyue/openwrt-extra/
    MAINTAINER:=nanpuyue <nanpuyue@gmail.com>
    PKGARCH:=all
    SUBMENU:=3. Applications
    TITLE:=$(if $(2),$(2),LuCI $(1) application)
	DEPENDS:=$(3)
  endef

  define Package/luci-app-$(1)/install
	$(call Package/luci/install/template,$$(1),applications/luci-$(1))
  endef

  ifneq ($(CONFIG_PACKAGE_luci-app-$(1)),)
    LUCI_SELECTED_MODULES+=applications/luci-$(1)
  endif

  LUCI_BUILD_PACKAGES += luci-app-$(1)
endef

define Package/luci-app-aria2/conffiles
/etc/config/aria2
endef

$(eval $(call application,aria2,LuCI Support for Aria2,\
    +PACKAGE_luci-app-aria2:aria2 \
    +PACKAGE_luci-app-aria2:yaaw))

$(eval $(call application,nwan,LuCI Support for NWAN,\
    +PACKAGE_luci-app-nwan:ip +kmod-macvlan +iptables +iptables-mod-conntrack +iptables-mod-conntrack-extra +iptables-mod-ipopt))

$(eval $(call application,qosv4,LuCI Support for qosv4,\
    +PACKAGE_luci-app-qosv4:tc +kmod-sched +iptables +iptables-mod-filter +iptables-mod-ipopt +iptables-mod-conntrack-extra +iptables-mod-imq +iptables-mod-hashlimit +iptables-mod-connlimit))
	 
$(eval $(call application,mentohust,LuCI Support for mentohust,\
    +PACKAGE_luci-app-mentohust:mentohust))

$(eval $(call application,shadowsocks,LuCI Support for shadowsocks,\
    +PACKAGE_luci-app-shadowsocks:shadowsocks-libev-polarssl \
    +PACKAGE_luci-app-shadowsocks:kmod-ipt-nat \
	+PACKAGE_luci-app-shadowsocks:kmod-ipt-nat-extra \
	+PACKAGE_luci-app-shadowsocks:kmod-ipt-ipset \
	+PACKAGE_luci-app-shadowsocks:ipset \
	+PACKAGE_luci-app-shadowsocks:resolveip))
	 
$(eval $(call application,redsocks2,LuCI Support for redsocks2,\
	+PACKAGE_luci-app-redsocks2:redsocks2 \
	+PACKAGE_luci-app-redsocks2:kmod-ipt-nat \
	+PACKAGE_luci-app-redsocks2:kmod-ipt-nat-extra \
	+PACKAGE_luci-app-redsocks2:kmod-ipt-ipset \
	+PACKAGE_luci-app-redsocks2:ipset \
	+PACKAGE_luci-app-shadowsocks:resolveip))
	 
$(eval $(call application,pdnsd,pdnsd configuration module,\
	+PACKAGE_luci-app-pdnsd:pdnsd))
	
$(eval $(call application,chinadns,LuCI Support for ChinaDNS-C,\
	+PACKAGE_luci-app-chinadns:ChinaDNS-C))

$(eval $(call application,wifidog,LuCI Support for the wifidog,\
        +PACKAGE_luci-app-wifidog:wifidog))

$(eval $(call application,vsftpd,LuCI Support for vsftpd,\
	+PACKAGE_luci-app-vsftpd:vsftpd))
	
$(eval $(call application,macvlan,LuCI Support for macvlan,\
	+PACKAGE_luci-app-macvlan:kmod-macvlan luci-app-mwan3))
	
$(eval $(call application,qos_gargoyle,LuCI Support for qos_gargoyle,\
	+PACKAGE_luci-app-qos_gargoyle:qos-gargoyle))
	
$(eval $(call application,cpulimit,LuCI Support for CPULIMIT,\
	+PACKAGE_luci-app-cpulimit:cpulimit-ng))

### Compile ###
PKG_CONFIG_DEPENDS=$(patsubst %,CONFIG_PACKAGE_%,$(LUCI_BUILD_PACKAGES))

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	@if [ ! -x $(LUCI_TOPDIR)/build/mkrevision.sh ]; then \
		echo "*** Repository layout changed!" >&2; \
		echo "*** Please change the LuCI url in feeds.conf to http://svn.luci.subsignal.org/luci/trunk and reinstall the feed with" >&2; \
		echo "*** ./scripts/feeds update luci; ./scripts/feeds install -a -p luci" >&2; \
		exit 1; \
	fi
	mkdir -p $(PKG_BUILD_DIR)
	$(TAR) c -C $(LUCI_TOPDIR) . \
		--exclude=.pc --exclude=.svn --exclude=.git \
		--exclude='boa-0*' --exclude='*.o' --exclude='*.so' \
		--exclude=dist | \
			tar x -C $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

MAKE_FLAGS += \
	MODULES="$(LUCI_SELECTED_MODULES)" \
	LUA_TARGET="$(LUA_TARGET)" \
	LUA_SHLIBS="-llua -lm -ldl -lcrypt" \
	CFLAGS="$(TARGET_CFLAGS) $(LUCI_CFLAGS) -I$(STAGING_DIR)/usr/include" \
	LDFLAGS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib" \
	NIXIO_TLS="$(NIXIO_TLS)" OS="Linux"


$(foreach b,$(LUCI_BUILD_PACKAGES),$(eval $(call BuildPackage,$(b))))
