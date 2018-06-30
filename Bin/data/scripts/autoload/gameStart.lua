function onGameStart()
	lockPlayerCameraInput()
	takePlayerControl()
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(1)
	givePlayerControl()
end