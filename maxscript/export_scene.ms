clearListener()
gc()

fileIn "export_mesh.ms"
fileIn "add-attributes.ms"
fileIn "utils.ms"

global uniqueCoins   = #()
global uniqueEnemies = #()
global uniqueAltars  = #()

global brokenMaps    = #()

--------------------------------------------------------------------------------
struct TJsonFormatter (
	ofilename,			-- Name of output file
	fs, 				-- File stream
	depth_level = 0,	-- To format json tabs

	-- --------------------
	fn begin new_ofilename = (
		ofilename = new_ofilename
		--fs = listener
		fs = createFile ofilename
		if fs == undefined then (
			-- Avoid error if the file didn't close properly last time.
			flush fs
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
	fn writeKeyEmpty key_str = (
		indent()
		format "\"%\": {},\n" key_str to:fs
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
	
	fn arrayOfStrings arr suffix:""= (
		beginArray()
		for idx = 1 to arr.count do (
			if idx != 1 then writeComma()
			writeName (arr[idx] + suffix)
		)
		endArray()
	)
)

-- -----------------------------------------------------------------------------
-- -----------------------------------------------------------------------------
-- -----------------------------------------------------------------------------
struct TMaterialExporter (
	fs = TJsonFormatter(),
	project_path,           -- Assigned in the ctor. "D:\Dev\MomentumEngine-Real\Bin\"
	base_path,              -- Assigned in the ctor. "data/"

	--TODO: Avoid exporting material if it already exists.
	--Map = dds file.
	fn exportMap map alias default_value = (
		if map == undefined then (
			local json_filename = base_path + "textures/" + default_value + ".dds"
			fs.writeKeyValue alias json_filename
			return false
		)

		-- "C:\engine\bin\data\textures\bricks.dds" but also "C:\download\bricks.dds"
		local mapFilename = map.filename
		local mapFileType = getFilenameType mapFilename
		--format "mapFileType: %\n" mapFileType --DEBUG
		if mapFileType != ".DDS" and mapFileType != ".dds" then throw ("Map filename " + mapFilename + " is NOT a DDS file.") 

		-- bricks
		local mapBaseName = getFilenameFile mapFilename
		-- "C:\engine\bin\data\textures\"
		local mapBasePath = getFilenamePath mapFilename
		-- "data\textures\bricks.dds"
		local json_filename = base_path + "textures/" + mapBaseName + ".dds"

		-- "C:\engine\bin\data\textures\bricks.dds"
		local ofull_path = project_path + json_filename
		
		--TODO: This ignores updates in textures.
		if isPathInsidePath mapBasePath (project_path + base_path) then (
			--Map is already inside project directory, no need to copy, just point to the right sub-folder.
			local strippedPath = stripPathFromPath mapBasePath project_path
			json_filename = strippedPath + mapBaseName + ".dds"
		) else (
			if not doesFileExist ofull_path then (
				format "We should copy from % to %\n" mapFilename ofull_path
				copyFile mapFilename ofull_path
			)
		)
		fs.writeKeyValue alias json_filename
	),

	fn exportMaterialContent mat: tech:"pbr" = (
		fs.beginObj()
			fs.writeKeyValue "technique" (tech + ".tech")
			fs.writeComma()
			fs.writeKeyValue "shadows" true
			fs.writeComma()
			fs.writeKey "textures"
			fs.beginObj()

			if mat != unsupplied then (
				if classOf mat == StandardMaterial then (
					exportMap mat.diffuseMap "albedo" "null_albedo"
					fs.writeComma()
					-- Make this work even if they used "Normal Bump" as Bump.
					if isKindOf mat.bumpMap normal_bump then
						exportMap mat.bumpMap.normal_map "normal" "null_normal"
					else exportMap mat.bumpMap "normal" "null_normal"
					fs.writeComma()
					exportMap mat.glossinessMap "metallic" "null_metallic"
					fs.writeComma()
					exportMap mat.specularLevelMap "roughness" "null_roughness"
					fs.writeComma()
					exportMap mat.selfIllumMap "self_illum" "null_self_illum"
					fs.writeComma()
					exportMap mat.displacementMap "height" "null_height"
					--More: ambientMap, specularMap, opacityMap, filterMap, reflectionMap, refractionMap.
				)
			) else ( -- Empty material.
				fs.writeKeyValue "albedo" "data/textures/null_albedo.dds"
				fs.writeComma()
				fs.writeKeyValue "normal" "data/textures/null_normal.dds"
				fs.writeComma()
				fs.writeKeyValue "metallic" "data/textures/null_metallic.dds"
				fs.writeComma()
				fs.writeKeyValue "roughness" "data/textures/null_roughness.dds"
				fs.writeComma()
				fs.writeKeyValue "self_illum" "data/textures/null_self_illum.dds"
				fs.writeComma()
				fs.writeKeyValue "height" "data/textures/null_height.dds"
			)
			fs.endObj()
		fs.endObj()
	),

	-- Exports a single std material to a json format
	fn exportStdMaterial mat mat_name pure:false = (
		if not pure then (
			--format "Exporting material % % %\n" mat_name mat (classof mat as string)
			fs.begin (project_path + mat_name + ".material")
				exportMaterialContent mat:mat
			fs.end()
		) else ( --Is pure.
			-- Generate normal material but with pureON.tech.
			fs.begin (project_path + mat_name + "_pureON.material")
				exportMaterialContent mat:mat tech:"pureON"
			fs.end()
		    -- Generate identical material but with pure.tech.
			fs.begin (project_path + mat_name + "_pure.material")
				exportMaterialContent mat:mat tech:"pure"
			fs.end()
		)
	),
	
	-- Exports an empty solid material to a json format
	fn exportEmptyMaterial mat_name pure:false = (
		local suffix = ""
		if pure then suffix = "_pure"
		fs.begin (project_path + mat_name + suffix + ".material")
			if pure then exportMaterialContent tech:"pure"
			else exportMaterialContent()
		fs.end()
		if pure then (
			suffix = "_pureON"
			fs.begin (project_path + mat_name + suffix + ".material")
				exportMaterialContent tech:"pureON"
			fs.end()
		)
	),
	
	-- Will return an array of all the materials names used by obj and exported by us
	fn exportMaterial mat mat_path obj = (
		local exported_materials = #()
		local matClass = classOf mat
		--format "material class: %\n" matClass as string --DEBUG
		
		local pure = obj.layer.name == "pure"

		if mat == undefined then (
			setMaterialIDs obj --set Material IDs to 1.
			--throw ("Obj " + obj.name + " does NOT have a material")
			local mat_name = mat_path + "empty"
			append exported_materials mat_name
			exportEmptyMaterial mat_name pure:pure
		)

		if matClass == StandardMaterial then (
			setMaterialIDs obj --set Material IDs to 1.
			local mat_name = mat_path + (cleanName mat.name)
			append exported_materials mat_name
			exportStdMaterial mat mat_name pure:pure
		) else if matClass == MultiMaterial then (
			local multi_mat = mat
		setMaterialIDs obj --set Material IDs to 1. TODO: REMOVE WHEN USING ACTUAL MULTIMATERIALS.
			local materials_of_mesh = getMaterialsUsedByMesh obj
			for mat_idx = 1 to materials_of_mesh.count do (
				if materials_of_mesh[ mat_idx ] == undefined then continue
				local mat_of_mesh = multi_mat[ mat_idx ]

				if mat_of_mesh == undefined then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string)+ " but the multimat does not have this submat")
				if classof mat_of_mesh != StandardMaterial then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string) + " but the multimat in this slot is not a stdMaterial")

				local mat_name = mat_path + mat_of_mesh.name
				append exported_materials mat_name
				exportStdMaterial mat_of_mesh mat_name pure:pure
			)
		)
		return exported_materials
	)
)

