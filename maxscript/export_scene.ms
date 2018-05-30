clearListener()
gc()

fileIn "export_mesh.ms" -- To avoid error on first execution.
fileIn "add-attributes.ms" -- To avoid error on first execution.
fileIn "utils.ms"

-------------------------------------------------------------
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
		if classof val == string then (
			if addIndent then indent()
			format " \"%\"" val to:fs
		) else if classof val == Point3 then (
			if addIndent then indent()
			format " \"% % %\"" val.x val.y val.z to:fs
		) else if classof val == quat then (
			if addIndent then indent()
			-- Quats rotate in the opposite direction
			format " \"% % % %\"" val.x val.y val.z -val.w to:fs
		) else (
			if addIndent then indent()
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
	
	fn arrayOfStrings arr = (
		beginArray()
		for idx = 1 to arr.count do (
			if idx != 1 then writeComma()
			writeName arr[idx]
		)
		endArray()
	)
)

-- ----------------------------------------------------------
-- ----------------------------------------------------------
-- ----------------------------------------------------------
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

		-- bricks
		local mapBaseName = getFilenameFile mapFilename
		-- "C:\engine\bin\data\textures\"
		local mapBasePath = getFilenamePath mapFilename
		-- "data\textures\bricks.dds"
		local json_filename = base_path + "textures/" + mapBaseName + ".dds"

		-- "C:\engine\bin\data\textures\bricks.dds"
		local ofull_path = project_path + json_filename
		
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

	-- Exports a single std material to a json format
	fn exportStdMaterial mat mat_name = (
		--format "Exporting material % % %\n" mat_name mat (classof mat as string)
		fs.begin (project_path + mat_name )
		fs.beginObj()
			fs.writeKeyValue "technique" "pbr.tech"
			fs.writeComma()
			fs.writeKeyValue "shadows" true
			fs.writeComma()
			fs.writeKey "textures"
			fs.beginObj()
		
			if classOf mat == StandardMaterial then (
				exportMap mat.diffuseMap "albedo" "null_albedo"
			    fs.writeComma()
				exportMap mat.bumpMap "normal" "null_normal"
				fs.writeComma()
				exportMap mat.glossinessMap "metallic" "null_metallic"
				fs.writeComma()
				exportMap mat.specularLevelMap "roughness" "null_roughness"
				
				--ambientMap
				--specularMap
				--selfIllumMap
				--opacityMap
				--filterMap
				--reflectionMap
				--refractionMap
				--displacementMap
			)

			fs.endObj()
		fs.endObj()
		fs.end()
	),
	
	-- Exports an empty solid material to a json format
	fn exportEmptyMaterial mat_name = (
		/*exportMap undefined "albedo" "null_albedo"
		fs.writeComma()
		exportMap undefined "normal" "null_normal"*/
		
		fs.begin (project_path + mat_name )
		fs.beginObj()
			fs.writeKeyValue "technique" "pbr.tech"
			fs.writeComma()
			fs.writeKeyValue "shadows" true
			fs.writeComma()
			fs.writeKey "textures"
			fs.beginObj()
				fs.writeKeyValue "albedo" "data/textures/null_albedo.dds"
				fs.writeComma()
				fs.writeKeyValue "normal" "data/textures/null_normal.dds"
			fs.endObj()
		fs.endObj()
		fs.end()
	),
	
	-- Will return an array of all the materials names used by obj and exported by us
	fn exportMaterial mat mat_path obj = (
		local exported_materials = #()
		local matClass = classOf mat
		--format "material class: %\n" matClass as string --DEBUG
		
		if mat == undefined then (
			--throw ("Obj " + obj.name + " does NOT have a material")
			local mat_name = mat_path + "empty.material"
			append exported_materials mat_name
			exportEmptyMaterial mat_name
		)
		
		if matClass == StandardMaterial then (
			local mat_name = mat_path + (cleanName mat.name) + ".material"
			append exported_materials mat_name
			exportStdMaterial mat mat_name

		) else if matClass == MultiMaterial then (
			local multi_mat = mat

			local materials_of_mesh = getMaterialsUsedByMesh obj
			for mat_idx = 1 to materials_of_mesh.count do (
				if materials_of_mesh[ mat_idx ] == undefined then continue
				local mat_of_mesh = multi_mat[ mat_idx ]

				if mat_of_mesh == undefined then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string)+ " but the multimat does not have this submat")
				if classof mat_of_mesh != StandardMaterial then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string) + " but the multimat in this slot is not a stdMaterial")

				local mat_name = mat_path + mat_of_mesh.name + ".material"
				append exported_materials mat_name
				exportStdMaterial mat_of_mesh mat_name
			)
		)
		return exported_materials
	)
)

