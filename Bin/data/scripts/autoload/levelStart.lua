-- function onLevelStart_game_state()
-- 	blockMenu(true)
-- 	takePlayerControl()
--     startSong("intro")
    
--     --teleportPlayer(-116.526, -22.974, 366.835) --Initial egg position.
--     --Start player animation.
--     setDummyState("cinematic_sleep_loop", true, 0, "cinematic_01")
--     changePlayerState("Dummy")

--     --Start cinematicas
--     wait(0.1)
--     showHUD(false)
-- 	disablePlayerOutline()

--     local tiempoBarridoHorizonal = 20
--     local tiempoMuralVertical    = 0.1 --10s para verse completo.
--     local blendTimeMural         = 0.001
--     local tiempoIntroFloteProta  = 6
--     local tiempoCenitalLevitar   = 8
--     local tiempoGeneralSubida    = 4
--     local tiempoGeneralSubidaTop = 3
--     local tiempoAterrizajeAndar  = 6
--     local tiempoTravellingSalir  = 4

--     local tiempoParaCancelarDialogo = 0.2

--     local fontSize = 46

--     -- setPlatformEnabled("cine_cam_011", true)
-- 	-- blendInCamera("cine_cam_011", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     -- wait(tiempoBarridoHorizonal)
    
--     -- -- Start murales -- --
--         setPlatformEnabled("cine_cam_001", true)
--         blendInCamera("cine_cam_001", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("There was a time when peace and magic reigned.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_002", true)
--         blendInCamera("cine_cam_002", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("One day a seed fell from the sky and The Tree grew.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_003", true)
--         blendInCamera("cine_cam_003", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("As the almighty entity appeared, the citizens prayed for a prosper future.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_004", true)
--         blendInCamera("cine_cam_004", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("With it, other creatures appeared and new knowledge blossomed.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_005", true)
--         blendInCamera("cine_cam_005", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("But it was known that their nature was full of envy and greed, and soon their friendship with The Tree's children turned into abuse and war.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_006", true)
--         blendInCamera("cine_cam_006", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("The battle lasted for hundreds of bloody moons, and the citizens lost.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_007", true)
--         blendInCamera("cine_cam_007", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("Massacred and defeated they fell, only the Tree and its dear children remained.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_008", true)
--         blendInCamera("cine_cam_008", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("But with a final gesture for surviving, a new being was created.", fontSize, false)
--         wait(tiempoParaCancelarDialogo)
--         setCancelableWithButton(true)
--         waitCondition(isDialogHidden)

--         setPlatformEnabled("cine_cam_009", true)
--         blendInCamera("cine_cam_009", blendTimeMural, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--         wait(tiempoMuralVertical)
--         showDialog("The prophecy explains: 'A flower will rise with the dawn of the new Momentum and it will bring revenge to our species. From our ashes she will be born.' We pray for you now.", fontSize, false)
--        wait(tiempoParaCancelarDialogo)
--        setCancelableWithButton(true)
--         waitCondition(isDialogHidden)
--     -- -- End murales -- --
    
--     setPlatformEnabled("cine_cam_012", true)
--     blendInCamera("cine_cam_012", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(tiempoIntroFloteProta)

--     --Start main cinematic animation.
--     setDummyState("cinematic_01", false, 0, "idle")
--     changePlayerState("Dummy")
    
--     setPlatformEnabled("cine_cam_013", true)
--     blendInCamera("cine_cam_013", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(tiempoCenitalLevitar)

--     setPlatformEnabled("cine_cam_014", true)
--     blendInCamera("cine_cam_014", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(tiempoGeneralSubida)

--     setPlatformEnabled("cine_cam_015", true)
--     blendInCamera("cine_cam_015", 2.000, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(tiempoGeneralSubidaTop)
    
--     setPlatformEnabled("cine_cam_016", true)
--     blendInCamera("cine_cam_016", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(tiempoAterrizajeAndar)
    

--     -- setPlatformEnabled("cine_cam_010", true)
--     -- blendInCamera("cine_cam_010", 0.001, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     -- --Start cinematic walking animation.
--     -- setDummyState("cinematic_walk", true, 3, "idle")
--     -- changePlayerState("Dummy")
--     wait(tiempoTravellingSalir)
    
--     changePlayerState("Idle")
--     wait(0.5)
--     --Return to player.
--     blendInCamera("aux_cam", 3, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     wait(3)
--     copyRotationFromCamToCam("copy_for_player_cam", "player_camera")
--     blendInCamera("player_camera", 3, CAMERA_PRIORITY_GAMEPLAY, INTERPOLATOR_CUBIC_OUT)
--     changePlayerState("Walk")
--     wait(0.1)
--     walkTo(-117, 360.5)
--     wait(4)
--     changePlayerState("Idle")

    
--     showHUD(true)
-- 	enablePlayerOutline()
-- 	wait(0.5)
--     --End cinematicas.

--     stopMusic(6)
-- 	givePlayerControl()
-- 	blockMenu(false)
-- end