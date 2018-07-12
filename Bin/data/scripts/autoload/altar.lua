function onAltarDestroyed_ms3_central_ring()
	wait(0.1)
	stopEntities()
	disablePlayerOutline()
	wait(1)
	blendInCamera("ms3_camera_chrysalis_004", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms3_camera_chrysalis_005", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms3_camera_chrysalis_009", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms3_camera_chrysalis_013", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	enablePlayerOutline()
	wait(0.5)
	resumeEntities()
end