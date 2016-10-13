include $(TOPDIR)/rules.mk

PKG_NAME:=gluon-p910nd-update
PKG_VERSION:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(GLUONDIR)/include/package.mk

#PKG_CONFIG_DEPENDS += $(GLUON_I18N_CONFIG)


define Package/gluon-p910nd-update
  SECTION:=gluon
  CATEGORY:=Gluon
  TITLE:=Firewall and start script update for p910nd
  DEPENDS:=gluon-config-mode-core-virtual +p910nd
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/gluon-p910nd-update/install
	mkdir -p $(1)/etc/hotplug.d/usb
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,gluon-p910nd-update))
