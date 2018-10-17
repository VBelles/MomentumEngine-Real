clearListener()
gc()

/*
http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=GUID-7EAA7D84-5775-4E4C-9936-D874EB7A42BB
http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_ADFD29E4_9751_4F55_98CC_F7C721C9AEE2_htm
http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_0A1E08F8_A8C5_4737_AA6F_39194AD16559_htm
*/

-- def #enemyData
global enemiesData = attributes enemyData (
	parameters main rollout:params (
		hp  	   type: #integer ui:HP				default:5
		movSpeed   type: #float   ui:moveSpeed		default:2.5
		pwrGiven   type: #integer ui:powerGiven 	default:3400 
		recallDist type: #integer ui:recallDistance default:28
		chaseFov   type: #integer ui:chaseFov		default:60
		atckFov    type: #integer ui:attackFov		default:60
		type   	   type: #string
	)
	rollout params "Enemy Data" (
		spinner HP "Hit Points"					 type: #integer
		spinner moveSpeed "Move Speed"			 type: #float
		spinner powerGiven "Power Given"		 type: #integer range:[0, 100000, 3400]
		spinner recallDistance "Recall distance" type: #integer
		spinner chaseFov "Chase fov"			 type: #integer
		spinner attackFov "Attack fov"			 type: #integer
		dropdownlist enemyType "Enemy Type" 	 items:# ("melee","air")
	)
)

-- def #rotateData
global rotationsData = attributes rotateData (
	parameters main rollout:params (
		x 		 type: #integer ui:xRot 	default:0
		y 		 type: #integer ui:yRot 	default:0
		z 		 type: #integer ui:zRot 	default:1 
		rotSpeed type: #float   ui:rotSpeed default:0.5

		rolSpeed type: #float   ui:rolSpeed default:0.0
		dir 	 type: #integer ui:plDir 	default:0
		--https://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_461915FA_31A2_49CE_84AF_2544B782ACA3_htm
		slot 	 type: #intTab ui:slt default:1 tabSize:1 tabSizeVariable:true
	)
	rollout params "Rotation Data" (
		group "Rotation:" (
		spinner xRot "x axis" 			  type: #integer range:[-1, 1, 0] --min, max, initial value
		spinner yRot "y axis" 			  type: #integer range:[-1, 1, 0]
		spinner zRot "z axis" 			  type: #integer range:[-1, 1, 1]
		spinner rotSpeed "Rotation speed" type: #float   range:[-100, 100, 0]
		)
		group "Roll:" (
		spinner rolSpeed "Roll speed" 	   type: #float  range:[-100, 100, 0]
		spinner plDir    "Platf director"  type: #integer range:[0, 100, 0]
		multiListBox slt "Platf dir slots"  items: #("1","2","3","4","5") \ 
			height:5 width:70 align:#right
		)
		on slt selectionEnd do slot = slt.selection as array
		on params open do slt.selection = (slot as Array) 
	)
)

-- def #platfDirData
global platfDirsData = attributes platfDirData (
	parameters main rollout:params (
		dirID   type: #integer ui:direcID 	default:1
		sl1wait type: #float   ui:sl1waitT 	default:0.0
		sl2wait type: #float   ui:sl2waitT 	default:0.0
		sl3wait type: #float   ui:sl3waitT 	default:0.0
		sl4wait type: #float   ui:sl4waitT 	default:0.0
		sl5wait type: #float   ui:sl5waitT 	default:0.0
	)
	rollout params "Platf director Data" (
		spinner direcID "Director ID" type: #integer range:[1, 100, 1] --min, max, initial value
		label lab1 "Wait time:"
		spinner sl1waitT "Slot 1" type: #float range:[0.0, 100.0, 0.0] --min, max, initial value
		spinner sl2waitT "Slot 2" type: #float range:[0.0, 100.0, 0.0]
		spinner sl3waitT "Slot 3" type: #float range:[0.0, 100.0, 0.0]
		spinner sl4waitT "Slot 4" type: #float range:[0.0, 100.0, 0.0]
		spinner sl5waitT "Slot 5" type: #float range:[0.0, 100.0, 0.0]
	)
)

-- def #moveData
global movesData = attributes moveData (
	parameters main rollout:params (
		auto type: #boolean ui:autoMove default:true
		loop type: #boolean ui:loops    default:true
		wait type: #float   ui:waitTime default:1.0 
		spd  type: #float   ui:movSpeed default:0.1
	)
	rollout params "Movement Data" (
		checkbox autoMove "Automove" 	   type: #boolean align:#right
		checkbox loops 	  "Loops" 		   type: #boolean align:#right
		spinner  waitTime "Wait time" 	   type: #float   range:[0, 100.0, 1.0] --min, max, initial value
		spinner  movSpeed "Move speed" type: #float   range:[-100.0, 100.0, 0.1]
	)
)

