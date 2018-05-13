-----------------------------------------------
-- Some generally useful  functions.
-----------------------------------------------

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

fn maxPosToMcv p = (
	return roundSmallPosToZeros (p * rotateXMatrix -90)
)

fn mcvPosToMax p = (
	return p * rotateXMatrix 90
)

fn maxRotToMcv obj = (
	-- From mcv, we will go to max, apply the max transform and go back to mcv coord system.
	local mcv_transform = rotateXMatrix 90 * obj.transform * rotateXMatrix -90

	-- Take just the rotation as quaternion
	return roundSmallQuatToZeros (mcv_transform.rotationPart as quat)
)

/*fn mcvRotToMax q = (
	return 
)*/

fn getPosRelativeToParent child = (
	if child.parent == undefined then return child.pos
	else return in coordsys child.parent child.pos
)

-- Returns the angle between two vectors (from 0 to 180 degrees).
-- TODO: Esto debería devolver el ángulo en negativo si se da el caso (añadiendo referencia).
fn getVecAngle v1 v2 = (
	local n1 = normalize v1
	local n2 = normalize v2
	local crossProd = cross v1 v2
	--return atan2 (dot (cross v2 v1) (normalize crossProd)) (dot v1 v2)
	return acos(dot n1 n2)
)

-- Returns the angle between 3 points (from 0 to 180 degrees).
fn get3pointAngle p1 p2 p3 = (
	local v1 = p2 - p1
	local v2 = p3 - p1
	return getVecAngle v1 v2
)

fn cleanMaterialName oldName = (
	local cleanName = substituteString oldName " " "_"
	return substituteString cleanName "ñ" "n"
)

fn isValidName aname = (
	return (findString aname " " == undefined) or
		   (findString aname "ñ" == undefined)
)

fn stringContainsSubstring strng substrng = (
	matchpattern ("_" + strng) pattern:("?*" + substrng + "*")
)

