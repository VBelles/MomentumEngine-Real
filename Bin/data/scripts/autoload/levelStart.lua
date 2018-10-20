function onLevelStart_game_state()
    startSong("intro")
    --aquí en medio debería estar toda la cinemática
    stopMusic(6)
    [[
    --teleportPlayer(-116.526, -22.974, 366.835) --Initial egg position.
    --Start player animation.
    setDummyState("cinematic_sleep_loop", true, 0, "cinematic_01")
    changePlayerState("Dummy")

    --Start cinematicas
    wait(0.1)
    showHUD(false)
	disablePlayerOutline()

    local tiempoBarridoHorizonal = 0.01 --20
    local tiempoMuralVertical    = 0.1 --10s para verse completo.
    local blendTimeMural         = 0.001
    local tiempoIntroFloteProta  = 6
    local tiempoCenitalLevitar   = 8
    local tiempoGeneralSubida    = 8
    local tiempoGeneralSubidaTop = 6
    local tiempoAterrizajeAndar  = 6
    local tiempoTravellingSalir  = 4

    local fontSize = 1

    setPlatformEnabled("cine_cam_011", true)
	blendInCamera("cine_cam_011", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoBarridoHorizonal)
    
    -- -- Start murales -- --
        setPlatformEnabled("cine_cam_001", true)
        blendInCamera("cine_cam_001", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("There was a time when in some place peace and magic reigned.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_002", true)
        blendInCamera("cine_cam_002", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("One day a seed fell from the sky and with it The Tree grew.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_003", true)
        blendInCamera("cine_cam_003", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("As the almighty entity appeared, they prayed for a prosper future.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_004", true)
        blendInCamera("cine_cam_004", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("With him other creatures appeared, and new knowledge blossomed.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_005", true)
        blendInCamera("cine_cam_005", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("But it was known that their nature was full of envy and greed, and soon the friendship with the new companions turned into abuse and war.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_006", true)
        blendInCamera("cine_cam_006", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("The battle lasted for hundreds of bloody moons, and the citizens lost.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_007", true)
        blendInCamera("cine_cam_007", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("Massacred and defeated they fell, only the Tree and its dear children remained.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_008", true)
        blendInCamera("cine_cam_008", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("But with a final gesture for surviving, a new being was created.", fontSize)
        waitCondition(isDialogHidden)

        setPlatformEnabled("cine_cam_009", true)
        blendInCamera("cine_cam_009", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
        wait(tiempoMuralVertical)
        showDialog("The prophecy explains:
        “A flower will rise with the dawn of the new Momentum and it will bring revenge to our species.
       From our ashes she’ll be born.”
       We pray for you now.", fontSize)
        waitCondition(isDialogHidden)
    -- -- End murales -- --
    
    setPlatformEnabled("cine_cam_012", true)
    blendInCamera("cine_cam_012", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoIntroFloteProta)

    --Start main cinematic animation.
    setDummyState("cinematic_01", false, 0, "cinematic_walk")
    changePlayerState("Dummy")
    
    setPlatformEnabled("cine_cam_013", true)
    blendInCamera("cine_cam_013", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoCenitalLevitar)

    setPlatformEnabled("cine_cam_014", true)
    blendInCamera("cine_cam_014", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoGeneralSubida)

    setPlatformEnabled("cine_cam_015", true)
    blendInCamera("cine_cam_015", 2.000, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoGeneralSubidaTop)
    
    setPlatformEnabled("cine_cam_016", true)
    blendInCamera("cine_cam_016", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    wait(tiempoAterrizajeAndar)
    
    setPlatformEnabled("cine_cam_010", true)
    blendInCamera("cine_cam_010", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    --Start cinematic walking animation.
    setDummyState("cinematic_walk", true, 3, "idle")
    changePlayerState("Dummy")
    wait(tiempoTravellingSalir)
    
    --Return to player.
    blendInCamera("player_camera", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
    
    showHUD(true)
	enablePlayerOutline()
	wait(0.5)
    --End cinematicas.
    ]]
end