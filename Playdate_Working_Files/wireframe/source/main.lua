import "scripts/core_libs"

local pd = playdate
local gfx <const> = pd.graphics

PI = math.pi

X = {}
Y = {}
Z = {}

X_PLOT = {}
Y_PLOT = {}

OD = 5  -- Observer Distance
HA = 45 -- Half Angle

SCR_H = 240
SCR_W = 400

local function rotate(axis, degrees)
    local A = nil
    local B = nil

    if axis == "x" then
        A = Y
        B = Z
    elseif axis == "y" then
        A = X
        B = Z
    elseif axis == "z" then
        A = X
        B = Y
    end

    if A == nil or B == nil then
        print("Error: A or B is nil")
        return
    end

    local radians = degrees * (PI / 180)
    local cos = math.cos(radians)
    local sin = math.sin(radians)
    for i = 1, #A do
        local temp = cos * A[i] - sin * B[i]
        B[i] = sin * A[i] + cos * B[i]
        A[i] = temp
    end
end

local function project()
    for i = 1, #X do
        Y_PLOT[i] =
            (OD * Y[i]) / (OD + Z[i]) *
            (SCR_H / 2) *
            ( 1 / OD * math.tan(HA * (PI / 180)) ) +
            (SCR_H / 2)
        X_PLOT[i] =
            (OD * X[i]) / (OD + Z[i]) *
            (SCR_W / 2) *
            ( 1 / OD * math.tan(HA * (PI / 180)) ) +
            (SCR_W / 2)
    end
end

local function save_line(x1, y1, z1, x2, y2, z2)
    local pos = #X + 1
    X[pos] = x1
    Y[pos] = -y1
    Z[pos] = z1
    X[pos + 1] = x2
    Y[pos + 1] = -y2
    Z[pos + 1] = z2
end

local function draw()
    for i = 1, #X_PLOT - 1 do
        local x1 = X_PLOT[i]
        local y1 = Y_PLOT[i]
        local x2 = X_PLOT[i + 1]
        local y2 = Y_PLOT[i + 1]
        gfx.drawLine(x1, y1, x2, y2)
    end
end

local function build_pyramid()
    X = {}
    Y = {}
    Z = {}
    local x = {}
    local z = {}
    local yv = -1

    local n = 4
    for k = 1, n do
        local angle = k * 2 * PI / n
        x[k] = math.cos(angle)
        z[k] = math.sin(angle)
    end

    for k = 1, n do
        local next_k = (k % n) + 1 -- Fixes the 1 index problem
        save_line(0, 2, 0, x[k], yv, z[k])
        save_line(x[k], yv, z[k], x[next_k], yv, z[next_k])
    end
end

local function init()
    build_pyramid()
end

function pd.update()
    gfx.clear()
    gfx.drawText("OD: " .. OD, 2, 2)
    gfx.drawText("HA: " .. HA, 2, 20)
    project()
    draw()
    TICKS = pd.getCrankTicks(360)
    if playdate.buttonIsPressed(pd.kButtonA) then
        rotate("x", TICKS)
    elseif playdate.buttonIsPressed(pd.kButtonB) then
        rotate("z", TICKS)
    else
        rotate("y", TICKS)
    end
end

init()
