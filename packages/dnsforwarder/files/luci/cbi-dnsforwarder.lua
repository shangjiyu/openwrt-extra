--[[
LuCI - Lua Configuration Interface

Copyright (C) 2014-2015 MATTHEW728960

This is free software, licensed under the GNU General Public License v2.
See /LICENSE for more information.

$Id$
]]--

local fs = require "nixio.fs"
local util = require "nixio.util"

local running=(luci.sys.call("pidof dnsforwarder > /dev/null") == 0)
if running then	
	m = Map("dnsforwarder", translate("dnsforwarder"), 
		translate("A DNS Cache Server with TCP ,UDP & GfwList. Use 'dnsforwarder -P' to find fake IP.   dnsforwarder is running（Refresh the page！）"))

else
	m = Map("dnsforwarder", translate("dnsforwarder"), 
		translate("A DNS Cache Server with TCP ,UDP & GfwList. Use 'dnsforwarder -P' to find fake IP.   dnsforwarder is not running（Refresh the page！）"))
end

b = m:section(TypedSection, "dnsforwarder", "Basic")
b.addremove = false
b.anonymous = true
en = b:option(Flag, "enabled", translate("Enable"))
en.rmempty = false

function en.cfgvalue(self, section)
	return luci.sys.init.enabled("dnsforwarder") and self.enabled or self.disabled
end

function en.write(self, section, value)
	if value == "1" then
		luci.sys.call("/etc/init.d/dnsforwarder enable >/dev/null")
		luci.sys.call("/etc/init.d/dnsforwarder start >/dev/null")
	else
		luci.sys.call("/etc/init.d/dnsforwarder stop >/dev/null")
		luci.sys.call("/etc/init.d/dnsforwarder disable >/dev/null")
	end
	Flag.write(self, section, value)
end

path = b:option(Value, "path", translate("Path"))
path.default = "/tmp/dnsforwarder.config"
path.optional = false

s = m:section(TypedSection, "dnsforwarder", translate("Configuration"))
s.anonymous = true

conf = s:tab("editconf_file", translate("File"))
editconf_file = s:taboption("editconf_file", Value, "_editconf_file", "", translate("Comment by #"))
editconf_file.template = "cbi/tvalue"
editconf_file.rows = 20
editconf_file.wrap = "off"

function editconf_file.cfgvalue(self, section)
	return fs.readfile("/etc/dnsforwarder.config") or ""
end

function editconf_file.write(self, section, value)
	if value then
		value = value:gsub("\r\n?", "\n")
		fs.writefile("/tmp/dnsforwarder.config", value)
		if (luci.sys.call("cmp -s /tmp/dnsforwarder.config /etc/dnsforwarder.config") == 1) then
			fs.writefile("/etc/dnsforwarder.config", value)
		end
		fs.remove("/tmp/dnsforwarder.config")
	end
end

return m
