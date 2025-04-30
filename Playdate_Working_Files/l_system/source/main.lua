import "CoreLibs/graphics"
import "CoreLibs/crank"

local pd <const> = playdate                -- Get the playdate SDK full table
local pdgfx <const> = pd.graphics          -- Get the playdate SDK graphics table
local DrawLine = pdgfx.drawLine            -- Make drawline a local function, for speed
local DrawCircle = pdgfx.fillCircleAtPoint -- Make fillCircleAtPoint a local function, for speed

-- Global Settings
DEPTH = 8

DRAW_WIDTH = 380
DRAW_HEIGHT = 220
CENTER_X = 400 / 2
CENTER_Y = 240 / 2
PEN_UP = 2147483647

-- L-System Production
AXIOM = ""
RULES = {}
THETA = 0
D_THETA = 0
OUTPUT = ""
PATH = {}
SCALED_PATH = {}
SCALE = 1

-- Drawing Details
BASE_WIDTH = 0
BASE_HEIGHT = 0
BASE_CENTER_X = 0
BASE_CENTER_Y = 0
SCALED_WIDTH = 0
SCALED_HEIGHT = 0
CUR_TICKS = 0


local function Builder()
    OUTPUT = AXIOM
    for _ = 1, DEPTH do
        local temp = ""
        for i = 1, #OUTPUT do
            local cur = OUTPUT:sub(i, i)
            if RULES[cur] then
                temp = temp .. RULES[cur]
            else
                temp = temp .. cur
            end
        end
        OUTPUT = temp
    end
end

local function Interpreter()
    local x, y = 0, 0
    local cur_theta = THETA
    local min_x, min_y = 0, 0
    local max_x, max_y = 0, 0
    local stack = {}

    for i = 1, #OUTPUT do
        local cur = OUTPUT:sub(i, i)
        if cur == "+" then
            cur_theta = cur_theta + D_THETA
            cur_theta = cur_theta % (2 * math.pi)
        elseif cur == "-" then
            cur_theta = cur_theta - D_THETA
            cur_theta = cur_theta % (2 * math.pi)
        elseif cur == "[" then
            stack[#stack + 1] = { x, y, cur_theta }
        elseif cur == "]" then
            local popped = stack[#stack]
            stack[#stack] = nil
            x = popped[1]
            y = popped[2]
            cur_theta = popped[3]
            PATH[#PATH + 1] = { PEN_UP, PEN_UP }
            PATH[#PATH + 1] = { x, y }
        elseif cur ~= "X" then
            x = x + math.cos(cur_theta)
            y = y + math.sin(cur_theta)
            PATH[#PATH + 1] = { x, y }
        end

        if x < min_x then
            min_x = x
        end

        if y < min_y then
            min_y = y
        end


        if x > max_x then
            max_x = x
        end

        if y > max_y then
            max_y = y
        end
    end

    BASE_WIDTH = max_x - min_x
    BASE_HEIGHT = max_y - min_y
    BASE_CENTER_X = (max_x + min_x) / 2
    BASE_CENTER_Y = (max_y + min_y) / 2
end

local function ScaleAndOffset()
    -- First scale the path to fix the width
    SCALE = DRAW_WIDTH / BASE_WIDTH

    -- Now see if it's too tall for the screen
    if BASE_HEIGHT * SCALE > DRAW_HEIGHT then
        SCALE = DRAW_HEIGHT / BASE_HEIGHT
    end

    local offset_x = CENTER_X - (BASE_CENTER_X * SCALE)
    local offset_y = CENTER_Y - (BASE_CENTER_Y * SCALE)

    for i = 1, #PATH do
        local x = (PATH[i][1] * SCALE) + offset_x
        local y = (PATH[i][2] * SCALE) + offset_y
        SCALED_PATH[#SCALED_PATH + 1] = { x, y }
    end
    return SCALED_PATH
end

local function SquareWave()
    THETA = 0.5 * math.pi
    D_THETA = 0.5 * math.pi

    AXIOM = "A"
    RULES.A = "f-f-B"
    RULES.B = "f+f+A"
end

local function Sierpinski()
    THETA = math.pi / 3
    D_THETA = math.pi / 3
    AXIOM = "A"
    RULES.A = "B-A-B"
    RULES.B = "A+B+A"
end

local function Dragon()
    THETA = 0
    D_THETA = 0.5 * math.pi
    AXIOM = "F"
    RULES.F = "F+G"
    RULES.G = "F-G"
end

local function Fern()
    THETA = 0
    D_THETA = 25 * math.pi / 180
    AXIOM = "X"
    RULES.X = "F+[[X]-X]-F[-FX]+X"
    RULES.F = "FF"
end

local function JaggedCoast()
    THETA = 90 * math.pi / 180
    D_THETA = 25 * math.pi / 180
    AXIOM = "A"
    RULES.A = "C+A+B"
    RULES.B = "A-B-C"
    RULES.C = "B+C-A"
end


local function SpaceIslands()
    -- Looks coolest at depth 10
    THETA = 0 * math.pi / 180
    D_THETA = 90 * math.pi / 180
    AXIOM = "A"
    RULES.A = "C+A+B"
    RULES.B = "A-B-C"
    RULES.C = "B+C-A"
end

local function Vines()
    THETA = 0 * math.pi / 180
    D_THETA = 42 * math.pi / 180
    AXIOM = "VXNXMX"
    RULES.V = "N+"
    RULES.X = "[+M]-N"
    RULES.N = "X+E"
    RULES.E = "NX-"
end



local function DrawIt()
    for i = 2, #SCALED_PATH do
        if SCALED_PATH[i][1] ~= PEN_UP and SCALED_PATH[i][2] ~= PEN_UP then
            DrawLine(SCALED_PATH[i - 1][1], SCALED_PATH[i - 1][2], SCALED_PATH[i][1], SCALED_PATH[i][2])
        end
    end
end



local function init()
    -- SquareWave()
    --Sierpinski()
    -- Dragon()
    -- Fern()
    -- JaggedCoast()
    -- SpaceIslands()
    Vines()
    Builder()
    Interpreter()
    ScaleAndOffset()
    pdgfx.setColor(pdgfx.kColorBlack)
end

---[[ Just draw it
function pd.update()
    DrawIt()
end
--]]

--[[ Draw it with a pen
function pd.update()
    pd.drawFPS(0, 0)
    TICKS = pd.getCrankTicks(180)
    CUR_TICKS = CUR_TICKS + TICKS
    if CUR_TICKS < 1 then
        CUR_TICKS = 1
    end

    if CUR_TICKS > #SCALED_PATH then
        CUR_TICKS = #SCALED_PATH
    end

    pdgfx.clear()

    DrawCircle(SCALED_PATH[CUR_TICKS][1], SCALED_PATH[CUR_TICKS][2], 3)

    for i = 2, CUR_TICKS do
        if SCALED_PATH[i][1] ~= PEN_UP and SCALED_PATH[i][2] ~= PEN_UP and
            SCALED_PATH[i - 1][1] ~= PEN_UP and SCALED_PATH[i - 1][2] ~= PEN_UP then
            DrawLine(SCALED_PATH[i - 1][1], SCALED_PATH[i - 1][2], SCALED_PATH[i][1], SCALED_PATH[i][2])
        end
    end
end
--]]

init()
