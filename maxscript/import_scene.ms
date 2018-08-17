clearListener()

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
)

-- Returns the string between two tokens.
fn findBetweenTokens str tokenOpen tokenClose = (
	local idxOpen = findString str tokenOpen
	if idxOpen == undefined then return ""

	local idxClose = findClosing str idxOpen tokenOpen tokenClose
	if idxClose < 0 then idxClose = str.count + 1

	return substring str (idxOpen + 1) (idxClose - idxOpen - 1)
)

fn findJValue arr id = (
	local idx = findItem arr id
	if idx > 0 then return arr[idx + 2]
	else return ""
)

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
)

pathToFile = "D:/Dev/MomentumEngine-Real/Bin/data/scenes/test_spikes.scene"
jData = getFileContents pathToFile

format jData --DEBUG

/*
json = python.import #json
decoded = json.loads(json.dumps(jData))
*/

jEntity = findBetweenTokens jData "{" "}"

jArray = filterString jEntity "\""
format "---------------------------\n"
for i = 1 to jArray.count do format (jArray[i] + "---end\n")

format ("name: " + findJValue jArray "name" + "\n")
format ("pos: "  + findJValue jArray "pos" + "\n")
	
	
