function onLevelStart_game_state()
    showDialog("Welcome to momentum.\nPress accept to continue.", 1)
    waitCondition(isDialogHidden)
    showDialog("Have fun.", 1)
end