--[[
LuCI - Lua Configuration Interface

Copyright 2011 Copyright 2011 flyzjhz <flyzjhz@gmail.com>


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

]]--


require("luci.tools.webadmin")


local sys = require "luci.sys"

m = Map("qosv4", translate("qosv4 title","QOSv4"),
                translate("qosv4 ip limit desc"))

m.redirect = luci.dispatcher.build_url("admin/network/qosv4")

if not arg[1] or m.uci:get("qosv4", arg[1]) ~= "qos_ip" then
	luci.http.redirect(m.redirect)
	return
end



s = m:section(NamedSection, arg[1], "qos_ip" ,translate("qos black ip","qos black ip"))
s.addremove = false
s.anonymous = false


enable = s:option(Flag, "enable", translate("enable", "enable"))
enable.default = false
enable.optional = false
enable.rmempty = false



limit_ips = s:option(Value, "limit_ips", translate("limit_ips","limit_ips"), translate("limit_ips","limit_ips"))
limit_ips.rmempty = true
luci.tools.webadmin.cbi_add_knownips(limit_ips)

limit_ipe = s:option(Value, "limit_ipe", translate("limitp_ipe","limit_ipe"),translate("limitp_ipe","limit_ipe"))
limit_ipe.rmempty = true
luci.tools.webadmin.cbi_add_knownips(limit_ipe)

DOWNLOADR = s:option(Value, "DOWNLOADR", translate("DOWNLOADR speed","DOWNLOADR speed"),translate("speed desc"))
DOWNLOADR.optional = false
DOWNLOADR.rmempty = false
DOWNLOADR.default = "5"

DOWNLOADC = s:option(Value, "DOWNLOADC", translate("DOWNLOADC speed","DOWNLOADC speed"),translate("speed desc"))
DOWNLOADC.optional = false
DOWNLOADC.rmempty = false
DOWNLOADC.default = "5"

UPLOADR = s:option(Value, "UPLOADR", translate("UPLOADR speed","UPLOADR speed"),translate("speed desc"))
UPLOADR.optional = false
UPLOADR.rmempty = false
UPLOADR.default = "5"

UPLOADC = s:option(Value, "UPLOADC", translate("UPLOADC speed","UPLOADC speed"),translate("speed desc"))
UPLOADC.optional = false
UPLOADC.rmempty = false
UPLOADC.default = "5"

tcplimit = s:option(Value, "tcplimit", translate("tcplimit","tcplimit"),translate("tcplimit desc"))
tcplimit.optional = false
tcplimit.rmempty = false
tcplimit.default = "0"

udplimit = s:option(Value, "udplimit", translate("udplimit","udplimit"),translate("udplimit desc"))
udplimit.optional = false
udplimit.rmempty = false
udplimit.default = "0"

ip_prio = s:option(ListValue, "ip_prio", translate("ip prio","ip prio"),
translate("ip prio desc"," default 5 "))
ip_prio.optional = false
ip_prio.rmempty = false
ip_prio:value("7", "7")
ip_prio:value("6", "6")
ip_prio:value("5", "5")
ip_prio:value("4", "4")
ip_prio:value("3", "3")

return m


