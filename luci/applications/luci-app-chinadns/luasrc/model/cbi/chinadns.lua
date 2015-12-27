--[[
RA-MOD
]]--

local fs = require "nixio.fs"

local running=(luci.sys.call("pidof chinadns > /dev/null") == 0)
if running then	
	m = Map("chinadns", translate("chinadns"), translate("chinadns is running"))
else
	m = Map("chinadns", translate("chinadns"), translate("chinadns is not running"))
end

s = m:section(TypedSection, "chinadns", "")
s.anonymous = true

switch = s:option(Flag, "enabled", translate("Enable"))
switch.rmempty = false

dnscpm = s:option(Flag, "dnscpm", translate("DNS compression pointer mutation"))
dnscpm.rmempty = false

bidirectional = s:option(Flag, "bidirectional", translate("DNS bi-directional CHNRoute filter"))
bidirectional.rmempty = false

upstream = s:option(Value, "dns", translate("Upstream DNS Server"))
upstream.optional = false
upstream.default = "114.114.114.114,8.8.4.4"

port = s:option(Value, "port", translate("Port"))
port.datatype = "range(0,65535)"
port.optional = false

delay = s:option(Value, "delay", translate("Delay"))
delay.datatype = "range(0,2)"
delay.optional = false
delay.default = 0.3

--iplist = s:option(Value, "iplist", translate("IP blacklist"), "")
--iplist.template = "cbi/tvalue"
--iplist.size = 30
--iplist.rows = 10
--iplist.wrap = "off"

--function iplist.cfgvalue(self, section)
--	return fs.readfile("/etc/chinadns_iplist.txt") or ""
--end
--function iplist.write(self, section, value)
--	if value then
--		value = value:gsub("\r\n?", "\n")
--		fs.writefile("/etc/chinadns_iplist.txt", value)
--	end
--end

chn = s:option(Value, "chn", translate("CHNRoute"), "")
chn.template = "cbi/tvalue"
chn.size = 30
chn.rows = 10
chn.wrap = "off"

function chn.cfgvalue(self, section)
	return fs.readfile("/etc/ipset/whitelist") or ""
end
function chn.write(self, section, value)
	if value then
		value = value:gsub("\r\n?", "\n")
		fs.writefile("/tmp/whitelist", value)
               if (fs.access("/etc/ipset/whitelist") ~= true or luci.sys.call("cmp -s /tmp/whitelist /etc/ipset/whitelist") == 1) then
                       fs.writefile("/etc/ipset/whitelist", value)
               end
               fs.remove("/tmp/whitelist")
	end
end

return m
