function onGameStart()
	blendInCamera("ms2_camera_altar", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	stopEntities()
	lockPlayerCameraInput()
	wait(5)
	blendInCamera("player_camera", 3, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	resumeEntities()
end