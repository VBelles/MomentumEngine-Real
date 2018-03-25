clearListener()
gc()

fileIn "export_mesh.ms" -- To avoid error on first execution.

-- 
struct TJsonFormatter (
	ofilename,			-- Name of output file
	fs, 				-- File stream
	depth_level = 0,	-- To format json tabs

	-- --------------------
	fn begin new_ofilename = (
		ofilename = new_ofilename
		fs = createFile ofilename
		if fs == undefined then (
			-- Avoid error if the file didn't close properly last time.
			flush fs
			fs = createFile ofilename
		)
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
				fs.writeKeyValue "technique" "solid.tech"
				fs.writeComma()
				fs.writeKeyValue "shadows" false
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

fn hasChildLine obj = (
	if obj.children == undefined then return false
	for child in obj.children do (
		if classOf child == line then return true
	)
	return false
)

fn isCollider obj = (
		if classof obj != Box 	 and
		   classof obj != Sphere and
		   classof obj != Plane then (
		format "Wrong object %. Collisions must be Spheres, Boxes or Planes.\n" obj.name
		return false
	)
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

		-- Add it in the user properties panel of max:  density = 10
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
		exportName obj
		fs.writeComma()
		exportTransform obj
		fs.writeComma()
		exportCompCollider obj
	),

	-- --SPLINE--------------------------------------------------
	fn exportSpline obj type: = (
		-- Executed only if we already know it has a child line.
		for child in obj.children do (
			if classOf child == line then (
				fs.writeComma()
				if type == unsupplied then fs.writeKey "move"
				else fs.writeKey type
				
				fs.beginObj()
					fs.writeKeyValue "type" "catmull-rom"
					fs.writeComma()
					fs.writeKeyValue "loop" false
					fs.writeComma()
					fs.writeKey "knots"
					fs.beginArray()
						knotsInSpline = numKnots child
						for i = 1 to (knotsInSpline) do (
							if i > 1 then fs.writeComma()
							-- getKnotPoint <shape> <spline_index_integer> <knot_index_integer>
							local knotPos = getKnotPoint child 1 i
							local mcvKnotPos = knotPos * rotateXMatrix -90
							fs.writeValue mcvKnotPos addIndent:true
						)
					fs.endArray()
				fs.endObj()
				return true -- Only exports the first line found.
			)
		)
	),
		
	-- ----------------------------------------------------------
	-- ----------------------------------------------------------

	-- --DEFAULT ------------------------------------------------
	fn exportDefault obj nValidObjs = (
		if classOf obj != Editable_mesh then (
			format "WARNING: Object % is NOT an Editable mesh, not exported.\n" obj.name
			return false
		)
		if nValidObjs > 0 then fs.writeComma()
		fs.beginObj()
			fs.writeKey "entity"
			fs.beginObj() -- - - - - - - - - - - - - - - - - - -
				exportEditableMesh obj
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
				exportEntityCollider obj
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
				local enemyAirPrefabName = "enemy_flying"
				local enemyLandPrefabName = "enemy_melee"
				local prefabName = enemyAirPrefabName -- Flying by default.
				if matchpattern obj.name pattern:"?*melee*" then
					prefabName = enemyLandPrefabName
				local prefabPath = base_path + "prefabs/" + prefabName + ".prefab"
				
				exportName obj
				fs.writeComma()
				local prefabPath = prefabPath
				fs.writeKeyValue "prefab" prefabPath
				fs.writeComma()
				local objPos = obj.position * (rotateXMatrix -90)
				local lookAtPos = [-1, objPos.y, -1]
				exportTransform obj lookAtCoord:lookAtPos
				
				-- If it's flying enemy.
				if prefabName != enemyLandPrefabName then (
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
	fn exportMovablePlatform obj = (
		local prefabPath = base_path + "prefabs/platform_move.prefab"
		
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
		exportTransform obj yShift:yShift
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
			
			-- Avoid creating scenes if layer is empty or invisible.
			if layerObjs.count >= 1 and not layer.isHidden then (
				format "Exporting to %\n" scenePath
				format "Please wait...\n"
				local nValidObjs = 0
				local wasValid = false
				-- Create scene file.
				fs.begin scenePath
				fs.beginArray()
				for object in layerObjs do (
					-- Avoid child objects, they will be added through parent.
					if object.parent == undefined then (
						local objectName = object.name
						format "Layer: %\t\tClass: %\t\t\tObject name: %\n" layerName ((classof object) as string) objectName --DEBUG
						case layerName of (
							"cameras":   ( wasValid = exportCamera    object nValidObjs )
							"chrysalis": ( wasValid = exportChrysalis object nValidObjs )
							"collisions":( wasValid = exportCollision object nValidObjs )
							"enemies":   ( wasValid = exportEnemy     object nValidObjs )
							"lights":    ( wasValid = exportLight     object nValidObjs )
							"movables":  ( wasValid = exportMovable   object nValidObjs )
							"triggers":  ( wasValid = exportTrigger   object nValidObjs )
							--"mechanisms":( wasValid = exportMechanism object nValidObjs )
							"0":		 ( wasValid = exportDefault   object nValidObjs )
							default:     ( format "WARNING: Unknown layer %, not exported.\n" layerName )
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
TODO: Decidir qué hacer con las plataformas móviles.
TODO: Añadir parámetros -attributes- a los objetos. Ej: HP para enemigos.
http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_ADFD29E4_9751_4F55_98CC_F7C721C9AEE2_htm
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
Lo visible sólido debe tener una forma básica (Box, Sphere o Plane) como hija.
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
 
Los enemigos que no tengan "melee" en el nombre son considerados voladores.
Aquí abajo en exporter se debe poner la ruta correcta al proyecto.
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

exporter.exportAll()
