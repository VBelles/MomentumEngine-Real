function onAltarDestroyed_ms2_PYRAMID001()
	wait(0.1)
	stopEntities()
	wait(1)
	blendInCamera("ms2_camera_chrysalis07", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms2_camera_chrysalis06", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms2_camera_chrysalis05", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("ms2_camera_chrysalis08", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(2)
	blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(0.5)
	resumeEntities()
end