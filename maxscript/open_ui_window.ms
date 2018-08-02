--

utility MyMomentumOpener "MCV Utils" (
	rollout openUtils "Momentum Utils" (
		button btn_open_utils "Open utils window"

		fn openUtilsWindow = (
			try (
				gc()
				mcv_scripts_path = getDir #userStartupScripts
				mcv_scripts_path += "/../"
				filein (mcv_scripts_path + "ui_floating.ms")
			) catch (
				MessageBox ("Error opening utils:\n" + getCurrentException())
			)
		)
		
		on btn_open_utils pressed do ( openUtilsWindow() )
	) -- end open_utils rollout

	on MyMomentumOpener open do (
		addRollout openUtils
	) 
	on MyMomentumOpener close do (
		removeRollout openUtils
	) 
) -- end utility MyMCVExporter 