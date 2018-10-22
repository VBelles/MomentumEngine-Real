function firstChrysalisTaken()
	takePlayerControl()
	wait(3.1)
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2.3)
	showDialog("Gather 12 chrysalis to open the gate.", 46, false)
	wait(2)
	setCancelableWithButton(true)
	waitCondition(isDialogHidden)
	wait(0.2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	givePlayerControl()
end

function lastChrysalisTaken()
	takePlayerControl()
	wait(3.1)
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2.3)
	showDialog("The gate awaits.", 46, false)
	wait(2)
	setCancelableWithButton(true)
	waitCondition(isDialogHidden)
	wait(0.2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	givePlayerControl()
end

function finalDoorCutscene()
	takePlayerControl()
	stopPlayerVelocity()
	changePlayerState("Idle")
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	showHUD(false)
	wait(1)
	enableRender("final_door_chrysalis_1", false)
	enableRender("final_door_chrysalis_2", false)
	enableRender("final_door_chrysalis_3", false)
	enableRender("final_door_chrysalis_4", false)
	enableRender("final_door_chrysalis_5", false)
	enableRender("final_door_chrysalis_6", false)
	enableRender("final_door_chrysalis_7", false)
	enableRender("final_door_chrysalis_8", false)
	enableRender("final_door_chrysalis_9", false)
	enableRender("final_door_chrysalis_10", false)
	enableRender("final_door_chrysalis_11", false)
	enableRender("final_door_chrysalis_12", false)
	wait(0.5)
	rotateFinalDoor()
	wait(6)
	wait(0.5)
	enableRender("luz_cegadora", true)
	wait(0.8)
	startScreenTransition(0,1,0,1,0)
	wait(0.5)
	startScreenTransition(1,1,1,1,3600)
	wait(5)
	goToMainMenu()
end