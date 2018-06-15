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

-- Returns a quaternion.
fn maxRotToMcv transf = (
	-- From mcv, we will go to max, apply the max transform and go back to mcv coord system.
	local mcvTransform = rotateXMatrix 90 * transf * rotateXMatrix -90

	-- Take just the rotation as quaternion
	return roundSmallQuatToZeros (mcvTransform.rotationPart as quat)
)

/*fn mcvRotToMax q = (
	return 
)*/

fn getTransfRelativeToParent child = (
	if child.parent == undefined then return child.transform
	else return child.transform * (inverse child.parent.transform)
)

fn getPosRelativeToParent child = (
	return (getTransfRelativeToParent child).pos
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

------- MATERIALS  AND NAMES ------------------------------------------------------

-- Sets to <id> the material ID of all faces of an object.
fn setMaterialIDs obj id:1 = (
	addModifier obj (MaterialModifier materialID:id)
	convertTo obj Editable_Mesh
)

fn setSelectionMaterialIDs id:1 = (
	for obj in objects do setMaterialIDs obj id:id
)

fn cleanName oldName = (
	local cleanName = substituteString oldName " " "_"
	return substituteString cleanName "ñ" "n"
)

fn isValidName aName = (
	return (findString aName " " == undefined) or
		   (findString aName "ñ" == undefined)
)

fn stringContainsSubstring strng substrng = (
	--Add "_" so we can find the substring right at the beginning.
	matchpattern ("_" + strng) pattern:("?*" + substrng + "*")
)

fn cleanPath originalPath = (
	return substituteString originalPath "\\" "/"
)

fn isPathInsidePath thisPath basePath = (
	if not doesDirectoryExist thisPath or
	   not doesDirectoryExist basePath then return false
	else return stringContainsSubstring (cleanPath thisPath) (cleanPath basePath)
)

-- "c:/ojete/data/" and "c:/ojete/" returns "data/"
fn stripPathFromPath originalPath pathToRemove = (
	originalPath = cleanPath originalPath
	local foundAt = findString originalPath (cleanPath pathToRemove)
	if foundAt == undefined then return originalPath
	return substring originalPath (foundAt + pathToRemove.count) -1
)
