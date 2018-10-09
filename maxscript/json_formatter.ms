struct TJsonFormatter (
	ofilename,			-- Name of output file.
	fs, 				-- File stream.
	depth_level = 0,	-- To format json tabs.

	-- --------------------
	fn begin new_ofilename = (
		ofilename = new_ofilename
		--fs = listener
		try fs = createFile ofilename
		catch (
			-- Avoid error if the file didn't close properly last time.
			gc()
			fs = createFile ofilename
		)
	),

	-- --------------------
	fn end = (
		if fs != undefined and fs != listener then close fs
		fs = undefined
	),

	-- --------------------
	fn indent = (
		for i = 1 to depth_level do format "  " to:fs
	),

	-- --------------------
	fn beginArray = (
		indent()
		format "[\n" to:fs
		depth_level = depth_level + 1
	),

	fn endArray = (
		format "\n" to:fs
		depth_level = depth_level - 1
		indent()
		format "]" to:fs
	),

	-- --------------------
	fn beginObj = (
		indent()
		format "{\n" to:fs
		depth_level = depth_level + 1
	),

	fn endObj = (
		format "\n" to:fs
		depth_level = depth_level - 1
		indent()
		format "}" to:fs
	),

	fn writeComma = (
		--indent()
		format ",\n" to:fs
	),

	-- --------------------
	-- "key_str":\n
	fn writeKey key_str = (
		indent()
		format "\"%\":\n" key_str to:fs
	),
	
	-- "key_str": {},\n
	fn writeKeyEmpty key_str comma:true= (
		indent()
		format "\"%\": {}" key_str to:fs
		if comma then format ",\n" to:fs
	),
	
	-- --------------------
	fn writeValue val addIndent:false = (
		if addIndent then indent()
		if classOf val == string then (
			format " \"%\"" val to:fs
		) else if classOf val == Point3 then (
			format " \"% % %\"" val.x val.y val.z to:fs
		) else if classOf val == quat then (
			-- Quats rotate in the opposite direction
			format " \"% % % %\"" val.x val.y val.z -val.w to:fs
		) else if val == undefined then (
			format " null" to:fs
		) else (
			format " %" val to:fs
		)
	),
	
	-- "key_str": "val"
	fn writeKeyValue key_str val = (
		indent()
		format "\"%\":" key_str to:fs
		writeValue val
	),

	-- --------------------
	-- "name"
	fn writeName name = (
		indent()
		format "\"%\"" name to:fs
	),

	fn writeDummyEntity prefix = (
		indent()
		format ("{ \"entity\": { \"name\": \"" + prefix + "Dummy\" } },\n") to:fs
	),
	
	fn writeText text newLine:true = (
		if newLine then format "%\n" text to:fs
		else format "%" text to:fs
	),

	fn arrayOfStrings arr suffix:"" = (
		beginArray()
		for idx = 1 to arr.count do (
			if idx != 1 then writeComma()
			writeName (arr[idx] + suffix)
		)
		endArray()
	)
)
