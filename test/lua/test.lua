--printMessage("hi")
--printMessage2("hi2")

--crashTest(1, 2, 3);

-- 元类
Rectangle = {area = 0, length = 0, breadth = 0}

-- 派生类的方法 new
function Rectangle:new (o,length,breadth)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.length = length or 0
    self.breadth = breadth or 0
    self.area = length*breadth;
    return o
end

-- 派生类的方法 printArea
function Rectangle:printArea ()
    print("矩形面积为 ",self.area)
end

local rect = Rectangle:new(nil,10,20)

function test()
    --crashTest(1, false, true, 3.111111111111111, {"hello", "world"}, rect)
    crashTest(1, 1, 3.111111111111111, "hello")
end

callTest()
test()
print("hello world")