-- ----------------------------------------------------------
-- ----------------------------------------------------------
-- ----------------------------------------------------------
struct TSceneExporter (
	fs = TJsonFormatter(),
	project_path = "",
	base_path = "data/",
	scenes_path = base_path + "scenes/",
	mesh_path = base_path + "meshes/",
	mats_path = base_path + "materials/",
	prefabPath = base_path + "prefabs/",
	
	fn exportName obj = (
		fs.writeKeyValue "name" obj.name
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
	
	fn isCollider obj = (
		if  classof obj != Box 	 and
			classof obj != Sphere and
			classof obj != Capsule and
			classof obj != Editable_Mesh and
			classof obj != Plane then (
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
	
	-- Ends one entity and starts a new one.
	fn startNewEntity = (
			fs.endObj() -- - - - - - - end previous entity - - -
		fs.endObj() -- - - - end previous json object - - - - - -
		fs.writeComma()
		fs.beginObj() -- - - - - start new json object - - - - - -
			fs.writeKey "entity"
			fs.beginObj() -- - - start new entity  - - - - - - -
	),
	
	-- --STRING ARRAY (generic for tags, groups, masks )---------
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
		local circlePoint = getKnotPoint circleObj 1 1
		local platformPos = platform.pos
		
		-- TODO: Aquí la idea sería aumentar el move_offset según el ángulo de la plataforma respecto
		-- al centro del círculo, si no todas las plataforms que comparten centro se mueven superpuestas.
		-- Ahora mismo no va fino, necesito ángulos negativos también.
		local anglePlatformToCircle = get3pointAngle center circlePoint platformPos
		if anglePlatformToCircle == 0 then return 0
		else return anglePlatformToCircle / 180
	),
	
	fn exportSplineKnots obj = (
		fs.beginArray()
			--Create a copy and convert it to SplineShape to be able to get the knots.
			--We need this if the object is a circle. Also we avoid modifying the max file.
			local objCopy = copy obj
			convertToSplineShape objCopy
		
			knotsInSpline = numKnots objCopy
			for i = 1 to (knotsInSpline) do (
				if i > 1 then fs.writeComma()
				else if classOf objCopy == SplineShape then (
					--Circular movement only needs the center and at least one knot.
					local center = maxPosToMcv objCopy.pos
					fs.writeValue center addIndent:true
					fs.writeComma()
				)
				
				-- getKnotPoint <shape> <spline_index_integer> <knot_index_integer>
				local knotPos = getKnotPoint objCopy 1 i
				local mcvKnotPos = maxPosToMcv knotPos
				fs.writeValue mcvKnotPos addIndent:true
				if (classOf objCopy == line or 
					classOf objCopy == SplineShape) and
				   (i == 1 or i == knotsInSpline) then (
					-- Duplicate first and last knots. This avoids error if the spline has less than 4 knots in catmull.
					-- This also forces following the real shape of the spline.
					fs.writeComma()
					fs.writeValue mcvKnotPos addIndent:true
				)
			)
			delete objCopy
		fs.endArray()
	),
	
	-- ----------------------------------------------------------------------
	-- -- · COMPONENTS · -------------------------------------------
	-- ----------------------------------------------------------------------

	-- --COMP: TRANSFORM-----------------------------------------------
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
					fs.writeKeyValue "rotation" (maxRotToMcv transf)
				) else (
					fs.writeKeyValue "lookat" lookAtCoord
				)
			)
		fs.endObj()
	),

	-- --RENDER MESH (also exports MATERIAL)----------------------------------------
	fn exportRenderMesh obj = (
		local mat = obj.material
		if mat == undefined then (
			format "Warning: Obj % doesn't have a material, a default will be assigned.\n" obj.name
		)
		else if not (isValidName mat.name) then (
			mat.name = cleanName mat.name
			format "Warning: material had invalid name, changed spaces to underscores.\n"
		)

		fs.beginObj()
			-- Export Mesh
			local mesh_name = mesh_path + obj.name + ".mesh"
			fs.writeKeyValue "mesh" mesh_name
			
			-- Generate the mesh file.
			exportMeshObject obj (project_path + mesh_name)

			fs.writeComma()

			-- Export material
			--local mat_name = mats_path + mat.name + ".material"
			--fs.writeKeyValue "material" mat_name
			local me = TMaterialExporter project_path:project_path base_path:base_path
			local exported_materials = me.exportMaterial mat mats_path obj

			fs.writeKey "materials"
			fs.arrayOfStrings exported_materials
		fs.endObj()
	),

	-- --SPLINE-------------------------------------------------------
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
				local followSpeed = 0.2
				if childClass == circle then closed = true
				else closed = isClosed child 1
				
				if closed then followSpeed = 0.001
				
				fs.writeComma()
				fs.writeKey type
				fs.beginObj()
					fs.writeKeyValue "rotation_axis" "0 1 0"
					fs.writeComma()
					fs.writeKeyValue "rotation_speed" 0
					fs.writeComma()
					fs.writeKeyValue "curve_type" curveType
					fs.writeComma()
					fs.writeKeyValue "automove" true
					fs.writeComma()
					fs.writeKeyValue "loop" true
					fs.writeComma()
					fs.writeKeyValue "closed" closed
					fs.writeComma()
					fs.writeKeyValue "move_offset" moveOffset
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
	
	-- --COMP: ROTATE (platform_simple)-----------------------------------------------------
	fn exportPlatfRotation = (
		fs.writeComma()
		fs.writeKey "platform_simple"
		fs.beginObj()
			fs.writeKeyValue "rotation_axis" "1 0 0"
			fs.writeComma()
			fs.writeKeyValue "rotation_speed" 1
		fs.endObj()
	),
	
	-- --COMP: BREAKABLE------------------------------------------------
	fn exportBreakable hp:1 = (
		fs.writeComma()
		fs.writeKey "breakable"
		fs.beginObj()
			fs.writeKeyValue "hp" hp
		fs.endObj()
	),
	
	-- --COMP: MECHANISM SYSTEM------------------------------------------------
	-- What is affected when activating a mechanism.
	fn exportMech obj = (
		fs.writeComma()
		fs.writeKey "mechanism_system"
		fs.beginObj()
			fs.writeKeyValue "number_of_mechanisms" 1
		fs.endObj()
	),

	-- --CONTENTS OF THE collider_mesh or tri_mesh collider----------
	fn exportColliderMeshContents obj meshShape:"convex_mesh" forceMesh:false mov:false = (
		if obj.layer.name == "trimesh" then meshShape = "tri_mesh"
		local mesh_name = mesh_path + obj.name + ".col_mesh"
		if forceMesh then mesh_name = mesh_path + "plataformaPuraSI_tri.col_mesh"

		local meshType = "static"
		if mov then meshType = "kinematic"
		fs.writeKeyValue "mesh_src" mesh_name
		fs.writeComma()
		fs.writeKeyValue "shape" meshShape

		if not forceMesh then
			exportMeshObject obj (project_path + mesh_name) vtxType:"Pos"
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
	
	-- --COMP: COLLIDER------------------------------------------
	fn exportCompCollider obj first:true meshMode:false colliderGroups: masks: trig:false dyn:false mov:false = (
		local objClass = classOf obj
		local layerName = obj.layer.name
		if objClass == Editable_poly then convertToMesh obj
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
				else if mov or layerName == "pure" then
					fs.writeKeyValue "type" "kinematic"
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
	
	-- --COLLIDER (name, transform and collider)-----------
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

	-- --COMP: CAMERA TRIGGER ------------------------------------------
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
	
	-- --PURITY--------------------------------------------------------------------
	fn exportPurity = (
		fs.beginObj()
			fs.writeKeyValue "mesh" (mesh_path + "plataforma_puraNO.mesh")
			fs.writeComma()
			fs.writeKey "materials"
			fs.beginArray()
				fs.writeValue (mats_path + "NOpure.material") addIndent:true
			fs.endArray()
			fs.writeComma()
			fs.writeKeyValue "enabled" false
		fs.endObj()
		fs.writeComma()
		fs.beginObj()
			fs.writeKeyValue "mesh" (mesh_path + "plataforma_pura_aristas.mesh")
			fs.writeComma()
			fs.writeKey "materials"
			fs.beginArray()
				fs.writeValue (mats_path + "pure.material") addIndent:true
			fs.endArray()
			fs.writeComma()
			fs.writeKeyValue "enabled" false
		fs.endObj()
	),

	-- --EDITABLE MESH (includes pure, break, mov, rot & mech children)--------------------------------
	fn exportEditableMesh obj = (
		--format "Exporting EditableMesh obj % of class %\n" obj.name ((classof obj) as string) --DEBUG
		local pure 		= obj.layer.name == "pure"
		local breakable = obj.layer.name == "breakables"
		local rot		= obj.layer.name == "rotate"
		local mech		= obj.layer.name == "mechanisms"
		
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		fs.writeKey "render"
		fs.beginArray()
			if pure then exportPurity() 
			else exportRenderMesh obj
		fs.endArray()				
		
		local colliderCount = 0
		if hasChildCollider obj then (
			for child in obj.children do (
				if isCollider child then (
					colliderCount += 1
					local isFirst = colliderCount == 1
					if isFirst then fs.writeComma()
					--format "Exporting child collider obj % of class %\n" child.name ((classof child) as string) --DEBUG
					local movable = hasChildLine child
					local groups = #("scenario")
					if breakable then append groups "enemy"

					local objToExport = child
					if pure then (
						fs.writeKeyEmpty "purity"
						objToExport = obj
					)
					exportCompCollider objToExport first:isFirst meshMode:pure mov:(movable or rot) colliderGroups:groups

					if breakable then exportBreakable()
					if movable then exportSpline child
					if rot then exportPlatfRotation()
					if mech then exportMech child
				)
			)
		)
	),

	-- ----------------------------------------------------------
	-- -- · ENTITIES · ------------------------------------
	-- ----------------------------------------------------------

	-- --ENTITY with only COLLISION ----------------------------------------------
	fn exportEntityCollision obj nValidObjs = (
		local objClass = classOf obj
		if objClass == Editable_poly then convertToMesh obj
		if not isCollider obj and
		   not objClass == Editable_mesh then return false
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEntityCollider obj meshMode:(not (isCollider obj and objClass != Editable_mesh)) dyn:(hasChildLine obj)
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),
	
	-- --ENTITY: COIN-------------------------------------------------------
	fn exportEntityCoin obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		else fs.writeDummyEntity "coin"
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportName obj
				fs.writeComma()
				exportTransform obj noRot:true
				fs.writeComma()
				fs.writeKeyValue "prefab" (prefabPath + "coin.prefab")
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --ENTITY: CAMERA (also exports child line)--------------------------
	fn exportEntityCamera obj nValidObjs = (
		local objClass = classOf obj
		format "name: %\n" obj.name
		if objClass == TargetCamera or
				objClass == FreeCamera then (
			if nValidObjs > 0 then fs.writeComma()
					
			fs.beginObj()
				fs.writeKey "entity"
				fs.beginObj() -- - - - - - - - - - - - - - - - - - -
					exportName obj
					fs.writeComma()
					if objClass == TargetCamera then (
						exportTransform obj lookAtCoord:obj.Target[3].pos --[3] is the Transform.
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
					if hasChildLine obj then (
						exportSpline obj
					)
				--format "Camera % target distance: %\n" obj.name obj.targetDistance
				fs.endObj() -- - - - - - - - - - - - - - - - - - - -
			fs.endObj()
		)

		if hasChildCollider obj then (
			local child = obj.children[1]
			if classof child == Box then (
				fs.writeComma()
				fs.beginObj()
					fs.writeKey "entity"
					fs.beginObj() -- - - - - - - - - - - - - - - - - - -
						-- Add trigger.
						if objClass == FreeCamera then exportCameraTrigger child
						else exportSuggestCameraTrigger child
					fs.endObj() -- - - - - - - - - - - - - - - - - - - -
				fs.endObj()
				return true
			)
		)
		return true
	),

	-- --ENTITY: CHRYSALIS (also exports child line)----------------------------
	fn exportEntityChrysalis obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		else fs.writeDummyEntity "chrysalis"
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --ENTITY: ENEMY (also exports child line)--------------------------------------
	fn exportEntityEnemy obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		else fs.writeDummyEntity "enemy"
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --ENTITY: LIGHT (also exports child line)---------------------------------------
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),
	
	-- --ENTITY: TRIGGERS (also exports child line)-------------------------------------
	fn exportEntityTrigger obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				if stringContainsSubstring obj.name "Bounds" then (
					fs.writeKeyEmpty "outOfBounds"
					exportEntityCollider obj trig:true dyn:true colliderGroups:#("all") masks:#("characters")
				)
				else exportEntityCollider obj trig:true colliderGroups:#("trigger") masks:#("player")

				if stringContainsSubstring obj.name "respawn" and 
					obj.children != undefined then (
						local respawnPos = "0 0 0"
						for child in obj.children do (
							if classOf child == Dummy then respawnPos = (maxPosToMcv child.position)
						)
						
						fs.writeComma()
						fs.writeKey "respawn_point"
						fs.beginObj()
							fs.writeKeyValue "respawn_position" respawnPos
						fs.endObj()
				)
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --ENTITY: DEFAULT (exports editable mesh)---------------------------
	fn exportEntityDefault obj nValidObjs = (
		if classOf obj == Editable_poly then convertToMesh obj
		if classOf obj != Editable_mesh then (
			format "WARNING: Object % is NOT an Editable mesh, not exported.\n" obj.name
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		else fs.writeDummyEntity obj.layer.name
		
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEditableMesh obj
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --ENTITY: MECHANISM (also exports children)----------------------
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
		
				fs.writeKey "mechanism"
				fs.beginObj()
					fs.writeKeyValue "deactivationTime" 5
					fs.writeComma()
					fs.writeKeyValue "isResettable" true
					fs.writeComma()
					fs.writeKeyValue "reactivationTime" 3
					fs.writeComma()
					fs.writeKey "systems"
					fs.beginArray()
						for i = 1 to obj.children.count do (
							if i > 1 then fs.writeComma()
							fs.writeValue obj.children[i].name addIndent:true
						)
					fs.endArray()
				fs.endObj()
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()

		for child in obj.children do (
			exportDefault child (nValidObjs + 1)
		)
		return true
	),
	
	-- ----------------------------------------------------------
	-- ----------------------------------------------------------
	
	-- --ENEMY PREFAB ---------------------------------------
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		
		exportChildHitboxes obj obj.height
		
		return true
	),
	
	-- --ENTITY: PLAYER HITBOXES--------------------------------------------
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
					fs.endObj() -- - - - - - - - - - - - - - - - - - - - -
				fs.endObj()
			)
		)
	),
	
	-- --ENTITY: PLAYER ---------------------------------------------------
	fn exportEntityPlayer obj nValidObjs = (
		if classOf obj == Editable_poly then convertToMesh obj
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
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		
		exportChildHitboxes obj capsuleHeight

		return true
	),
	
	-- -------------------------------------------------------------------
	-- -------------------------------------------------------------------
	
	fn exportUniques = (
		local filePath = project_path + scenes_path + ofilename + "-uniques.json"
		fs.begin scenePath
				fs.beginArray()
		
				fs.endArray()
		fs.end()
	),
	
	fn exportAll = (
		deselect objects -- Deselect all objects, export is faster.
		
		global uniqueCoins  = #()
		global uniqueAltars = #()
		
		-- Loop over all layers.
		for i = 0 to LayerManager.count - 1 do (
			-- Get the layer by index.
			local layer = LayerManager.getLayer i
			local layerName = layer.name
			local layerObjs
			layer.nodes &layerObjs
			-- Output filename based on .max filename.
			--format "Layer: %, objs: %\n" layerName layerObjs --DEBUG
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
						format "Layer: %\t\tClass: %\t\t\tObject name: %\n" layerName ((classof obj) as string) obj.name --DEBUG
						case layerName of (
							"cameras":      ( wasValid = exportEntityCamera    obj nValidObjs )
							"chrysalis":    ( wasValid = exportEntityChrysalis obj nValidObjs )
							"coins": 	    ( wasValid = exportEntityCoin      obj nValidObjs )
							"collisions":   ( wasValid = exportEntityCollision obj nValidObjs )
							"enemies":      ( wasValid = exportEntityEnemy	   obj nValidObjs )
							"lights":       ( wasValid = exportEntityLight	   obj nValidObjs )
							"movables":     ( wasValid = exportEntityDefault   obj nValidObjs )
							"player":       ( wasValid = exportEntityPlayer	   obj nValidObjs )
							"prefabBall":   ( wasValid = exportEnemyPrefab	   obj nValidObjs )
							"prefabFlying": ( wasValid = exportEnemyPrefab 	   obj nValidObjs )
							"prefabMelee":  ( wasValid = exportEnemyPrefab	   obj nValidObjs )
							"rotate": 		( wasValid = exportEntityDefault   obj nValidObjs )
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
		format "---------------------------------------------------------------------\n"
		)
		-- All layers and objects processed, begin export of stored unique objects.
		exportUniques()
	)
)

