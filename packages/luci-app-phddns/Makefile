include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-phddns
PKG_RELEASE:=0.0.1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

PO2LMO:=$(shell find $(TOPDIR)/build_dir/target-*/luci* -name po2lmo | awk '{if(match($$0,/build\/po2lmo/)) print $$0}')

include $(INCLUDE_DIR)/package.mk

define Package/luci-app-phddns
  SECTION:=luci
  CATEGORY:=LuCI
  SUBMENU:=3. Applications
  TITLE:=jmtoo License Key 
  DEPENDS:=+luci +phddns +subversion
endef

define Package/luci-app-phddns/description
  License Key Save Application.
  Developer by bywayboy<bywayboy@qq.com>
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./luasrc $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/luasrc
endef

define Package/luci-app-phddns/postinst
#!/bin/sh
[ -n "${IPKG_INSTROOT}" ] || {
	rm -f /tmp/luci-indexcache
	exit 0
}
endef

define Package/luci-app-phddns/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/controller
	$(CP) $(PKG_BUILD_DIR)/luasrc/controller/* $(1)/usr/lib/lua/luci/controller

	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi/admin_services
	$(CP) $(PKG_BUILD_DIR)/luasrc/model/cbi/admin_services/* $(1)/usr/lib/lua/luci/model/cbi/admin_services	

	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/view/admin_services
	$(CP) $(PKG_BUILD_DIR)/luasrc/view/admin_services/* $(1)/usr/lib/lua/luci/view/admin_services
	
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/i18n
	$(PO2LMO) ./lang/phddns.zh-cn.po $(1)/usr/lib/lua/luci/i18n/phddns.zh-cn.lmo
	
#	$(INSTALL_DIR) $(1)/www/luci-static/resources
#	$(CP) ./luci-static/resources/* $(1)/www/luci-static/resources
endef

$(eval $(call BuildPackage,luci-app-phddns))

