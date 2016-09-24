include $(TOPDIR)/rules.mk

PKG_NAME:=gluon-mesh-vpn-tunneldigger
PKG_VERSION:=3

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(GLUONDIR)/include/package.mk


define Package/gluon-mesh-vpn-tunneldigger
  SECTION:=gluon
  CATEGORY:=Gluon
  TITLE:=Support for connecting batman-adv meshes via tunneltigger/l2tpv3 pseudowire
  DEPENDS:=+gluon-core +libgluonutil gluon-mesh-batman-adv +gluon-wan-dnsmasq +tunneldigger +iptables  +iptables-mod-extra +simple-tc +libuci +libplatforminfo +respondd
endef

define Package/gluon-mesh-vpn-tunneldigger/description
	Gluon community wifi mesh firmware framework: tunneldigger support
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/gluon-mesh-vpn-tunneldigger/install
	$(CP) ./files/* $(1)/
	$(INSTALL_DIR) $(1)/lib/gluon/respondd
	$(CP) $(PKG_BUILD_DIR)/respondd.so $(1)/lib/gluon/respondd/mesh-vpn-tunneldigger.so
endef

define Package/gluon-mesh-vpn-tunneldigger/postinst
#!/bin/sh
$(call GluonCheckSite,check_site.lua)
endef

$(eval $(call BuildPackage,gluon-mesh-vpn-tunneldigger))
