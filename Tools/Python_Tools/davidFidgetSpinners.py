from math import cos, pi, sin

from Gkit import (G_clear, G_fill_circle, G_fill_rectangle, G_init_graphics,
                  G_line, G_rgb, G_wait_key)

G_init_graphics(600, 600)


def my_clear():
    G_rgb(1, 1, 1)
    G_clear()
    G_rgb(0, 0, 0)
    G_fill_rectangle(3, 3, 599 - 3, 599 - 3)


# note, angle in degrees
def draw_fidget(angle, tail, sx0, sy0, sx1, sy1, sx2, sy2, target, max_target):

    r = 12
    sr = 6

    # convert degress to radians
    dtr = pi / 180

    # draw center circle
    rc = 0
    gc = 0.4
    bc = 0.4
    G_rgb(rc, gc, bc)
    G_fill_circle(300, 300, 30)
    G_rgb(0, 0, 0)
    G_fill_circle(300, 300, 28)
    G_rgb(rc, gc, bc)
    G_fill_circle(300, 300, 26)

    # calculate arms' centers
    x0 = 150 * cos(angle * dtr) + 300
    y0 = 150 * sin(angle * dtr) + 300

    x1 = 150 * cos(angle * dtr + 120 * dtr) + 300
    y1 = 150 * sin(angle * dtr + 120 * dtr) + 300

    x2 = 150 * cos(angle * dtr + 240 * dtr) + 300
    y2 = 150 * sin(angle * dtr + 240 * dtr) + 300

    # draw arms
    # arm 0:
    G_rgb(1, 1, 1)
    G_line(300, 300, x0, y0)
    # arm 1:
    G_rgb(1, 1, 1)
    G_line(300, 300, x1, y1)
    # arm 2:
    G_rgb(1, 1, 1)
    G_line(300, 300, x2, y2)

    # calculate and draw arm circle trail :
    k = tail - a
    if k < 1:
        k = 1
    while k < tail:
        sf = k / tail
        tr = r * sf
        ta = angle - (tail - k)

        # calculate trails' centers
        tx0 = 150 * cos(ta * dtr) + 300
        ty0 = 150 * sin(ta * dtr) + 300

        tx1 = 150 * cos(ta * dtr + 120 * dtr) + 300
        ty1 = 150 * sin(ta * dtr + 120 * dtr) + 300

        tx2 = 150 * cos(ta * dtr + 240 * dtr) + 300
        ty2 = 150 * sin(ta * dtr + 240 * dtr) + 300

        # arm 0:
        G_rgb(sf, 0, 0)
        G_fill_circle(tx0, ty0, 2)  # try making radius = tr
        # arm 1:
        G_rgb(sf, 0, 0)
        G_fill_circle(tx1, ty1, 2)  # try making radius = tr
        # arm 2:
        G_rgb(sf, 0, 0)
        G_fill_circle(tx2, ty2, 2)  # try making radius = tr

        k = k + 1

    # draw circles on arms
    # arm 0:
    rc = 0.7
    gc = 0
    bc = 0
    G_rgb(rc, gc, bc)
    G_fill_circle(x0, y0, r)
    G_rgb(0, 0, 0)
    G_fill_circle(x0, y0, r - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(x0, y0, r - 4)

    # arm 1:
    G_rgb(rc, gc, bc)
    G_fill_circle(x1, y1, r)
    G_rgb(0, 0, 0)
    G_fill_circle(x1, y1, r - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(x1, y1, r - 4)

    # arm 2:
    G_rgb(rc, gc, bc)
    G_fill_circle(x2, y2, r)
    G_rgb(0, 0, 0)
    G_fill_circle(x2, y2, r - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(x2, y2, r - 4)

    # draw satellites
    # calculate satellites' centers
    su = 7
    sax0 = 75 * cos(su * angle * dtr) + x0
    say0 = 75 * sin(su * angle * dtr) + y0

    sax1 = 75 * cos(su * angle * dtr + 120 * dtr) + x1
    say1 = 75 * sin(su * angle * dtr + 120 * dtr) + y1

    sax2 = 75 * cos(su * angle * dtr + 240 * dtr) + x2
    say2 = 75 * sin(su * angle * dtr + 240 * dtr) + y2

    # draw satellite arms
    # arm 0:
    G_rgb(1, 1, 1)
    G_line(x0, y0, sax0, say0)
    # arm 1:
    G_rgb(1, 1, 1)
    G_line(x1, y1, sax1, say1)
    # arm 2:
    G_rgb(1, 1, 1)
    G_line(x2, y2, sax2, say2)

    # draw circles on satellite arms
    # arm 0:
    rc = 0.5
    gc = 0
    bc = 1
    G_rgb(rc, gc, bc)
    G_fill_circle(sax0, say0, sr)
    G_rgb(0, 0, 0)
    G_fill_circle(sax0, say0, sr - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(sax0, say0, sr - 4)

    # arm 1:
    G_rgb(rc, gc, bc)
    G_fill_circle(sax1, say1, sr)
    G_rgb(0, 0, 0)
    G_fill_circle(sax1, say1, sr - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(sax1, say1, sr - 4)

    # arm 2:
    G_rgb(rc, gc, bc)
    G_fill_circle(sax2, say2, sr)
    G_rgb(0, 0, 0)
    G_fill_circle(sax2, say2, sr - 2)
    G_rgb(rc, gc, bc)
    G_fill_circle(sax2, say2, sr - 4)

    # calculate and draw satellite trail :

    if target < max_target:
        sx0[target] = 75 * cos(su * angle * dtr) + x0
        sy0[target] = 75 * sin(su * angle * dtr) + y0

        sx1[target] = 75 * cos(su * angle * dtr + 120 * dtr) + x1
        sy1[target] = 75 * sin(su * angle * dtr + 120 * dtr) + y1

        sx2[target] = 75 * cos(su * angle * dtr + 240 * dtr) + x2
        sy2[target] = 75 * sin(su * angle * dtr + 240 * dtr) + y2

    else:
        k = 0
        while k < target - 1:
            sx0[k] = sx0[k + 1]
            sy0[k] = sy0[k + 1]

            sx1[k] = sx1[k + 1]
            sy1[k] = sy1[k + 1]

            sx2[k] = sx2[k + 1]
            sy2[k] = sy2[k + 1]

            k = k + 1
        sx0[k] = 75 * cos(su * angle * dtr) + x0
        sy0[k] = 75 * sin(su * angle * dtr) + y0
        sx1[k] = 75 * cos(su * angle * dtr + 120 * dtr) + x1
        sy1[k] = 75 * sin(su * angle * dtr + 120 * dtr) + y1
        sx2[k] = 75 * cos(su * angle * dtr + 240 * dtr) + x2
        sy2[k] = 75 * sin(su * angle * dtr + 240 * dtr) + y2

    k = 0
    # print("target = ", target)
    while k < target:
        sf = k / target
        G_rgb(0.5 * sf, 0, sf)
        G_fill_circle(sx0[k], sy0[k], 2)
        G_fill_circle(sx1[k], sy1[k], 2)
        G_fill_circle(sx2[k], sy2[k], 2)
        k = k + 1

    # tail = 10*tail
    # k = tail - a
    # if(k < 1) :
    # 	k = 1
    # while(k < tail) :
    # 	sf = k/tail
    # 	tr = sr*sf
    # 	ta = su*angle-(tail-k)

    # 	# calculate trails' centers
    # 	tx0 = 75*cos(ta*dtr) + x0
    # 	ty0 = 75*sin(ta*dtr) + y0

    # 	tx1 = 75*cos(ta*dtr + 120*dtr) + x1
    # 	ty1 = 75*sin(ta*dtr + 120*dtr) + y1

    # 	tx2 = 75*cos(ta*dtr + 240*dtr) + x2
    # 	ty2 = 75*sin(ta*dtr + 240*dtr) + y2

    # 	# arm 0:
    # 	G_rgb(sf,sf,sf)
    # 	G_fill_circle(tx0,ty0,1) # try making radius = tr
    # 	# arm 1:
    # 	G_rgb(sf,sf,sf)
    # 	G_fill_circle(tx1,ty1,1) # try making radius = tr
    # 	# arm 2:
    # 	G_rgb(sf,sf,sf)
    # 	G_fill_circle(tx2,ty2,1) # try making radius = tr

    # 	k = k + 1


## main #############################################################
# initializing trailing lists:
max_target = 120
sx0 = []
sy0 = []
sx1 = []
sy1 = []
sx2 = []
sy2 = []
k = 0
while k < max_target:
    sx0.append(-1)
    sy0.append(-1)
    sx1.append(-1)
    sy1.append(-1)
    sx2.append(-1)
    sy2.append(-1)
    k = k + 1


print("Type any key for next frame and 'q' to quit.")

tail = 100
q = 0
a = 0
target = 0
while q != 113:
    my_clear()
    draw_fidget(a, tail, sx0, sy0, sx1, sy1, sx2, sy2, target, max_target)
    q = G_wait_key()
    a = a + 0.7
    if a >= 360 + tail:
        a = tail
    if target < max_target:
        target = target + 1
