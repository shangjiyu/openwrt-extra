local m, s, o
m = Map("cdns", translate("CureDNS"), translate("CureDNS is an experimental tool to cure your poisoned DNS."))

s = m:section(TypedSection, "cdns", translate("General Settings"))
s.anonymous = true

s:option(Flag, "enabled", translate("Enable"))
s:option(Flag, "log_debug", translate("Debug Logging"))
o = s:option(Value, "listen_ip", translate("Listen IP"))
o.datatype = "ipaddr"
o.default = '127.0.0.1'

o = s:option(Value, "listen_port", translate("Listen Port"))
o.datatype = "port"
o.default = '1153'

o = s:option(Value, "timeout", translate("Wait Timeout"))
o.datatype = "uinteger"
o.placeholder = "2"
o.default = 2

s:option(DynamicList, "server", translate("DNS Servers")).empty = false

return m
