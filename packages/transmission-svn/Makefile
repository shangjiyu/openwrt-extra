#
# Copyright (C) 2009-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=transmission-svn
PKG_VERSION:=2.90
PKG_RELEASE:=3
# FIXME: update commit and SVN revision
PKG_REV:=13a98a02b85bf25b705098f5f477753bd47158ad

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/transmission/transmission.git

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/transmission-svn/template
  SUBMENU:=BitTorrent
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A free, lightweight BitTorrent client
  URL:=https://trac.transmissionbt.com/timeline
  MAINTAINER:=Cezary Jackiewicz <cezary@eko.one.pl>
endef

define Package/transmission-svn-daemon
  $(call Package/transmission-svn/template)
  DEPENDS:=+libcurl +libopenssl +libpthread +libevent2 +librt
  MENU:=1
  USERID:=transmission=224:transmission=224
endef

define Package/transmission-svn-daemon/description
 Transmission is a simple BitTorrent client.
 It features a very simple, intuitive interface
 on top on an efficient, cross-platform back-end.
 This package contains the daemon itself.
endef


define Package/transmission-svn-daemon/conffiles
/etc/config/transmission
endef

CONFIGURE_VARS += \
	LIBEVENT_LIBS="$(STAGING_DIR)/usr/lib/libevent-2.0.so.5"

CONFIGURE_ARGS += \
	--enable-daemon \
	--enable-cli \
	--enable-utp \
	--without-gtk \
	--enable-largefile \
	--enable-lightweight

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS) -D_GNU_SOURCE"

# run autogen.sh by hand and use PKG_FIXUP to reconfigure
# generate REVISION file by hand since we're not in a SVN source tree
define Build/Prepare
	echo "14666" > $(PKG_BUILD_DIR)/REVISION
	$(call Build/Prepare/Default)
	(cd $(PKG_BUILD_DIR); \
		./autogen.sh \
	)
endef

### For record: if you want to use the cmake build system, use the method below as a workaround, since cmake
###             doesn't support https protocol if not built with system libcurl
###             Related Link: http://stackoverflow.com/questions/29816529/unsupported-protocol-while-downlod-tar-gz-package
###             Also, the Cmake large file support tests use try-compile method to determine whether it works,
###             and it may NOT work properly when cross compiling.
# $1: pkg name
# $2: github user and repo
# $3: commit id
# Find from CMakeList.txt and cmake/TrMacros.cmake
#define TrDownFromGithub
#	(cd $(PKG_BUILD_DIR)/third-party/$(1)-$(3)/src; \
#		wget https://github.com/$(2)/archive/$(3).tar.gz \
#	)
#endef
# temporary fix github download issue before make
# You have to update these strings when upstream updated entries in CMakeList
#define Build/Configure
#	$(call Build/Configure/Default)
#	$(call TrDownFromGithub,b64,mikedld/libb64,c1e3323498)
#	$(call TrDownFromGithub,dht,jech/dht,cc379e406d)
#	$(call TrDownFromGithub,miniupnpc,miniupnp/miniupnp,5de2bcb561)
#	$(call TrDownFromGithub,natpmp,miniupnp/libnatpmp,cf7f452d66)
#	$(call TrDownFromGithub,utp,bittorrent/libutp,7c4f19abdf)
#endef

define Package/transmission-svn-daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/transmission-daemon $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) files/transmission.init $(1)/etc/init.d/transmission
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) files/transmission.config $(1)/etc/config/transmission
endef


$(eval $(call BuildPackage,transmission-svn-daemon))
