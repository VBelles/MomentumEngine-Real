clearListener()
gc()

fileIn "export_mesh.ms" -- To avoid error on first execution.
fileIn "add-attributes.ms" -- To avoid error on first execution.

-- 
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
	fn writeKey key_str = (
		indent()
		format "\"%\":\n" key_str to:fs
	),
	
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
	
	fn writeKeyValue key_str val = (
		indent()
		format "\"%\":" key_str to:fs
		writeValue val
	),

	-- --------------------
	fn writeName name = (
		indent()
		format "\"%\"" name to:fs
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

fn cleanMaterialName oldName = (
	return substituteString oldName " " "_"
)

-- ----------------------------------------------------------
-- ----------------------------------------------------------
-- ----------------------------------------------------------
struct TMaterialExporter (
	fs = TJsonFormatter(),
	project_path,              -- Assigned in the ctor
	base_path,                 -- Assigned in the ctor

	-- TODO: Avoid exporting material if it already exists.
	
	fn exportMap map alias default_value = (
		if map == undefined then (
			local json_filename = base_path + "textures/" + default_value + ".dds"
			fs.writeKeyValue alias json_filename
			return false
		)

		-- "C:\engine\bin\data\textures\bricks.dds"
		-- "C:\download\bricks.dds"
		local map_filename = map.filename

		-- bricks
		local base_name = getFilenameFile map_filename

		-- data\textures\bricks.dds"
		local json_filename = base_path + "textures/" + base_name + ".dds"

		-- "C:\engine\bin\data\textures\bricks.dds"
		local ofull_path = project_path + json_filename

		-- Check if ofull_path exists
		if not doesFileExist ofull_path then (
			format "We should copy from % to %\n" map_filename ofull_path
			copyFile map_filename ofull_path
		)

		fs.writeKeyValue alias json_filename
	),

	-- Exports a single std material to a json format
	fn exportStdMaterial mat mat_name = (
		--format "Exporting material % % %\n" mat_name mat (classof mat as string)
		fs.begin (project_path + mat_name )
		fs.beginObj()
			fs.writeKeyValue "technique" "textured.tech"
			fs.writeComma()
			fs.writeKeyValue "shadows" true
			fs.writeComma()
			fs.writeKey "textures"
			fs.beginObj()
		
			if classof mat == Standardmaterial then (
				exportMap mat.diffusemap "albedo" "null_albedo"
			    fs.writeComma()
				exportMap mat.bumpMap "normal" "null_normal"
				-- ... other maps..
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
				fs.writeKeyValue "technique" "textured.tech"
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
		local matClass = classof mat
		--format "material class: %\n" matClass as string
		
		if mat == undefined then (
			--throw ("Obj " + obj.name + " does NOT have a material")
			local mat_name = mat_path + "empty.material"
			append exported_materials mat_name
			exportEmptyMaterial mat_name
		)
		
		if matClass == StandardMaterial then (
			local mat_name = mat_path + (cleanMaterialName mat.name) + ".material"
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

	fn isValidName aname = (
		return findString aname " " == undefined
	),

	fn stringContainsSubstring strng substrng = (
		matchpattern strng pattern:("?*" + substrng + "*")
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
				classof obj != Plane then (
			format "Wrong object %. Collisions must be Spheres, Boxes, Capsules or Planes.\n" obj.name
			return false
		)
		return true
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
	
	-- Converts a very small number to zero.
	fn smallNumToZero num = (
		if (num <  0.00001 and num > 0) or
		   (num > -0.00001 and num < 0) then return 0
		else return num
	),
	
	fn roundSmallPosToZeros pos = (
		local auxPos = pos
		auxPos.x = smallNumToZero pos.x
		auxPos.y = smallNumToZero pos.y
		auxPos.z = smallNumToZero pos.z
		return auxPos
	),
	
	fn roundSmallQuatToZeros q = (
		local auxQuat = q
		auxQuat.x = smallNumToZero q.x
		auxQuat.y = smallNumToZero q.y
		auxQuat.z = smallNumToZero q.z
		auxQuat.w = smallNumToZero q.w
		return auxQuat
	),
	
	-- --TRANSFORM-----------------------------------------------
	fn exportTransform obj yShift: lookAtCoord: = (
		fs.writeKey "transform"
	
		local max2mcv = rotateXMatrix -90
		local mcv2max = rotateXMatrix 90
		local mcv_position = obj.position * max2mcv
		
		if yShift != unsupplied then (
			local objY = mcv_position.y
			local adjustedPos = objY + yShift
			mcv_position.y = adjustedPos
		)
		
		-- From mcv, we will go to max, apply the max transform
		-- and go back to mcv coord system.
		local mcv_transform = mcv2max * obj.transform * max2mcv

		-- Take just the rotation as quaternion
		local mcv_quat = mcv_transform.rotationPart as quat
		
		mcv_position = roundSmallPosToZeros mcv_position
		mcv_quat = roundSmallQuatToZeros mcv_quat

		fs.beginObj()
			fs.writeKeyValue "pos" mcv_position
			fs.writeComma()
			if lookAtCoord == unsupplied then (
				fs.writeKeyValue "rotation" mcv_quat
			) else (
				fs.writeKeyValue "lookat" lookAtCoord
			)
		fs.endObj()
	),

	-- --RENDER MESH---------------------------------------------
	fn exportRenderMesh obj = (
		local mat = obj.material
		if mat == undefined then (
			format "Warning: Obj % doesn't have a material, a default will be assigned.\n" obj.name
			-- TODO: assign default material.
		)
		else if not (isValidName mat.name) then (
			mat.name = cleanMaterialName mat.name
			format "Warning: material had invalid name, changed spaces to underscores.\n"
		)

		fs.beginObj()
			-- Export Mesh
			local mesh_name = mesh_path + obj.name + ".mesh"
			fs.writeKeyValue "mesh" mesh_name
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

	-- --COMP COLLIDER------------------------------------------
	fn exportCompCollider obj colliderGroups: masks: trig: dyn: = (
		fs.writeKey "collider"
		fs.beginObj()

		if classof obj == Sphere then (
			fs.writeKeyValue "shape" "sphere"
			fs.writeComma()
			fs.writeKeyValue "radius" obj.radius
		) else if classof obj == Box then (
			fs.writeKeyValue "shape" "box"
			fs.writeComma()
			-- Warning, order might be incorrect!!!
			fs.writeKeyValue "halfExtent" [abs obj.width / 2, abs obj.height / 2, abs obj.length / 2]
		) else if classof obj == Plane then (
			fs.writeKeyValue "shape" "plane"
			fs.writeComma()
			-- Last value should be Y position.
			fs.writeKeyValue "planeDesc" "0 1 0 0"
		) else if classof obj == Capsule then (
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
		)

		-- Add it in the user properties panel of max:  density = 10
		local density = getUserProp obj "density"
		if density != undefined then (
			fs.writeComma()
			fs.writeKeyValue "density" density
		)

		if obj.layer.name == "triggers" or
		   (trig != unsupplied and trig) then (
			fs.writeComma()
			fs.writeKeyValue "is_trigger" trig
		)
		
		if dyn != unsupplied and dyn then (
			fs.writeComma()
			fs.writeKeyValue "type" "dynamic"
		) else (
			fs.writeComma()
			fs.writeKeyValue "type" "static"
		)
		
		if colliderGroups != unsupplied then (
			fs.writeComma()
			exportStringArray "group" colliderGroups
		)
		
		if masks != unsupplied then (
			fs.writeComma()
			exportStringArray "mask" masks
		)

		fs.endObj()
	),
	
	-- -- Purity----------
	fn exportPurity = (
		fs.writeKeyEmpty "purity"
		exportStringArray "tags" #("pure")		
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
					local center = objCopy.pos * rotateXMatrix -90
					fs.writeValue center addIndent:true
					fs.writeComma()
				)
				
				-- getKnotPoint <shape> <spline_index_integer> <knot_index_integer>
				local knotPos = getKnotPoint objCopy 1 i
				local mcvKnotPos = knotPos * rotateXMatrix -90
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
	
	-- --SPLINE--------------------------------------------------
	fn exportSpline obj type: = (
		-- Executed only if we already know it has a child line.
		local curveType = "catmull-rom"
		for child in obj.children do (
			local childClass = classOf child
			if isLine child then (
				if childClass == line then curveType = "catmull-rom"
				if childClass == circle or
			       childClass == SplineShape then curveType = "circular"
				
				fs.writeComma()
				if type == unsupplied then fs.writeKey "follow_curve"
				else fs.writeKey type
				
				fs.beginObj()
					fs.writeKeyValue "curve_type" curveType
					fs.writeComma()
					fs.writeKeyValue "automove" true
					fs.writeComma()
					fs.writeKeyValue "loop" true
					fs.writeComma()
					fs.writeKeyValue "closed" (isClosed child 1)
					fs.writeComma()
					fs.writeKeyValue "speed" 0.2
					fs.writeComma()
					fs.writeKey "knots"
					exportSplineKnots child
					
				fs.endObj()
				return true -- Only exports the first line found.
			)
		)
	),
	
	-- --EDITABLE MESH-------------------------------------------
	fn exportEditableMesh obj pure: breakable: = (
		--format "Exporting EditableMesh obj % of class %\n" obj.name ((classof obj) as string)
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		fs.writeKey "render"
		fs.beginArray()
			exportRenderMesh obj
		fs.endArray()

		for child in obj.children do (
			if isCollider child then (
				fs.writeComma()
				--format "Exporting child collider obj % of class %\n" child.name ((classof child) as string)
				local movable = hasChildLine child
				if pure == unsupplied then pure = false
				exportCompCollider child dyn:(pure or movable)
				if pure then (
					fs.writeComma()
					exportPurity()
				)
				if breakable != unsupplied and breakable then (
					fs.writeComma()
					fs.writeKey "breakable"
					fs.beginObj()
						fs.writeKeyValue "hp" 1
					fs.endObj()
				)
				if movable then (
					exportSpline child
				)
			)
		)
	),

	-- --ENTITY COLLIDER-----------------------------------------
	fn exportEntityCollider obj colliderGroups: trig: dyn: = (
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		exportCompCollider obj colliderGroups:colliderGroups trig:trig dyn:dyn
	),
		
	-- --SHADOW --------------------------------------------------
	fn exportShadow = (
		fs.writeKey "shadow"
		fs.beginObj()
			fs.writeKeyValue "offset" "0 0.1 0"
			fs.writeComma()
			fs.writeKeyValue "mesh" "data/meshes/shadow.mesh"
			fs.writeComma()
			fs.writeKey "materials"
			fs.beginArray()
				fs.writeValue "data/materials/black.material" addIndent:true
			fs.endArray()
		fs.endObj()
	),
	
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
						fs.writeKey "hierarchy"
						fs.beginObj()
							fs.writeKeyValue "parent" obj.name
						fs.endObj()
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
							exportSpline child type:"hitboxPath"
						)
					fs.endObj() -- - - - - - - - - - - - - - - - - - - - -
				fs.endObj()
			)
		)
	),
	
	-- ----------------------------------------------------------
	-- ----------------------------------------------------------

	-- --PLAYER ---------------------------------------------------
	fn exportPlayer obj nValidObjs = (
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
	
	-- --Give power --
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
	
	-- --Behavior tree --
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
	
	-- --ENEMY PREFAB ---------------------------------------
	fn exportEnemyPrefab obj nValidObjs = (
		if classOf obj != Capsule and classOf obj != Sphere then return false
		if obj.parent != undefined then return false
		local meshPath = ""
		local enemyType = "flying"
		if stringContainsSubstring obj.layer.name "melee" then (
			meshPath = "data/meshes/enemyMelee.mesh"
			enemyType = "melee"
		)
		else if stringContainsSubstring obj.layer.name "ball" then (
			meshPath = "data/meshes/enemigoBola.mesh"
			enemyType = "ball"
		)
		else (
			meshPath = "data/meshes/crisalida.mesh"
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
	
	-- --DEFAULT ------------------------------------------------
	fn exportDefault obj nValidObjs pure: breakable: = (
		if classOf obj != Editable_mesh then (
			format "WARNING: Object % is NOT an Editable mesh, not exported.\n" obj.name
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEditableMesh obj pure:pure breakable:breakable
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --COLLISION ----------------------------------------------
	fn exportCollision obj nValidObjs = (
		if not isCollider obj then return false
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				--format "name: % childLine: %\n" obj.name ((hasChildLine obj) as string)
				exportEntityCollider obj dyn:(hasChildLine obj)
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),
	
	-- --CAMERA--------------------------------------------------
	fn exportCamera obj nValidObjs = (
		local objClass = classOf obj
		if objClass != TargetCamera and
				objClass != FreeCamera  then (
			if objClass != Targetobject then format "Warning: invalid object % of class % in cameras layer, not exported.\n" obj.name objClass
			return false
		)
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
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			--format "Camera % target distance: %\n" obj.name obj.targetDistance
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --CHRYSALIS-----------------------------------------------
	fn exportChrysalis obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				local prefabPath = base_path + "prefabs/crisalida.prefab"
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" prefabPath
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
	
	-- --ENEMY---------------------------------------------------
	fn exportEnemy obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				local enemyAirPrefabName   = "enemy_flying"
				local enemyMeleePrefabName = "enemy_melee"
				local enemyBolaPrefabName  = "enemy_ball"
				local prefabName = enemyAirPrefabName -- Flying by default.
				if stringContainsSubstring obj.name "melee" then
					prefabName = enemyMeleePrefabName
				if stringContainsSubstring obj.name "bola" then
					prefabName = enemyBolaPrefabName
				local prefabPath = base_path + "prefabs/" + prefabName + ".prefab"
				
				local attributeManager = TAttributeManager()
				local hp = attributeManager.getAttributeValue obj #enemyData #hitPoints
				if hp != undefined then format "would export hp: %\n" hp
				
				exportName obj
				fs.writeComma()
				local prefabPath = prefabPath
				fs.writeKeyValue "prefab" prefabPath
				fs.writeComma()
				local objPos = obj.position * (rotateXMatrix -90)
				local lookAtPos = [-1, objPos.y, -1]
				exportTransform obj lookAtCoord:lookAtPos
				
				-- If it's flying enemy.
				if prefabName == enemyAirPrefabName then (
					fs.writeComma()
					fs.writeKey "collider"
					fs.beginObj()
						fs.writeKeyValue "shape" "sphere"
						fs.writeComma()
						fs.writeKeyValue "radius" 0.3
						fs.writeComma()
						fs.writeKeyValue "group" "Enemy"
					fs.endObj()
				)
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj type:"patrolPath"
				)
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --LIGHT---------------------------------------------------
	fn exportLight obj nValidObjs = (
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

	-- --MOVABLE-------------------------------------------------
	-- TO DO: this object should be nested with associated trigger,
	-- right now we do two entities at once, manually.
	fn exportMovable obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				local prefabPath = base_path + "prefabs/platform_move.prefab"
				local triggerName = obj.name + "_trigger"
				exportName obj
				fs.writeComma()
				fs.writeKeyValue "prefab" prefabPath
				fs.writeComma()
				local objPos = obj.position * (rotateXMatrix -90)
				exportTransform obj
				fs.writeComma()
				-- collider
				fs.writeKey "collider"
				fs.beginObj()
					fs.writeKeyValue "shape" "box"
					fs.writeComma()
					fs.writeKeyValue "type" "dynamic"
					fs.writeComma()
					local platformHalfExtent = [abs obj.width / 2, abs obj.height / 2, abs obj.length / 2]
					fs.writeKeyValue "halfExtent" platformHalfExtent
					fs.writeComma()
					fs.writeKeyValue "group" "scenario"
				fs.endObj()
				-- child spline
				if hasChildLine obj then exportSpline obj
				-- entity trigger
				fs.endObj()-- end previous entity
				fs.endObj()-- end previous object
				------ child entity ------
				fs.writeComma()
				fs.beginObj()
				fs.writeKey "entity"
				fs.beginObj()-- begin trigger entity
				fs.writeKeyValue "name" triggerName
				fs.writeComma()
				fs.writeKeyEmpty "transform"
				-- collider
				fs.writeKey "collider"
				fs.beginObj()
					fs.writeKeyValue "shape" "box"
					fs.writeComma()
					fs.writeKeyValue "type" "dynamic"
					fs.writeComma()
					fs.writeKeyValue "is_trigger" true
					fs.writeComma()
					fs.writeKeyValue "halfExtent" platformHalfExtent
					fs.writeComma()
					exportStringArray "mask" #("player")
				fs.endObj()
				fs.writeComma()
				-- platform
				fs.writeKeyEmpty "platform"
				-- hierarchy
				fs.writeKey "hierarchy"
				fs.beginObj()
					fs.writeKeyValue "parent" obj.name
				fs.endObj()
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	-- --TRIGGERS -----------------------------------------------
	fn exportTrigger obj nValidObjs = (
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEntityCollider obj
				-- Add child spline if it exists.
				if hasChildLine obj then (
					exportSpline obj
				)
			fs.endObj() -- - - - - - - - - - - - - - - - - - - -
		fs.endObj()
		return true
	),

	fn exportAll = (
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
					-- Avoid child objects, they will be added through parent.
					if obj.parent == undefined then (
						local objectName = obj.name
						format "Layer: %\t\tClass: %\t\t\tObject name: %\n" layerName ((classof obj) as string) objectName --DEBUG
						case layerName of (
							"cameras":      ( wasValid = exportCamera      obj nValidObjs )
							"chrysalis":    ( wasValid = exportChrysalis   obj nValidObjs )
							"collisions":   ( wasValid = exportCollision   obj nValidObjs )
							"enemies":      ( wasValid = exportEnemy       obj nValidObjs )
							"lights":       ( wasValid = exportLight       obj nValidObjs )
							"movables":     ( wasValid = exportMovable     obj nValidObjs )
							"player":       ( wasValid = exportPlayer      obj nValidObjs )
							"prefabMelee":  ( wasValid = exportEnemyPrefab obj nValidObjs )
							"prefabFlying": ( wasValid = exportEnemyPrefab obj nValidObjs )
							"triggers":     ( wasValid = exportTrigger     obj nValidObjs )
							--"mech":( wasValid = exportMechanism   obj nValidObjs )
							"pure":		    ( wasValid = exportDefault     obj nValidObjs pure:true )
							"break":		( wasValid = exportDefault     obj nValidObjs breakable:true )
							"0":		    ( wasValid = exportDefault     obj nValidObjs )
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
	)
)

/*
TODO: ¿Sacar el fov de las luces de algún parámetro? Ahora siempre le pongo 40.
TODO: Ahora saca transform de hitboxes, no debería. ¿O debería usarse el transform en lugar del offset? (De todas formas se ignoran teniendo parent.)
TODO: Las splines de attack hitboxes tienen coordenadas de mundo.
TODO: Modificar editor prefab enemigo bola (ahora mismo es idéntico al melee).
TODO: Poder exportar cualquier mesh como collider.
TODO: Probar spline cerrada, habría que poner el último knot al principio seguramente.
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
Lo visible sólido debe tener una forma básica (Box, Sphere o Plane) como hija. Si no es hija
se exportará en la escena collisions.
Todo lo tangible debe estar como forma básica en una Layer llamada "collisions",
incluidas las que son hijas de Meshes visibles.
Los enemigos		     deben estar en una layer llamada "enemies".
Las crisálidas 		     deben estar en una layer llamada "chrysalis".
Las plataformas móviles  deben estar en una layer llamada "movables".
Las cámaras				 deben estar en una layer llamada "cameras".
Las luces				 deben estar en una layer llamada "lights".
Los triggers			 deben estar en una layer llamada "triggers".
Las meshes				 deben estar en una layer llamada "0".
Los colliders invisibles deben estar en una layer llamada "collisions".
 
Se sabe el tipo de cada enemigo por el nombre ("melee" o "ball"), por defecto son voladores. --> En el futuro igual cambia con attributes.
El proyecto debe tener las siguientes carpetas:
 bin/data/scenes/
 bin/data/meshes/
 bin/data/materials/
 maxscript/  --> Aquí tienen que estar estos scripts.
----------------------------------------------------------------------------------*/

-- Take the project path from this script path. Result should be ~"D:\Dev\MomentumEngine-Real\Bin\"
global exportPath = (substituteString (getThisScriptFilename()) "maxscript\\export_scene.ms" "") + "Bin\\"
exportPath = substituteString exportPath "\\" "/"
exporter = TSceneExporter project_path: exportPath

--mexporter = TMaterialExporter project_path:"pep/" base_path:"path/"
--mexporter.exportStdMaterial $.mat "pep" 

exporter.exportAll()