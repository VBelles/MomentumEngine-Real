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
	
	rollout attribute "Momentum Attributes" (
		button btn_addAttrEnemy "+ enemy" across:2 tooltip:"Adds enemy attributes to selected objects"
		button btn_delAttrEnemy "- enemy" tooltip:"Deletes enemy attributes from selected objects"
		
		button btn_addRotation "+ rotation" across:2 tooltip:"Adds rotation attributes to selected objects"
		button btn_delRotation "- rotation" tooltip:"Deletes rotation attributes from selected objects"
		
		fn addAttribute defName = (
			try (
				gc()
				local attributeManager = TAttributeManager()
				attributeManager.addAttributeToSel(defName)	
				--MessageBox "Attributes added."
			) catch (
				MessageBox ("Error Exporting:\n" + getCurrentException())
			)
		)
		
		fn delAttribute defName = (
			try (
				gc()
				local attributeManager = TAttributeManager()
				attributeManager.delAttributeFromSel(defName)	
				--MessageBox "Attributes deleted."
			) catch (
				MessageBox ("Error Exporting:\n" + getCurrentException())
			)
		)
		
		on btn_addAttrEnemy pressed do ( addAttribute "enemyData" )
		on btn_delAttrEnemy pressed do ( delAttribute "enemyData" )
		
		on btn_addRotation pressed do ( addAttribute "rotateData" )
		on btn_delRotation pressed do ( delAttribute "rotateData" )
		
	) -- end attribute rollout creator

	on MyMCVExporter open do (
		addRollout exporter
		addRollout attribute
	) 
	on MyMCVExporter close do (
		removeRollout exporter
		removeRollout attribute
	) 
) -- end utility MyMCVExporter 