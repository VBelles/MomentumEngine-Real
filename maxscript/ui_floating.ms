fileIn "utils.ms"
fileIn "export_scene.ms"
fileIn "export_mesh.ms"
fileIn "export_rigid_anims.ms"
fileIn "export_skeletons.ms"

macroscript Momentum
category: "Momentum"
tooltip: "Open the Momentum utility window"
(
try closeRolloutFloater floater catch()

floater = newRolloutFloater "Momentum utils" 218 430 --Width and height.
exportPath = cleanPath (substituteString (getThisScriptFilename()) "maxscript\\ui_floating.ms" "Bin\\")

rollout exporter "Momentum Exporter" (
	button btn_scene "Export Scene"
	button btn_mesh "Export Mesh" tooltip:"Exports the current selection mesh to file"
	button btn_col "Export Collider" tooltip:"Exports current selection colliders to file and generates a scene"
	button btn_skeleton "Export Skeleton & Meshes"
	button btn_skel_anims "Export Animation" tooltip:"Uses max animation range and frame rate to specify the range to export (name matches max file)"
	button btn_rigid_anims "Export Rigid Animation"

	on btn_scene pressed do (
		try (
			gc()
			local start = timeStamp()
			local exporter = TSceneExporter project_path:exportPath
			exporter.exportAll()
			local end = timeStamp()
			MessageBox ("All exported OK. Took " + ((end - start) / 1000.0) as string + " seconds.")
		) catch (
			MessageBox ("Error Exporting:\n" + getCurrentException())
		)
	)

	-- Exports the current selection mesh to file.
	on btn_mesh pressed do (
		try (
			gc()
			local ofilename = exportPath + "data/meshes/" + $.name + ".mesh"
			exportMeshObject $ ofilename
			MessageBox "Single mesh exported OK"
		) catch (
			MessageBox ("Error Exporting Single Mesh:\n" + getCurrentException())
		)
	)

	-- Exports current selection colliders to file and generates a scene.
	on btn_col pressed do (
		try (
			gc()
			local exporter = TSceneExporter project_path:exportPath
			exporter.exportSelectedColliders()
			MessageBox "Selected colliders exported OK"
		) catch (
			MessageBox ("Error Exporting colliders:\n" + getCurrentException())
		)
	)

	-- Exports the current selection mesh to file.
	on btn_skeleton pressed do (
		try (
			gc()
			local skelName = getFilenameFile maxFileName
			local se = TSkeletonsExporter project_path:exportPath skel_name:skelName
			se.exportSkelAndMeshes()
			MessageBox "Skeleton And Meshes exported OK"
		) catch (
			MessageBox ("Error Exporting Skeleton:\n" + getCurrentException())
		)
	)

	-- 
	on btn_skel_anims pressed do (
		try (
			gc()
			local skelName = getFilenameFile maxFileName
			local se = TSkeletonsExporter project_path:exportPath skel_name:skelName
			se.exportAnim()
			MessageBox ("Skeleton Animation exported OK with name " + skelName)
		) catch (
			MessageBox ("Error Exporting Skeleton Animation:\n" + getCurrentException())
		)
	)

	--  
	on btn_rigid_anims pressed do (
		try (
			gc()
			local rae = TRigidAnimExporter project_path:exportPath
			rae.exportAll ($* as array)
			MessageBox "Rigid Animation exported OK"
		) catch (
			MessageBox ("Error Exporting Rigid Animation:\n" + getCurrentException())
		)
	)
) -- end exporter rollout creator

rollout utils "Momentum Utils" (
	button btn_attach_elems "Attach elements" tooltip:"Attaches all selected objects."
	button btn_detach_elems "Detach elements" tooltip:"Detaches all elements of selected object."

	fn attachElems = (
		try (
			gc()
			local utilsManager = TUtils()
			utilsManager.attachSel()	
		) catch (
			MessageBox ("Error attaching:\n" + getCurrentException())
		)
	)

	fn detachElems = (
		try (
			gc()
			local utilsManager = TUtils()
			utilsManager.detachElementsFromSel()	
		) catch (
			MessageBox ("Error detaching:\n" + getCurrentException())
		)
	)
	
	on btn_attach_elems pressed do ( attachElems() )
	on btn_detach_elems pressed do ( detachElems() )
) -- end utils rollout

rollout attribute "Momentum Attributes" (
	button btn_addAttrEnemy "+ enemy" across:2 tooltip:"Adds enemy attributes to selected objects"
	button btn_delAttrEnemy "- enemy" tooltip:"Deletes enemy attributes from selected objects"
	
	button btn_addRotation "+ rotation" across:2 tooltip:"Adds rotation attributes to selected objects"
	button btn_delRotation "- rotation" tooltip:"Deletes rotation attributes from selected objects"
	
	button btn_addMovement "+ move" across:2 tooltip:"Adds movement attributes to selected objects"
	button btn_delMovement "- move" tooltip:"Deletes movement attributes from selected objects"
	
	button btn_addMechanism "+ mech" across:2 tooltip:"Adds mechanism attributes to selected objects"
	button btn_delMechanism "- mech" tooltip:"Deletes mechanism attributes from selected objects"

	button btn_addUnique "+ unique" across:2 tooltip:"Adds unique attributes to selected objects"
	button btn_delUnique "- unique" tooltip:"Deletes unique attributes from selected objects"

	fn addAttribute defName = (
		try (
			gc()
			local attributeManager = TAttributeManager()
			attributeManager.addAttributeToSel(defName)	
			--MessageBox "Attributes added."
		) catch (
			MessageBox ("Error adding attribute:\n" + getCurrentException())
		)
	)
	
	fn delAttribute defName = (
		try (
			gc()
			local attributeManager = TAttributeManager()
			attributeManager.delAttributeFromSel(defName)	
			--MessageBox "Attributes deleted."
		) catch (
			MessageBox ("Error deleting attribute:\n" + getCurrentException())
		)
	)
	
	on btn_addAttrEnemy pressed do ( addAttribute "enemyData" )
	on btn_delAttrEnemy pressed do ( delAttribute "enemyData" )
	
	on btn_addRotation pressed do ( addAttribute "rotateData" )
	on btn_delRotation pressed do ( delAttribute "rotateData" )
	
	on btn_addMovement pressed do ( addAttribute "moveData" )
	on btn_delMovement pressed do ( delAttribute "moveData" )

	on btn_addMechanism pressed do ( addAttribute "mechData" )
	on btn_delMechanism pressed do ( delAttribute "mechData" )

	on btn_addUnique pressed do ( addAttribute "uniqueData" )
	on btn_delUnique pressed do ( delAttribute "uniqueData" )
) -- end attribute rollout creator

addRollout exporter floater
addRollout utils floater
addRollout attribute floater
) --End script.