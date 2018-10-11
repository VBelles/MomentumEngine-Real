fileIn "utils.ms"
fileIn "export_scene.ms"
fileIn "export_mesh.ms"
fileIn "export_rigid_anims.ms"
fileIn "export_skeletons.ms"
fileIn "import_scene.ms"
--fileIn "add-attributes.ms"

macroscript Momentum
category: "Momentum"
internalCategory: "Momentum"
tooltip: "Open the Momentum utility window"
ButtonText: "Momentum"
(
	global floater
	try closeRolloutFloater floater catch()

	local width = 218 --Default max width.

	local baseHeight = 66
	local heightPerButton = 26
	local numberOfButtons = 6
	local height = baseHeight + heightPerButton * numberOfButtons

	/* To get desktop resolution, valid for multiple screens:
		screen = (dotNetClass "System.Windows.Forms.Screen").PrimaryScreen.Bounds
		format "%x%\n" screen.Width screen.Height --DEBUG
	*/
	local MAXWinSize = getMAXWindowSize()
	local left = MAXWinSize[1] * 0.77 --77% of screen width.
	local top  = MAXWinSize[2] * 0.08 --08% of screen height.

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
		
		button btn_sel_parents "Select parents" across:2 tooltip:"Selects parents of selected objects"
		button btn_sel_children "Select children" tooltip:"Selects children of selected objects"

		button btn_import_scene "Import scene..." tooltip:"Import scene objects from scene file"
		button btn_copy_posrot "Copy pos & rot" across:2 tooltip:"Copy name, position and rotation of selected object"
		button btn_paste_posrot "Paste pos & rot" tooltip:"Paste name, position and rotation on selected object"
		
		button btn_hideOtherLayers "Hide other layers" across:2 tooltip:"Hide all layers except selected object's layer"
		button btn_showAllLayers "Show all layers" tooltip:"Shows all layers"

		button btn_collapAllLayers "Collapse all layers" tooltip:"Collapses all layers"

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
		
		fn importScene = (
			try (
				gc()
				clearListener()
				local filePath = getOpenFileName caption:"select scene" \
								 filename:(exportPath + "data/scenes/")
				if filePath != undefined then (
					local se = TSceneImporter pathToFile:filePath
					se.importScene()
				)
			) catch (
				MessageBox ("Error importing scene:\n" + getCurrentException()) title:"Error"
			)
		)

		fn copyNamePosRot = (
			try (
				gc()
				clearListener()
				if selection.count == 1 then (
					global copiedName = $.name
					global copiedRot  = $.rotation
					global copiedPos  = $.pos
				) else if selection.count > 1 then MessageBox ("Select only one object")
			) catch (
				MessageBox ("Error copying:\n" + getCurrentException()) title:"Error"
			)
		)

		fn pasteNamePosRot = (
			try (
				gc()
				clearListener()
				if selection.count == 1 then (
					$.name		= copiedName
					$.rotation	= copiedRot
					$.pos		= copiedPos
				)
			) catch (
				MessageBox ("Error pasting:\n" + getCurrentException()) title:"Error"
			)
		)

		fn hideAllLayersButCurrent = (
			try (
				gc()
				clearListener()
				local utilsManager = TUtils()
				local selectedLayers = utilsManager.getSelectedLayers()

				if selectedLayers != undefined then (
					for i = 0 to LayerManager.count - 1 do ( --Special indexing!
						local currentLayer = LayerManager.getLayer i
						--If not found is an unselected layer, so hide it.
						if findItem selectedLayers currentLayer == 0 then
							currentLayer.isHidden = true
					)
				)
			) catch (
				MessageBox ("Error hiding layer:\n" + getCurrentException()) title:"Error"
			)
		)

		fn showAllLayers = (
			try (
				gc()
				clearListener()

				for i = 0 to LayerManager.count - 1 do (
					local currentLayer = LayerManager.getLayer i
					currentLayer.isHidden = false
				)
			) catch (
				MessageBox ("Error hiding layer:\n" + getCurrentException()) title:"Error"
			)
		)

		fn collapseAllLayers = (
			try (
				gc()
				clearListener()
				local utilsManager = TUtils()
				utilsManager.collapseAllLayers()
			) catch (
				MessageBox ("Error hiding layer:\n" + getCurrentException()) title:"Error"
			)
		)

		on btn_attach_elems 	pressed do ( attachElems() )
		on btn_detach_elems 	pressed do ( detachElems() )
		on btn_check_dds 		pressed do ( checkDDS() )
		on btn_create_col 		pressed do ( createColliders() )
		on btn_sel_children 	pressed do ( selectChildren() )
		on btn_sel_parents  	pressed do ( selectParents() )
		on btn_import_scene 	pressed do ( importScene() )
		on btn_copy_posrot  	pressed do ( copyNamePosRot() )
		on btn_paste_posrot 	pressed do ( pasteNamePosRot() )
		on btn_hideOtherLayers  pressed do ( hideAllLayersButCurrent() )
		on btn_showAllLayers    pressed do ( showAllLayers() )
		on btn_collapAllLayers  pressed do ( collapseAllLayers() )

		-- Resize window if rollout collapses.
		on utils rolledUp state do (
			local utilsButtons = 4
			if state then height += heightPerButton * utilsButtons
			else height -= heightPerButton * utilsButtons

			floater.size = [width, height]
		)
	) -- end utils rollout

	rollout attribute "Momentum Attributes" (
		button btn_addAttrEnemy "+ enemy" across:3 tooltip:"Adds default enemy attributes to selected objects"
		button btn_delAttrEnemy "- enemy" tooltip:"Deletes enemy attributes from selected objects"
		button btn_setAttrEnemy "Set..." tooltip:"Sets enemy attributes to selected objects"
		
		button btn_addRotation "+ rot" across:3 tooltip:"Adds rotation attributes to selected objects"
		button btn_delRotation "- rot" tooltip:"Deletes rotation attributes from selected objects"
		button btn_setAttrRot "Set..." tooltip:"Sets rotation attributes to selected objects"

		button btn_addPlatfDir "+ platf dir" across:3 tooltip:"Adds platform director attributes to selected objects"
		button btn_delPlatfDir "- platf dir" tooltip:"Deletes platform director attributes from selected objects"
		button btn_setAttrPlatfDir "Set..." tooltip:"Sets platform director attributes to selected objects"
		
		button btn_addMovement "+ move" across:3 tooltip:"Adds movement attributes to selected objects"
		button btn_delMovement "- move" tooltip:"Deletes movement attributes from selected objects"
		button btn_setAttrMove "Set..." tooltip:"Sets move attributes to selected objects"
		
		button btn_addMechTrig "+ mech trig" across:3 tooltip:"Adds mech trigger attributes to selected objects"
		button btn_delMechTrig "- mech trig" tooltip:"Deletes mech trigger attributes from selected objects"
		button btn_setAttrMechTrig "Set..." tooltip:"Sets mechanism trigger attributes to selected objects"

		button btn_addMechanism "+ mech" across:3 tooltip:"Adds mechanism attributes to selected objects"
		button btn_delMechanism "- mech" tooltip:"Deletes mechanism attributes from selected objects"
		button btn_setAttrMech "Set..." tooltip:"Sets mechanism attributes to selected objects"

		button btn_addUnique "+ unique" across:3 tooltip:"Adds unique attributes to selected objects"
		button btn_delUnique "- unique" tooltip:"Deletes unique attributes from selected objects"
		button btn_setAttrUnique "Set..." tooltip:"Sets unique attributes to selected objects"

		button btn_addAnim "+ anim" across:3 tooltip:"Adds anim attributes to selected objects"
		button btn_delAnim "- anim" tooltip:"Deletes anim attributes from selected objects"
		button btn_setAttrAnim "Set..." tooltip:"Sets anim attributes to selected objects"

		button btn_addLight "+ light" across:3 tooltip:"Adds light attributes to selected objects"
		button btn_delLight "- light" tooltip:"Deletes light attributes from selected objects"
		button btn_setAttrLight "Set..." tooltip:"Sets light attributes to selected objects"

		button btn_addColl "+ collider" across:3 tooltip:"Adds collider attributes to selected objects"
		button btn_delColl "- collider" tooltip:"Deletes collider attributes from selected objects"
		button btn_setAttrColl "Set..." tooltip:"Sets collider attributes to selected objects"

		button btn_addCam "+ camera" across:3 tooltip:"Adds camera attributes to selected objects"
		button btn_delCam "- camera" tooltip:"Deletes camera attributes from selected objects"
		button btn_setAttrCam "Set..." tooltip:"Sets camera attributes to selected objects"

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
				attributeManager.setAttributeInSel defName attribute value
			) catch (
				MessageBox ("Error setting attribute:\n" + getCurrentException()) title:"Error"
			)
		)

		fn openSetAttrWindow defName = (
			try (
				gc()
				local attrManager = TAttributeManager()

				rollout enemyParams "Enemy parameters" (
					spinner HP "Hit Points" type: #integer align:#right across:2 align:#right
					button btnSetHP "Set" width:24 align:#left
					spinner moveSpeed "Move Speed" type: #float across:2 align:#right
					button btnSetmoveSpeed "Set" width:24 align:#left
					spinner powerGiven "Power Given" type: #integer across:2 align:#right range:[0, 100000, 3400] --min, max, initial value.
					button btnSetpowerGiven "Set" width:24 align:#left
					spinner recallDistance "Recall dist" type: #integer across:2 align:#right
					button btnSetrecallDistance "Set" width:24 align:#left
					spinner chaseFov "Chase fov" type: #integer across:2 align:#right
					button btnSetchaseFov "Set" width:24 align:#left
					spinner attackFov "Attack fov" type: #integer across:2 align:#right
					button btnSetattackFov "Set" width:24 align:#left
					button btnSetAll "SetAll"

					on btnSetHP				pressed do (setAttribute "enemyData" #hp 	 	 HP.value)
					on btnSetmoveSpeed		pressed do (setAttribute "enemyData" #movSpeed 	 moveSpeed.value)
					on btnSetpowerGiven		pressed do (setAttribute "enemyData" #pwrGiven	 powerGiven.value)
					on btnSetrecallDistance	pressed do (setAttribute "enemyData" #recallDist recallDistance.value)
					on btnSetchaseFov		pressed do (setAttribute "enemyData" #chaseFov	 chaseFov.value)
					on btnSetattackFov		pressed do (setAttribute "enemyData" #atckFov	 attackFov.value)
					on btnSetAll			pressed do (
						setAttribute "enemyData" #hp 	 	 HP.value
						setAttribute "enemyData" #movSpeed 	 moveSpeed.value
						setAttribute "enemyData" #pwrGiven	 powerGiven.value
						setAttribute "enemyData" #recallDist recallDistance.value
						setAttribute "enemyData" #chaseFov	 chaseFov.value
						setAttribute "enemyData" #atckFov	 attackFov.value
					)
				)

				rollout colParams "Collider parameters" (
					checkbox cType "Tri mesh" type: #boolean default:true across:2 align:#right
					button btnSetType "Set" width:24 align:#left
					checkbox spike "Spikes"   type: #boolean default:false across:2 align:#right
					button btnSetSpike "Set" width:24 align:#left
					dropdownlist powerUp "Power-up" across:2 items:#("-NONE-", \
															"AirborneLong", \
															"AirborneNormal", \
															"AirborneWallJump", \
															"AirDodge", \
															"Death", \
															"Dodge", \
															"FallingAttack", \
															"FastAttack", \
															"FastAttack2", \
															"FastAttackAir", \
															"FastFinisher1", \
															"FastFinisher2", \
															"GhostJumpSquat", \
															"GhostJumpSquatLong", \
															"GhostJumpWindow", \
															"GrabHigh", \
															"GrabLong", \
															"HardKnockbackAir", \
															"HardKnockbackGround", \
															"HardLanding", \
															"HorizontalLauncher", \
															"HuggingWall", \
															"HuggingWallLongJumpSquat", \
															"Idle", \
															"IdleTurnAround", \
															"JumpSquat", \
															"JumpSquatLong", \
															"JumpSquatSpring", \
															"PitFalling", \
															"PropelHigh", \
															"PropelLong", \
															"ReleasePowerAir", \
															"ReleasePowerGround", \
															"Run", \
															"Slide", \
															"SoftKnockbackAir", \
															"SoftKnockbackGround", \
															"SoftLanding", \
															"SpendCoins", \
															"SpringJump", \
															"StrongAttack", \
															"StrongAttack2", \
															"StrongAttack3", \
															"StrongFinisher1", \
															"StrongFinisher2", \
															"TurnAround", \
															"VerticalLauncher", \
															"Walk", \
															"WallJumpPlummet", \
															"WallJumpSquat", \
															"WallJumpSquatPlummet")
					button btnSetPwrUp "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetType	pressed do (setAttribute "collData" #collType cType.state)
					on btnSetSpike	pressed do (setAttribute "collData" #spke 	  spike.state)
					on btnSetPwrUp	pressed do (setAttribute "collData" #pwrUp	  powerUp.selection)
					on btnSetAll	pressed do (
						setAttribute "collData" #collType cType.state
						setAttribute "collData" #spke 	  spike.state
						setAttribute "collData" #pwrUp	  powerUp.selection
					)
				)

				rollout rotationParams "Rotation parameters" (
					group "Rotation:" (
					spinner xRot "x axis" type: #integer across:2 align:#right range:[-1, 1, 0] --min, max, initial value
					button btnSetXrot "Set" width:24 align:#left
					spinner yRot "y axis" type: #integer across:2 align:#right range:[-1, 1, 0]
					button btnSetYrot "Set" width:24 align:#left
					spinner zRot "z axis" type: #integer across:2 align:#right range:[-1, 1, 1]
					button btnSetZrot "Set" width:24 align:#left
					spinner rotSpeed "Rot speed" type: #float across:2 align:#right range:[-100, 100, 0]
					button btnSetRotSpd "Set" width:24 align:#left
					)

					group "Roll:" (
					spinner rolSpeed "Roll speed" 	   type: #float   across:2 align:#right range:[-100, 100, 0]
					button btnSetRolSpd "Set" width:24 align:#left
					spinner dir     "Platf dir"  type: #integer across:2 align:#right range:[0, 1000, 0]
					button btnSetDir "Set" width:24 align:#left
					multiListBox slt "Platf dir slots" items: #("1","2","3","4","5") height:5 width:70 across:2 align:#right
					button btnSetSlot "Set" width:24 align:#left
					)
					button btnSetAll "SetAll"

					on btnSetXrot	pressed do (setAttribute "rotateData" #x 		xRot.value)
					on btnSetYrot	pressed do (setAttribute "rotateData" #y 		yRot.value)
					on btnSetZrot	pressed do (setAttribute "rotateData" #z 		zRot.value)
					on btnSetRotSpd	pressed do (setAttribute "rotateData" #rotSpeed rotSpeed.value)
					on btnSetRolSpd	pressed do (setAttribute "rotateData" #rolSpeed rolSpeed.value)
					on btnSetDir	pressed do (setAttribute "rotateData" #dir	 	dir.value)
					on btnSetSlot	pressed do (setAttribute "rotateData" #slot	 	slt.selection)

					on btnSetAll	pressed do (
						setAttribute "rotateData" #x 		xRot.value
						setAttribute "rotateData" #y 		yRot.value
						setAttribute "rotateData" #z 		zRot.value
						setAttribute "rotateData" #rotSpeed rotSpeed.value
						setAttribute "rotateData" #rolSpeed rolSpeed.value
						setAttribute "rotateData" #dir	 	dir.value
						setAttribute "rotateData" #slot	 	slt.selection
					)
				)

				rollout platfDirParams "Platform director parameters" (
					spinner direcID "Director ID" type: #integer across:2 align:#right range:[1, 100, 1] --min, max, initial value
					button btnSetDirID "Set" width:24 align:#left
					label lab1 "Wait time:"
					spinner sl1waitT "Slot 1" type: #float across:2 align:#right range:[0.0, 100.0, 0.0]
					button btnSetSl1wait "Set" width:24 align:#left
					spinner sl2waitT "Slot 2" type: #float across:2 align:#right range:[0.0, 100.0, 0.0]
					button btnSetSl2wait "Set" width:24 align:#left
					spinner sl3waitT "Slot 3" type: #float across:2 align:#right range:[0.0, 100.0, 0.0]
					button btnSetSl3wait "Set" width:24 align:#left
					spinner sl4waitT "Slot 4" type: #float across:2 align:#right range:[0.0, 100.0, 0.0]
					button btnSetSl4wait "Set" width:24 align:#left
					spinner sl5waitT "Slot 5" type: #float across:2 align:#right range:[0.0, 100.0, 0.0]
					button btnSetSl5wait "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetDirID	 pressed do (setAttribute "platfDirData" #dirID   xRot.value)
					on btnSetSl1wait pressed do (setAttribute "platfDirData" #sl1wait sl1waitT.value)
					on btnSetSl2wait pressed do (setAttribute "platfDirData" #sl2wait sl2waitT.value)
					on btnSetSl3wait pressed do (setAttribute "platfDirData" #sl3wait sl3waitT.value)
					on btnSetSl4wait pressed do (setAttribute "platfDirData" #sl4wait sl4waitT.value)
					on btnSetSl5wait pressed do (setAttribute "platfDirData" #sl5wait sl5waitT.value)

					on btnSetAll	 pressed do (
						setAttribute "platfDirData" #dirID   xRot.value
						setAttribute "platfDirData" #sl1wait sl1waitT.value
						setAttribute "platfDirData" #sl2wait sl2waitT.value
						setAttribute "platfDirData" #sl3wait sl3waitT.value
						setAttribute "platfDirData" #sl4wait sl4waitT.value
						setAttribute "platfDirData" #sl5wait sl5waitT.value
					)
				)

				rollout moveParams "Move parameters" (
					checkbox autoMove "Automove" 	   type: #boolean default:true across:2 align:#right
					button btnSetAutoM "Set" width:24 align:#left
					checkbox loops 	  "Loops" 		   type: #boolean default:true across:2 align:#right
					button btnSetLoop "Set" width:24 align:#left
					spinner  waitTime "Wait time" 	   type: #float   across:2 align:#right range:[0, 100.0, 1.0] --min, max, initial value
					button btnSetWtime "Set" width:24 align:#left
					spinner  movSpeed "Move speed" type: #float   across:2 align:#right range:[-100.0, 100.0, 0.1]
					button btnSetMovSpd "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetAutoM 	pressed do (setAttribute "moveData" #auto autoMove.state)
					on btnSetLoop 	pressed do (setAttribute "moveData" #loop loops.state)
					on btnSetWtime 	pressed do (setAttribute "moveData" #wait waitTime.value)
					on btnSetMovSpd	pressed do (setAttribute "moveData" #spd  movSpeed.value)

					on btnSetAll	pressed do (
						setAttribute "moveData" #auto autoMove.state
						setAttribute "moveData" #loop loops.state
						setAttribute "moveData" #wait waitTime.value
						setAttribute "moveData" #spd  movSpeed.value
					)
				)

				rollout mechTrigParams "Mech trigger parameters" (
					spinner  deacTime "Deact time" type: #float   across:2 align:#right range:[0, 1000.0, 5.0] --min, max, initial value
					button btnSetDeacT "Set" width:24 align:#left
					spinner  reacTime "React time" type: #float   across:2 align:#right range:[0, 1000.0, 3.0]
					button btnSetReacT "Set" width:24 align:#left
					checkbox isResettable "Resettable" type: #boolean default:true across:2 align:#right
					button btnSetReset "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetDeacT pressed do (setAttribute "mechTrigData" #deacT deacTime.value)
					on btnSetReacT pressed do (setAttribute "mechTrigData" #reacT reacTime.value)
					on btnSetReset pressed do (setAttribute "mechTrigData" #reset isResettable.state)

					on btnSetAll   pressed do (
						setAttribute "mechTrigData" #deacT deacTime.value
						setAttribute "mechTrigData" #reacT reacTime.value
						setAttribute "mechTrigData" #reset isResettable.state
					)
				)

				rollout mechParams "Mech parameters" (
					checkbox change_render 		"Change rend" type: #boolean default:true across:2 align:#right
					button btnSetChRender "Set" width:24 align:#left
					checkbox change_collider 	"Change coll" type: #boolean default:true across:2 align:#right
					button btnSetChCol "Set" width:24 align:#left
					checkbox change_platform 	"Change platf" type: #boolean default:true across:2 align:#right
					button btnSetChPlatf "Set" width:24 align:#left
					checkbox change_animation 	"Change anim" type: #boolean default:true across:2 align:#right
					button btnSetChAnim "Set" width:24 align:#left
					checkbox render 			"Rend enabled" type: #boolean default:true across:2 align:#right
					button btnSetRender "Set" width:24 align:#left
					checkbox collider 			"Coll enabled" type: #boolean default:true across:2 align:#right
					button btnSetCol "Set" width:24 align:#left
					checkbox platform 			"Platf enabled" type: #boolean default:true across:2 align:#right
					button btnSetPlatf "Set" width:24 align:#left
					checkbox animation 			"Anim enabled" type: #boolean default:true across:2 align:#right
					button btnSetAnim "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetChRender	pressed do (setAttribute "mechData" #change_rend change_render.state)
					on btnSetChCol		pressed do (setAttribute "mechData" #change_coll change_collider.state)
					on btnSetChPlatf	pressed do (setAttribute "mechData" #change_plat change_platform.state)
					on btnSetChAnim		pressed do (setAttribute "mechData" #change_anim change_animation.state)
					on btnSetRender		pressed do (setAttribute "mechData" #rend render.state)
					on btnSetCol		pressed do (setAttribute "mechData" #coll collider.state)
					on btnSetPlatf		pressed do (setAttribute "mechData" #plat platform.state)
					on btnSetAnim		pressed do (setAttribute "mechData" #anim animation.state)

					on btnSetAll   pressed do (
						setAttribute "mechData" #change_rend change_render.state
						setAttribute "mechData" #change_coll change_collider.state
						setAttribute "mechData" #change_plat change_platform.state
						setAttribute "mechData" #change_anim change_animation.state
						setAttribute "mechData" #rend render.state
						setAttribute "mechData" #coll collider.state
						setAttribute "mechData" #plat platform.state
						setAttribute "mechData" #anim animation.state
					)
				)

				rollout uniqueParams "Unique parameters" (
					checkbox isUnique "Is unique" type: #boolean default:false across:2 align:#right
					button btnSetUnique "Set" width:24 align:#left

					on btnSetUnique pressed do (setAttribute "uniqueData" #unique isUnique.state)
				)

				rollout animParams "Anim parameters" (
					checkbox moves "Is moving" type: #boolean default:false across:2 align:#right
					button btnSetMoves "Set" width:24 align:#left
					checkbox loops "Loops" type: #boolean default:false across:2 align:#right
					button btnSetLoops "Set" width:24 align:#left
					checkbox killAfter "Kill on finish"	type: #boolean default:true across:2 align:#right
					button btnSetKill "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetMoves	pressed do (setAttribute "animData" #mov  moves.state)
					on btnSetLoops	pressed do (setAttribute "animData" #loop loops.state)
					on btnSetKill	pressed do (setAttribute "animData" #kill killAfter.state)

					on btnSetAll   pressed do (
						setAttribute "animData" #mov  moves.state
						setAttribute "animData" #loop loops.state
						setAttribute "animData" #kill killAfter.state
					)
				)

				rollout lightParams "Light parameters" (
					spinner lfov "FoV" type: #float across:2 range:[0, 1000.0, 40.0] --min, max, initial value
					button btnSetLfov "Set" width:24 align:#left
					group "Flicker" (
						checkbox flick "Add flicker" type: #boolean default:false across:2 align:#right
						button btnSetFlick "Set" width:24 align:#left
						spinner inVar "Int var"		 type: #float across:2 align:#right range:[0, 1000.0, 0.1]
						button btnSetinVar "Set" width:24 align:#left
						spinner varSpd "Var speed"	 type: #float across:2 align:#right range:[0, 1000.0, 0.5]
						button btnSetVarSpd "Set" width:24 align:#left
					)
					button btnSetAll "SetAll"

					on btnSetLfov   pressed do (setAttribute "lightData" #fov lfov.value)
					on btnSetFlick  pressed do (setAttribute "lightData" #fli flick.state)
					on btnSetinVar  pressed do (setAttribute "lightData" #inV inVar.value)
					on btnSetVarSpd pressed do (setAttribute "lightData" #vSp varSpd.value)
					on btnSetAll   pressed do (
						setAttribute "lightData" #fov lfov.value
						setAttribute "lightData" #fli flick.state
						setAttribute "lightData" #inV inVar.value
						setAttribute "lightData" #vSp varSpd.value
					)
				)

				rollout camParams "Camera parameters" (
					checkbox retToPl "Return to player"  type: #boolean default:false across:2 align:#right
					button btnSetRetToPl "Set" width:24 align:#left
					checkbox modPlRo "Mod player rot" type: #boolean default:true across:2 align:#right
					button btnSetModPlRo "Set" width:24 align:#left
					spinner tMixO "Time mix out" type: #float across:2 align:#right range:[0, 1000.0, 1.0] --min, max, initial value
					button btnSetTMixO "Set" width:24 align:#left
					spinner panSpd "Pan speed"  type: #float across:2 align:#right range:[-1000.0, 1000.0, 0.0]
					button btnSetPanSpd "Set" width:24 align:#left
					spinner panRad "Pan radius" type: #float across:2 align:#right range:[0, 1000.0, 3.0]
					button btnSetPanRad "Set" width:24 align:#left

					button btnSetAll "SetAll"

					on btnSetRetToPl pressed do (setAttribute "camData" #retToPlyr retToPl.state)
					on btnSetModPlRo pressed do (setAttribute "camData" #modPlRot  modPlRo.state)
					on btnSetTMixO	 pressed do (setAttribute "camData" #tMixOut   tMixO.value)
					on btnSetPanSpd	 pressed do (setAttribute "camData" #panSpeed  panSpd.value)
					on btnSetPanRad	 pressed do (setAttribute "camData" #panRadius panRad.value)

					on btnSetAll     pressed do (
						setAttribute "camData" #retToPlyr retToPl.state
						setAttribute "camData" #modPlRot  modPlRo.state
						setAttribute "camData" #tMixOut   tMixO.value
						setAttribute "camData" #panSpeed  panSpd.value
						setAttribute "camData" #panRadius panRad.value
					)
				)

				local rollToOpen = undefined
				local setAttrWinWidth  = 240
				local setAttrWinHeight = 270
				case defName of (
					"animData":     ( rollToOpen = animParams     
									  setAttrWinHeight = 3 + heightPerButton * 5 )
					"camData":      ( rollToOpen = camParams      
									  setAttrWinHeight = 3 + heightPerButton * 7 )
					"collData":     ( rollToOpen = colParams      
									  setAttrWinHeight = 3 + heightPerButton * 6 )
					"enemyData":    ( rollToOpen = enemyParams	  
									  setAttrWinHeight = 3 + heightPerButton * 8 )
					"lightData":    ( rollToOpen = lightParams    
									  setAttrWinHeight = 3 + heightPerButton * 7 )
					"mechData":     ( rollToOpen = mechParams	  
									  setAttrWinHeight = 3 + heightPerButton * 10 )
					"mechTrigData": ( rollToOpen = mechTrigParams 
									  setAttrWinHeight = 3 + heightPerButton * 5 )
					"moveData":     ( rollToOpen = moveParams	  
									  setAttrWinHeight = 3 + heightPerButton * 6 )
					"rotateData":   ( rollToOpen = rotationParams
									  setAttrWinHeight = 15 + heightPerButton * 14 )
					"platfDirData": ( rollToOpen = platfDirParams
									  setAttrWinHeight = 3 + heightPerButton * 9 )
					"uniqueData":   ( rollToOpen = uniqueParams   
									  setAttrWinHeight = 3 + heightPerButton * 2 )
					default: ( format "ERROR: invalid defName.\n" )
				)

				global setAttrsFloat
				try closeRolloutFloater setAttrsFloat catch()
				if setAttrsFloat != undefined then closeRolloutFloater setAttrsFloat
				setAttrsFloat = newRolloutFloater "Set attributes Tool" setAttrWinWidth setAttrWinHeight
				addRollout rollToOpen setAttrsFloat
			) catch (
				MessageBox ("Error openning attr window:\n" + getCurrentException()) title:"Error"
			)
		)
		
		on btn_addAttrEnemy pressed do ( addAttribute "enemyData" )
		on btn_delAttrEnemy pressed do ( delAttribute "enemyData" )
		on btn_setAttrEnemy pressed do ( openSetAttrWindow("enemyData") )

		on btn_addPlatfDir 	   pressed do ( addAttribute "platfDirData" )
		on btn_delPlatfDir 	   pressed do ( delAttribute "platfDirData" )
		on btn_setAttrPlatfDir pressed do  ( openSetAttrWindow("platfDirData") )

		on btn_addRotation pressed do ( addAttribute "rotateData" )
		on btn_delRotation pressed do ( delAttribute "rotateData" )
		on btn_setAttrRot pressed do  ( openSetAttrWindow("rotateData") )
		
		on btn_addMovement pressed do ( addAttribute "moveData" )
		on btn_delMovement pressed do ( delAttribute "moveData" )
		on btn_setAttrMove pressed do ( openSetAttrWindow("moveData") )

		on btn_addMechTrig pressed do ( addAttribute "mechTrigData" )
		on btn_delMechTrig pressed do ( delAttribute "mechTrigData" )
		on btn_setAttrMechTrig pressed do ( openSetAttrWindow("mechTrigData") )

		on btn_addMechanism pressed do ( addAttribute "mechData" )
		on btn_delMechanism pressed do ( delAttribute "mechData" )
		on btn_setAttrMech pressed do ( openSetAttrWindow("mechData") )

		on btn_addUnique pressed do ( addAttribute "uniqueData" )
		on btn_delUnique pressed do ( delAttribute "uniqueData" )
		on btn_setAttrUnique pressed do ( openSetAttrWindow("uniqueData") )

		on btn_addAnim pressed do ( addAttribute "animData" )
		on btn_delAnim pressed do ( delAttribute "animData" )
		on btn_setAttrAnim pressed do ( openSetAttrWindow("animData") )

		on btn_addLight pressed do ( addAttribute "lightData" )
		on btn_delLight pressed do ( delAttribute "lightData" )
		on btn_setAttrLight pressed do ( openSetAttrWindow("lightData") )

		on btn_addColl pressed do ( addAttribute "collData" )
		on btn_delColl pressed do ( delAttribute "collData" )
		on btn_setAttrColl pressed do ( openSetAttrWindow("collData") )

		on btn_addCam pressed do ( addAttribute "camData" )
		on btn_delCam pressed do ( delAttribute "camData" )
		on btn_setAttrCam pressed do ( openSetAttrWindow("camData") )

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
