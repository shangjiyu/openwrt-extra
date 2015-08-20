local sys = require "luci.sys"
local datatypes = require "luci."
local uci    = require "luci.model.uci"
local cur = uci.cursor();
local http = require "luci.http";
local _ = luci.i18n.translate

m = Map("vlmcsd", _("KMS Server"),
		_("This page allows you set you owner KMS Server."))

m.on_after_commit = function(self)
	luci.sys.call("/etc/init.d/vlmcsd restart >/dev/null") 
end

s = m:section(TypedSection, "vlmcsd", translate("KMS Server"))
s.anonymous = true
s.addremove = false

o = s:option(Flag , "enabled", translate("Enable"))

o = s:option(Value, "port", translate("Port"))
o.datatype="port"
o:depends("enabled", "1")

return m
