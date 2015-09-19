#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tvheadend

PKG_REV:=fb6b56ce0f7a2edc5885c5e455e4ab07e27acfa2
PKG_VERSION:=4.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.bz2
PKG_SOURCE_URL:=https://github.com/tvheadend/tvheadend.git

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/tvheadend
  SECTION:=multimedia
  CATEGORY:=Multimedia
  DEPENDS:=$(ICONV_DEPENDS) +libopenssl +librt +libffmpeg-full
  TITLE:=Tvheadend is a TV streaming server for Linux
  URL:=https://www.lonelycoder.com/hts/tvheadend_overview.html
endef

define Package/tvheadend/description
  Tvheadend is a TV streaming server for Linux supporting DVB-S, DVB-S2, DVB-C, DVB-T, ATSC, IPTV, and Analog video (V4L) as input sources.
  It also comes with a powerful and easy to use web interface both used for configuration and day-to-day operations,
  such as searching the EPG and scheduling recordings. 
  Even so, the most notable feature of Tvheadend is how easy it is to set up: Install it, navigate to the web user interface, 
  drill into the TV adapters tab, select your current location and Tvheadend will start scanning channels and present them to you in just 
  a few minutes
endef

define Package/tvheadend/config
  menu "Configuration"
  depends on PACKAGE_tvheadend
  source "$(SOURCE)/Config.in"
  endmenu
endef

ifeq ($(CONFIG_TVHEADEND_CWC_SUPPORT),)
  CONFIGURE_ARGS += --disable-cwc
endif

ifeq ($(CONFIG_TVHEADEND_V4L_SUPPORT),)
  CONFIGURE_ARGS += --disable-v4l
endif

ifeq ($(CONFIG_TVHEADEND_LINUXDVB_SUPPORT),)
  CONFIGURE_ARGS += --disable-linuxdvb
endif

ifeq ($(CONFIG_TVHEADEND_DVBSCAN_SUPPORT),)
  CONFIGURE_ARGS += --disable-dvbscan
endif

ifeq ($(CONFIG_TVHEADEND_AVAHI_SUPPORT),)
  CONFIGURE_ARGS += --disable-avahi
else
  CONFIGURE_ARGS += --enable-avahi
endif

ifeq ($(CONFIG_TVHEADEND_DVBCSA_SUPPORT),)
  CONFIGURE_ARGS += --disable-dvbcsa
else
  CONFIGURE_ARGS += --enable-dvbcsa
endif


CONFIGURE_ARGS += \
		--release \
		--enable-bundle 

TARGET_LDFLAGS += \
		-Wl,-rpath-link=$(STAGING_DIR)/usr/lib \
		-liconv


define Build/Configure
	#remove unused resources
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/images/yourtheme
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/yourtheme.css

	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/images/access
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/theme-access
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/xtheme-access.css

	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/images/gray
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/theme-gray
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/xtheme-gray.css
	
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/images/vista

	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/structure
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/visual
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/README.txt
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/xtheme-blue.css
	$(RM) $(PKG_BUILD_DIR)/src/webui/static/extjs/resources/css/ext-all-notheme.css

	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/adapter/ext/ext-base-debug.js
	$(RM) -r $(PKG_BUILD_DIR)/src/webui/static/extjs/ext-all-debug.js

	$(CP) $(PKG_BUILD_DIR)/docs/docresources/tvheadendlogo.png $(PKG_BUILD_DIR)/src/webui/static/img
	$(call Build/Configure/Default)
endef
				
define Package/tvheadend/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/tvheadend.init $(1)/etc/init.d/tvheadend

	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build.linux/tvheadend $(1)/usr/bin/
endef

define Package/tvheadend/prerm
	#!/bin/sh
	# check if we are on real system
	if [ -z "$${IPKG_INSTROOT}" ]; then
        	echo "Stopping tvheadend"
        	/etc/init.d/tvheadend stop
	fi
	exit 0
endef

$(eval $(call BuildPackage,tvheadend))
