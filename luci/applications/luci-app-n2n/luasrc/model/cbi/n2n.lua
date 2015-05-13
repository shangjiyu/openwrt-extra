--[[
Lua Configuration Interface for N2N. 

config setting
       option enable            '1'
        option server_cline     '1'

config edge
        option ipaddr           '0.0.0.0'
        option supernode        '88.86.108.50'
        option port             '82'
        option community        'openwrt'     
        option key              '1234'      
        option route            '1'



]]--



m = Map("n2n", translate("N2N VPN Server"),translate("This is N2N VPN Server ."))

s = m:section(TypedSection, "setting", translate("setting"))
s.addremove = false
s.anonymous = true

enable = s:option(Flag, "enable", translate("enable"),translate("Enable or Disable N2N Server."))

server_cline = s:option(Flag, "server_cline", translate("N2N VPN Client mode"),translate("Client,select as Client."))

s = m:section(TypedSection, "edge", translate("N2N conf"))
s.addremove = false
s.anonymous = true


ipaddr = s:option(Value, "ipaddr", translate("ipaddr"),translate("ipaddr,;default: 10.0.0.1"))
ipaddr.rmempty = true

supernode = s:option(Value, "supernode", translate("supernode IP"),translate("N2N VPN Server IP supernode addr. ; default: 88.86.108.50"))
supernode.rmempty = true

port = s:option(Value, "port", translate("supernode port"),translate("N2N  VPN supernode port.; default: 82"))
port.rmempty = true

community = s:option(Value, "community", translate("community"),translate("community name ; default: openwrt"))
community.rmempty = true


key = s:option(Value, "key", translate("community key"),translate("community key,default: 1234"))
key.rmempty = true
key.password = true

route = s:option(Flag, "route", translate("route"),translate("route"))
route.rmempty = true

mtu = s:option(Value, "mtu", translate("mtu"),translate("mtu; default: 1300"))
mtu.rmempty = true


s = m:section(TypedSection, "supernode", translate("supernode"),translate("supernode desc"))
s.addremove = false
s.anonymous = true

enable = s:option(Flag, "enable", translate("N2N VPN supernode Server"),translate("Enable or Disable N2N supernode Server."))

port = s:option(Value, "port", translate("supernode port"),translate("N2N VPN supernode port.; default: 82"))
port.rmempty = true

return m
