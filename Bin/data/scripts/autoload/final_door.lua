function firstChrysalisTaken()
	blockMenu(true)
	takePlayerControl()
	freezePower()
	wait(2)
	makeInvulnerable(3600)
	wait(1.1)
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2.3)
	showDialog("Gather 12 chrysalides to open the gate.", 46, false)
	wait(2)
	setCancelableWithButton(true)
	waitCondition(isDialogHidden)
	wait(0.2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	unfreezePower()
	givePlayerControl()
	blockMenu(false)
	wait(1)
	makeVulnerable()
end

function lastChrysalisTaken()
	blockMenu(true)
	takePlayerControl()
	freezePower()
	wait(2)
	makeInvulnerable(3600)
	wait(1.1)
	blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2.3)
	showDialog("The gate awaits.", 46, false)
	wait(2)
	setCancelableWithButton(true)
	waitCondition(isDialogHidden)
	wait(0.2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	unfreezePower()
	givePlayerControl()
	blockMenu(false)
	wait(1)
	makeVulnerable()
end

function finalDoorCutscene()
	makeInvulnerable(3600)
	freezePower()
	blockMenu(true)
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
	turnOffLight("final_door_chrysalis_1")
	turnOffLight("final_door_chrysalis_2")
	turnOffLight("final_door_chrysalis_3")
	turnOffLight("final_door_chrysalis_4")
	turnOffLight("final_door_chrysalis_5")
	turnOffLight("final_door_chrysalis_6")
	turnOffLight("final_door_chrysalis_7")
	turnOffLight("final_door_chrysalis_8")
	turnOffLight("final_door_chrysalis_9")
	turnOffLight("final_door_chrysalis_10")
	turnOffLight("final_door_chrysalis_11")
	turnOffLight("final_door_chrysalis_12")
	wait(0.5)
	rotateFinalDoor()
	emitSoundEvent("event:/INTERACTUABLE/abrir_puerta")
	wait(6)
	emitSoundEvent("event:/INTERACTUABLE/cerrar_puerta")
	wait(0.5)
	enableRender("luz_cegadora", true)
	wait(0.8)
	startScreenTransition(0,1,0,1,0)
	wait(0.5)
	startScreenTransition(1,1,1,1,3600)
	wait(5)
	goToCredits()
	blockMenu(false)
end