clearlistener()

posArray = #()
rotArray = #()

for o in selection do (
	append posArray o.pos
	append rotArray o.rotation
)

spikes = #("spike_crystal_01",
		   "spike_crystal_02",
		   "spike_crystal_03",
		   "spike_crystal_04",
		   "spike_crystal_05")

for i = 1 to rotArray.count do (
	for spike in spikes do (
		local s = getNodeByName spike
		local c = copy s
		for child in s.children do (
			local ch = copy child
			ch.parent = c
		)
		c.rotation = rotArray[i]
		c.pos = posArray[i]
	)
)
