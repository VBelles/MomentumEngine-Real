
function onGameStart()
player:setHp(2)
end

function onLevelStart(n)
     player:setHp(2)
	 callDelayed(2.0, "player:setHp(8)")
end