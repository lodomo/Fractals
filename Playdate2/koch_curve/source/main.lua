import "CoreLibs/graphics"
import "CoreLibs/crank"

local pd <const> = playdate       -- Get the playdate SDK full table
local pdgfx <const> = pd.graphics -- Get the playdate SDK graphics table
local DrawLine = pdgfx.drawLine   -- Make drawline a local function, for speed

-- Settings
local START_X = 0.0
local START_Y = -400.0
local END_X = 0.0
local END_Y = 400.0
local INPUT_SHIFT = 5

-- Global Constants, usually to save speed
local SQRT_3_OVER_2 <const> = 0.86602540378
local ONE_THIRD <const> = 1 / 3
local ONE_HALF <const> = 0.5
local TWO_THIRDS <const> = 2 / 3
local INITIAL_DEPTH <const> = 3
local RENDER_DEPTH <const> = 3
local MAX_X = 400
local MAX_Y = 240
local MIN_LEN = 2 * math.sqrt(START_X * START_X + START_Y * START_Y)
local MAX_LEN = 3 * MIN_LEN
-- local SCALE_POINT_X, SCALE_POINT_Y -- Initializes in init()

-- Globals
local LEFT_DOWN = false
local RIGHT_DOWN = false
local UP_DOWN = false
local DOWN_DOWN = false
local B_DOWN = false
local LAST_TICK = 0
local TICKS = 0

local function init()
    pdgfx.setClipRect(0, 0, 400, 240)
    SCALE_POINT_X, SCALE_POINT_Y = GenMiddle(START_X, START_Y, END_X, END_Y)
end

--[[
-- Linear Interpolation
-- @param a: start value
-- @param b: end value
-- @param t: interpolation factor
-- -- @return: interpolated value
--
-- When t < 0, extrapolate beyond a
-- When t > 1, extrapolate beyond b
--]]
function Lerp(a, b, t)
    return a + (b - a) * t
end

--[[
-- Generate the middle point in the Koch curve
-- @param x1: x-coordinate of the first point
-- @param y1: y-coordinate of the first point
-- @param x5: x-coordinate of the last point
-- @param y5: y-coordinate of the last point
-- @return: x and y coordinates of the middle point
--
-- Used for finding the scale position
--]]
function GenMiddle(x1, y1, x5, y5)
    local p2x = Lerp(x1, x5, ONE_THIRD)
    local p2y = Lerp(y1, y5, ONE_THIRD)

    local p4x = Lerp(x1, x5, TWO_THIRDS)
    local p4y = Lerp(y1, y5, TWO_THIRDS)

    local p3x = Lerp(x1, x5, ONE_HALF) + (SQRT_3_OVER_2 * (p4y - p2y))
    local p3y = Lerp(y1, y5, ONE_HALF) - (SQRT_3_OVER_2 * (p4x - p2x))

    return p3x, p3y
end

--[[
-- Render the Koch Curve at a given depth
-- @param x1: x-coordinate of the first point
-- @param y1: y-coordinate of the first point
-- @param x5: x-coordinate of the last point
-- @param y5: y-coordinate of the last point
-- @param depth: recursion depth
-- @return: nil
--]]
function RenderKoch(x1, y1, x5, y5, depth)
    if depth == 0 then
        DrawLine(x1, y1, x5, y5)
        return
    end

    -- Get the 2nd point
    local p2x = Lerp(x1, x5, ONE_THIRD)
    local p2y = Lerp(y1, y5, ONE_THIRD)

    -- Get the 4th point
    local p4x = Lerp(x1, x5, TWO_THIRDS)
    local p4y = Lerp(y1, y5, TWO_THIRDS)

    -- Get the 3rd point, offset from the 2nd and 4th points
    local p3x = Lerp(x1, x5, ONE_HALF) + (SQRT_3_OVER_2 * (p4y - p2y))
    local p3y = Lerp(y1, y5, ONE_HALF) - (SQRT_3_OVER_2 * (p4x - p2x))

    RenderKoch(x1, y1, p2x, p2y, depth - 1)
    RenderKoch(p2x, p2y, p3x, p3y, depth - 1)
    RenderKoch(p3x, p3y, p4x, p4y, depth - 1)
    RenderKoch(p4x, p4y, x5, y5, depth - 1)
