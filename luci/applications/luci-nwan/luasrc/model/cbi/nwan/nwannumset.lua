require("luci.tools.webadmin")

--[[
config macvlan_numset
	option enable   1
	option wan_start_idx 2
	option macvlan_num 4
	option macvlan_switch 1 

config 'switch_numset'
  option 'route_modle' 'db120'
	option 'line_num' '2'
	option 'enable' '0'
	
config 'wireless_numset'
        option 'enable' '0'
        option 'channel' '11'
        option 'hwmode' '11g'
        option 'encryption' 'none'
        option 'device' 'radio0'
        option 'ssid' 'MECURY'
        option 'mode' 'sat'
]]--

	local isddnas = luci.util.exec("cat /proc/cpuinfo|grep -c DDNAS") or 0 
	local isddnas = tonumber(isddnas) 

m = Map("nwannumset", translate("nwan_numset_title","Set Virtual WAN Number"),
                    translate("nwan_numset_title_desc","Set Virtual WAN Number and wireless mode"))


if isddnas < 1 then 
s = m:section(TypedSection, "switch_numset", translate("switch_numset","line totle number setting"),
                translate("switch_numset_desc","line num setting select "))
s.anonymous = true
s.addremove = false

enable = s:option(Flag, "enable", translate("switch_enable", "enable switch wan settings"))
enable.default = false
enable.optional = false
enable.rmempty = false

route_modle = s:option(ListValue, "route_modle", translate("route_modle","route modle"),
          translate("route_modle_desc","  db120,rg100a, 47xx i.e 614 500u 7231-4P,71xx i.e 941N v2 841N V3."))
route_modle:value("rg100a", translate("rg100a","rg100a"))
route_modle:value("db120", translate("db120","db120"))
route_modle:value("47xx", translate("47xx","47xx"))
route_modle:value("71xx", translate("71xx","71xx"))
route_modle.default = "47xx"
route_modle.optional = false
route_modle.rmempty = false

line_num= s:option(ListValue, "line_num", translate("line_num","line num"),
                     translate("line_num_desc","line num"))
line_num:value("4", "4")
line_num:value("3", "3")
line_num:value("2", "2")
line_num:value("1", "1")
line_num.default = 1
line_num.optional = false
line_num.rmempty = false

end

s = m:section(TypedSection, "macvlan_numset", translate("nwan_macvlan_numset","settings"),
translate("nwan_numset_desc","Set virtual wan number by macvlan "))
s.anonymous = true
s.addremove = false

enable = s:option(Flag, "enable", translate("nwan_enable_macvlan", "enable virtual wan settings"))
enable.default = false
enable.optional = false
enable.rmempty = false

enable = s:option(Flag, "macvlan_switch", translate("virtual_wan_macvlan", "virtual wan use macvlan"))
enable.default = false
enable.optional = false
enable.rmempty = false

macvlan_num = s:option(Value, "macvlan_num", translate("nwan_macvlan_num","macvlan num"),
translate("nwan_macvlan_num_desc","vitual num"))
macvlan_num.default = 3
macvlan_num.optional = false
macvlan_num.rmempty = false

wan_start_idx = s:option(ListValue,"wan_start_idx", translate("wan_start_idx","wan start line"),
                        translate("wan_start_idx_desc","wan start line no"))
wan_start_idx:value("0", "wan")
wan_start_idx:value("100", "wan100")
wan_start_idx:value("200", "wan200")
wan_start_idx:value("300", "wan300")
wan_start_idx.default = 1
wan_start_idx.optional = false
wan_start_idx.rmempty = false

return m

