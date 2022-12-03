local cbi = require "luci.cbi"
local i18n = require "luci.i18n"
local uci = require('simple-uci').cursor()

local M = {}

function M.section(form)
  local msg = ''
  local msg = i18n.translate('You can set the WAN port fixed MAC address.')
  local s = form:section(cbi.SimpleSection, nil, msg)
  
  local o

  o = s:option(cbi.Flag, "_fixedmac", i18n.translate("Enable fixed WAN mac address"))
  o.default = uci:get_bool("fixedmacs", "wan", "enabled") and o.enabled or o.disabled
  o.rmempty = false

  o = s:option(cbi.Value, "_fixedmac_address", i18n.translate("WAN port MAC address"))
  o:depends("_fixedmac", "1")
  o.default = uci:get("fixedmacs", "wan", "macaddr")
  o.rmempty = false
  o.datatype = "string"
  o.description = i18n.translate("xx:xx:xx:xx:xx:xx")
  o.maxlen = 140

end

function M.handle(data)
  
  if data._fixedmac ~= nil then
    uci:set("fixedmacs", "wan", "enabled", data._fixedmac)
    if data._fixedmac == '1' then
      if data._fixedmac_address ~= nil then
        if data._fixedmac_address ~= '' then
          uci:set("fixedmacs", "wan", "macaddr", data._fixedmac_address)
          uci:set("network", "wan", "macaddr", data._fixedmac_address)
        else
          uci:set("fixedmacs", "wan", "enabled", '0')  
          uci:delete('fixedmacs', 'wan', 'macaddr')
        end
      else
        uci:set("fixedmacs", "wan", "enabled", '0')
        uci:delete('fixedmacs', 'wan', 'macaddr')
      end
    end
  end
  uci:save("fixedmacs")
  uci:commit("fixedmacs")
end

return M
