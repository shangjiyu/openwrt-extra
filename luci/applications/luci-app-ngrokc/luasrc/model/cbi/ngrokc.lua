-- Mantainer : maz-1 < ohmygod19993 at gmail dot com >


m = Map("ngrokc", translate("Ngrok"),translate("Secure tunnels to localhost."))
m:section(SimpleSection).template  = "ngrokc_status"

local apply = luci.http.formvalue("cbi.apply")               
if apply then                                                
        os.execute("/etc/init.d/ngrokc reload &")       -- reload configuration
end 
tunnels = m:section(TypedSection, "tunnels")
enabled=tunnels:option(Flag, "enabled", translate("Enable"))
enabled.anonymous = true
enabled.addremove = false
nhost=tunnels:option(Value, "host", translate("Ngrok Host"))
nhost.rmempty = false
nhost.datatype = "host"
hport=tunnels:option(Value, "port", translate("Ngrok Port"))
hport.rmempty = false
hport.datatype = "port"
tunnels:option(Value, "atoken", translate("Auth Token")).rmempty = true
ptype=tunnels:option(ListValue, "type", translate("Type"))
ptype:value("tcp", translate("TCP"))
ptype:value("http", translate("HTTP"))
ptype:value("https", translate("HTTPS"))

lhost=tunnels:option(Value, "lhost", translate("Local Address"))
lhost.rmempty = true
lhost.placeholder="127.0.0.1"
lhost.datatype = "ip4addr"

lport=tunnels:option(Value, "lport", translate("Local Port"))
lport.datatype = "port"
lport.rmempty = false

sdname=tunnels:option(Value, "sdname", translate("Subdomain"))
sdname.datatype = "hostname"
sdname.rmempty = false
sdname:depends("type", "http")
sdname:depends("type", "https")

rport=tunnels:option(Value, "rport", translate("Remote Port"))
rport.datatype = "port"
rport.rmempty = false
rport:depends("type", "tcp")

tunnels.addremove = true
tunnels.anonymous = false



return m
