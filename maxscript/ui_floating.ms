fileIn "utils.ms"
fileIn "export_scene.ms"
fileIn "export_mesh.ms"
fileIn "export_rigid_anims.ms"
fileIn "export_skeletons.ms"

macroscript Momentum
category: "Momentum"
internalCategory: "Momentum"
tooltip: "Open the Momentum utility window"
ButtonText: "Momentum"
(
	global floater
	try closeRolloutFloater floater catch()

	local width  = 218

	local baseHeight = 66
	local heightPerButton = 26
	local numberOfButtons = 1
	local height = baseHeight + heightPerButton * numberOfButtons

	local left = sysInfo.desktopSize[1] * 0.8 --80% of screen width.
	local top =	 sysInfo.desktopSize[2] * 0.1 --10% of screen height.

	floater = newRolloutFloater "Momentum utils" width height left top
	exportPath = cleanPath (substituteString (getThisScriptFilename()) "maxscript\\ui_floating.ms" "Bin\\")

	rollout exporter "Momentum Exporter" (
		button btn_scene "Export Scene"
		button btn_mesh "Export Mesh" tooltip:"Exports selected meshes to files"
		button btn_col "Export Collider" tooltip:"Exports current selection colliders to file and generates a scene"
		button btn_skeleton "Export Skeleton & Meshes"
		button btn_skel_anims "Export Animation" tooltip:"Uses max animation range and frame rate to specify the range to export (name matches max file)"
		button btn_rigid_anims "Export Rigid Animation"

		on btn_scene pressed do (
			try (
				gc()
				clearListener()
				local start = timeStamp()
				local exporter = TSceneExporter project_path:exportPath
				exporter.exportAll()
				local end = timeStamp()
				MessageBox ("All exported OK. Took " + ((end - start) / 1000.0) as string + " seconds.") title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting:\n" + getCurrentException()) title:"Error"
			)
		)

		-- Exports selected meshes to files.
		on btn_mesh pressed do (
			try (
				gc()
				clearListener()
				for o in selection do (
					ofilename = exportPath + "data/meshes/" + o.name + ".mesh"
					exportMeshObject o ofilename
				)
				MessageBox "Meshes exported OK" title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting Meshes:\n" + getCurrentException()) title:"Error"
			)
		)

		-- Exports current selection colliders to file and generates a scene.
		on btn_col pressed do (
			try (
				gc()
				clearListener()
				local exporter = TSceneExporter project_path:exportPath
				exporter.exportSelectedColliders()
				MessageBox "Selected colliders exported OK" title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting colliders:\n" + getCurrentException()) title:"Error"
			)
		)

		-- Exports the current selection mesh to file.
		on btn_skeleton pressed do (
			try (
				gc()
				clearListener()
				local skelName = getFilenameFile maxFileName
				local se = TSkeletonsExporter project_path:exportPath skel_name:skelName
				se.exportSkelAndMeshes()
				MessageBox "Skeleton And Meshes exported OK" title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting Skeleton:\n" + getCurrentException()) title:"Error"
			)
		)

		-- 
		on btn_skel_anims pressed do (
			try (
				gc()
				clearListener()
				local skelName = getFilenameFile maxFileName
				local se = TSkeletonsExporter project_path:exportPath skel_name:skelName
				se.exportAnim()
				MessageBox ("Skeleton Animation exported OK with name " + skelName) title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting Skeleton Animation:\n" + getCurrentException()) title:"Error"
			)
		)

		--  
		on btn_rigid_anims pressed do (
			try (
				gc()
				clearListener()
				local rae = TRigidAnimExporter project_path:exportPath
				rae.exportAll ($* as array)
				MessageBox "Rigid Animation exported OK" title:"All fine" beep:false
			) catch (
				MessageBox ("Error Exporting Rigid Animation:\n" + getCurrentException()) title:"Error"
			)
		)

		-- Resize window if rollout collapses.
		on exporter rolledUp state do (
			local exporterButtons = 6
			if state then height += heightPerButton * exporterButtons
			else height -= heightPerButton * exporterButtons

			floater.size = [width, height]
		)
	) -- end exporter rollout creator

	rollout utils "Momentum Utils" (
		button btn_attach_elems "Attach sel" across:2 tooltip:"Attaches all selected objects"
		button btn_detach_elems "Detach elements" tooltip:"Detaches all elements of selected object"
		button btn_check_dds "Check DDS" tooltip:"Checks if any texture is not DDS"
		button btn_create_col "Create colliders" tooltip:"Creates child colliders of selected objects"
		button btn_sel_children "Select children" across:2 tooltip:"Selects children of selected objects"
		button btn_sel_parents "Select parents" tooltip:"Selects parents of selected objects"

		fn attachElems = (
			try (
				gc()
				local utilsManager = TUtils()
				utilsManager.attachSel()	
			) catch (
				MessageBox ("Error attaching:\n" + getCurrentException()) title:"Error"
			)
		)

		fn detachElems = (
			try (
				gc()
				local utilsManager = TUtils()
				utilsManager.detachElementsFromSel()	
			) catch (
				MessageBox ("Error detaching:\n" + getCurrentException()) title:"Error"
			)
		)

		fn checkDDS = (
			try (
				gc()
				clearListener()
				local me = TMaterialExporter()
				me.listBadDDSofScene()	
			) catch (
				MessageBox ("Error checking dds:\n" + getCurrentException()) title:"Error"
			)
		)

		fn createColliders = (
			try (
				gc()
				clearListener()
				local utilsManager = TUtils()
				utilsManager.createChildColliders()
			) catch (
				MessageBox ("Error creating colliders:\n" + getCurrentException()) title:"Error"
			)
		)

		fn selectChildren = (
			try (
				gc()
				clearListener()
				local utilsManager = TUtils()
				utilsManager.selectChildren()
			) catch (
				MessageBox ("Error selecting children:\n" + getCurrentException()) title:"Error"
			)
		)

		fn selectParents = (
			try (
				gc()
				clearListener()
				local utilsManager = TUtils()
				utilsManager.selectParents()
			) catch (
				MessageBox ("Error selecting parents:\n" + getCurrentException()) title:"Error"
			)
		)
		
		on btn_attach_elems pressed do ( attachElems() )
		on btn_detach_elems pressed do ( detachElems() )
		on btn_check_dds 	pressed do ( checkDDS() )
		on btn_create_col 	pressed do ( createColliders() )
		on btn_sel_children pressed do ( selectChildren() )
		on btn_sel_parents  pressed do ( selectParents() )

		-- Resize window if rollout collapses.
		on utils rolledUp state do (
			local utilsButtons = 4
			if state then height += heightPerButton * utilsButtons
			else height -= heightPerButton * utilsButtons

			floater.size = [width, height]
		)
	) -- end utils rollout

	rollout attribute "Momentum Attributes" (
		button btn_addAttrEnemy "+ enemy" across:2 tooltip:"Adds enemy attributes to selected objects"
		button btn_delAttrEnemy "- enemy" tooltip:"Deletes enemy attributes from selected objects"
		
		button btn_addRotation "+ rotation" across:2 tooltip:"Adds rotation attributes to selected objects"
		button btn_delRotation "- rotation" tooltip:"Deletes rotation attributes from selected objects"
		
		button btn_addMovement "+ move" across:2 tooltip:"Adds movement attributes to selected objects"
		button btn_delMovement "- move" tooltip:"Deletes movement attributes from selected objects"
		
		button btn_addMechTrig "+ mech trig" across:2 tooltip:"Adds mech trigger attributes to selected objects"
		button btn_delMechTrig "- mech trig" tooltip:"Deletes mech trigger attributes from selected objects"

		button btn_addMechanism "+ mech" across:2 tooltip:"Adds mechanism attributes to selected objects"
		button btn_delMechanism "- mech" tooltip:"Deletes mechanism attributes from selected objects"

		button btn_addUnique "+ unique" across:2 tooltip:"Adds unique attributes to selected objects"
		button btn_delUnique "- unique" tooltip:"Deletes unique attributes from selected objects"

		button btn_addAnim "+ anim" across:2 tooltip:"Adds anim attributes to selected objects"
		button btn_delAnim "- anim" tooltip:"Deletes anim attributes from selected objects"

		button btn_addLight "+ light" across:2 tooltip:"Adds light attributes to selected objects"
		button btn_delLight "- light" tooltip:"Deletes light attributes from selected objects"

		button btn_addColl "+ collider" across:2 tooltip:"Adds collider attributes to selected objects"
		button btn_delColl "- collider" tooltip:"Deletes collider attributes from selected objects"

		button btn_set_trimesh "Set trimesh" across:2 tooltip:"Sets trimesh attribute of selected objects"
		button btn_unset_trimesh "Unset trimesh" tooltip:"Unsets trimesh attribute of selected objects"

		fn addAttribute defName = (
			try (
				gc()
				local attributeManager = TAttributeManager()
				attributeManager.addAttributeToSel(defName)	
			) catch (
				MessageBox ("Error adding attribute:\n" + getCurrentException()) title:"Error"
			)
		)
		
		fn delAttribute defName = (
			try (
				gc()
				local attributeManager = TAttributeManager()
				attributeManager.delAttributeFromSel(defName)	
			) catch (
				MessageBox ("Error deleting attribute:\n" + getCurrentException()) title:"Error"
			)
		)

		fn setAttribute defName attribute value = (
			try (
				gc()
				local attributeManager = TAttributeManager()
				attributeManager.setAttributeInSel defName #collType value
			) catch (
				MessageBox ("Error setting attribute:\n" + getCurrentException()) title:"Error"
			)
		)
		
		on btn_set_trimesh pressed   do ( setAttribute "collData" #collType true )
		on btn_unset_trimesh pressed do ( setAttribute "collData" #collType false )

		on btn_addAttrEnemy pressed do ( addAttribute "enemyData" )
		on btn_delAttrEnemy pressed do ( delAttribute "enemyData" )
		
		on btn_addRotation pressed do ( addAttribute "rotateData" )
		on btn_delRotation pressed do ( delAttribute "rotateData" )
		
		on btn_addMovement pressed do ( addAttribute "moveData" )
		on btn_delMovement pressed do ( delAttribute "moveData" )

		on btn_addMechTrig pressed do ( addAttribute "mechTrigData" )
		on btn_delMechTrig pressed do ( delAttribute "mechTrigData" )

		on btn_addMechanism pressed do ( addAttribute "mechData" )
		on btn_delMechanism pressed do ( delAttribute "mechData" )

		on btn_addUnique pressed do ( addAttribute "uniqueData" )
		on btn_delUnique pressed do ( delAttribute "uniqueData" )

		on btn_addAnim pressed do ( addAttribute "animData" )
		on btn_delAnim pressed do ( delAttribute "animData" )

		on btn_addLight pressed do ( addAttribute "lightData" )
		on btn_delLight pressed do ( delAttribute "lightData" )

		on btn_addColl pressed do ( addAttribute "collData" )
		on btn_delColl pressed do ( delAttribute "collData" )

		-- Resize window if rollout collapses.
		on attribute rolledUp state do (
			local attrButtons = 10
			if state then height += heightPerButton * attrButtons
			else height -= heightPerButton * attrButtons

			floater.size = [width, height]
		)
	) -- end attribute rollout creator

	addRollout exporter floater
	addRollout utils floater
	addRollout attribute floater
) --End script.
