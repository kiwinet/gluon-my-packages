include $(TOPDIR)/rules.mk

PKG_NAME:=gluon-config-wan-mac
PKG_VERSION:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(GLUONDIR)/include/package.mk

PKG_CONFIG_DEPENDS += $(GLUON_I18N_CONFIG)


define Package/gluon-config-wan-mac
  SECTION:=gluon
  CATEGORY:=Gluon
  TITLE:=We can set wan-br mac now
  DEPENDS:=gluon-config-mode-core-virtual
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(call GluonBuildI18N,gluon-config-wan-mac,i18n)
endef

define Package/gluon-config-wan-mac/install
	$(CP) ./files/* $(1)/
	$(call GluonInstallI18N,gluon-config-wan-mac,$(1))
endef

$(eval $(call BuildPackage,gluon-config-wan-mac))
