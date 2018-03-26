clearListener()
gc()

--http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=GUID-7EAA7D84-5775-4E4C-9936-D874EB7A42BB
--http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_ADFD29E4_9751_4F55_98CC_F7C721C9AEE2_htm
--http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_0A1E08F8_A8C5_4737_AA6F_39194AD16559_htm

global enemiesData = attributes enemyData (
	parameters main rollout:params (
		hitPoints type: #integer ui:HP default:5
		enemyType type: #string
	)
	rollout params "Enemy Data" (
		spinner HP "Hit Points" type: #integer
		dropdownlist enemyType "Enemy Type" items:# ("melee","air")
	)
)
-- -----------------------------------------------------------

-- Adds Attribute Holder if it doesn't have one. Just to keep attributes separated in some special modifier.
fn addAttributeHolderModifier obj = (
	local emptyModif = EmptyModifier()
	if not validModifier obj emptyModif or
	   obj.modifiers.count <= 0 then (
		-- If it doesn't have any modifier validModifier returns true, so we also have to check that.
		format "Doesn't have Attribute Holder empty modifier.\n"
		addModifier obj emptyModif
	)
)

fn hasAttribute obj defName = (
	local objDefs = custAttributes.getDefs obj.modifiers[#Attribute_Holder]
	for objdef in objDefs do (
		format "objdef: %\n" (custAttributes.getDefClass objdef) as string -- DEBUG
		format "objdef.name: %\n" objdef.name -- DEBUG
		if objdef.name == defName then return true
	)
	return false
)

fn findAttribute obj defName = (
	local objDefs = custAttributes.getDefs obj.modifiers[#Attribute_Holder]
	for i = 1 to objDefs.count do (
		if objDefs[i].name == defName then return i
	)
	return undefined
)

-- Adds attribute of type defName to object's Attribute Holder modifier.
fn addAttribute obj defName = (
	addAttributeHolderModifier obj
	local attr = custAttributes.get obj.modifiers[#Attribute_Holder] 1
	if hasAttribute obj defName then format "hasAttribute=TRUE\n" else format "hasAttribute=FALSE\n" -- DEBUG
	if attr == undefined then format "attr=UNDEFINED\n" else format "attr=OK\n" -- DEBUG
	if /*attr != undefined and*/
	   not hasAttribute obj defName then (
		case defName of (
			enemyData: attrDef = enemiesData
			default: format "ERROR: invalid attribute definition for object %.\n" obj.name
		)
		--TODO: Problema si no tiene ningún attribute, aunque tenga el Holder.
		--Type error: custAttributes.add requires MSCustAttribDef, got: undefined
		custAttributes.add obj.modifiers[#Attribute_Holder] attrDef
		format "Added attribute to %.\n" object.name -- DEBUG
	) else format "Attribute % not added.\n" defName -- DEBUG
)

-- Deletes attribute if it exists (inside #Attribute_Holder).
fn delAttribute obj defName = (
	local idxFound = findAttribute obj defName
	if idxFound != undefined then (
		custAttributes.delete obj.modifiers[#Attribute_Holder] idxFound
	)
)

fn addAttributeToSel defName = (
	for obj in selection do (
		addAttribute obj defName
	)
)

fn delAttributeFromSel defName = (
	for obj in selection do (
		delAttribute obj defName
	)
)

--deleteModifier obj 1
--custAttributes.getDef obj.modifiers[#Attribute_Holder] 1
--custAttributes.get obj.modifiers[#Attribute_Holder] 1
--hasAttribute $ #enemyData

addAttributeToSel #enemyData
--delAttributeFromSel #enemyData


--custAttributes.get obj.modifiers[#Attribute_Holder] 1