-- -----------------------------------------------------------------------------
-- -----------------------------------------------------------------------------
-- -----------------------------------------------------------------------------
-- -----------------------------------------------------------------------------
struct TSceneExporter (
	fs = TJsonFormatter(),
	fss = TJsonFormatter(), --We need another stream for sub-files.
	attrManager = TAttributeManager(),
	project_path = "",
	base_path    = "data/",
	scenes_path  = base_path + "scenes/",
	uniques_path = base_path + "uniques/",
	mesh_path    = base_path + "meshes/",
	mats_path    = base_path + "materials/",
	prefabPath   = base_path + "prefabs/",
	instancePath = base_path + "instancing/",
	
	fn exportName obj = (
		fs.writeKeyValue "name" obj.name
	),

	fn isCamera obj = (
		return classOf obj == FreeCamera or 
			   classOf obj == TargetCamera
	),
	
	fn isLine obj = (
		return classOf obj == line or 
			   classOf obj == circle or
			   classOf obj == SplineShape
	),
	
	fn hasChildLine obj = (
		if obj.children == undefined then return false
		for child in obj.children do (
			if isLine child then return true
		)
		return false
	),
	
	fn isMovable obj = (
		if obj.children == undefined then return false
		for child in obj.children do (
			if hasChildLine child then return true
		)
		return false
	),
	
	fn isCollider obj = (
		if  classOf obj != Box 	 and
			classOf obj != Sphere and
			classOf obj != Capsule and
			classOf obj != Editable_Mesh and
			classOf obj != Plane then (
			format "Wrong object %. Collisions must be Spheres, Boxes, Capsules, Planes or meshes.\n" obj.name
			return false
		)
		return true
	),
	
	fn hasChildCollider obj = (
		if obj.children == undefined then return false
		for child in obj.children do (
			if isCollider child then return true
		)
		return false
	),
	
	fn isDummyLOD obj = (
		if classOf obj == Dummy and
		(stricmp obj.name "lod") == 0 then return true
		else return false
	),
	
	-- Returns true if has dummy child and the dummy also has children.
	fn hasChildDummy obj = (
		if obj.children == undefined then return false
		for child in obj.children do (
			if classOf child == Dummy and
			   child.children.count > 0 then return true
		)
		return false
	),
	
	fn getChildrenNames obj = (
		if obj.children == undefined then return undefined
		local childNames = #()
		for child in obj.children do (
			format "child name: %\n" child.name
			append childNames child.name
		)
		return childNames
	),
	
	fn isUnique obj = (
		return (stringContainsSubstring obj.name "unique")
	),
	
	fn endEntity = (
			fs.endObj() -- - - - - - - end previous entity - - -
		fs.endObj() -- - - - end previous json object - - - - - -
	),
	
	-- Ends one entity and starts a new one.
	fn startNewEntity = (
		endEntity()
		fs.writeComma()
		fs.beginObj() -- - - - - start new json object - - - - - -
			fs.writeKey "entity"
			fs.beginObj() -- - - start new entity  - - - - - - -
	),
	
	-- --CHILD POSITIONS  ------------------------------------------------------
	fn exportChildPositions obj = (
		fs.beginArray()		
			for i = 1 to obj.children.count do (
				if i > 1 then fs.writeComma()
				fs.writeValue (maxPosToMcv obj.children[i].pos) addIndent:true
			)
		fs.endArray()
	),
	
	-- --STRING ARRAY (generic for tags, groups, masks )------------------------
	fn exportStringArray arrayName strings = (
		fs.writeKey arrayName
		fs.beginArray()
			for i = 1 to strings.count do (
				if i > 1 then fs.writeComma()
				fs.writeValue strings[i] addIndent:true
			)
		fs.endArray()
	),

	fn getCircularMovementOffset platform circleObj = (
		local center = circleObj.pos
		local objCopy = copy circleObj
		convertToSplineShape objCopy
		local circlePoint = getKnotPoint objCopy 1 1
		delete objCopy
		local platformPos = platform.pos

		-- Avoid all platforms starting at the same point of the circle.
		-- 90º -> 0.25, 180º -> 0.5, 270º -> 0.75, 360º -> 0.0
		local anglePlatformToCircle = get3pointAngle center circlePoint platformPos
		--format "center: % circlePoint: % platfPos: % anglePlatfToCircle: %\n" center circlePoint platformPos anglePlatformToCircle --DEBUG
		if anglePlatformToCircle == 0 then return 0
		else (
			if anglePlatformToCircle < 0 then return (360 + anglePlatformToCircle) / 360
			else return anglePlatformToCircle / 360
		)
	),

	fn isClosedLine obj = (
		return classOf obj == Line and isClosed obj 1
	),

	fn exportSplineKnots obj = (
		fs.beginArray()
			--Create a copy and convert it to SplineShape to be able to get the knots.
			--We need this if the object is a circle. Also we avoid modifying the max file.
			local objCopy = copy obj
			convertToSplineShape objCopy --A line is still a line after this.

			knotsInSpline = numKnots objCopy
			local firstKnotPos = maxPosToMcv (getKnotPoint objCopy 1 1)
			for i = 1 to knotsInSpline do (
				if i > 1 then fs.writeComma()
				else if classOf objCopy == SplineShape then (
					--Circular movement only needs the center and at least one knot.
					local center = maxPosToMcv objCopy.pos
					fs.writeValue center addIndent:true
					fs.writeComma()
				)

				local knotPos = getKnotPoint objCopy 1 i --getKnotPoint <shape> <spline_index> <knot_index>
				local mcvKnotPos = maxPosToMcv knotPos
				fs.writeValue mcvKnotPos addIndent:true
				-- Duplicate first and last knots (except in closed lines).
				-- This avoids error if the spline has less than 4 knots in catmull.
				-- This also forces following the real shape of the spline.
				if i == 1 then (
					fs.writeComma()
					fs.writeValue mcvKnotPos addIndent:true
				) else if i == knotsInSpline then ( --Last knot.
					fs.writeComma()
					if isClosedLine objCopy then (
						--Special case, add twice the first knot at the end.
						fs.writeValue firstKnotPos addIndent:true
						fs.writeComma()
						fs.writeValue firstKnotPos addIndent:true
					) else (
						--Duplicate last knot.
						fs.writeValue mcvKnotPos addIndent:true
					)
				)
			)
			delete objCopy
		fs.endArray()
	),

	/*fn confirmReset = (
		local hwnd = dialogMonitorOps.getWindowHandle()

		if UIAccessor.isWindow hwnd and
		   UIAccessor.GetWindowText hwnd == "Unwrap UVW" then (
			uiAccessor.pressDefaultButton()
			return true
		)
		else return false
	),*/

	fn areMapsOk obj = (
		try meshop.getMapFace obj 1 1 --If this fails, we got the Runtime error: Map support not enabled for specified map channel.
		catch (
			format "ERROR: Map support error in object %, should unwrap_reset.\n" obj.name
			append brokenMaps obj.name
			return false

			/*
			This provokes an EXCEPTION_ACCESS_VIOLATION.
			addModifier obj (Unwrap_UVW())

			dialogMonitorOps.enabled = true
			dialogMonitorOps.interactive = false
			dialogMonitorOps.registerNotification confirmReset id:#unwrap_reset
			obj.modifiers[#unwrap_uvw].reset()
			dialogMonitorOps.enabled = false
			convertToMesh obj*/
		)
		return true
	),

	-- -------------------------------------------------------------------------
	-- -- · COMPONENTS · -------------------------------------------------------
	-- -------------------------------------------------------------------------

	-- --COMP: TRANSFORM--------------------------------------------------------
	fn exportTransform obj lookAtCoord: noRot:false relative:false = (
		fs.writeKey "transform"
		fs.beginObj()
			local objPos = obj.pos
			if relative then objPos = getPosRelativeToParent obj
			fs.writeKeyValue "pos" (maxPosToMcv objPos)
			if not noRot then (
				fs.writeComma()
				if lookAtCoord == unsupplied then (
					local transf = obj.transform
					if relative then transf = getTransfRelativeToParent obj
					if classOf obj == FreeCamera then (
						local camCopy = copy obj
						camCopy.transform = (rotateXMatrix 90) * camCopy.transform --Compensate camera axis shenanigans.
						fs.writeKeyValue "rotation" (maxRotToMcv camCopy.transform)
						delete camCopy
					)
					else fs.writeKeyValue "rotation" (maxRotToMcv transf)
				) else (
					fs.writeKeyValue "lookat" lookAtCoord
				)
			)
		fs.endObj()
	),

	-- --RENDER MESH (also exports MATERIAL)------------------------------------
	fn exportRenderMesh obj pure:false = (
		local mat = obj.material
		if mat == undefined then (
			format "Warning: Obj % doesn't have a material, a default will be assigned.\n" obj.name
		)
		else if not (isValidName mat.name) then (
			mat.name = cleanName mat.name
			format "Warning: material had invalid name, changed spaces to underscores.\n"
		)

		fs.beginObj()
			-- -- ·Export Mesh· -- --
			if not areMapsOk obj then return false

			local mesh_name = mesh_path + obj.name + ".mesh"
			fs.writeKeyValue "mesh" mesh_name
			
			-- Generate the mesh file.
			exportMeshObject obj (project_path + mesh_name)

			fs.writeComma()

			-- -- ·Export material· -- --
			--local mat_name = mats_path + mat.name + ".material"
			--fs.writeKeyValue "material" mat_name
			local me = TMaterialExporter project_path:project_path base_path:base_path
			local exported_materials = me.exportMaterial mat mats_path obj

			fs.writeKey "materials"
			local suffix = ".material"
			if pure then suffix = "_pureON.material"
			fs.arrayOfStrings exported_materials suffix:suffix
			if pure then (
					fs.writeComma()
					fs.writeKeyValue "enabled" false
				fs.endObj() -- End first pure material object.
				fs.writeComma()
				fs.beginObj() -- Begin second pure material object.
					fs.writeKeyValue "mesh" mesh_name
					fs.writeComma()
					fs.writeKey "materials"
					local matName
					if mat == undefined then matName = "empty"
					else matName = cleanName mat.name
					fs.arrayOfStrings #(mats_path + matName + "_pure.material")
					fs.writeComma()
					fs.writeKeyValue "enabled" false
			)
		fs.endObj() -- End render object.
		return true
	),

	-- --COMP: ROTATE (platform_simple)-----------------------------------------
	fn exportPlatfRotation obj = (
		local rotAxis = Point3 0 0 1
		local rotSpeed = 0
		--Get from attribute if it exists.
		if attrManager.hasAttribute obj "rotateData" then (
			local xRot = attrManager.getAttributeValue obj "rotateData" #x
			local yRot = attrManager.getAttributeValue obj "rotateData" #z -- !!! inverted.
			local zRot = attrManager.getAttributeValue obj "rotateData" #y -- !!! inverted.

			rotAxis = Point3 xRot yRot zRot
			rotSpeed = attrManager.getAttributeValue obj "rotateData" #rotSpeed
		)
		fs.writeKeyValue "rotation_axis" rotAxis
		fs.writeComma()
		fs.writeKeyValue "rotation_speed" rotSpeed
	),
	
	fn exportPlatfRotationOnly obj = (
		fs.writeComma()
		fs.writeKey "platform_simple"
		fs.beginObj()
			exportPlatfRotation obj
		fs.endObj()
	),
	
	-- --SPLINE-----------------------------------------------------------------
	fn exportSpline obj type:"platform_simple" = (
		-- Executed only if we already know it has a child line.
		local curveType = "catmull-rom"
		for child in obj.children do (
			local childClass = classOf child
			local moveOffset = 0
			if isLine child then (
				if childClass == line then curveType = "catmull-rom"
				if childClass == circle or
			       childClass == SplineShape then (
					curveType = "circular"
					moveOffset = getCircularMovementOffset obj child
				)

				local closed = false
				local followSpeed = 0.3
				if childClass == circle then closed = true
				else closed = isClosed child 1

				if closed then followSpeed = 0.04

				local auto = true
				local loop = true
				local wait = 0.0
				--Get from attribute if it exists.
				if attrManager.hasAttribute obj.parent "moveData" then (
					auto 		= attrManager.getAttributeValue obj.parent "moveData" #auto
					loop 		= attrManager.getAttributeValue obj.parent "moveData" #loop
					wait 		= attrManager.getAttributeValue obj.parent "moveData" #wait
					followSpeed = attrManager.getAttributeValue obj.parent "moveData" #spd
				)

				fs.writeComma()
				fs.writeKey type
				fs.beginObj()
					exportPlatfRotation obj.parent
					fs.writeComma()
					fs.writeKeyValue "curve_type" curveType
					fs.writeComma()
					fs.writeKeyValue "automove" auto
					fs.writeComma()
					fs.writeKeyValue "loop" loop
					fs.writeComma()
					fs.writeKeyValue "closed" closed
					fs.writeComma()
					fs.writeKeyValue "move_offset" moveOffset
					fs.writeComma()
					fs.writeKeyValue "travel_wait_time" wait
					fs.writeComma()
					fs.writeKeyValue "speed" followSpeed
					fs.writeComma()
					fs.writeKey "knots"
					exportSplineKnots child
				fs.endObj()
				return true -- Only exports the first line found.
			)
		)
	),
	
	-- --COMP: BREAKABLE--------------------------------------------------------
	fn exportBreakable hp:1 = (
		fs.writeComma()
		fs.writeKey "breakable"
		fs.beginObj()
			fs.writeKeyValue "hp" hp
		fs.endObj()
	),
	
	-- --COMP: MECHANISM SYSTEM-------------------------------------------------
	-- What is affected when activating a mechanism.
	fn exportMech obj = (
		fs.writeComma()
		fs.writeKey "mechanism_system"
		fs.beginObj()
			fs.writeKeyValue "number_of_mechanisms" 1
		fs.endObj()

		--Get from attribute if it exists.
		if attrManager.hasAttribute obj.parent.parent "mechData" then (
			local appear = attrManager.getAttributeValue obj.parent.parent "mechData" #appear

			fs.writeComma()
			fs.writeKey "door"
			fs.beginObj()
				fs.writeKeyValue "startsClosed" (not appear) --false means it starts invisible.
			fs.endObj()
		)
	),

	-- --CONTENTS OF THE collider_mesh or tri_mesh collider---------------------
	fn exportColliderMeshContents obj meshShape:"convex_mesh" forceMesh:false mov:false = (
		if obj.layer.name == "trimesh" then meshShape = "tri_mesh"
		local mesh_name = mesh_path + obj.name + ".col_mesh"
		if forceMesh then mesh_name = mesh_path + "plataformaPuraSI_tri.col_mesh"

		local meshType = "static"
		if mov then meshType = "kinematic"
		fs.writeKeyValue "mesh_src" mesh_name
		fs.writeComma()
		fs.writeKeyValue "shape" meshShape

		if not forceMesh then (
			if not areMapsOk obj then return false

			exportMeshObject obj (project_path + mesh_name) vtxType:"Pos"
		)
		return true
	),
	
	fn exportSphere obj = (
		fs.writeKeyValue "shape" "sphere"
		fs.writeComma()
		fs.writeKeyValue "radius" obj.radius
	),
	
	fn exportBox obj = (
		--format "exportBox obj.name: %\n" obj.name --DEBUG
		local boxCopy = copy obj
		convertToMesh boxCopy
		boxCopy.name = obj.name
		exportColliderMeshContents boxCopy
		delete boxCopy
		/*fs.writeKeyValue "shape" "box"
		fs.writeComma()
		fs.writeKeyValue "halfExtent" [abs obj.width / 2, abs obj.height / 2, abs obj.length / 2]*/
	),
	
	fn exportPlane obj = (
		fs.writeKeyValue "shape" "plane"
		fs.writeComma()
		-- Last value should be Y position.
		fs.writeKeyValue "planeDesc" "0 1 0 0"
	),
	
	fn exportCapsule obj = (
		fs.writeKeyValue "shape" "capsule"
		fs.writeComma()
		fs.writeKeyValue "type" "cct"
		fs.writeComma()
		fs.writeKeyValue "radius" obj.radius
		fs.writeComma()
		--Warning: Max capsule height is the real height, NOT HALF as in PhysX.
		fs.writeKeyValue "height" (obj.height / 2)
		fs.writeComma()
		fs.writeKeyValue "step" 0.5
		fs.writeComma()
		fs.writeKeyValue "slope" 45
	),
	
	fn exportEmptyAABBs = (
		fs.writeComma()
		fs.writeKeyValue "local_aabb" undefined
		fs.writeComma()
		fs.writeKeyValue "abs_aabb" undefined
	),
	
	fn exportLOD obj = (
		exportEmptyAABBs()
		fs.writeComma()
		fs.writeKey "lod"
		fs.beginObj()
			fs.writeKeyValue "threshold" 0.08
			
			for child in obj.children do (
				if isDummyLOD child then (
					local childNames = getChildrenNames child
					if childNames != undefined and
					   childNames.count > 0 then (
						fs.writeComma()
						exportStringArray "children" childNames
					)
				)
			)
		fs.endObj()
	),
	
	fn exportHierarchy parentName = (
		fs.writeKey "hierarchy"
		fs.beginObj()
			fs.writeKeyValue "parent" parentName
		fs.endObj()
	),
	
	fn exportSeparateColliderEntity obj = (
		startNewEntity()
		exportName obj
		fs.writeComma()
		exportTransform obj relative:true
		fs.writeComma()
		exportHierarchy obj.parent.name
		fs.writeComma()
	),
	
	-- --COMP: COLLIDER---------------------------------------------------------
	fn exportCompCollider obj first:true meshMode:false colliderGroups: masks: trig:false dyn:false mov:false = (
		local objClass = classOf obj
		local layerName = obj.layer.name
		-- If the object has more than one child collider we need to add them in separate entities.
		if not first then exportSeparateColliderEntity obj
		
		fs.writeKey "collider"
		fs.beginObj()
			--format "exportCompCollider of obj: %\n" obj.name --DEBUG
			if meshMode then exportColliderMeshContents obj meshShape:"convex_mesh" mov:mov forceMesh:true
			else (
				if      objClass == Sphere		  then exportSphere obj
				else if objClass == Box			  then exportBox obj
				else if objClass == Plane		  then exportPlane obj
				else if objClass == Capsule		  then exportCapsule obj
				else if objClass == Editable_mesh then exportColliderMeshContents obj

				if layerName == "triggers" or trig then (
					fs.writeComma()
					fs.writeKeyValue "is_trigger" trig
				)
				
				fs.writeComma()
				if dyn then fs.writeKeyValue "type" "dynamic"
				else if mov then fs.writeKeyValue "type" "kinematic"
				else fs.writeKeyValue "type" "static"
			)

			if colliderGroups != unsupplied then (
				fs.writeComma()
				exportStringArray "group" colliderGroups
			)
			
			if masks != unsupplied then (
				fs.writeComma()
				exportStringArray "mask" masks
			)
		fs.endObj() -- End collider component.
	),
	
	-- --COLLIDER (name, transform and collider)--------------------------------
	fn exportEntityCollider obj meshMode: colliderGroups: masks: trig:false dyn:false = (
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		
		exportCompCollider obj colliderGroups:colliderGroups masks:masks trig:trig dyn:dyn
	),
	
	-- --COMP: Give power --
	fn exportGivePower maxPow: multi: = (
		fs.writeKey "givePower"
		fs.beginObj()
			if maxPow != unsupplied then (
				fs.writeKeyValue "maxPowerToGive" maxPow
				if multi != unsupplied then fs.writeComma()
			)
			if multi != unsupplied then fs.writeKeyValue "baseMultiplier" multi
		fs.endObj()
	),
	
	-- --COMP: Behavior tree --
	fn exportBehaviorTree enemyType = (
		if enemyType == "melee" then (
			fs.writeKeyEmpty "behaviorTree_melee_enemy"
		) else (
			fs.writeKey "behaviorTree_flying_ranged_enemy"
			fs.beginObj()
				fs.writeKeyValue "attackSpawnOffset" "0.0 0.1 0.5"
				fs.writeComma()
				fs.writeKeyValue "attackTargetOffset" "0.0 0.76 0.0"
			fs.endObj()
			fs.writeComma()
		)
	),

	-- --COMP: CAMERA TRIGGER --------------------------------------------------
	fn exportCameraTrigger obj = (
		exportEntityCollider obj colliderGroups:#("trigger") masks:#("player") trig:true
		fs.writeComma()
		fs.writeKey "mixCameraTrigger"
		fs.beginObj()
			fs.writeKeyValue "cameraToMix" obj.parent.name
			fs.writeComma()
			fs.writeKeyValue "timeToMixIn" 1
			fs.writeComma()
			fs.writeKeyValue "timeToMixOut" 1
			fs.writeComma()
			fs.writeKeyValue "modifyPlayerCameraRotation" true
		fs.endObj()
	),
	
	-- --COMP: SUGGEST CAMERA TRIGGER ------------------------------------------
	fn exportSuggestCameraTrigger obj = (
		exportEntityCollider obj colliderGroups:#("trigger") masks:#("player") trig:true
		fs.writeComma()
		fs.writeKey "suggestCameraTrigger"
		fs.beginObj()
			fs.writeKeyValue "cameraSuggestion" obj.parent.name
			fs.writeComma()
			fs.writeKeyValue "suggestYaw" false
			fs.writeComma()
			fs.writeKeyValue "hasOppositeYaw" false
			fs.writeComma()
			fs.writeKeyValue "suggestPitch" true
			fs.writeComma()
			fs.writeKeyValue "forceDistance" false
			fs.writeComma()
			local distanceToCam = distance obj.pos obj.parent.Target[3].pos
			fs.writeKeyValue "distanceToCamera" distanceToCam
			fs.writeComma()
			fs.writeKeyValue "changeCenteringCamera" true
			fs.writeComma()
			fs.writeKeyValue "forcePositionOnEnter" true
			fs.writeComma()
			fs.writeKeyValue "centeringCameraSpeed" "18 9"
		fs.endObj()
	),
	
	-- --EDITABLE MESH (includes pure, break, mov, rot & mech children)---------
	fn exportEditableMesh obj = (
		--format "Exporting EditableMesh obj % of class %\n" obj.name ((classof obj) as string) --DEBUG
		local pure 		= obj.layer.name == "pure"
		local breakable = obj.layer.name == "breakables"
		local mech		= obj.layer.name == "mechanisms"
				
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		fs.writeKey "render"
		fs.beginArray()
			exportRenderMesh obj pure:pure
		fs.endArray()
		if hasChildDummy obj then exportLOD obj
		
		local colliderCount = 0
		if hasChildCollider obj then (
			for child in obj.children do (
				if isCollider child then (
					colliderCount += 1
					local isFirst = colliderCount == 1
					if isFirst then fs.writeComma()
					--format "Exporting child collider obj % of class %\n" child.name ((classof child) as string) --DEBUG
					local movable = hasChildLine child
					local onlyRot = not movable and attrManager.hasAttribute obj "rotateData"
					local groups = #("scenario")
					if breakable then append groups "enemy"

					local objToExport = child
					if pure then (
						fs.writeKeyEmpty "purity"
						objToExport = obj
					)
					exportCompCollider objToExport first:isFirst mov:(movable or onlyRot) colliderGroups:groups

					if breakable then exportBreakable()
					if movable then exportSpline child
					if onlyRot then exportPlatfRotationOnly obj
					if mech then exportMech child
				)
				else if isDummyLOD child then (
					for i = 1 to child.children.count do (
						startNewEntity()
						exportEditableMesh child.children[i]
						exportEmptyAABBs()
					)
				)
			)
		)
	),

	-- --COMP: ALTAR -----------------------------------------------------------
	fn exportAltar obj = (
		fs.writeKey "altar"
		fs.beginObj() -- - - - - - - - - - - - - - - - - - -
			fs.writeKeyValue "prefab" (prefabPath + "crisalida.prefab")
			fs.writeComma()
			fs.writeKey "spawn_positions"
			exportChildPositions obj
		fs.endObj() -- - - - - - - - - - - - - - - - - - - -
	),
	
	-- -------------------------------------------------------------------------
	-- -- · ENTITIES · ---------------------------------------------------------
	-- -------------------------------------------------------------------------

	-- --ENTITY with only COLLISION --------------------------------------------
	fn exportEntityCollision obj nValidObjs = (
		local objClass = classOf obj
		if not isCollider obj and
		   not objClass == Editable_mesh then return false
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEntityCollider obj meshMode:(not (isCollider obj and objClass != Editable_mesh)) dyn:(hasChildLine obj)
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),
	
	-- --ENTITY: ALTAR ---------------------------------------------------------
	fn exportEntityAltar obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		if isUnique obj then append uniqueAltars obj.name

		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "altar.prefab")
				fs.writeComma()
				exportTransform obj
				exportBreakable()
				fs.writeComma()
				exportAltar obj
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),
	
	-- --ENTITY: COIN-----------------------------------------------------------
	fn exportEntityCoin obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		if isUnique obj then append uniqueCoins obj.name

		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				exportTransform obj noRot:true
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "coin.prefab")
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),

	-- --ENTITY: CAMERA (also exports child line)-------------------------------
	fn exportEntityCamera obj nValidObjs = (
		local objClass = classOf obj
		--format "name: %\n" obj.name --DEBUG
		if objClass == TargetCamera or
		   objClass == FreeCamera then (
			if nValidObjs > 0 then fs.writeComma()

			fs.beginObj()
				fs.writeKey "entity"
				fs.beginObj() -- - - - - - - - - - - - - - - - - - -
					exportName obj
					fs.writeComma()
					if objClass == TargetCamera then (
						--Target[3] is the Transform.
						exportTransform obj lookAtCoord:(maxPosToMcv obj.Target[3].pos)
					) else exportTransform obj

					fs.writeComma()
					fs.writeKey "camera"
					fs.beginObj()
						fs.writeKeyValue "fov" obj.fov
					fs.endObj()

					fs.writeComma()
					fs.writeKey "fixed_camera"
					fs.beginObj()
						fs.writeKeyValue "returnToPlayerCameraWithInput" false
						fs.writeComma()
						fs.writeKeyValue "modifyPlayerCameraRotation" true
						fs.writeComma()
						fs.writeKeyValue "timeToMixOut" 1
						fs.writeComma()
						fs.writeKeyValue "panningSpeed" 0
						fs.writeComma()
						fs.writeKeyValue "panningRadius" 3
					fs.endObj()

					-- Add child spline if it exists.
					if hasChildLine obj then exportSpline obj.children[1]

				--format "Camera % target distance: %\n" obj.name obj.targetDistance
				endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		)
		else return false

		if hasChildCollider obj then (
			local child = obj.children[1]
			if isCollider child then (
				fs.writeComma()
				fs.beginObj()
					fs.writeKey "entity"
					fs.beginObj() -- - - - - - - - - - - - - - - - - - -
						-- Add trigger.
						if objClass == FreeCamera then exportCameraTrigger child
						else exportSuggestCameraTrigger child
					endEntity() -- - - - - - - - - - - - - - - - - - - - - -
				return true
			)
		)
		return true
	),

	-- --ENTITY: CHRYSALIS (also exports child line)----------------------------
	fn exportEntityChrysalis obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "crisalida.prefab")
				fs.writeComma()
				exportTransform obj
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),

	-- --ENTITY: ENEMY (also exports child line)--------------------------------
	fn exportEntityEnemy obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		if isUnique obj then append uniqueEnemies obj.name

		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				local enemyAirPrefabName   = "enemy_flying"
				local enemyMeleePrefabName = "enemy_melee"
				local enemyBolaPrefabName  = "enemy_ball"
				local prefabName = enemyAirPrefabName -- Flying by default.
				if stringContainsSubstring obj.name "melee" then
					prefabName = enemyMeleePrefabName
				if stringContainsSubstring obj.name "Ball" then
					prefabName = enemyBolaPrefabName
				
				local attributeManager = TAttributeManager()
				local hp = attributeManager.getAttributeValue obj #enemyData #hitPoints
				if hp != undefined then format "would export hp: %\n" hp
				
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + prefabName + ".prefab")
				fs.writeComma()
				local objPos = maxPosToMcv obj.position
				local lookAtPos = [-1, objPos.y, -1]
				exportTransform obj lookAtCoord:lookAtPos
				
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj type:"patrolPath"
				)
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),

	-- --ENTITY: LIGHT (also exports child line)--------------------------------
	fn exportEntityLight obj nValidObjs = (
		local objClass = classOf obj
		if objClass != TargetDirectionallight and
		   objClass != Directionallight	  	  and
		   objClass != freeSpot			  	  and
		   objClass != Omnilight			  and
		   objClass != targetSpot			then (
			if objClass != Targetobject then format "Warning: invalid object % of class % in lights layer, not exported.\n" obj.name objClass
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
			-- Target Direct light. TODO: add more types.
			-- http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_E6767B0D_B5BA_4422_A59F_FD5673FE550E_htm
				exportName obj
				
				if objClass == TargetDirectionallight or
					 objClass == targetSpot then (
					fs.writeComma()
					exportTransform obj lookAtCoord:obj.Target[3].pos --[3] is the Transform.
				)
				fs.writeComma()
				fs.writeKey "light_dir"
				fs.beginObj()
					fs.writeKeyValue "intensity" obj.multiplier
					fs.writeComma()
					fs.writeKeyValue "color" (obj.color as point3)
					if obj.projector then (
						fs.writeComma()
						fs.writeKeyValue "projector" obj.projectorMap
					)
					fs.writeComma()
					-- obj.nearAttenEnd for z_near
					fs.writeKeyValue "z_far" obj.farAttenEnd
					fs.writeComma()
					fs.writeKeyValue "fov" 40 -- TODO: ¿Pick from some light parameter?
				fs.endObj()
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),
	
	-- --ENTITY: TRIGGERS (also exports child line)-----------------------------
	fn exportEntityTrigger obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				if stringContainsSubstring obj.name "Bounds" then (
					fs.writeKeyEmpty "outOfBounds"
					exportEntityCollider obj trig:true colliderGroups:#("trigger") masks:#("characters")
				)
				else exportEntityCollider obj trig:true colliderGroups:#("trigger") masks:#("player")

				if stringContainsSubstring obj.name "respawn" and 
				   obj.children != undefined then (
						local respawnPos = "0 0 0"
						local respawnRot = "0 0 0 0"
						for child in obj.children do (
							if classOf child == Dummy then (
								respawnPos = (maxPosToMcv child.position)
								respawnRot = (maxRotToMcv child.transform)
							)
						)
						
						fs.writeComma()
						fs.writeKey "respawn_point"
						fs.beginObj()
							fs.writeKeyValue "respawn_position" respawnPos
							fs.writeComma()
							fs.writeKeyValue "respawn_rotation" respawnRot
						fs.endObj()
				)
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),
	
	-- --ENTITY: SPIKES---------------------------------------------------------
	fn exportEntitySpikes obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()

		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "spikes.prefab")
				fs.writeComma()
				exportTransform obj
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),

	-- --ENTITY: DEFAULT (exports editable mesh)--------------------------------
	fn exportEntityDefault obj nValidObjs = (
		if classOf obj != Editable_mesh then (
			format "WARNING: Object % is NOT an Editable mesh, not exported.\n" obj.name
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - -
				exportEditableMesh obj
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		return true
	),

	-- --ENTITY: MECHANISM (also exports children)------------------------------
	-- The activator of the mechanism system.
	fn exportEntityMechSys obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
	
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "mech.prefab")
				fs.writeComma()
				exportTransform obj
				fs.writeComma()

				local deacTime = 5
				local reacTime = 3
				local resettable = true

				--Get from attribute if it exists.
				if attrManager.hasAttribute obj "mechData" then (
					deacTime	= attrManager.getAttributeValue obj "mechData" #deacT
					reacTime 	= attrManager.getAttributeValue obj "mechData" #reacT
					resettable  = attrManager.getAttributeValue obj "mechData" #reset
				)
		
				fs.writeKey "mechanism"
				fs.beginObj()
					fs.writeKeyValue "deactivationTime" deacTime
					fs.writeComma()
					fs.writeKeyValue "reactivationTime" reacTime
					fs.writeComma()
					fs.writeKeyValue "isResettable" resettable
					fs.writeComma()
					fs.writeKey "systems"
					fs.beginArray()
						for i = 1 to obj.children.count do (
							if i > 1 then fs.writeComma()
							fs.writeValue obj.children[i].name addIndent:true
						)
					fs.endArray()
				fs.endObj()
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -

		for child in obj.children do (
			exportEntityDefault child (nValidObjs + 1)
		)
		return true
	),

	fn exportInstancing obj instanceCat:"" = (
		local instancesPath = project_path + instancePath + "instancing_" + instanceCat + ".json"

		fss.begin instancesPath --Create file.
			fss.beginObj()
			fss.writeKey "TInstance"
			fss.beginArray()
			fss.beginObj()
				fss.writeKeyValue "mesh" (mesh_path + instanceCat + ".instanced_mesh")
				fss.writeComma()
				fss.writeKey "data"
				fss.beginArray()
				for i = 1 to obj.children.count do (
					if i > 1 then fss.writeComma()
					fss.beginObj()
						fss.writeKeyValue "pos" (maxPosToMcv obj.children[i].pos)
						fss.writeComma()
						fss.writeKeyValue "scale" obj.children[i].scale.x --x,y,z should be the same.
						fss.writeComma()
						fss.writeKeyValue "rot" (maxRotToMcv obj.children[i].transform)
					fss.endObj()
				)
				fss.endArray()
			fss.endObj()
			fss.endArray()
			fss.endObj()
		fss.end()
	),

	fn exportInstancedMeshFile instanceCat:"" = (
		local instancesPath = project_path + mesh_path + instanceCat + ".instanced_mesh"

		fss.begin instancesPath --Create file.
			fss.beginObj()
				fss.writeKeyValue "instanced_mesh" (instanceCat + ".mesh")
				fss.writeComma()
				fss.writeKeyValue "instances_type" "Instance"
				fss.writeComma()	
				fss.writeKeyValue "num_instances_reserved" 8
			fss.endObj()
		fss.end()
	),

	-- --ENTITY: INSTANCES (also exports children)------------------------------
	-- Each object is a dummy containing instances of dummy's name type.
	fn exportEntityInstance obj nValidObjs = (
		if obj.children == undefined or obj.children.count == 0 then return false
		if nValidObjs > 0 then fs.writeComma()

		local instanceCategory = obj.name

		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				exportTransform obj
				fs.writeComma()
				fs.writeKey "render"
				fs.beginArray()
					fs.beginObj()
						fs.writeKeyValue "mesh" (mesh_path + instanceCategory + ".instanced_mesh")
						fs.writeComma()
						fs.writeKey "materials"
							fs.beginArray()
								 fs.writeValue (mats_path + instanceCategory + ".material") addIndent:true
							fs.endArray()
					fs.endObj()
				fs.endArray()
			endEntity() -- - - - - - - - - - - - - - - - - - -
		--fs.endObj() is made outside this function.

		exportInstancedMeshFile instanceCat:instanceCategory --.instanced_mesh

		exportInstancing obj instanceCat:instanceCategory --/instancing/instanceCat_instancing.json

		return true
	),

	-- -------------------------------------------------------------------------
	-- -------------------------------------------------------------------------
	
	-- --ENEMY PREFAB ----------------------------------------------------------
	fn exportEnemyPrefab obj nValidObjs = (
		if classOf obj != Capsule and classOf obj != Sphere then return false
		if obj.parent != undefined then return false
		local meshPath = ""
		local enemyType = "flying"
		if stringContainsSubstring obj.layer.name "melee" then (
			meshPath = mesh_path + "enemyMelee.mesh"
			enemyType = "melee"
		)
		else if stringContainsSubstring obj.layer.name "ball" then (
			meshPath = mesh_path + "enemigoBola.mesh"
			enemyType = "ball"
		)
		else (
			meshPath = mesh_path + "crisalida.mesh"
		)
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				fs.writeKeyEmpty "transform"
		
				fs.writeKey "render"
				fs.beginArray()
					fs.beginObj()
						fs.writeKeyValue "mesh" meshPath
						fs.writeComma()
						fs.writeKey "materials"
						fs.beginArray()
							fs.writeValue "data/materials/piedras1.material" addIndent:true
						fs.endArray()
						fs.writeComma()
						fs.writeKeyValue "color" "0.45 0.28 0.8 1.0"
					fs.endObj()
				fs.endArray() -- End render
				fs.writeComma()
		
				exportStringArray "tags" #("enemy")
				fs.writeComma()
				
				fs.writeKey "respawner"
				fs.beginObj()
					fs.writeKeyValue "respawnTime" 10
				fs.endObj()
				fs.writeComma()
				
				exportShadow()
				fs.writeComma()
				
				exportGivePower maxPow:50000 multi:1
				fs.writeComma()
				
				exportBehaviorTree enemyType
				
				exportCompCollider obj colliderGroups:#("enemy") masks:#("all")
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		
		exportChildHitboxes obj obj.height
		
		return true
	),
	
	-- --ENTITY: PLAYER HITBOXES------------------------------------------------
	fn exportChildHitboxes obj capsuleHeight = (
		for child in obj.children do (
			if isCollider child and
			   classOf child != Capsule then (
				fs.writeComma()
				fs.beginObj()
					fs.writeKey "entity"
					fs.beginObj() -- - - - - - - - - - - - - - - - - - -
					--format "Exporting child collider obj % of class %\n" child.name ((classof child) as string) --DEBUG
						exportEntityCollider child colliderGroups:#("Hitbox") trig:true dyn:true
						fs.writeComma()
						exportHierarchy obj.name
						fs.writeComma()
						fs.writeKey "hitbox"
						fs.beginObj()
							-- PhysX takes center of Capsule as pivot, so we have to correct it for the offset.
							local correctedObjTransf = obj.transform
							correctedObjTransf.pos.z += capsuleHeight / 2
							local posInParentSpace = child.transform.pos * inverse correctedObjTransf
							-- Change xyz order.
							posInParentSpace = [posInParentSpace.y, posInParentSpace.z, posInParentSpace.x]
							posInParentSpace = roundSmallPosToZeros posInParentSpace
							fs.writeKeyValue "offset" posInParentSpace
						fs.endObj()
						
						-- Export spline if available.
						if hasChildLine child then (
							--TODO: this should be different now that we use platform_simple instead of follow_curve.
							exportSpline child type:"hitboxPath"
						)
					endEntity() -- - - - - - - - - - - - - - - - -
			)
		)
	),
	
	-- --ENTITY: PLAYER --------------------------------------------------------
	fn exportEntityPlayer obj nValidObjs = (
		if classOf obj != Editable_mesh then (
			format "WARNING: Object % is NOT an Editable mesh, not exported.\n" obj.name
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				exportTransform obj
				fs.writeComma()
				fs.writeKey "render"
				fs.beginArray()
					exportRenderMesh obj
				fs.endArray()
				fs.writeComma()
				fs.writeKeyEmpty "player_controller"
				fs.writeKey "player_model"
				fs.beginObj()
					fs.writeKeyValue "gravity" -55
					fs.writeComma()
					fs.writeKeyValue "walkingSpeed" 3
					fs.writeComma()
					fs.writeKey "ssj1"
					fs.beginObj()
						fs.writeKeyValue "rotation_speed" 11.0
						fs.writeComma()
						fs.writeKeyValue "maxSpeed" 7.0
						fs.writeComma()
						fs.writeKeyValue "shortHopVelocity" 16.0
						fs.writeComma()
						fs.writeKeyValue "jumpVelocity" "0 20 0"
						fs.writeComma()
						fs.writeKeyValue "longJumpVelocity" "0 11 12"
						fs.writeComma()
						fs.writeKeyValue "maxVelocityVertical"  30.0
						fs.writeComma()
						fs.writeKeyValue "acceleration" 30.0
						fs.writeComma()
						fs.writeKeyValue "deceleration" 7.0
						fs.writeComma()
						fs.writeKeyValue "airAcceleration" 30.0
						fs.writeComma()
						fs.writeKeyValue "falling_multiplier" 1.05
						fs.writeComma()
						fs.writeKeyValue "long_gravity_multiplier" 0.60
					fs.endObj()
					fs.writeComma()
					fs.writeKey "ssj2"
					fs.beginObj()
						fs.writeKeyValue "rotation_speed" 14.0
						fs.writeComma()
						fs.writeKeyValue "maxSpeed" 11.0
						fs.writeComma()
						fs.writeKeyValue "shortHopVelocity" 18.0
						fs.writeComma()
						fs.writeKeyValue "jumpVelocity" "0 25 0"
						fs.writeComma()
						fs.writeKeyValue "longJumpVelocity" "0 12 17"
						fs.writeComma()
						fs.writeKeyValue "maxVelocityVertical" 30.0
						fs.writeComma()
						fs.writeKeyValue "acceleration" 32.0
						fs.writeComma()
						fs.writeKeyValue "deceleration" 8.0
						fs.writeComma()
						fs.writeKeyValue "airAcceleration" 35.0
						fs.writeComma()
						fs.writeKeyValue "falling_multiplier" 1.05
						fs.writeComma()
						fs.writeKeyValue "long_gravity_multiplier" 0.60
					fs.endObj()
					fs.writeComma()
					fs.writeKey "ssj3"
					fs.beginObj()
						fs.writeKeyValue "rotation_speed" 17.0
						fs.writeComma()
						fs.writeKeyValue "maxSpeed" 14.0
						fs.writeComma()
						fs.writeKeyValue "shortHopVelocity" 20.0
						fs.writeComma()
						fs.writeKeyValue "jumpVelocity" "0 30 0"
						fs.writeComma()
						fs.writeKeyValue "longJumpVelocity" "0 13 22.5"
						fs.writeComma()
						fs.writeKeyValue "maxVelocityVertical" 30.0
						fs.writeComma()
						fs.writeKeyValue "acceleration" 35.0
						fs.writeComma()
						fs.writeKeyValue "deceleration" 9.0
						fs.writeComma()
						fs.writeKeyValue "airAcceleration" 40.0
						fs.writeComma()
						fs.writeKeyValue "falling_multiplier" 1.05
						fs.writeComma()
						fs.writeKeyValue "long_gravity_multiplier" 0.60
					fs.endObj()
					fs.writeComma()
					fs.writeKeyEmpty "power_gauge"
				fs.endObj() -- End player_model
				fs.writeComma()
				local capsuleHeight = 0
				for child in obj.children do (
					if isCollider child and
						-- Pick the capsule. Shouldn't have more than one.
						classOf child == Capsule then (
							exportCompCollider child colliderGroups:#("Player") masks:#("all")
							capsuleHeight = child.height
					)
				)
				fs.writeComma()
				fs.writeKeyEmpty "render_ui"
				exportShadow()
			endEntity() -- - - - - - - - - - - - - - - - - - - - - -
		
		exportChildHitboxes obj capsuleHeight

		return true
	),
	
	-- -------------------------------------------------------------------------
	-- -------------------------------------------------------------------------
	
	fn exportUniqueObj obj = (
		local type = "none"

		case obj.layer.name of (
			"altars":  type = "altars"
			"coins":   type = "coins"
			"enemies": (
				if (stringContainsSubstring obj.name "melee") then type = "dreidel" --peonza
				else type = "kippah" --medusa
			)
			default:   type = "none"
		)

		fs.beginObj()
			fs.writeKeyValue "id" obj.name
			fs.writeComma()
			fs.writeKeyValue "done" false
			fs.writeComma()
			fs.writeKeyValue "position" (maxPosToMcv obj.pos)
			fs.writeComma()
			fs.writeKeyValue "level" "level00"
			fs.writeComma()
			fs.writeKeyValue "type" type
		fs.endObj()
	),

	fn exportUniqueFile uniqueCategory:"" = (
		local uniquesPath = project_path + uniques_path + "unique_" + uniqueCategory + ".json"

		local uniquesArray = #()
		case uniqueCategory of (
			"altars":  uniquesArray = uniqueAltars
			"coins":   uniquesArray = uniqueCoins
			"enemies": uniquesArray = uniqueEnemies
			default:   return false --Unknown unique, export nothing.
		)
		fs.begin uniquesPath --Create file.
			fs.beginObj()
				fs.writeKey ("unique_" + uniqueCategory)
				fs.beginArray()
				for i = 1 to uniquesArray.count do (
					local obj = getNodeByName uniquesArray[i]
					if obj != undefined then (
						if i > 1 then fs.writeComma()
						exportUniqueObj obj
					)
				)
				fs.endArray()
			fs.endObj()
		fs.end()
	),

	-- Globals uniqueCoins, uniqueAltars and uniqueEnemies contain the object names.
	fn exportUniques = (
		if uniqueAltars.count  > 0 then exportUniqueFile uniqueCategory:"altars"
		if uniqueCoins.count   > 0 then exportUniqueFile uniqueCategory:"coins"
		if uniqueEnemies.count > 0 then exportUniqueFile uniqueCategory:"enemies"
	),

	fn showBrokenMaps = (
		for objName in brokenMaps do (
			format "ERROR: Map support error in object %, should unwrap_reset.\n" objName
		)
		if brokenMaps.count > 0 then format "------EXPORT FAILED!!!!------"
	),

	fn exportAll = (
		deselect objects -- Deselect all objects so export is faster.

		-- Loop over all layers.
		for i = 0 to LayerManager.count - 1 do (
			-- Get the layer by index.
			local layer = LayerManager.getLayer i
			local layerName = layer.name
			local layerObjs
			layer.nodes &layerObjs
			-- Output filename based on .max filename.
			local ofilename = getFilenameFile maxFileName
			local scenePath = project_path + scenes_path + ofilename + "-" + layerName + ".scene"
			if layerName == "player" then scenePath = project_path + scenes_path + "test_player.scene"
			if layerName == "prefabMelee"  then scenePath = project_path + prefabPath + "test_enemyMelee.prefab"
			if layerName == "prefabFlying" then scenePath = project_path + prefabPath + "test_enemyFlying.prefab"

			-- Avoid creating scenes if layer is empty or invisible.
			if layerObjs.count >= 1 and not layer.isHidden then (
				format "Exporting to %\n" scenePath
				format "Please wait...\n"
				local nValidObjs = 0
				local wasValid = false
				-- Create scene file.
				fs.begin scenePath
				fs.beginArray()
				for obj in layerObjs do (
					obj.name = cleanName obj.name
					-- Avoid child objects, they will be added through parent. Also, don't export hidden objects.
					if obj.parent == undefined and not obj.isHidden then (
						format "Layer: %\t\tClass: %\t\t\tObject name: %\n" layerName ((classOf obj) as string) obj.name --DEBUG
						if classOf obj == Editable_poly then convertToMesh obj
						case layerName of (
							"altars":       ( wasValid = exportEntityAltar     obj nValidObjs )
							"cameras":      ( wasValid = exportEntityCamera    obj nValidObjs )
							"chrysalis":    ( wasValid = exportEntityChrysalis obj nValidObjs )
							"coins": 	    ( wasValid = exportEntityCoin      obj nValidObjs )
							"collisions":   ( wasValid = exportEntityCollision obj nValidObjs )
							"enemies":      ( wasValid = exportEntityEnemy	   obj nValidObjs )
							"instancing":   ( wasValid = exportEntityInstance  obj nValidObjs )
							"lights":       ( wasValid = exportEntityLight	   obj nValidObjs )
							"movables":     ( wasValid = exportEntityDefault   obj nValidObjs )
							"player":       ( wasValid = exportEntityPlayer	   obj nValidObjs )
							"prefabBall":   ( wasValid = exportEnemyPrefab	   obj nValidObjs )
							"prefabFlying": ( wasValid = exportEnemyPrefab 	   obj nValidObjs )
							"prefabMelee":  ( wasValid = exportEnemyPrefab	   obj nValidObjs )
							"triggers":     ( wasValid = exportEntityTrigger   obj nValidObjs )
							"trimesh":		( wasValid = exportEntityCollision obj nValidObjs )
							"mechanisms":	( wasValid = exportEntityMechSys   obj nValidObjs )
							"pure":		    ( wasValid = exportEntityDefault   obj nValidObjs )
							"spikes":	    ( wasValid = exportEntitySpikes	   obj nValidObjs )
							"breakables":	( wasValid = exportEntityDefault   obj nValidObjs )
							"0":		    ( wasValid = exportEntityDefault   obj nValidObjs )
							default: ( format "WARNING: Unknown layer %, not exported.\n" layerName )
						)
						if wasValid then (
							nValidObjs = nValidObjs + 1
						)
					)
				)
				fs.endArray()
				fs.end()
				format "Export finished.\n"

			) else format "WARNING: Layer % [%] NOT exported.\n" layerName scenePath
		format "----------------------------------------------------------------\n"
		)
		-- All layers and objects processed, begin export of stored unique objects.
		exportUniques()
		showBrokenMaps()
	)
)

