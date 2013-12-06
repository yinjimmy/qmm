
local MenuScene = class("MenuScene", function()
    return display.newScene("MenuScene")
end)

-- Create a simple button
local function newButton(imageName, name, movable, listener)
    local sprite = display.newSprite(imageName)

    if name then
        local cs = sprite:getContentSize()
        local label = ui.newTTFLabel({text = name, color = display.COLOR_BLACK})
        label:setPosition(cs.width / 2, cs.height / 2)
        sprite:addChild(label)
    end

    sprite:setTouchEnabled(true) -- enable sprite touch
    sprite:addTouchEventListener(function(event, x, y, prevX, prevY)
        if event == "began" then
            sprite:setOpacity(128)
            -- return cc.TOUCH_BEGAN -- stop event dispatching
            return cc.TOUCH_BEGAN_NO_SWALLOWS -- continue event dispatching
        end

        local touchInSprite = sprite:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
        if event == "moved" then
            sprite:setOpacity(128)

            if movable then
                local offsetX = x - prevX
                local offsetY = y - prevY
                local sx, sy = sprite:getPosition()
                sprite:setPosition(sx + offsetX, sy + offsetY)
                return cc.TOUCH_MOVED_RELEASE_OTHERS -- stop event dispatching, remove others node
                -- return cc.TOUCH_MOVED_SWALLOWS -- stop event dispatching
            end

        elseif event == "ended" then
            if touchInSprite then listener() end
            sprite:setOpacity(255)
        else
            sprite:setOpacity(255)
        end
    end, cc.MULTI_TOUCHES_ON)

    return sprite
end

function MenuScene:ctor()
    newButton("GreenButton.png", "GreenButton1", true, function()
        print("GreenButton1 TAP")
    end):addTo(self):pos(display.right - 100, display.bottom + 200)

    local parentButton = newButton("PinkButton.png", "PinkButton1", true, function()
        print("PinkButton1 TAP")
    end):addTo(self):pos(display.right - 200, display.bottom + 300):zorder(1000)

    newButton("BlueButton.png", "BlueButton1", false, function()
        print("BlueButton1 TAP")
        -- parentButton:setVisible(false)
    end):addTo(parentButton):scale(0.5):pos(100, 100)

    newButton("WhiteButton.png", "WhiteButton1", true, function()
        print("WhiteButton TAP")
    end):addTo(self):pos(display.right - 300, display.bottom + 400)

    newButton("BlueButton.png", "BlueButton1", true, function()
        print("BlueButton TAP")
    end):addTo(self):pos(display.right - 400, display.bottom + 500)

    ----

    local batch = display.newBatchNode(GAME_TEXTURE_IMAGE_FILENAME):addTo(self)

    newButton("#GreenButton2.png", nil, true, function()
        print("GreenButton2 TAP")
    end):addTo(batch):pos(display.right - 100, display.bottom + 600)

    local parentButton2 = newButton("#PinkButton2.png", nil, true, function()
        print("PinkButton2 TAP")
    end):addTo(batch):pos(display.right - 200, display.bottom + 700):zorder(1000)

    newButton("#BlueButton2.png", nil, false, function()
        print("BlueButton2 TAP")
        parentButton2:setVisible(false)
    end):addTo(parentButton2):scale(0.5):pos(100, 100)

    newButton("#WhiteButton2.png", nil, true, function()
        print("WhiteButton2 TAP")
    end):addTo(batch):pos(display.right - 300, display.bottom + 800)

    newButton("#BlueButton2.png", nil, true, function()
        print("BlueButton2 TAP")
    end):addTo(batch):pos(display.right - 400, display.bottom + 900)

end

return MenuScene
