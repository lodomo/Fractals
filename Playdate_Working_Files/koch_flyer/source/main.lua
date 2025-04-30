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
local SHRINKING = false
local STARS = {}
local STAR_HOLDER = nil
local WARP_COOLDOWN <const> = 15
local WARP_TICKS = 0

-- THE SHIP!
local SHIP_TILES = pdgfx.imagetable.new("images/ship")
local SHIP_POSITION = { x = 200, y = 120 }
local SHIP_PADDING = 30
local SHIP_BOUNDS = {
    x1 = SHIP_PADDING,
    y1 = SHIP_PADDING,
    x2 = MAX_X - SHIP_PADDING,
    y2 = MAX_Y - SHIP_PADDING
}
local SHIP_PARTICLES = {}

-- Music
local music = pd.sound.sampleplayer.new("sounds/themoon.wav")
music:play(0)

local function init()
    pdgfx.setClipRect(0, 0, 400, 240)
    SCALE_POINT_X, SCALE_POINT_Y = GenMiddle(START_X, START_Y, END_X, END_Y)
    STARS = GenerateStars()

    if SCALE_POINT_X > 200 then
        START_X = START_X - (SCALE_POINT_X - 200)
        END_X = END_X - (SCALE_POINT_X - 200)
        SCALE_POINT_X = 200
    elseif SCALE_POINT_X < 200 then
        START_X = START_X + (200 - SCALE_POINT_X)
        END_X = END_X + (200 - SCALE_POINT_X)
        SCALE_POINT_X = 200
    end

    if SCALE_POINT_Y > 120 then
        START_Y = START_Y - (SCALE_POINT_Y - 120)
        END_Y = END_Y - (SCALE_POINT_Y - 120)
        SCALE_POINT_Y = 120
    elseif SCALE_POINT_Y < 120 then
        START_Y = START_Y + (120 - SCALE_POINT_Y)
        END_Y = END_Y + (120 - SCALE_POINT_Y)
        SCALE_POINT_Y = 120
    end
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

--[[
-- Generate Stars
--]]
function GenerateStars()
    local stars = {}
    for i = 1, 75 do
        local x = math.random(0, MAX_X)
        local y = math.random(0, MAX_Y)
        table.insert(stars, { x = x, y = y })
    end
    return stars
end

function DrawStars(stars)
    local size = 0
    for _, star in ipairs(stars) do
        pdgfx.setColor(pdgfx.kColorWhite)
        size = math.random(1, 3)
        pdgfx.fillRect(star.x, star.y, size, size)
    end
end

function DrawWarpSpeed(stars1, stars2)
    for i = 1, #stars1 do
        local star1 = stars1[i]
        local star2 = stars2[i]

        pdgfx.setColor(pdgfx.kColorWhite)
        pdgfx.drawLine(star1.x, star1.y, star2.x, star2.y)
    end
end

function ScaleStars(stars, scale)
    for _, star in ipairs(stars) do
        star.x = Lerp(200, star.x, scale)
        star.y = Lerp(120, star.y, scale)
    end
end

function LerpTrails(stars1, stars2, t)
    for i = 1, #stars1 do
        stars2[i].x = Lerp(stars2[i].x, stars1[i].x, t)
        stars2[i].y = Lerp(stars2[i].y, stars1[i].y, t)
    end
end

function CleanupStars(stars)
    for _, star in ipairs(stars) do
        if star.x > 650 or star.y > 650 or star.x < -250 or star.y < -250 or
            (math.abs(star.x - 200) < 10 and
                math.abs(star.y - 120) < 10) then
            star.x = math.random(0, MAX_X)
            star.y = math.random(0, MAX_Y)
        end
    end
end

function CleanupWarpStars(stars1, stars2)
    for i = 1, #stars1 do
        local star1 = stars1[i]
        local star2 = stars2[i]

        if star1.x > 650 or star1.y > 650 or star1.x < -250 or star1.y < -250 or
            (math.abs(star1.x - 200) < 10 and
                math.abs(star1.y - 120) < 10) then
            tempx = math.random(0, MAX_X)
            tempy = math.random(0, MAX_Y)
            star1.x = tempx
            star1.y = tempy
            star2.x = tempx
            star2.y = tempy
        end
    end
end

function RotateStars(stars, angle)
    local cos = math.cos(angle)
    local sin = math.sin(angle)

    for _, star in ipairs(stars) do
        local x = star.x - 200
        local y = star.y - 120

        star.x = x * cos - y * sin + 200
        star.y = x * sin + y * cos + 120
    end
end

function DrawShip()
    local x_index = SHIP_POSITION.x // (400 / 3) + 1
    local y_index = SHIP_POSITION.y // (240 / 3) + 1
    SHIP_TILES:getImage(x_index, y_index):drawCentered(SHIP_POSITION.x, SHIP_POSITION.y)
end

function ShipParticles()
    local x = SHIP_POSITION.x + math.random(-10, 10)
    local y = SHIP_POSITION.y + math.random(-10, 10)
    table.insert(SHIP_PARTICLES, { x = x, y = y, age = 0 })

    for i = #SHIP_PARTICLES, 1, -1 do
        local particle = SHIP_PARTICLES[i]
        particle.age = particle.age + 1

        if particle.age > 50 then
            table.remove(SHIP_PARTICLES, i)
        else
            particle.x = particle.x + math.random(-1, 1)
            particle.y = particle.y + math.random(-1, 1)
            local x_offset = Lerp(200, SHIP_POSITION.x, 1) - 200
            local y_offset = Lerp(120, SHIP_POSITION.y, 1) - 120
            local magnitude = math.sqrt((x_offset * x_offset) + (y_offset * y_offset))

            -- Fix when the ship is at 200, 120 dead center
            if x_offset ~= 0 then
                x_offset = x_offset / magnitude * 4
            end

            if y_offset ~= 0 then
                y_offset = y_offset / magnitude * 4
            else
                y_offset = 1
            end

            particle.x = particle.x + x_offset
            particle.y = particle.y + y_offset
            pdgfx.fillRect(particle.x, particle.y, 2 + particle.age / 5, 2 + particle.age / 5)
        end
    end