/*
TODO: ¿Sacar el fov de las luces de algún parámetro? Ahora siempre le pongo 40.
TODO: Las splines de attack hitboxes tienen coordenadas de mundo.
TODO: Modificar editor prefab enemigo bola (ahora mismo es idéntico al melee).
TODO: Si la spline hija es de una cámara o luz, habría que exportarlo diferente.

TODO: NO PONER ID 1 A MULTIMATERIAL CUANDO LOS USEMOS, está provisional
	  para whitebox (está en función exportMaterial).

NOTA: Si se actualiza un material que ya existía, hay que copiarlo a mano porque
	  se evita exportarlo si ya existe. 
NOTA: Si un mesh tiene box hija, ambas tienen que tener el pivot en el mismo sitio.
*/

/*------------------------------------------------------------------------------
-- Notas de uso:
--------------------------------------------------------------------------------
Pasos para instalar los scripts:
------------------------------------------
1. Customize > Configure System Paths... > Additional Startup Scripts > Modify...
2. Seleccionar Proyecto\Bin\maxscript\autoboot\
3. Darle a Use Path.
4. Reiniciar Max.
5. Ir a pestaña Utilities, darle al botón MAXScript, en el desplegable Utilities que aparece abajo seleccionar MCV Exporter.
6. Ya se pueden usar los scripts dándole a los botones que han aparecido debajo.
-------------------------------
Condiciones de exportación:
-------------------------------
Las layers invisibles o vacías no se exportan.
Lo visible como Editable Mesh.
Lo visible sólido debe tener una forma básica (Box, Sphere o Plane) o una mesh como hija. Si no es hija
se exportará en la escena collisions.
Las meshes		   	 	 deben estar en una layer llamada "0".
Los enemigos		     deben estar en una layer llamada "enemies".
Las crisálidas 		     deben estar en una layer llamada "chrysalis".
Las plataformas móviles  deben estar en una layer llamada "movables" (excepto si son puras, en ese caso van en pure).
Las cámaras		    	 deben estar en una layer llamada "cameras".
Las puras			     deben estar en una layer llamada "pure".
Las luces			     deben estar en una layer llamada "lights".
Los triggers		     deben estar en una layer llamada "triggers".
Los colliders invisibles deben estar en una layer llamada "collisions".

Las monedas únicas tienen que contener "unique" en el nombre.

El objeto low-res del LOD debe ser padre de un dummy que contiene los objetos a los que sustituye cuando está activo.

Se sabe el tipo de cada enemigo por el nombre ("melee" o "ball"), por defecto son voladores. --> En el futuro igual cambia con attributes.
El proyecto debe tener las siguientes carpetas:
 bin/data/scenes/
 bin/data/meshes/
 bin/data/materials/
 maxscript/  --> Aquí tienen que estar estos scripts.
--------------------------------------------------------------------------------*/

-- Take the project path from this script path. Result should be ~"D:\Dev\MomentumEngine-Real\Bin\"
global exportPath = cleanPath (substituteString (getThisScriptFilename()) "maxscript\\export_scene.ms" "Bin\\")
exporter = TSceneExporter project_path:exportPath

--mexporter = TMaterialExporter project_path:"pep/" base_path:"path/"
--mexporter.exportStdMaterial $.mat "pep" 

exporter.exportAll()