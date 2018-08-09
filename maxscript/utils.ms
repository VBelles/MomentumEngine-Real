--------------------------------------------------------------------------------
-- Some generally useful functions.
--------------------------------------------------------------------------------

-- Returns an array of an editablePoly's elements positions.
-- Elements should be planes, gets the center position of the face.
fn getPolyElementsPos obj = (
	local elemPos = #()
	for i = 1 to (polyop.getNumFaces obj) do (
		append elemPos polyop.getFaceCenter obj i
	)
	return elemPos
)

struct TUtils (
	fn clusterAttach objArr = (
		--http://forums.cgsociety.org/showthread.php?f=98&t=922140
		j = 1
		count = objArr.count

		undo off (
		while objArr.count > 1 do (
			if classOf objArr[j] != Editable_Poly then convertToPoly objArr[j]

			polyop.attach objArr[j] objArr[j + 1]
			deleteItem objArr (j + 1)

			j += 1

			if (j + 1) > objArr.count then j = 1
		)
		)
		return objArr[1]
	),

	fn attachSel = (
		clusterAttach (selection as array)
	),

	--Detach all elements of an object.
	fn detachElements obj baseName = (
		if convertToPoly obj != undefined do (
			while polyOp.getNumFaces obj != 0 do (
				local f_list = polyOp.getElementsUsingFace obj #{1}
				local elem = polyOp.detachFaces obj f_list asNode:true name:(uniqueName baseName)
			)
			delete obj
		)
	),

	fn detachElementsFromSel = (
		for o in selection do (
			detachElements o o.name
		)
	)
)

fn convertToInstance objs baseObj = (
	if objs[1] == undefined then objs = #(objs)
	for o in objs do instanceReplace o baseObj
)

-- Returns array with all instances of obj (obj itself is not added).
fn getInstances obj = (
	local inst
	InstanceMgr.GetInstances obj &inst
	for n in inst where (areNodesInstances obj n) and n != obj
		collect n
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

-- Returns the angle between two vectors (from 0 to 180 degrees, with sign).
fn getVecAngle v1 v2 = (
	local n1 = normalize v1
	local n2 = normalize v2
	local d = dot v1 v2
	local det = v1.x*v2.y - v1.y*v2.x
	return atan2 det d
)

-- Returns the angle between 3 points (from 0 to 180 degrees, with sign).
fn get3pointAngle p1 p2 p3 = (
	local v1 = p2 - p1
	local v2 = p3 - p1
	return getVecAngle v1 v2
)

------- MATERIALS AND NAMES ----------------------------------------------------

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

-- "c:/folder/data/" and "c:/folder/" returns "data/"
fn stripPathFromPath originalPath pathToRemove = (
	originalPath = cleanPath originalPath
	local foundAt = findString originalPath (cleanPath pathToRemove)
	if foundAt == undefined then return originalPath
	return substring originalPath (foundAt + pathToRemove.count) -1
)

fn areFilesIdentical pathA pathB = (
	if pathA == undefined or
	   pathB == undefined then return false
	if not (doesFileExist pathA) or
	   not (doesFileExist pathB) then return false

	return  (getFileModDate pathA == getFileModDate pathB) and
			(getFileSize pathA == getFileSize pathB) and
			(getFilenameFile pathA == getFilenameFile pathB) and
			(getFilenameType pathA == getFilenameType pathB)
)

fn getNewerFile pathA pathB = (
	if (pathA == undefined or not (doesFileExist pathA)) and
	   (pathB != undefined and (doesFileExist pathB)) then return pathB

	if (pathB == undefined or not (doesFileExist pathB)) and
	   (pathA != undefined and (doesFileExist pathA)) then return pathA

	if getFileModDate pathA >= getFileModDate pathB then return pathA
	else return pathB
)
