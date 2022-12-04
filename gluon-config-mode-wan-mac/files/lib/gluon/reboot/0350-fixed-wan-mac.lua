#!/usr/bin/lua

local uci = require('simple-uci').cursor()

local wan_mac_enabled = uci:get_bool('fixedmacs', 'wan', 'enabled')
local wan_macaddress = uci:get('fixedmacs', 'wan', 'macaddr')
local wan_base_macaddress = uci:get('fixedmacs', 'wan', 'base_macaddr')
local wan_net_macaddress = uci:get('network', 'wan', 'macaddr')

if wan_base_macaddress == nil and wan_net_macaddress ~= nil then
  uci:set('fixedmacs', 'wan', 'base_macaddr', wan_net_macaddress)
  uci:set('network', 'wan', 'macaddr', wan_net_macaddress)
  uci:set('fixedmacs', 'wan', 'enabled', false)

  uci:save('fixedmacs')
  uci:save('network')
  uci:commit('fixedmacs')
  uci:commit('network')
  wan_base_macaddress = wan_net_macaddress
end