function onLevelStart(n)
	setPlayerHp(1)
	callDelayed(3, "testFunc", "8")
end

function testFunc(n)
	setPlayerHp(2)
	wait(2)
	setPlayerHp(n)
end