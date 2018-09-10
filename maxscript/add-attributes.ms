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
		hitPoints 	   type: #integer ui:HP				default:5
		moveSpeed 	   type: #float   ui:moveSpeed		default:2.5
		powerGiven	   type: #integer ui:powerGiven 	default:3400 
		recallDistance type: #integer ui:recallDistance default:28
		chaseFov	   type: #integer ui:chaseFov		default:60
		attackFov	   type: #integer ui:attackFov		default:60
		enemyType	   type: #string
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

		rolSpeed type: #float   ui:rolSpeed default:5
		wait 	 type: #float   ui:waitDur 	default:2
		grup	 type: #integer ui:grup 	default:0
	)
	rollout params "Rotation Data" (
		label lab1 "Rotation:"
		spinner xRot "x axis" type: #integer range:[-1, 1, 0] --min, max, initial value
		spinner yRot "y axis" type: #integer range:[-1, 1, 0]
		spinner zRot "z axis" type: #integer range:[-1, 1, 1]
		spinner rotSpeed "Rotation speed" type: #float range:[-100, 100, 0]

		label lab2 "Roll:"
		spinner rolSpeed "Roll speed" 	  type: #float range:[-100, 100, 0]
		spinner waitDur  "Wait duration"  type: #float
		spinner grup     "Platform group" type: #integer range:[0, 1000, 0]
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
		spinner  movSpeed "Movement speed" type: #float   range:[-100.0, 100.0, 0.1]
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
		rend type: #boolean ui:render   default:true
		coll type: #boolean ui:collider default:true
	)
	rollout params "Mech Data" (
		checkbox render "Render enabled on active" 		type: #boolean align:#right
		checkbox collider "Collider enabled on active" 	type: #boolean align:#right
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
	)
	rollout params "Lights Data" (
		spinner lfov "FoV" type: #float range:[0, 1000.0, 40.0] --min, max, initial value
	)
)

-- def #collData
global colsData = attributes collData (
	parameters main rollout:params (
		collType type: #boolean ui:cType default:true
		pwrUp    type: #string default:"-NONE-"
	)
	rollout params "Collider Data" (
		checkbox cType "Tri mesh" type: #boolean align:#right
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
	
		on powerUp selected i do pwrUp = powerUp.items[i]
		on params open do powerUp.selection = findItem powerUp.items pwrUp
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

	fn getAttributeValue obj defName attribute = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			local objDefs = custAttributes.getDefs obj
			a = custAttributes.get obj idxFound

			--a[#pwrUp] returns undefined, I guess because it's a dropdownlist?
			--so pick this manually.
			if attribute == #pwrUp then return obj.collData.pwrUp

			return a[attribute].value
		)
		return undefined
	),
	
	fn setAttributeValue obj defName attribute newValue = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			local objDefs = custAttributes.getDefs obj
			a = custAttributes.get obj idxFound

			a[attribute].value = newValue
		)
	),

	-- Adds attribute of type defName to object.
	fn addAttribute obj defName = (
		if not hasAttribute obj defName then (
			case defName of (
				"animData":     ( attrDef = animsData     )
				"collData":     ( attrDef = colsData      )
				"enemyData":    ( attrDef = enemiesData	  )
				"lightData":    ( attrDef = lightsData    )
				"mechData":     ( attrDef = mechsData	  )
				"mechTrigData": ( attrDef = mechsTrigData )
				"moveData":     ( attrDef = movesData	  )
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
			setAttributeValue obj defName attribute newValue
		)
	)
)

--format "------------------------\n" -- DEBUG
--attributeManager = TAttributeManager()
--attributeManager.addAttributeToSel("enemyData")
--attributeManager.delAttributeFromSel("enemyData")
--attributeManager.getAttributeValue $ #enemyData #hitPoints
----
--attributeManager.addAttribute $ "rotateData"
--attributeManager.delAttributeFromSel("rotateData")
--attributeManager.getAttributeValue $ #rotateData #y
