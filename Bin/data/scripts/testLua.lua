function onLevelStart_1()
	setPlayerHp(1)
	callDelayed(3, "testFunc", "8")
end

function testFunc(n)
	setPlayerHp(2)
	wait(2)
	setPlayerHp(n)
end

function onTriggerEnter_aaa(n)
	setPlayerHp(5)
end