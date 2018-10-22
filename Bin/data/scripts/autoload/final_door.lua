function firstChrysalisTaken()
	takePlayerControl()
	wait(3.1)
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2.3)
	showDialog("Gather 12 chrysalis to open the gate.", 1, false)
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
	showDialog("The gate awaits.", 1, false)
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
	rotateFinalDoor()
	wait(6)
end