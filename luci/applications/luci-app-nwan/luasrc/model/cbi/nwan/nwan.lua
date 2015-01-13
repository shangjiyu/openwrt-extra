--[[
LuCI - Lua Configuration Interface

Copyright 2011 flyzjhz <flyzjhz@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

]]--

require("luci.tools.webadmin")
local sys = require "luci.sys"

m = Map("nwan", "N-WAN",translate("nwan_desc",
"N-WAN allows for the use of multiple uplinks for load balancing and failover."))

-- translate("nwan_route_desc","setting n-wan routes."))

local iproute = luci.sys.iproute()

v = m:section(Table, iproute, translate("ip route"))
ipmas = v:option(DummyValue, "ipmas")
dev1 = v:option(DummyValue, "dev1")
dev2 = v:option(DummyValue, "dev2")
proto = v:option(DummyValue, "proto")
kernel = v:option(DummyValue, "kernel")
scope = v:option(DummyValue, "scope")
link = v:option(DummyValue, "link")
src = v:option(DummyValue, "src")
ipadd = v:option(DummyValue, "ipadd")


s = m:section(TypedSection, "settings", translate("nwan_route","SETTING PAGE"))
s.addremove = false
s.anonymous = true

enable = s:option(Flag, "enable", translate("nwan_enable","N_WAN ON OR OFF"))
enable.optional = false
enable.rmempty = false

ping_ck = s:option(Flag, "ping_ck", translate("nwan_ping_ck","ON_LINE CHECK ON OR OFF"))
ping_ck.optional = false
ping_ck.rmempty = false

force_all_wan_up = s:option(Flag, "force_all_wan_up", translate("force_all_wan_up","FORCE ALL WAN UP"),
                   translate("force_all_wan_up_desc","force all wan up desc"))
force_all_wan_up:depends("ping_ck", "1")
force_all_wan_up.optional = false
force_all_wan_up.rmempty = false


dl_route_table = s:option(Flag, "dl_route_table", translate("nwan_dl_route_table","AUTO DOWNLOAD ROUTE TABLE"),
         translate("nwan_dl_route_table_desc","nwan dl route table desc"))
dl_route_table.optional = false
dl_route_table.rmempty = false


debug = s:option(ListValue, "debug", translate("nwan_debug","DEBUG  LOG_RECORD"))
debug :value("1", translate("nwan_off","OFF"))
debug :value("5", translate("nwan_on","ON"))
debug.optional = false
debug.rmempty = false

sleeptime = s:option(Value, "sleeptime", translate("nwan_sleeptime","PING WAIT TIME / MIN"))
sleeptime.size = 3
sleeptime.maxlength = 3
sleeptime.optional = false
sleeptime.rmempty = false

testip = s:option(Value, "testip", translate("nwan_testip","INTETNET TEST IP"))
testip.size = 55
testip.maxlength = 55
testip.optional = false
testip.rmempty = false

int = m:section(TypedSection, "interface", translate("nwan_interface","WAN Interfaces"))
int.template = "cbi/tblsection"
int.addremove = true

name = int:option(ListValue, "name", translate("nwan_isp_name","isp name"))
name:value("mobile", translate("nwan_isp_mobile","mobile"))
name:value("other", translate("nwan_isp_other","other"))
name:value("telecom", translate("nwan_isp_telecom","telecom"))
name:value("unicom", translate("nwan_isp_unicom","unicom"))
name.default = "telecom"
name.optional = false
name.rmempty = true
name.widget = "select"



route = int:option(ListValue, "route", translate("nwan_route_method","wan route method"))
route:value("intelligent_routes", translate("nwan_intelligent_routes","intelligent routes"))
route:value("balance", translate("nwan_balance","balance"))
route.default = "balance"
route.optional = false
route.rmempty = true
route.widget = "select"

weight = int:option(ListValue, "weight", translate("nwan_weight","Load Balancer weight"))
weight:value("10", "10")
weight:value("9", "9")
weight:value("8", "8")
weight:value("7", "7")
weight:value("6", "6")
weight:value("5", "5")
weight:value("4", "4")
weight:value("3", "3")
weight:value("2", "2")
weight:value("1", "1")
weight.default = "1"
weight.widget = "select"
weight.optional = false
weight.rmempty = true




s = m:section(TypedSection, "mwanfw", translate("nwan_mwanfw","N-WAN ASSIGN OUT Rules"),	
	translate("nwan_mwanfw_desc","Configure rules for directing outbound traffic through specified WAN Uplinks."))
s.template = "cbi/tblsection"
s.anonymous = true
s.addremove = true
s.sortable  = true

enable = s:option(Flag, "enable", translate("enable", "enable"))
enable.default = false
enable.optional = false
enable.rmempty = false


src = s:option(Value, "src", translate("nwan_src","Source Address"))
src.rmempty = true
src.default = "all"
src:value("all", translate("all"))
luci.tools.webadmin.cbi_add_knownips(src)

dst = s:option(Value, "dst", translate("nwan_dst","Destination Address"))
dst.rmempty = true
dst:value("all", translate("all"))
dst.default = "all"
luci.tools.webadmin.cbi_add_knownips(dst)

proto = s:option(Value, "proto", translate("nwan_proto","Protocol"))
proto.rmempty = true
proto:value("all", translate("all"))
proto:value("tcp_udp", translate("tcp/udp"))
local pats = io.popen("cat /etc/protocols|awk {'print $1 '}|grep -v \"#\"|tail -n +2")
if pats then
	local l
	while true do
		l = pats:read("*l")
		if not l then break end

		if l then
			proto:value(l)
		end
	end
	pats:close()
end


ports = s:option(Value, "ports", translate("nwan_ports","Ports"))
ports:depends( "proto" , "tcp" )
ports:depends( "proto" , "udp" )
ports:depends( "proto" , "tcp_udp" )
ports:value("all", translate("all", translate("all")))
ports:value("80", "80")
ports:value("8000", "8000")
ports:value("443", "443")
ports.rmempty = true


wanrule = s:option(Value, "wanrule", translate("nwan_wanrule","WAN Uplink"))
luci.tools.webadmin.cbi_add_networks(wanrule)
wanrule:value("wan", translate("wan"))
wanrule.optional = false
wanrule.rmempty = true
wanrule.default = "wan"


return m


