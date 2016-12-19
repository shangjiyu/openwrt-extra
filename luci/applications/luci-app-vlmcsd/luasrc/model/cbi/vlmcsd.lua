require("luci.sys")
require("nixio.fs")

local fexist = nixio.fs.access("/etc/config/vlmcsd")
if not fexist then
	local f = io.open("/etc/config/vlmcsd", "w+")
	if f then
		f:write("config vlmcsd Global config\n\toption port 1680\n")
		f:close()
	end
end

m=Map("vlmcsd", translate("vlmcsd"),
    translate("Configure vlmcsd service (Windows/Office KMS activation service)."))

s=m:section(TypedSection, "vlmcsd", translate("Settings"))
s.annonymous=true
s:option(Flag, "enabled", translate("Enable"))
port =s:option(Value, "port", translate("Port"))
port.rmempty = true
s:option(Flag, "useepifile", translate("Use File"), translate("Use vlmcsd.ini file, not required"))

local apply = luci.http.formvalue("cbi.apply")
if apply then
        io.popen("/etc/init.d/vlmcsd restart")
end

return m