end

--[[
-- HELPER FUNCTION
-- Generate the Koch Curve, clip off lines that are out of bounds
-- Recurses to depth 0, then sends off to the render function
-- @param x1: x-coordinate of the first point
-- @param y1: y-coordinate of the first point
-- @param x5: x-coordinate of the last point
-- @param y5: y-coordinate of the last point
-- @param depth: recursion depth
-- @return: nil
--]]
function KochClipOutliers(x1, y1, x5, y5, depth)
    if depth == 0 then
        if x1 < -1 and x5 < -1 or
            x1 > MAX_X + 1 and x5 > MAX_X + 1 or
            y1 < -1 and y5 < -1 or
            y1 > MAX_Y + 1 and y5 > MAX_Y + 1 then
            return
        end

        RenderKoch(x1, y1, x5, y5, RENDER_DEPTH)
        return
    end

    local p2x = Lerp(x1, x5, ONE_THIRD)
    local p2y = Lerp(y1, y5, ONE_THIRD)

    local p4x = Lerp(x1, x5, TWO_THIRDS)
    local p4y = Lerp(y1, y5, TWO_THIRDS)

    local p3x = Lerp(x1, x5, ONE_HALF) + (SQRT_3_OVER_2 * (p4y - p2y))
    local p3y = Lerp(y1, y5, ONE_HALF) - (SQRT_3_OVER_2 * (p4x - p2x))

    KochClipOutliers(x1, y1, p2x, p2y, depth - 1)
    KochClipOutliers(p2x, p2y, p3x, p3y, depth - 1)
    KochClipOutliers(p3x, p3y, p4x, p4y, depth - 1)
    KochClipOutliers(p4x, p4y, x5, y5, depth - 1)
end


--[[
-- Sets all the buttons to boolean values.
-- This makes it so the D-Pad is sensed every single frame instead of per press.
--]]
local buttonsToBoolsHandler = {
    leftButtonDown = function()
        LEFT_DOWN = true
    end,

    leftButtonUp = function()
        LEFT_DOWN = false
    end,

    rightButtonDown = function()
        RIGHT_DOWN = true
    end,

    rightButtonUp = function()
        RIGHT_DOWN = false
    end,

    upButtonDown = function()
        UP_DOWN = true
    end,

    upButtonUp = function()
        UP_DOWN = false
    end,

    downButtonDown = function()
        DOWN_DOWN = true
    end,

    downButtonUp = function()
        DOWN_DOWN = false
    end,

    BButtonDown = function()
        B_DOWN = true
    end,

    BButtonUp = function()
        B_DOWN = false
    end,
}
playdate.inputHandlers.push(buttonsToBoolsHandler)