-- def #mechTrigData
global mechsTrigData = attributes mechTrigData (
	parameters main rollout:params (
		deacT  type: #float   ui:deacTime 	  default:5.0 
		reacT  type: #float   ui:reacTime 	  default:3.0
		reset  type: #boolean ui:isResettable default:true
	)
	rollout params "Mech trigger Data" (
		spinner  deacTime "Deactivation time" type: #float   range:[0, 1000.0, 5.0] --min, max, initial value
		spinner  reacTime "Reactivation time" type: #float   range:[0, 1000.0, 3.0]
		checkbox isResettable "Is resettable" type: #boolean align:#right
	)
)

-- def #mechData
global mechsData = attributes mechData (
	parameters main rollout:params (
		change_rend type: 	#boolean ui:change_render   	default:true
		change_coll type: 	#boolean ui:change_collider 	default:true
		change_plat type: 	#boolean ui:change_platform 	default:true
		change_anim type: 	#boolean ui:change_animation 	default:true
		rend type: 			#boolean ui:render   			default:true
		coll type: 			#boolean ui:collider 			default:true
		plat type: 			#boolean ui:platform 			default:true
		anim type: 			#boolean ui:animation 			default:true
	)
	rollout params "Mech Data" (
		checkbox change_render 		"Change render" 				type: #boolean align:#right
		checkbox change_collider 	"Change collider" 				type: #boolean align:#right
		checkbox change_platform 	"Change platform" 				type: #boolean align:#right
		checkbox change_animation 	"Change animation" 				type: #boolean align:#right
		checkbox render 			"Render enabled on active" 		type: #boolean align:#right
		checkbox collider 			"Collider enabled on active" 	type: #boolean align:#right
		checkbox platform 			"Platf enabled on active" 	type: #boolean align:#right
		checkbox animation 			"Anim enabled on active" 	type: #boolean align:#right
	)
)

-- def #uniqueData
global uniquesData = attributes uniqueData (
	parameters main rollout:params (
		unique type: #boolean ui:isUnique default:false
	)
	rollout params "Uniques Data" (
		checkbox isUnique "Is unique" type: #boolean align:#right
	)
)

-- def #animData
global animsData = attributes animData (
	parameters main rollout:params (
		mov  type: #boolean ui:moves 	 default:false
		loop type: #boolean ui:loops 	 default:false
		kill type: #boolean ui:killAfter default:true
	)
	rollout params "Rigid anims Data" (
		checkbox moves "Is moving" 			type: #boolean align:#right
		checkbox loops "Loops"     			type: #boolean align:#right
		checkbox killAfter "Kill on finish"	type: #boolean align:#right
	)
)

-- def #lightData
global lightsData = attributes lightData (
	parameters main rollout:params (
		fov type: #float ui:lfov default:40.0

		fli type: #boolean ui:flick  default:false
		inV type: #float   ui:inVar  default:0.1
		vSp type: #float   ui:varSpd default:0.5
	)
	rollout params "Lights Data" (
		spinner lfov "FoV" type: #float range:[0, 1000.0, 40.0] --min, max, initial value
		
		group "Flicker" (
			checkbox flick "Add flicker" type: #boolean align:#right
			spinner inVar "Int var"		 type: #float range:[0, 1000.0, 0.1]
			spinner varSpd "Var speed"	 type: #float range:[0, 1000.0, 0.5]
		)
	)
)

-- def #collData
global colsData = attributes collData (
	parameters main rollout:params (
		collType type: #boolean ui:cType default:true
		spke     type: #boolean ui:spike default:false
		pwrUp    type: #string default:"-NONE-"
		pwrUpSel type: #integer default:1
	)
	rollout params "Collider Data" (
		checkbox cType "Tri mesh" type: #boolean align:#right
		checkbox spike "Spikes"   type: #boolean align:#right
		dropdownlist powerUp "Power-up" items:#("-NONE-", \
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
	
		on powerUp selected i do (
			pwrUp = powerUp.items[i]
			pwrUpSel = powerUp.selection
		)
		on params open do powerUp.selection = pwrUpSel
	)
)

-- def #camData
global camsData = attributes camData (
	parameters main rollout:params (
		retToPlyr type: #boolean ui:retToPl default:false
		modPlRot  type: #boolean ui:modPlRo default:true
		tMixOut   type: #float   ui:tMixO   default:1.0
		panSpeed  type: #float   ui:panSpd  default:0.0
		panRadius type: #float   ui:panRad  default:3.0
	)
	rollout params "Cameras Data" (
		checkbox retToPl "Return to player cam"  type: #boolean align:#right
		checkbox modPlRo "Modify player cam rot" type: #boolean align:#right
		spinner tMixO "Time to mix out" type: #float range:[0, 1000.0, 1.0] --min, max, initial value
		spinner panSpd "Panning speed"  type: #float range:[-1000.0, 1000.0, 0.0]
		spinner panRad "Panning radius" type: #float range:[0, 1000.0, 3.0]
	)
)

