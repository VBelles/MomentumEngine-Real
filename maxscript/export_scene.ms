clearListener()
gc()

-- 
struct TJsonFormatter (
	ofilename,			-- Name of output file
	fs, 				-- File stream
	depth_level = 0,	-- To format json tabs

	-- --------------------
	fn begin new_ofilename = (
		ofilename = new_ofilename
		fs = createFile ofilename
	),

	-- --------------------
	fn end = (
		if fs != undefined then close fs
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
		-- --------------------
	fn writeValue val = (
		if classof val == string then (
			format " \"%\"" val to:fs
		) else if classof val == Point3 then (
			format " \"% % %\"" val.x val.y val.z to:fs
		) else if classof val == quat then (
			-- Quats rotate in the opposite direction
			format " \"% % % %\"" val.x val.y val.z -val.w to:fs
		) else (
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
	
	fn exportMap map alias = (
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
				exportMap mat.diffusemap "albedo"
				-- ... other maps..
			)

			fs.endObj()
		fs.endObj()
		fs.end()
	),
	-- Exports an empty solid material to a json format
	fn exportEmptyMaterial mat_name = (
		fs.begin (project_path + mat_name )
		fs.beginObj()
			fs.writeKeyValue "technique" "solid.tech"
			fs.writeComma()
			fs.writeKeyValue "shadows" false
		fs.endObj()
		fs.end()
	),

	-- Will return an array of all the materials names used by obj and exported by us
	fn exportMaterial mat mat_path obj = (
		local exported_materials = #()

		if mat == undefined then (
			--throw ("Obj " + obj.name + " does NOT have a material")
			local mat_name = mat_path + "empty.material"
			append exported_materials mat_name
			exportEmptyMaterial mat_name
		)

		if classof mat == StandardMaterial then (
			local mat_name = mat_path + (cleanMaterialName mat.name) + ".material"
			append exported_materials mat_name
			exportStdMaterial mat mat_name

		) else if classof mat == MultiMaterial then (
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

fn collisionCheck obj = (
	if classof obj != Box and classof obj != Sphere and classof obj != Plane then (
		throw "Collisions must be Spheres, Boxes or Planes."
	)
)

fn isCollider obj = (
	if obj.layer.name != "collisions" and obj.layer.name != "triggers" then return false
	collisionCheck obj
	-- if name starts with col_...
	return true
)

fn isEnemy obj = (
	if obj.layer.name != "enemyAir" and obj.layer.name != "enemyLand" then return false
	collisionCheck obj
	return true
)

fn isChrysalis obj = (
	if obj.layer.name != "chrysalis" then return false
	collisionCheck obj
	return true
)

fn isMovablePlatform obj = (
	if obj.layer.name != "movables" then return false
	collisionCheck obj
	return true
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
	
	fn exportName obj = (
		fs.writeKeyValue "name" obj.name
	),

	fn isValidName aname = (
		return findString aname " " == undefined
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
			--throw ("Obj " + obj.name + " does NOT have a material")
			format "Warning: Obj doesn't have a material, a default will be assigned.\n"
			-- TODO: assign default material.
		)
		else if not (isValidName mat.name) then (
			--throw ("Obj " + obj.name + " has a material with an invalid name " + mat.name )
			mat.name = cleanMaterialName mat.name
			format "Warning: material had invalid name, changed spaces to underscores.\n"
		)

		fs.beginObj()
			-- Export Mesh
			local mesh_name = mesh_path + obj.name + ".mesh"
			fs.writeKeyValue "mesh" mesh_name
			exportMeshObject obj (project_path + mesh_name )

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
	fn exportCompCollider obj = (
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
		)

		-- Add it in the user properties panel of max:     density = 10
		local density = getUserProp obj "density"
		if density != undefined then (
			fs.writeComma()
			fs.writeKeyValue "density" density
		)

		if obj.layer.name == "triggers" then (
			fs.writeComma()
			fs.writeKeyValue "trigger" true
		)

		fs.endObj()
	),

	-- --EDITABLE MESH-------------------------------------------
	fn exportEditableMesh obj = (
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
				exportCompCollider child
			)
		)
	),

	-- --ENTITY COLLIDER-----------------------------------------
	fn exportEntityCollider obj = (
		-- It will be exported as part of the parent.
		-- Ejemplo del cofre y su caja de colision.
		if obj.parent != undefined then (
			--format "NOT Exporting entity collider obj % of class %\n" obj.name ((classof obj) as string)	
			return false
		)
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		exportCompCollider obj
	),

	-- --CAMERA--------------------------------------------------
	fn exportCamera obj = (
		fs.writeKeyValue "name" obj.name
		if (classof obj == FreeCamera or
			classof obj == TargetCamera) then (
				fs.writeComma()
				exportTransform obj 0 obj.Target
				format "Exporting camera\n"
				fs.writeKey "camera"
				fs.beginObj()
					fs.writeKeyValue "fov" obj.fov
				fs.endObj()
		)
	--format "Camera % target distance: %\n" obj.name obj.targetDistance
	),
	
	-- --SPLINE--------------------------------------------------
	fn exportSpline obj = (
		fs.writeKeyValue "type" "catmull-rom"
		fs.writeComma()
		fs.writeKeyValue "loop" false
		fs.writeComma()
		fs.writeKey "knots"
		fs.beginArray()
			knotsInSpline = numKnots obj
			for i = 1 to (knotsInSpline) do (
				if i > 1 then fs.writeComma()
				-- getKnotPoint <shape> <spline_index_integer> <knot_index_integer>
				local knotPos = getKnotPoint obj 1 i
				local mcvKnotPos = knotPos * rotateXMatrix -90
				fs.writeValue mcvKnotPos
			)
		fs.endArray()
	),
	
	-- --ENEMY---------------------------------------------------
	fn exportEnemy obj = (
		local enemyAirPrefabName = "enemy_flying"
		local enemyLandPrefabName = "enemy_melee"
		local prefabName = enemyAirPrefabName
		if obj.layer.name == "enemyLand" then prefabName = enemyLandPrefabName
		local prefabPath = base_path + "prefabs/" + prefabName + ".prefab"
		
		fs.writeKeyValue "name" obj.name
		fs.writeComma()
		local prefabPath = prefabPath
		fs.writeKeyValue "prefab" prefabPath
		fs.writeComma()
		
		local lookAtPos = [-1, objPos.y, -1]
		exportTransform obj 0 lookAtPos
		
		if obj.layer.name == "enemyAir" then (
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
	),
	
	-- --CHRYSALIS-----------------------------------------------
	fn exportChrysalis obj = (
		local prefabPath = base_path + "prefabs/crisalida.prefab"
		fs.writeKeyValue "name" obj.name
		fs.writeComma()
		fs.writeKeyValue "prefab" prefabPath
		fs.writeComma()
		fs.writeKey "transform"
		fs.beginObj()
			local objPos = obj.position * (rotateXMatrix -90)
			fs.writeKeyValue "pos" objPos
		fs.endObj()
	),

	-- --MOVABLE-------------------------------------------------
	-- TO DO: this object should be nested with associated trigger,
	-- right now we do two entities at once, manually.
	fn exportMovablePlatform obj = (
		local prefabPath = base_path + "prefabs/platform_move.prefab"
		
		fs.writeKeyValue "name" obj.name
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
			fs.writeKeyValue "is_dynamic" true
			fs.writeComma()
			local platformHalfExtent = [abs obj.width / 2, abs obj.height / 2, abs obj.length / 2]
			fs.writeKeyValue "halfExtent" platformHalfExtent
			fs.writeComma()
			fs.writeKeyValue "group" "scenario"
		fs.endObj()
		fs.writeComma()
		-- platform_move
		fs.writeKey "platform_move"
		fs.beginObj()
			fs.writeKeyValue "type" "up"
			fs.writeComma()
			fs.writeKeyValue "speed" 4.0
			fs.writeComma()
			fs.writeKey "waypoints"
			fs.beginArray()
				fs.writeValue objPos -- First point is position
				fs.writeComma()
				local nextPos = objPos + [0, 10, 0]
				fs.writeValue nextPos -- TODO: Pick from spline?
			fs.endArray()
		fs.endObj()
		fs.writeComma()
		-- hierarchy
		fs.writeKey "hierarchy"
		fs.beginObj()
			local parentName = obj.name + "_trigger"
			fs.writeKeyValue "parent" parentName
		fs.endObj()
		-- entity trigger
		fs.endObj()-- end previous entity
		fs.endObj()
		fs.writeComma()
		fs.beginObj()
		fs.writeKey "entity"
		fs.beginObj()-- begin trigger entity
		fs.writeKeyValue "name" parentName
		fs.writeComma()
		local yShift = platformHalfExtent[2]
		exportTransform obj yShift
		fs.writeComma()
		-- collider
		fs.writeKey "collider"
		fs.beginObj()
			fs.writeKeyValue "shape" "box"
			fs.writeComma()
			fs.writeKeyValue "is_dynamic" true
			fs.writeComma()
			fs.writeKeyValue "is_trigger" true
			fs.writeComma()
			fs.writeKeyValue "halfExtent" platformHalfExtent -- Adjustment required?
			fs.writeComma()
			fs.writeKeyValue "group" "scenario"
			fs.writeComma()
			fs.writeKeyValue "mask" "player"
		fs.endObj()
		fs.writeComma()
		-- fwd_msg
		fs.writeKey "fwd_msg"
		fs.beginObj()
			fs.writeKeyValue "target" obj.name
		fs.endObj()
	),
	
	-- --ENTITY--------------------------------------------------
	fn exportEntity obj nValidObjs = (
		-- Avoid empty entities if it's a child collider.
		if (obj.parent == undefined and not isCollider obj) then (
			if nValidObjs != 0 then fs.writeComma()
			fs.beginObj()
			fs.writeKey "entity"
				--format "Obj created, name: %\t\tClass: %\t\tLayer: %\n" obj.name ((classof obj) as string) obj.layer.name
				fs.beginObj()
				if (classof obj == FreeCamera or
					classof obj == TargetCamera) then exportCamera obj
				if classof obj == Editable_Mesh then exportEditableMesh obj
				if classof obj == Line then exportSpline obj
				else if isCollider obj then exportEntityCollider obj
				else if isEnemy obj then exportEnemy obj
				else if isChrysalis obj then exportChrysalis obj
				else if isMovablePlatform obj then exportMovablePlatform obj
				else (
					format "Warning: Don't know how to export obj % of class %. Empty entity generated.\n" obj.name ((classof obj) as string) 
				)
				fs.endObj()
			fs.endObj()
			return true
		) else return false -- It was not a valid entity.
	),

	fn exportAll = (
		-- Output filename based on .max filename
		local ofilename = getFilenameFile maxFileName
		local full_path = project_path + scenes_path + ofilename + ".scene"
		format "Exporting to %\n" full_path
		format "Please wait...\n"
		fs.begin full_path
		fs.beginArray()
			local nValidObjs = 0
			for obj in objects do (
				--format "Obj Name: %\t\tClass: %\t\tLayer: %\n" obj.name ((classof obj) as string) obj.layer.name
				if exportEntity obj nValidObjs then (
					nValidObjs = nValidObjs + 1
				)
			)
		fs.endArray()
		fs.end()
		format "Export finished.\n"
	)
)

-- TODO: ¿Interpretar automáticamente colliders sin parent como triggers invisibles?
--       Ahora mismo no los exporta.
-- TODO: Poder exportar triggers sin parent.
-- TODO: Exportar enemigos, crisálidas y plataformas móviles en escena separada.

------------------------------------------------------------------------------------
-- Notas de uso:
------------------------------------------------------------------------------------
-- Lo visible como Editable Mesh.
-- Lo visible sólido debe tener una forma básica (Box, Sphere o Plane) como hija.
-- Todo lo tangible debe estar como forma básica en una Layer llamada "collisions",
--  incluidas las que son hijas de Meshes visibles.
-- Los enemigos de aire    deben estar en una layer llamada "enemyAir".
-- Los enemigos terrestres deben estar en una layer llamada "enemyLand".
-- Las crisálidas 		   deben estar en una layer llamada "chrysalis".
-- Las plataformas móviles deben estar en una layer llamada "movables".
-- Los triggers			   deben estar en una layer llamada "triggers", pero ahora
--  mismo no se exportan.
-- Aquí abajo en exporter se debe poner la ruta correcta al proyecto.
-- El proyecto debe tener las siguientes carpetas:
--  bin/data/scenes
--  bin/data/meshes
--  bin/data/materials
------------------------------------------------------------------------------------

--exporter = TSceneExporter project_path: "C:/Code/MomentumEngine-Real/Bin/"
exporter = TSceneExporter project_path: "C:/CPPprojects/MomentumEngine-Real/Bin/"
--exporter = TSceneExporter project_path: "D:/MomentumEngine-Real/Bin/"
--exporter = TSceneExporter project_path: "D:/Dev/MomentumEngine-Real/Bin/"
exporter.exportAll()
