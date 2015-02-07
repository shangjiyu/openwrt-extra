--[[
Luci configration for dns2sock.Made By shangjiyu
]]--

local fs = require "nixio.fs"
local util = require "nixio.util"

local running=(luci.sys.call("pidof adbyby > /dev/null") == 0)
if running then	
	m = Map("adbyby", translate("ADBYBY"), translate("ADBYBY is running"))
else
	m = Map("adbyby", translate("ADBYBY"), translate("ADBYBY is not running"))
end

en = m:section(TypedSection, "adbyby", translate("Basic"))
en.anonymous = true
enable = en:option(Flag, "enabled", translate("Enable"))
enable.rmempty = false
path = en:option(Value, "path", translate("Path"))
path.default = "/usr/bin/adbyby"
path.optional = false

s = m:section(TypedSection, "adbyby", translate("Rules"))
s.anonymous = true

user = s:tab("editconf_user", translate("User Rules"))
editconf_user = s:taboption("editconf_user", Value, "_editconf_user", "", translate("Comment by !"))
editconf_user.template = "cbi/tvalue"
editconf_user.rows = 20
editconf_user.wrap = "off"

function editconf_user.cfgvalue(self, section)
	return fs.readfile("/usr/bin/data/user.txt") or ""
end

function editconf_user.write(self, section, value)
	if value then
		value = value:gsub("\r\n?", "\n")
		fs.writefile("/tmp/user.txt", value)
		if (luci.sys.call("cmp -s /tmp/user.txt /usr/bin/data/user.txt") == 1) then
			fs.writefile("/usr/bin/data/user.txt", value)
		end
		fs.remove("/tmp/user.txt")
	end
end

return m
