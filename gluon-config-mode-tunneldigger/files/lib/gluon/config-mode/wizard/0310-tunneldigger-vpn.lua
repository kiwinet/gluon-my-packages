local cbi = require "luci.cbi"
local i18n = require "luci.i18n"
local uci = luci.model.uci.cursor()

local M = {}

function M.section(form)
  local msg = ''
--  local msg = i18n.translate('Your internet connection can be used to establish a ' ..
--                             'L2TP VPN connection with other nodes. ' ..
--                             'Enable this option if there are no other nodes reachable ' ..
--                             'over WLAN in your vicinity or you want to make a part of ' ..
--                             'your connection\'s bandwidth available for the network. You can limit how ' ..
--                             'much bandwidth the node will use at most.')
  local s = form:section(cbi.SimpleSection, nil, msg)
  
  local o

  local fast_enabled = uci:get("fastd", "mesh_vpn", "enabled")

  o = s:option(cbi.Flag, "_meshvpn_t", i18n.translate("Use internet connection (mesh VPN via L2TP)"))
  o:depends("_meshvpn", "")
  if fast_enabled == '' then
    o.default = uci:get_bool("tunneldigger", uci:get_first("tunneldigger", "broker"), "enabled") and o.enabled or o.disabled
  else
    o.default = uci:get_bool("tunneldigger", uci:get_first("tunneldigger", "broker"), "enabled") and o.disabled or o.enabled
  end
  o.rmempty = false

  o = s:option(cbi.Flag, "_limit_enabled_t", i18n.translate("Limit bandwidth"))
  o:depends("_meshvpn_t", "1")
  o.default = uci:get_bool("simple-tc", "mesh_vpn", "enabled") and o.enabled or o.disabled
  o.rmempty = false

  o = s:option(cbi.Value, "_limit_ingress_t", i18n.translate("Downstream (kbit/s)"))
  o:depends("_limit_enabled_t", "1")
  o.value = uci:get("simple-tc", "mesh_vpn", "limit_ingress")
  o.rmempty = false
  o.datatype = "uinteger"

  o = s:option(cbi.Value, "_limit_egress_t", i18n.translate("Upstream (kbit/s)"))
  o:depends("_limit_enabled_t", "1")
  o.value = uci:get("simple-tc", "mesh_vpn", "limit_egress")
  o.rmempty = false
  o.datatype = "uinteger"
end

function M.handle(data)
  if data._meshvpn ~= nil then
      uci:set("fastd", "mesh_vpn", "enabled", data._meshvpn)
  else
      uci:set("fastd", "mesh_vpn", "enabled", "0")
  end
  uci:save("fastd")
  uci:commit("fastd")
  
  if data._meshvpn_t ~= nil then
     uci:set("tunneldigger", uci:get_first("tunneldigger", "broker"), "enabled", data._meshvpn_t)
  else
     uci:set("tunneldigger", uci:get_first("tunneldigger", "broker"), "enabled", "0")
  end
  uci:save("tunneldigger")
  uci:commit("tunneldigger")

  -- checks for nil needed due to o:depends(...)
  if data._limit_enabled_t ~= nil then
    uci:set("simple-tc", "mesh_vpn", "interface")
    uci:set("simple-tc", "mesh_vpn", "enabled", data._limit_enabled_t)
    uci:set("simple-tc", "mesh_vpn", "ifname", "mesh-vpn")

    if data._limit_ingress_t ~= nil then
      uci:set("simple-tc", "mesh_vpn", "limit_ingress", data._limit_ingress_t)
    end

    if data._limit_egress_t ~= nil then
      uci:set("simple-tc", "mesh_vpn", "limit_egress", data._limit_egress_t)
    end

    uci:save("simple-tc")
    uci:commit("simple-tc")
  end
end

return M
