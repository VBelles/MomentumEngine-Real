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
		hitPoints type: #integer ui:HP default:5
		moveSpeed type: #float ui:moveSpeed default:2.5
		powerGiven type: #integer ui:powerGiven default:3400 
		recallDistance type: #integer ui:recallDistance default:28
		chaseFov type: #integer ui:chaseFov default:60
		attackFov type: #integer ui:attackFov default:60
		enemyType type: #string
	)
	rollout params "Enemy Data" (
		spinner HP "Hit Points" type: #integer
		spinner moveSpeed "Move Speed" type: #float
		spinner powerGiven "Power Given" type: #integer range:[0, 100000, 3400]
		spinner recallDistance "Recall distance" type: #integer
		spinner chaseFov "Chase fov" type: #integer
		spinner attackFov "Attack fov" type: #integer
		dropdownlist enemyType "Enemy Type" items:# ("melee","air")
	)
)

-- def #rotateData
global rotationsData = attributes rotateData (
	parameters main rollout:params (
		x type: #integer ui:xRot default:1
		y type: #integer ui:yRot default:0
		z type: #integer ui:zRot default:0 
		rotSpeed type: #float ui:rotSpeed default:1.0
	)
	rollout params "Rotation Data" (
		spinner xRot "x axis" type: #integer range:[0, 1, 0]
		spinner yRot "y axis" type: #integer range:[0, 0, 0]
		spinner zRot "z axis" type: #integer range:[0, 0, 0]
		spinner rotSpeed "Rotation speed" type: #float
	)
)

-- -----------------------------------------------------------

struct TAttributeManager (
	fn hasValidModifier obj = (
		local emptyModif = EmptyModifier()
		return validModifier obj emptyModif and obj.modifiers.count > 0
	),
	
	-- Adds Attribute Holder if it doesn't have one. Just to keep attributes separated in some special modifier.
	fn addAttributeHolderModifier obj = (
		if not hasValidModifier obj then (
			-- If it doesn't have any modifier validModifier returns true, so we also have to check that.
			format "Doesn't have Attribute Holder empty modifier.\n"
			local emptyModif = EmptyModifier()
			addModifier obj emptyModif
		)
	),

	fn hasAttribute obj defName = (
		local objDefs = custAttributes.getDefs obj.modifiers[#Attribute_Holder]
		if objDefs == undefined then return false
		for objdef in objDefs do (
			format "objdef: %\n" (custAttributes.getDefClass objdef) as string -- DEBUG
			format "objdef.name: %\n" objdef.name -- DEBUG
			if objdef.name == defName then return true
		)
		return false
	),

	fn findAttribute obj defName = (
		local objDefs = custAttributes.getDefs obj.modifiers[#Attribute_Holder]
		if objDefs == undefined then return undefined
		for i = 1 to objDefs.count do (
			if objDefs[i].name == defName then return i
		)
		return undefined
	),

	--TODO: Esto debería encontrar en cualquier parte, porque al colapsar nuestros atributos se van a Edit_Mesh. !!!!
	fn getAttributeValue obj defName attribute = (
		format "has valid modifier: %\n" (hasValidModifier obj) as string --DEBUG
		if not hasValidModifier obj then return undefined
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			local objDefs = custAttributes.getDefs obj.modifiers[#Attribute_Holder]
			a = custAttributes.get obj.modifiers[#Attribute_Holder] idxFound
			return a[attribute].value
			--return obj.modifiers[#Attribute_Holder].defName.attribute
		)
		return undefined
	),
	
	-- Adds attribute of type defName to object's Attribute Holder modifier.
	-- TODO: Si ya tiene el atributo en otro lado, usar ese. Importante si se ha colapsado la mesh. !!!
	fn addAttribute obj defName = (
		addAttributeHolderModifier obj
		format "------------\n"
		--if hasAttribute obj defName then format "hasAttribute=TRUE\n" else format "hasAttribute=FALSE\n" -- DEBUG
		if not hasAttribute obj defName then (
			--format "defName: %\n" (defName as string) --DEBUG
			case defName of (
				#enemyData:  ( attrDef = enemiesData )
				#rotateData: ( attrDef = rotationsData )
				default:     ( format "ERROR: invalid attribute definition for object %.\n" obj.name )
			)
			--format "defName: %.\n" defName -- DEBUG
			--format "attrDef: %.\n" attrDef -- DEBUG
			custAttributes.add obj.modifiers[#Attribute_Holder] attrDef
			--format "Added attribute to %.\n" obj.name -- DEBUG
			obj.modifiers[#Attribute_Holder].enabled = false -- If it's enabled, the class of the object will be Editable_mesh.
		) else format "Attribute % not added.\n" defName -- DEBUG
	),

	-- Deletes attribute if it exists (inside #Attribute_Holder).
	fn delAttribute obj defName = (
		local idxFound = findAttribute obj defName
		if idxFound != undefined then (
			custAttributes.delete obj.modifiers[#Attribute_Holder] idxFound
			obj.modifiers[#Attribute_Holder].enabled = false
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
	)
)

format "------------------------\n" -- DEBUG
attributeManager = TAttributeManager()
--attributeManager.addAttributeToSel(#enemyData)
--attributeManager.delAttributeFromSel(#enemyData)
attributeManager.getAttributeValue $ #rotateData #x
