-----------------------------------------------
-- Some generally useful  functions.
-----------------------------------------------

fn cleanMaterialName oldName = (
	return substituteString oldName " " "_"
)

fn isValidName aname = (
	return findString aname " " == undefined
)

fn stringContainsSubstring strng substrng = (
	matchpattern strng pattern:("?*" + substrng + "*")
)

-- Converts a very small number to zero.
fn smallNumToZero num = (
	if (num <  0.00001 and num > 0) or
	   (num > -0.00001 and num < 0) then return 0
	else return num
)

fn roundSmallPosToZeros pos = (
	local auxPos = pos
	auxPos.x = smallNumToZero pos.x
	auxPos.y = smallNumToZero pos.y
	auxPos.z = smallNumToZero pos.z
	return auxPos
)

fn roundSmallQuatToZeros q = (
	local auxQuat = q
	auxQuat.x = smallNumToZero q.x
	auxQuat.y = smallNumToZero q.y
	auxQuat.z = smallNumToZero q.z
	auxQuat.w = smallNumToZero q.w
	return auxQuat
)