-- def #camTrigData
global camsTrigData = attributes camTrigData (
	parameters main rollout:params (
		suggYaw type: #boolean ui:sugYaw default:false
		oposYaw type: #boolean ui:opoYaw default:false
		suggPit type: #boolean ui:sugPit default:true
		forcDis type: #boolean ui:forDis default:false
		centCam type: #boolean ui:cenCam default:false
		fPosEnt type: #boolean ui:fPosEn default:false

		cCamSpdX type: #float   ui:cCamSpX  default:18.0
		cCamSpdY type: #float   ui:cCamSpY  default:9.0
	)
	rollout params "Camera trigger Data" (
		checkbox sugYaw "Suggest Yaw"			type: #boolean align:#right
		checkbox opoYaw "Has opposite Yaw"		type: #boolean align:#right
		checkbox sugPit "Suggest Pitch"			type: #boolean align:#right
		checkbox forDis "Force distance"		type: #boolean align:#right
		checkbox cenCam "Change centering cam"	type: #boolean align:#right
		checkbox fPosEn "Force pos on enter"	type: #boolean align:#right

		spinner cCamSpX "Center cam spd x" type: #float range:[-1000.0, 1000.0, 18.0] --min, max, initial value
		spinner cCamSpY "Center cam spd y" type: #float range:[-1000.0, 1000.0, 9.0]
	)
)

-- -----------------------------------------------------------------------------

struct TAttributeManager (
	fn hasAttribute obj defName = (
		for i = 1 to custAttributes.count obj do (
			att = custAttributes.get obj i
			if att.name == defName then return true
		)
		return false
	),

	fn findAttribute obj defName = (
		local objDefs = custAttributes.getDefs obj
		--format "objDefs: %\n" objDefs --DEBUG
		if objDefs == undefined then return undefined
		for i = 1 to objDefs.count do (
			--format "objDef.name: % -- defName: %\n" (objDefs[i].name as string) defName -- DEBUG
			if objDefs[i].name as string == defName then return i
		)
		return undefined
	),

	fn getAttrValue obj defName attribute = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			local objDefs = custAttributes.getDefs obj
			a = custAttributes.get obj idxFound

			--a[#pwrUp] returns undefined for lists, so pick this manually.
			--format "obj: % attribute: %\n" obj.name (attribute as string) --DEBUG
			if attribute == #pwrUp then return obj.collData.pwrUp
			if attribute == #slt then return obj.rotateData.slot

			if a[attribute] != undefined then return a[attribute].value
		)
		return undefined
	),
	
	fn setAttrValue obj defName attribute newValue = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			local objDefs = custAttributes.getDefs obj
			a = custAttributes.get obj idxFound

			if attribute == #pwrUp then obj.collData.pwrUpSel = newValue
			if attribute == #slot then obj.rotateData.slot = (newValue as array)

			if a[attribute] != undefined then a[attribute].value = newValue
		)
	),

	-- Adds attribute of type defName to object.
	fn addAttribute obj defName = (
		if not hasAttribute obj defName then (
			case defName of (
				"animData":     ( attrDef = animsData     )
				"camData":      ( attrDef = camsData      )
				"camTrigData":  ( attrDef = camsTrigData  )
				"collData":     ( attrDef = colsData      )
				"enemyData":    ( attrDef = enemiesData	  )
				"lightData":    ( attrDef = lightsData    )
				"mechData":     ( attrDef = mechsData	  )
				"mechTrigData": ( attrDef = mechsTrigData )
				"moveData":     ( attrDef = movesData	  )
				"platfDirData": ( attrDef = platfDirsData )
				"rotateData":   ( attrDef = rotationsData )
				"uniqueData":   ( attrDef = uniquesData   )
				default: ( format "ERROR: invalid attribute definition for object %.\n" obj.name )
			)
			custAttributes.add obj attrDef
		) else format "Attribute % not added, already exists.\n" defName -- DEBUG
	),

	-- Deletes attribute if it exists.
	fn delAttribute obj defName = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			custAttributes.delete obj idxFound
		)
	),

	fn addAttributeToSel defName = (
		for obj in selection do (
			addAttribute obj defName
		)
	),

	fn delAttributeFromSel defName = (
		for obj in selection do (
			delAttribute obj defName
		)
	),

	fn setAttributeInSel defName attribute newValue = (
		for obj in selection do (
			setAttrValue obj defName attribute newValue
		)
	)
)

--format "------------------------\n" -- DEBUG
--attributeManager = TAttributeManager()
--attributeManager.addAttributeToSel("enemyData")
--attributeManager.delAttributeFromSel("enemyData")
--attributeManager.getAttrValue $ #enemyData #hitPoints
----
--attributeManager.addAttribute $ "rotateData"
--attributeManager.delAttributeFromSel("rotateData")
--attributeManager.getAttrValue $ #rotateData #y
