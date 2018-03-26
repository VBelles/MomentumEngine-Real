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

emptyModif = EmptyModifier()
--deleteModifier obj 1

fn addAttribute obj = (
	if validModifier obj emptyModif then (
		format "Has Attribute Holder empty modifier.\n"
	) else (
		addModifier obj emptyModif
	)
)

for obj in selection do (
	format "%\n" obj.name
	custAttributes.add obj.modifiers[#Attribute_Holder] enemiesData
)

custAttributes.get obj 1
--custAttributes.delete obj 1