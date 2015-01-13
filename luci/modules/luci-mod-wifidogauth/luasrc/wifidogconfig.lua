local util = require "luci.util"
module("luci.wifidogconfig",
		function(m)
			if pcall(require, "luci.model.uci") then
				local config = util.threadlocal()
				setmetatable(m, {
					__index = function(tbl, key)
						if not config[key] then
							config[key] = luci.model.uci.cursor():get_all("wifidogauth", key)
						end
						return config[key]
					end
				})
			end
		end)
