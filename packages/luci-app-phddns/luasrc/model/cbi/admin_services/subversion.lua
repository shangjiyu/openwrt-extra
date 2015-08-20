local sys = require "luci.sys"
local datatypes = require "luci."
local uci    = require "luci.model.uci"
local cur = uci.cursor();
local http = require "luci.http";
local _ = luci.i18n.translate

m = Map("subversion", "Subversion Server",
		_("This page allows you to set Subversion Server."))

m.on_after_commit = function(self)
	luci.sys.call("/etc/init.d/subversion restart >/dev/null") 
end

s = m:section(TypedSection, "server")
s.anonymous = true
s.addremove = false


o = s:option( Flag , "enabled", translate("Enable"))

o = s:option(Value, "root", translate("Svn Root"))
o.datatype="string"
o:depends("enabled", "1")

o = s:option(Value, "addr", translate("Listen Address"))
o.datatype="ipaddr"
o:depends("enabled", "1")

o = s:option(Value, "port", translate("Listen Port"))
o.datatype="port"
o:depends("enabled", "1")

o = s:option(ListValue, "memcache", translate("Cache Size"))
o.datatype = "list(integer)"
o.optional = true


o:value(4,'4M')
o:value(6,'6M')
o:value(8,'8M')
o:value(10,'10M')
o:value(12,'12M')
o:value(14,'14M')
o:value(16,'16M')

o:depends("enabled", "1")


return m