function pd.update()
    LAST_TICK = TICKS
    TICKS = pd.getCrankTicks(360)

    local growing = false
    local shrinking = false

    if TICKS > 0 and LAST_TICK > 0 then
        growing = true
    elseif TICKS < 0 and LAST_TICK < 0 then
        shrinking = true
    end

    pdgfx.clear()
    local scale = 1
    local base_length = math.sqrt((END_X - START_X) * (END_X - START_X) + (END_Y - START_Y) * (END_Y - START_Y))

    if B_DOWN then
        -- Rotate the line around the scale axis
        local angle = math.rad(TICKS)
        local cos = math.cos(angle)
        local sin = math.sin(angle)
        local x1 = START_X - SCALE_POINT_X
        local y1 = START_Y - SCALE_POINT_Y
        local x2 = END_X - SCALE_POINT_X
        local y2 = END_Y - SCALE_POINT_Y
        START_X = x1 * cos - y1 * sin + SCALE_POINT_X
        START_Y = x1 * sin + y1 * cos + SCALE_POINT_Y
        END_X = x2 * cos - y2 * sin + SCALE_POINT_X
        END_Y = x2 * sin + y2 * cos + SCALE_POINT_Y
    else
        -- Otherwise, change the scale of the line
        scale = 1 + (1 * TICKS / 360)
    end

    --[[
    -- Handle shifting the line, and keep the line in bounds
    --]]
    if LEFT_DOWN then
        START_X = START_X - INPUT_SHIFT
        END_X = END_X - INPUT_SHIFT
        SCALE_POINT_X = SCALE_POINT_X - INPUT_SHIFT

        if (SCALE_POINT_X < 0) then
            local small_shift = -SCALE_POINT_X
            START_X = START_X + small_shift
            END_X = END_X + small_shift
            SCALE_POINT_X = 0
        end
    end

    if RIGHT_DOWN then
        START_X = START_X + INPUT_SHIFT
        END_X = END_X + INPUT_SHIFT
        SCALE_POINT_X = SCALE_POINT_X + INPUT_SHIFT

        if (SCALE_POINT_X > MAX_X) then
            local small_shift = SCALE_POINT_X - MAX_X
            START_X = START_X - small_shift
            END_X = END_X - small_shift
            SCALE_POINT_X = MAX_X
        end
    end

    if UP_DOWN then
        START_Y = START_Y - INPUT_SHIFT
        END_Y = END_Y - INPUT_SHIFT
        SCALE_POINT_Y = SCALE_POINT_Y - INPUT_SHIFT

        if (SCALE_POINT_Y < 0) then
            local small_shift = -SCALE_POINT_Y
            START_Y = START_Y + small_shift
            END_Y = END_Y + small_shift
            SCALE_POINT_Y = 0
        end
    end

    if DOWN_DOWN then
        START_Y = START_Y + INPUT_SHIFT
        END_Y = END_Y + INPUT_SHIFT
        SCALE_POINT_Y = SCALE_POINT_Y + INPUT_SHIFT

        if (SCALE_POINT_Y > MAX_Y) then
            local small_shift = SCALE_POINT_Y - MAX_Y
            START_Y = START_Y - small_shift
            END_Y = END_Y - small_shift
            SCALE_POINT_Y = MAX_Y
        end
    end

    --[[
    -- This is where the zooming is cheated. 
    -- If the line gets too big, scale it by 1/3
    -- If the line gets too small, scale it by 3
    -- This keeps overflow errors from happening
    --]]
    if growing and base_length > MAX_LEN * 1.05 then
        START_X = Lerp(SCALE_POINT_X, START_X, ONE_THIRD)
        START_Y = Lerp(SCALE_POINT_Y, START_Y, ONE_THIRD)
        END_X = Lerp(SCALE_POINT_X, END_X, ONE_THIRD)
        END_Y = Lerp(SCALE_POINT_Y, END_Y, ONE_THIRD)
    elseif shrinking and base_length < MIN_LEN then
        START_X = Lerp(SCALE_POINT_X, START_X, 3)
        START_Y = Lerp(SCALE_POINT_Y, START_Y, 3)
        END_X = Lerp(SCALE_POINT_X, END_X, 3)
        END_Y = Lerp(SCALE_POINT_Y, END_Y, 3)
    end

    -- Actually scale the line
    START_X = Lerp(SCALE_POINT_X, START_X, scale)
    START_Y = Lerp(SCALE_POINT_Y, START_Y, scale)
    END_X = Lerp(SCALE_POINT_X, END_X, scale)
    END_Y = Lerp(SCALE_POINT_Y, END_Y, scale)

    pd.drawFPS(0, 0)

    -- Finally, we draw the fractals.
    KochClipOutliers(START_X, START_Y, END_X, END_Y, INITIAL_DEPTH)
end

init()
