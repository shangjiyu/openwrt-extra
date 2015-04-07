--[[
LuCI - Lua Configuration Interface
youku for KOS
$Id$
]]--
local kosqd = luci.http.formvalue("cbi.apply")
m = Map("youku", translate("优酷路由宝"), translate("买个优酷路由宝，躺着赚钱就是屌。"))
s = m:section(TypedSection, "youku", translate("屌宝"))
s.anonymous = true
o = s:option(Flag, "enable", translate("是否启用框机"))
o = s:option(Flag, "oksn", translate("是否使用自己的SN"), translate("不勾选则根据mac算出唯一的SN"))
o = s:option(Value, "opsn", translate("原版SN"))
o:depends("oksn","1")
o = s:option(ListValue, "wkmod", translate("挖矿模式"))
o:value("0", translate("激进模式"))
o:value("2", translate("平衡模式"))
o:value("3", translate("保守模式"))
pth = s:option(ListValue, "path", translate("缓存文件路径"))
pth:value("", translate("-- Please choose --"))
local p_user
for _, p_user in luci.util.vspairs(luci.util.split(luci.sys.exec("df|grep '/mnt/'|awk '{print$6}'"))) do
	pth:value(p_user)
end
o = s:option(Value, "cqboot", translate("定时重启"), translate("定时重启，可以自定义重启时间，例：3点重启就输入0300即可，5点半重启就输入0530即可."))
o:value("", translate("不重启"))
o:value("0100", translate("1点整重启"))
o:value("0245", translate("2点45重启"))
o:value("0300", translate("3点重启"))
if kosqd then
luci.sys.call("killall -9 ikuacc youkudome")
luci.sys.call("/lib/youkudome&")
end
return m
