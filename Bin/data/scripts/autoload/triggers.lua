function onTriggerEnter_enter_main_theme_trigger(entityName)
	if entityName == "The Player" then
		destroyCollider("trigger_message_teleport")
	end
end