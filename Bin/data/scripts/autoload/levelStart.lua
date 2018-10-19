function onLevelStart_game_state()
    [[
    --Start cinematicas
    wait(0.1)
    showHUD(false)
	disablePlayerOutline()

    local tiempoBarridoHorizonal = 2
    local tiempoMuralVertical    = 8
    local tiempoIntroFloteProta  = 6
    local tiempoCenitalLevitar   = 8
    local tiempoGeneralSubida    = 8
    local tiempoGeneralSubidaTop = 6
    local tiempoAterrizajeAndar  = 6
    local tiempoTravellingSalir  = 4

    setPlatformEnabled("cine_cam_011", true)
	blendInCamera("cine_cam_011", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoBarridoHorizonal)
    
    setPlatformEnabled("cine_cam_001", true)
    blendInCamera("cine_cam_001", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_002", true)
    blendInCamera("cine_cam_002", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
	wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_003", true)
    blendInCamera("cine_cam_003", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_004", true)
    blendInCamera("cine_cam_004", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_005", true)
    blendInCamera("cine_cam_005", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_006", true)
    blendInCamera("cine_cam_006", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_007", true)
    blendInCamera("cine_cam_007", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_008", true)
    blendInCamera("cine_cam_008", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)
    setPlatformEnabled("cine_cam_009", true)
    blendInCamera("cine_cam_009", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoMuralVertical)

    setPlatformEnabled("cine_cam_012", true)
    blendInCamera("cine_cam_012", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoIntroFloteProta)

    setPlatformEnabled("cine_cam_013", true)
    blendInCamera("cine_cam_013", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoCenitalLevitar)

    setPlatformEnabled("cine_cam_014", true)
    blendInCamera("cine_cam_014", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoGeneralSubida)

    setPlatformEnabled("cine_cam_015", true)
    blendInCamera("cine_cam_015", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoGeneralSubidaTop)
    
    setPlatformEnabled("cine_cam_016", true)
    blendInCamera("cine_cam_016", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoAterrizajeAndar)
    
    setPlatformEnabled("cine_cam_010", true)
    blendInCamera("cine_cam_010", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoTravellingSalir)
    
    --Return to player.
    blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    
    showHUD(true)
	enablePlayerOutline()
	wait(0.5)
    --End cinematicas.
    ]]
    showDialog("Welcome to momentum.\nPress accept to continue.", 1)
    waitCondition(isDialogHidden)
    showDialog("Have fun.", 1)
end