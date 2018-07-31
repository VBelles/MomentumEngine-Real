--fileIn "export_scene.ms"
--fileIn "export_skeletons.ms"
--fileIn "export_rigid_anims.ms"

utility MyMCVExporter "MCV Exporter" (
	rollout exporter "Momentum Exporter" (
		button btn_scene "Export Scene"
		button btn_mesh "Export Mesh" tooltip:"Exports the current selection mesh to file"
		button btn_skeleton "Export Skeleton & Meshes"
		button btn_skel_anims "Export Animation" tooltip:"Uses max animation range and frame rate to specify the range to export"
		button btn_rigid_anims "Export Rigid Animation"

		on btn_scene pressed do (
			try (
				gc()
				global exportPath = cleanPath (substituteString (getThisScriptFilename()) "maxscript\\export_ui.ms" "Bin\\")
				local exporter = TSceneExporter project_path:exportPath
				exporter.exportAll()	
				MessageBox "All exported OK"
			) catch (
				MessageBox ("Error Exporting:\n" + getCurrentException())
			)
		)

		-- Exports the current selection mesh to file
		on btn_mesh pressed do (
			try (
				gc()
				local ofilename = undefined  -- to be defined by you.
				exportMeshObject $ ofilename
				MessageBox "Single mesh exported OK"
			) catch (
				MessageBox ("Error Exporting Single Mesh:\n" + getCurrentException())
			)
		)

		-- Exports the current selection mesh to file
		on btn_skeleton pressed do (
			try (
				gc()
				local se = TSkeletonsExporter()
				se.exportSkelAndMeshes()
				MessageBox "Skeleton And Meshes exported OK"
			) catch (
				MessageBox ("Error Exporting Skeleton:\n" + getCurrentException())
			)
		)

		-- Exports the current selection mesh to file
		on btn_skel_anims pressed do (
			try (
				gc()
				local se = TSkeletonsExporter()
				se.exportAnim()
				MessageBox "Skeleton Animation exported OK"
			) catch (
				MessageBox ("Error Exporting Skeleton Animation:\n" + getCurrentException())
			)
		)

		-- Exports the current selection mesh to file
		on btn_rigid_anims pressed do (
			try (
				gc()
				local se = TRigidAnimExporter()
				se.exportAll()
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

	on MyMCVExporter open do (
		addRollout exporter
		addRollout utils
		addRollout attribute
	) 
	on MyMCVExporter close do (
		removeRollout exporter
		removeRollout utils
		removeRollout attribute
	) 
) -- end utility MyMCVExporter 