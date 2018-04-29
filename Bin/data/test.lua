
function onGameStart()
player:setHp(2)
end

function onLevelStart(n)
	if n == 1 then
     player:setHp(2)
    elseif n == 2 then
      player:setHp(4)
    end
end