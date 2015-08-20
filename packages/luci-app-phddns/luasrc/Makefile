LUAC = luac
LUAC_OPTIONS = -s

world: compile

compile:
	for i in $$(find -name *.lua -not -name debug.lua); do $(LUAC) $(LUAC_OPTIONS) -o $$i $$i; done

