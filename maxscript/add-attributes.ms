clearListener()
gc()

--http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=GUID-7EAA7D84-5775-4E4C-9936-D874EB7A42BB
--http://help.autodesk.com/view/3DSMAX/2018/ENU/?guid=__files_GUID_ADFD29E4_9751_4F55_98CC_F7C721C9AEE2_htm

global enemiesData = attributes enemyData
Redefine:CAT_CurrentDef
version:0
(
	parameters main rollout:params
	(
		hitPoints type: #integer ui:HP default:5
		hitPoints2 type: #integer ui:HP2 default:5
		enemyType type: #string
	)
	rollout params "Custom Attributes"
	( 
		spinner HP "Hit Points" type: #integer
		spinner HP2 "Hit Points2" type: #integer
		dropdownlist enemyType "Enemy Type" items:# ("melee","air")
	)
)

fn addAttribute obj = (
	
)
/*
for obj in selection do (
	format "%\n" obj.name
	custAttributes.add obj enemiesData
)
*/