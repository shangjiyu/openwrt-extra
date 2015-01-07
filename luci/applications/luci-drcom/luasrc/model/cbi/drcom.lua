--[[
ZJUT Drcom Cliet Luci configration page. Made By Shangjiyu
]]--

local fs = require "nixio.fs"

local drcom =(luci.sys.call("ps | grep "drcom" | grep -v "grep" | awk '{print $5}' > /dev/null") == 0)
if drcom then	
	m = Map("drcom", translate("Dr.com"), translate("Dr.com is running"))
else
	m = Map("drcom", translate("Dr.com"), translate("Dr.com is not running"))
end

s = m:section(TypedSection, "drcom", "")
s.anonymous = true

switch = s:option(Flag, "enabled", translate("Enable"))
switch.rmempty = false

editconf = s:option(Value, "_data", " ")
editconf.template = "cbi/tvalue"
editconf.rows = 25
editconf.wrap = "off"

function editconf.cfgvalue(self, section)
	return fs.readfile("/etc/drcom.conf") or ""
end
function editconf.write(self, section, value)
	if value then
		value = value:gsub("\r\n?", "\n")
		fs.writefile("/tmp/drcom.conf", value)
		if (luci.sys.call("cmp -s /tmp/drcom.conf /etc/drcom.conf") == 1) then
			fs.writefile("/etc/drcom.conf", value)
		end
		fs.remove("/tmp/drcom.conf")
	end


return m


