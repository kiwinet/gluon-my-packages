local cbi = require "luci.cbi"
local i18n = require "luci.i18n"
local uci = luci.model.uci.cursor()

local M = {}

function M.section(form)
  local msg = ''
  local msg = i18n.translate('You can set the WAN port fixed MAC address.')
  local s = form:section(cbi.SimpleSection, nil, msg)
  
  local o

  o = s:option(cbi.Flag, "_fixedmac", i18n.translate("Enable fixed WAN mac address"))
  o.default = uci:get_bool("fixedmacs", "wan", "enabled") and o.disabled or o.enabled
  o.rmempty = false

  o = s:option(cbi.Flag, "_fixedmac_address", i18n.translate("WAN port MAC address (xx:xx:xx:xx:xx:xx)"))
  o:depends("_fixedmac", "1")
  o.default = uci:get("fixedmacs", "wan", "macaddr")
  o.rmempty = false
  o.datatype = "string"

end

function M.handle(data)
  
  if data._fixedmac ~= nil then
     uci:set("fixedmacs", "wan", "enabled", data._fixedmac)
     uci:set("fixedmacs", "wan", "macaddr", data._fixedmac_address)
  end
  uci:save("fixedmacs")
  uci:commit("fixedmacs")
end

return M
