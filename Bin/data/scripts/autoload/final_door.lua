function onTriggerEnter_dani_chrysalis_002(entity)
	if entity == "The Player" 
	then
		takePlayerControl()
		wait(3.1)
		blendInCamera("final_door_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
		wait(1)
		showDialog("Gather 12 chrysalis to open the gate.", 1, false)
		wait(2)
		setCancelableWithButton(true)
		waitCondition(isDialogHidden)
		wait(0.2)
		blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
		wait(0.5)
		givePlayerControl()
	end
end