function onGameStart()
	stopEntities()
	blendInCamera("ms2_camera_altar", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(5)
	blendInCamera("player_camera", 3, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	resumeEntities()
end