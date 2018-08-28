clearListener()

fileIn "utils.ms"

struct TSceneImporter (
	pathToFile = "",

	-- Finds closing position of a pair of tokens.
	fn findClosing str openPos tokenOpen tokenClose = (
	    local closePos = openPos
	    local counter = 1
	    while counter > 0 do (
	    	closePos += 1
	    	if closePos > str.count then return -1
	        c = str[closePos]
	        if c == tokenOpen then counter += 1
	        else if c == tokenClose then counter -= 1
	    )
	    return closePos
	),

	-- Returns the string between two tokens (tokens not included).
	fn findBetweenTokens str tokenOpen tokenClose = (
		local idxOpen = findString str tokenOpen
		if idxOpen == undefined then return ""

		local idxClose = findClosing str idxOpen tokenOpen tokenClose
		if idxClose < 0 then idxClose = str.count + 1

		return substring str (idxOpen + 1) (idxClose - idxOpen - 1)
	),

	-- Returns array of JSON objects.
	fn getAllJObjects str = (
		local idx = 1
		local strLen = str.count
		local objArr = #()
		local objStr = str
		while idx <= strLen and
			  objStr != "" do (
			objStr = findBetweenTokens (substring str idx -1) "{" "}"
			idx += objStr.count + 1
			if objStr != "" then append objArr objStr
		)
		return objArr
	),

	-- Finds JSON element value in an array from filterString "\"".
	fn findJValue arr id = (
		local idx = findItem arr id
		if idx > 0 then return arr[idx + 2]
		else return ""
	),

	-- Returns string with full file contents.
	fn getFileContents pathToFile = (
		file = openFile pathToFile
		local fileData = ""
		if file != undefined then (
			seek file #eof
			maxlen = filepos file
			seek file 0
			fileData = readChars file maxlen errorAtEOF:false
			
			close file
		)
		return fileData
	),

	fn createObject name pos:"0 0 0" rot:"0 0 0 1" = (
		obj = Sphere()
		obj.radius = 0.5
		obj.name = name
		obj.rotation = strRotToRot rot
		obj.pos = strPosToPos pos
	),
	
	fn importScene = (
		--pathToFile = "D:/Dev/MomentumEngine-Real/Bin/data/scenes/testExporter-0.scene" --DEBUG
		jData = getFileContents pathToFile

		jObjects = getAllJObjects jData
		for jObj in jObjects do (
			jArray = filterString jObj "\""
			--format (jObj + "---end\n")
			local objName = findJValue jArray "name"
			local objPos  = findJValue jArray "pos"
			local objRot  = findJValue jArray "rotation"
			createObject objName pos:objPos rot:objRot
			format "Imported:\n" --DEBUG
			format ("name: " 	 + findJValue jArray "name" 	+ "\n") --DEBUG
			format ("pos: "  	 + findJValue jArray "pos" 		+ "\n") --DEBUG
			format ("rotation: " + findJValue jArray "rotation" + "\n") --DEBUG
			format "---------------------------\n" --DEBUG
		)
	)
)