/*
TODO: ¿Sacar el fov de las luces de algún parámetro? Ahora siempre le pongo 40.
TODO: Las splines de attack hitboxes tienen coordenadas de mundo.
TODO: Modificar editor prefab enemigo bola (ahora mismo es idéntico al melee).
TODO: Probar spline cerrada, habría que poner el último knot al principio seguramente.
TODO: Si la spline hija es de una cámara o luz, habría que exportarlo diferente.

TODO: Generar arrays de strings con los nombres de los objetos únicos,
	    luego recuperar el objeto con (getNodeByName "altar") y generar el archivo uniques/unique.json.

NOTA: Si un mesh tiene box hija, ambas tienen que tener el pivot en el mismo sitio.
*/

/*----------------------------------------------------------------------------------
-- Notas de uso:
------------------------------------------------------------------------------------
Pasos para instalar los scripts:
------------------------------------------
1. Customize > Configure System Paths... > Additional Startup Scripts > Modify...
2. Seleccionar Proyecto\Bin\maxscript\autoboot\
3. Darle a Use Path.
4. Reiniciar Max.
5. Ir a pestaña Utilities, en el desplegable Utilities seleccionar MCV Exporter.
6. Ya se pueden usar los scripts dándole a los botones que han aparecido debajo.
-------------------------------
Condiciones de exportación:
-------------------------------
Las layers invisibles o vacías no se exportan.
Lo visible como Editable Mesh.
Lo visible sólido debe tener una forma básica (Box, Sphere o Plane) o una mesh como hija. Si no es hija
se exportará en la escena collisions.
Las meshes		    deben estar en una layer llamada "0".
Los enemigos		    deben estar en una layer llamada "enemies".
Las crisálidas 		    deben estar en una layer llamada "chrysalis".
Las plataformas móviles  deben estar en una layer llamada "movables".
Las cámaras		    deben estar en una layer llamada "cameras".
Las luces			    deben estar en una layer llamada "lights".
Los triggers		    deben estar en una layer llamada "triggers".
Los colliders invisibles     deben estar en una layer llamada "collisions".
 
Se sabe el tipo de cada enemigo por el nombre ("melee" o "ball"), por defecto son voladores. --> En el futuro igual cambia con attributes.
El proyecto debe tener las siguientes carpetas:
 bin/data/scenes/
 bin/data/meshes/
 bin/data/materials/
 maxscript/  --> Aquí tienen que estar estos scripts.
----------------------------------------------------------------------------------*/

-- Take the project path from this script path. Result should be ~"D:\Dev\MomentumEngine-Real\Bin\"
global exportPath = cleanPath (substituteString (getThisScriptFilename()) "maxscript\\export_scene.ms" "Bin\\")
exporter = TSceneExporter project_path:exportPath

--mexporter = TMaterialExporter project_path:"pep/" base_path:"path/"
--mexporter.exportStdMaterial $.mat "pep" 

exporter.exportAll()