end

function pd.update()
    LAST_TICK = TICKS
    TICKS = pd.getCrankTicks(360)

    SHRINKING = false
    if TICKS < 0 and LAST_TICK < 0 then
        SHRINKING = true
    end

    local scale = 1.005
    local warp_scale = 1.004
    local warp_scale2 = 1.06
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
        RotateStars(STARS, 0.25 * angle)
        if STAR_HOLDER then
            RotateStars(STAR_HOLDER, 0.25 * angle)
        end
    end

    scale = scale + (1 * TICKS / 360)

    --[[
    -- Handle shifting the line, and keep the line in bounds
    --]]
    if LEFT_DOWN then
        SHIP_POSITION.x = SHIP_POSITION.x - INPUT_SHIFT
        if SHIP_POSITION.x < SHIP_BOUNDS.x1 then
            SHIP_POSITION.x = SHIP_BOUNDS.x1
        end
        if SHIP_POSITION.x > SHIP_BOUNDS.x2 then
            SHIP_POSITION.x = SHIP_BOUNDS.x2
        end
    end

    if RIGHT_DOWN then
        SHIP_POSITION.x = SHIP_POSITION.x + INPUT_SHIFT
        if SHIP_POSITION.x < SHIP_BOUNDS.x1 then
            SHIP_POSITION.x = SHIP_BOUNDS.x1
        end
        if SHIP_POSITION.x > SHIP_BOUNDS.x2 then
            SHIP_POSITION.x = SHIP_BOUNDS.x2
        end
    end

    if UP_DOWN then
        SHIP_POSITION.y = SHIP_POSITION.y - INPUT_SHIFT
        if SHIP_POSITION.y < SHIP_BOUNDS.y1 then
            SHIP_POSITION.y = SHIP_BOUNDS.y1
        end
    end

    if DOWN_DOWN then
        SHIP_POSITION.y = SHIP_POSITION.y + INPUT_SHIFT
        if SHIP_POSITION.y > SHIP_BOUNDS.y2 then
            SHIP_POSITION.y = SHIP_BOUNDS.y2
        end
    end

    --[[
    -- This is where the zooming is cheated.
    -- If the line gets too big, scale it by 1/3
    -- If the line gets too small, scale it by 3
    -- This keeps overflow errors from happening
    --]]
    if SHRINKING and base_length < MIN_LEN then
        START_X = Lerp(SCALE_POINT_X, START_X, 3)
        START_Y = Lerp(SCALE_POINT_Y, START_Y, 3)
        END_X = Lerp(SCALE_POINT_X, END_X, 3)
        END_Y = Lerp(SCALE_POINT_Y, END_Y, 3)
    elseif base_length > MAX_LEN * 1.05 then
        START_X = Lerp(SCALE_POINT_X, START_X, ONE_THIRD)
        START_Y = Lerp(SCALE_POINT_Y, START_Y, ONE_THIRD)
        END_X = Lerp(SCALE_POINT_X, END_X, ONE_THIRD)
        END_Y = Lerp(SCALE_POINT_Y, END_Y, ONE_THIRD)
    end

    -- Actually scale the line
    START_X = Lerp(SCALE_POINT_X, START_X, scale)
    START_Y = Lerp(SCALE_POINT_Y, START_Y, scale)
    END_X = Lerp(SCALE_POINT_X, END_X, scale)
    END_Y = Lerp(SCALE_POINT_Y, END_Y, scale)

    -- DRAWING
    -- Draw the screen black
    --pdgfx.fillRect(0, 0, 400, 240)

    ScaleStars(STARS, scale)

    pdgfx.clear(pdgfx.kColorBlack)
    pdgfx.setColor(pdgfx.kColorWhite)

    if TICKS > 0 then
        if STAR_HOLDER == nil then
            STAR_HOLDER = {}
            for i, star in ipairs(STARS) do
                STAR_HOLDER[i] = { x = star.x, y = star.y }
            end
        end
        CleanupWarpStars(STARS, STAR_HOLDER)
        LerpTrails(STARS, STAR_HOLDER, 0.01)
        DrawWarpSpeed(STARS, STAR_HOLDER)
    else
        if STAR_HOLDER == nil then
            CleanupStars(STARS)
        else
            WARP_TICKS = WARP_TICKS + 1
            LerpTrails(STARS, STAR_HOLDER, 2 / WARP_COOLDOWN)
            DrawWarpSpeed(STARS, STAR_HOLDER)
            if WARP_TICKS > WARP_COOLDOWN then
                STAR_HOLDER = nil
                WARP_TICKS = 0
            end
        end
    end
    DrawStars(STARS)

    -- Finally, we draw the fractals.
    pdgfx.setDrawOffset(Lerp(200, SHIP_POSITION.x, -0.1) - 200,
        Lerp(120, SHIP_POSITION.y, -0.1) - 120)
    KochClipOutliers(START_X, START_Y, END_X, END_Y, INITIAL_DEPTH)
    pdgfx.setDrawOffset(0, 0)

    DrawShip()
    ShipParticles()
end

init()
