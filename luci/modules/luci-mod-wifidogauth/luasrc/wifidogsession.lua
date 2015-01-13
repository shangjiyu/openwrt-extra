--[[
Copy from sauth.lua and modify.
]]--

module("luci.wifidogsession", package.seeall)
require("luci.util")
require("luci.sys")
local wdcfg = require("luci.wifidogconfig")
local nixio = require "nixio", require "nixio.util"
local fs = require "nixio.fs"

spath = wdcfg.auth.sessionpath
timeout = tonumber(wdcfg.auth.sessionexpire)

function sane(file)
	return fs.stat(file or spath, "modestr") ==
		(file and "rw-------" or "rwx------")
end

function prepare()
	fs.mkdir(spath, 700)
	if not sane() then
		error("Security Exception: Session path is not sane!")
	end
end

function kill(id)
	fs.unlink(spath .. id)
end

local function _checkid(id)
	return not not (id and #id == 32 and id:match("^[a-fA-F0-9]+$"))
end

local function _read(id)
	if not id or #id == 0 or not _checkid(id) then
		return nil
	end

	if not sane(spath .. id) then
		return nil
	end

	local blob = fs.readfile(spath .. id)
	local func = loadstring(blob)
	setfenv(func, {})

	local sess = func()
	assert(type(sess) == "table", "Session data invalid!")

	if sess.atime and sess.atime + timeout < luci.sys.uptime() then
		kill(id)
		return nil
	end

	return sess
end

function checktimeout()
	if sane() then
		local id
		for id in nixio.fs.dir(spath) do
			if not _read(id) then kill(id) end
		end
	end
end

local function _write(id, data)
	local f = nixio.open(spath .. id, "w", 600)
	f:writeall(data)
	f:close()
end

function write(id, data)
	if not sane() then
		prepare()
	end

	assert(type(data) == "table", "Security Exception: Session data invalid!")
	data.atime = luci.sys.uptime()
	_write(id, luci.util.get_bytecode(data))
	checktimeout()
end

function read(id)
	local sess = _read(id)
	if sess then write(id, sess) end
	return sess
end
