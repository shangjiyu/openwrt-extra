
local wa = require "luci.tools.webadmin"
local fs = require "nixio.fs"

m = Map("qos_gargoyle", translate("Global"),translate("Global set"))


s = m:section(TypedSection, "global", translate("Global"), translate("<b><font color=\"#FF0000\" size=\"4\"></font></b>"))

s.anonymous = false
--s.sortable  = true
local count = 0
for line in io.lines("/etc/config/qos_gargoyle") do
	line = string.match(line, "config ['\"]*global['\"]* ")
	if line ~= nil then
		count = count + 1
	end
end
if count == 0 then
	os.execute("echo \"\nconfig global 'global'\" >> /etc/config/qos_gargoyle")
end


mtu = s:option(Value, "mtu", translate("mtu"))
mtu.datatype = "and(uinteger,min(1))"

network = s:option(Value, "network", translate("network"),translate("wan,lan....."))
network.default = ""
wa.cbi_add_networks(network)

interface=s:option(Value, "interface", translate("interface"),translate("eth0,eth1,wlan0....."))
interface.anonymous = true
interface.rmempty = true
interface.default = ""
for k, v in pairs(luci.sys.net.devices()) do
	interface:value(v)
end


s = m:section(TypedSection, "upload", translate("UpLoad"))

monenabled = s:option(ListValue, "monenabled", translate("enable upload qos"))
monenabled:value("false")
monenabled:value("true")
monenabled.default = "false"

uclass = s:option(Value, "default_class", translate("default_class"))
uclass.rmempty = "true"
a = m.uci:get("qos_gargoyle", "upload", "default_class")
if a then
	if m.uci:get("qos_gargoyle", a) then
	else
		m.uci:set("qos_gargoyle", "upload", "default_class", "")
	end
end
for line in io.lines("/etc/config/qos_gargoyle") do
	local str = line
	line = string.gsub(line, "config ['\"]*upload_class['\"]* ", "")
	if str ~= line then
		line = string.gsub(line, "^'", "")
		line = string.gsub(line, "^\"", "")
		line = string.gsub(line, "'$", "")
		line = string.gsub(line, "\"$", "")
		if m.uci:get("qos_gargoyle", line, "name") then
			uclass:value(line, translate(m.uci:get("qos_gargoyle", line, "name")))
		end
	end
end

tb = s:option(Value, "total_bandwidth", translate("total_bandwidth"), translate("kbit/s"))
tb.datatype = "and(uinteger,min(1))"


s = m:section(TypedSection, "download", translate("DownLoad"))

monenabled = s:option(ListValue, "monenabled", translate("enable upload qos"))
monenabled:value("false")
monenabled:value("true")
monenabled.default = "false"

dclass = s:option(Value, "default_class", translate("default_class"))
dclass.rmempty = "true"
if m.uci:get("qos_gargoyle", "download", "default_class") then
	if m.uci:get("qos_gargoyle", m.uci:get("qos_gargoyle", "download", "default_class")) then
	else
		m.uci:set("qos_gargoyle", "download", "default_class", "")
	end
end

for l in io.lines("/etc/config/qos_gargoyle") do
	local s = l
	l = string.gsub(l, "config ['\"]*download_class['\"]* ", "")
	if s ~= l then
		l = string.gsub(l, "^'", "")
		l = string.gsub(l, "^\"", "")
		l = string.gsub(l, "'$", "")
		l = string.gsub(l, "\"$", "")
		if m.uci:get("qos_gargoyle", l, "name") then
			dclass:value(l, translate(m.uci:get("qos_gargoyle", l, "name")))
		end
	end
end

tb = s:option(Value, "total_bandwidth", translate("total_bandwidth"), translate("kbit/s"))
tb.datatype = "and(uinteger,min(1))"

return m