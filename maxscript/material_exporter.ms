struct TMaterialExporter (
	fs = TJsonFormatter(),
	project_path,           -- Assigned in the ctor. "D:\Dev\MomentumEngine-Real\Bin\"
	base_path,              -- Assigned in the ctor. "data/"
	texturesPath,			-- Assigned in the ctor. "data/textures/"

	fn getListBadDDS mat &badm &badmn = (
		if mat != undefined then (
			if classOf mat == StandardMaterial then (
				for map in mat.maps where map != undefined do (
					if isKindOf map normal_bump then (
						mapFilename = map.normal_map.filename
					) else mapFilename = map.filename

					mapFileType = getFilenameType mapFilename
					if mapFileType != ".DDS" and
					   mapFileType != ".dds" then (
					   	--Avoid duplicates.
					   	if (appendIfUnique badmn mapFilename) then (
					   		append badm ("-Mat: \"" + mat.name + "\" | Map: \"" + \
					   		map.name + "\" \n File: " + getFilenameFile mapFilename + "\n")
					   	)
					)
				)
			)
			else if classOf mat == MultiMaterial then (
				for m in mat.MaterialList where m != undefined do getListBadDDS m &badm &badmn
			)
		)
	),

	-- Check all objects in the file for non-DDS maps. This is very fast.
	fn listBadDDSofScene = (
		local badMaps = ""
		local badMapNames = #()
		for o in $* do getListBadDDS o.material &badMaps &badMapNames
		if badMaps == "" then messageBox ("All maps are DDS.\n") title:"All fine" beep:false
		else (
			messageBox ("Maps without DDS files:\n" + badMaps) title:"Error"
			format ("Maps without DDS files:\n" + badMaps) --Also print in listener.
		)
	),

	--Overwrites target file if it already exists.
	fn tryToCopyFile sourcePath targetPath = (
		if doesFileExist targetPath then deleteFile targetPath
		if (copyFile sourcePath targetPath) then --Try to copy file.
			format "Copying from % to %\n" sourcePath targetPath
		else format "WARNING: couldn't copy %, do it manually.\n" sourcePath
	),

	--Map = dds file.
	fn exportMap map alias default_value = (
		if map == undefined then (
			local json_filename = texturesPath + default_value + ".dds"
			fs.writeKeyValue alias json_filename
			return false
		)

		-- "C:\engine\bin\data\textures\bricks.dds" but also "C:\download\bricks.dds"
		local mapFilename = map.filename
		local mapFileType = getFilenameType mapFilename
		if mapFileType != ".DDS" and
		   mapFileType != ".dds" then throw ("Map " + mapFilename + " is NOT a DDS file.") 

		-- bricks
		local mapName = getFilenameFile mapFilename
		-- "C:\engine\bin\data\textures\"
		local mapMaxPath = getFilenamePath mapFilename
		-- "data/textures/bricks.dds"
		local json_filename = texturesPath + mapName + ".dds"

		-- "C:/engine/bin/data/textures/bricks.dds"
		local ofull_path = project_path + json_filename

		if isPathInsidePath mapMaxPath (project_path + base_path) then (
			--Map is already inside project directory, no need to copy,
			--just point to the right sub-folder.
			local strippedPath = stripPathFromPath mapMaxPath project_path
			json_filename = strippedPath + mapName + ".dds"
			if not doesFileExist (cleanPath (mapMaxPath + mapName + ".dds")) then
				format "WARNING: % not found.\n" (mapMaxPath + mapName + ".dds")
		) else ( --Map path is outside the project.
			if not doesFileExist ofull_path then ( --Doesn't exist inside project.
				tryToCopyFile mapFilename ofull_path
			) else ( --Already exists inside project.
				--Check if it's the same map, could be a new version.
				if not (areFilesIdentical mapFilename ofull_path) and
				   (getNewerFile mapFilename ofull_path == mapFilename) then (
						tryToCopyFile mapFilename ofull_path --Copy new one.
					) --Else the file in the project is more recent, do nothing.
			)
		)
		fs.writeKeyValue alias json_filename
		appendIfUnique resources json_filename
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
					exportMap mat.specularLevelMap "metallic" "null_metallic"
					fs.writeComma()
					exportMap mat.glossinessMap "roughness" "null_roughness"
					fs.writeComma()
					exportMap mat.selfIllumMap "self_illum" "null_self_illum"
					fs.writeComma()
					exportMap mat.displacementMap "height" "null_height"
					fs.writeComma()
					--refractionMap as detail_normal because we don't use it.
					exportMap mat.refractionMap "detail_normal" "null_detail_normal"

					--More: ambientMap, specularMap, opacityMap, filterMap, reflectionMap.
				)
			) else ( -- Empty material.
				fs.writeKeyValue "albedo" (texturesPath + "null_albedo.dds")
				fs.writeComma()
				fs.writeKeyValue "normal" (texturesPath + "null_normal.dds")
				fs.writeComma()
				fs.writeKeyValue "metallic" (texturesPath + "null_metallic.dds")
				fs.writeComma()
				fs.writeKeyValue "roughness" (texturesPath + "null_roughness.dds")
				fs.writeComma()
				fs.writeKeyValue "self_illum" (texturesPath + "null_self_illum.dds")
				fs.writeComma()
				fs.writeKeyValue "height" (texturesPath + "null_height.dds")
				fs.writeComma()
				fs.writeKeyValue "detail_normal" (texturesPath + "null_detail_normal.dds")
			)
			fs.endObj()
		fs.endObj()
	),

	-- Exports a single std material to a json format.
	fn exportStdMaterial mat mat_name pure:false = (
		if not pure then (
			--format "Exporting material % % %\n" mat_name mat (classOf mat as string)
			fs.begin (project_path + mat_name + ".material")
				exportMaterialContent mat:mat
				appendIfUnique resources (mat_name + ".material")
			fs.end()
		) else ( --Is pure.
			-- Generate normal material but with pureON.tech.
			fs.begin (project_path + mat_name + "_" + pureOnSuffix + ".material")
				exportMaterialContent mat:mat tech:pureOnSuffix
				appendIfUnique resources (mat_name + "_" + pureOnSuffix + ".material")
			fs.end()
		    -- Generate identical material but with pure.tech.
			fs.begin (project_path + mat_name + "_" + pureSuffix + ".material")
				exportMaterialContent mat:mat tech:pureSuffix
				appendIfUnique resources (mat_name + "_" + pureSuffix + ".material")
			fs.end()
		)
	),

	fn exportMix mats matPath mat: tech:"pbr_mix" = (
		fs.beginObj()
			fs.writeKeyValue "technique" (tech + ".tech")
			fs.writeComma()
			fs.writeKeyValue "type" "mix"
			fs.writeComma()
			fs.writeKeyValue "shadows" true
			fs.writeComma()
			fs.writeKeyEmpty "textures"
			fs.writeKey "mats"
			local matsNames = #()
			for m in mats do append matsNames (matPath + m.name + ".material")
			join resources matsNames
			fs.arrayOfStrings matsNames
			fs.writeComma()
			exportMap mat.mask "mix_blend_weights" "null_blend"
		fs.endObj()
	),

	-- Gets all materials connected to map1 and map2, including multimaterials.
	fn getRecursiveMats mat = (
		local mats = #()

		if mat.map1 != undefined then (
			if classOf mat.map1 != MultiMaterial then append mats mat.map1
			else for m in mat.map1 where m != undefined do append mats m
		)
		if mat.map2 != undefined then (
			if classOf mat.map2 != MultiMaterial then append mats mat.map2
			else for m in mat.map2 where m != undefined do append mats m
		)
		return mats
	),

	fn exportBlendMat mat matPath = (
		local mats = getRecursiveMats mat
		for m in mats do (
			fs.begin (project_path + matPath + m.name + ".material")
				exportMaterialContent mat:m
				appendIfUnique resources (matPath + m.name + ".material")
			fs.end()
		)

		fs.begin (project_path + matPath + mat.name + ".material")
			exportMix mats matPath mat:mat
			appendIfUnique resources (matPath + mat.name + ".material")
		fs.end()
	),

	-- Exports an empty solid material to a json format.
	fn exportEmptyMaterial mat_name pure:false = (
		local suffix = ""
		if pure then suffix = "_" + pureSuffix
		fs.begin (project_path + mat_name + suffix + ".material")
			if pure then exportMaterialContent tech:pureSuffix
			else exportMaterialContent()
		fs.end()
		if pure then (
			suffix = "_" + pureOnSuffix
			fs.begin (project_path + mat_name + suffix + ".material")
				exportMaterialContent tech:pureOnSuffix
			fs.end()
		)
		appendIfUnique resources (mat_name + suffix + ".material")
	),

	-- Will return an array of all the materials names used by obj and exported by us.
	fn exportMaterial mat mat_path obj = (
		local exported_materials = #()
		local matClass = classOf mat
		local pure = obj.layer.name == "pure"

		if mat == undefined then (
			setMaterialIDs obj --set Material IDs to 1.
			--throw ("Obj " + obj.name + " does NOT have a material")
			local mat_name = "empty"
			append exported_materials mat_name
			exportEmptyMaterial (mat_path + mat_name) pure:pure
		)

		if matClass == StandardMaterial then (
			setMaterialIDs obj --set Material IDs to 1.
			local mat_name = cleanName mat.name
			append exported_materials mat_name
			exportStdMaterial mat (mat_path + mat_name) pure:pure
		) else if matClass == MultiMaterial then (
			local multi_mat = mat
		--setMaterialIDs obj --set Material IDs to 1. TODO: REMOVE WHEN USING ACTUAL MULTIMATERIALS.
			local materials_of_mesh = getMaterialsUsedByMesh obj
			format "materials_of_mesh.count: %\n" materials_of_mesh.count
			for mat_idx = 1 to materials_of_mesh.count do (
				if materials_of_mesh[ mat_idx ] == undefined then continue
				local mat_of_mesh = multi_mat[ mat_idx ]

				if mat_of_mesh == undefined then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string) + " but the multimat does not have this submat.")
				if classOf mat_of_mesh != StandardMaterial then throw ("Mesh " + obj.name + " is using a multimaterial in slot " + (mat_idx as string) + " but the multimat in this slot is not a stdMaterial.")

				local mat_name = mat_of_mesh.name
				append exported_materials mat_name
				exportStdMaterial mat_of_mesh (mat_path + mat_name) pure:pure
			)
		) else if matClass == Blend then (
			if (mat.map1 != undefined or
			   mat.map2 != undefined) and
			   mat.mask != undefined then (
				local mat_name = cleanName mat.name
				append exported_materials mat_name
				exportBlendMat mat mat_path
			) else format "WARNING: Material % has incorrect inputs.\n" mat.name

		)
		return exported_materials
	)
)
