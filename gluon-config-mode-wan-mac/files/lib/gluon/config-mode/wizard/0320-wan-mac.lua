return function(form, uci)

  local pkg_i18n = i18n 'gluon-config-mode-wan-mac'
  local s = form:section(Section, nil, pkg_i18n.translate('You can set the WAN port fixed MAC address.'))
  local o
  local def_mac

  fixedmac = s:option(Flag, "_fixedmac", pkg_i18n.translate("Enable fixed WAN mac address"))
  fixedmac.default = uci:get_bool("fixedmacs", "wan", "enabled")
  function fixedmac:write(data)
    uci:set("fixedmacs", "wan", "enabled", data)

    local test_mac = uci:get("fixedmacs", "wan", "base_macaddr")
    if test_mac == nil or test_mac == '' then
      def_mac = uci:get("network", "wan", "macaddr")
      uci:set("fixedmacs", "wan", "base_macaddr", def_mac)
    end
    if data == true then
      uci:set("fixedmacs", "wan", "macaddr", def_mac)
    else
      def_mac = uci:get("fixedmacs", "wan", "base_macaddr")
      uci:set("fixedmacs", "wan", "macaddr", def_mac)
      uci:set("network", "wan", "macaddr", def_mac)
    end
  end

  o = s:option(Value, "_fixedmac_address", pkg_i18n.translate("WAN port MAC address"))
  o:depends(fixedmac, true)
  if fixedmac.default then
    o.default = uci:get("fixedmacs", "wan", "macaddr")
  else
    o.default = uci:get("fixedmacs", "wan", "base_macaddr")
  end
  o.datatype = "minlength(17)"
  o.description = "xx:xx:xx:xx:xx:xx"
  o.maxlength = 17

  function o:write(data)
    if data ~= nil and data ~= '' then
      uci:set("fixedmacs", "wan", "macaddr", data)
      uci:set("network", "wan", "macaddr", data)
    else
      def_mac = uci:get("fixedmacs", "wan", "base_macaddr")
      uci:set("fixedmacs", "wan", "macaddr", def_mac)
      uci:set("network", "wan", "macaddr", def_mac)
    end
  end

  function s:handle()
    Section.handle(s)
    uci:save('fixedmacs')
    uci:save('network')
  end

  return {'fixedmacs', 'network'}
end